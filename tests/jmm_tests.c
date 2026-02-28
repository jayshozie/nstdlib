#if defined(__JMM_DEBUG)
/* tests/jmm_tests.c - JMM Implementation Tests
Copyright (C) 2026  Emir Baha Yıldırım */

#include "jmm.h"
#include <stdio.h>
#include <string.h>
#include <stdint.h>
#include <stdbool.h>
#include <assert.h>

extern bool g_test_failed;

#define TEST_PRINT(msg) printf("[TEST] %s\n", msg)
#define TEST_PASS(msg) printf("[PASS] %s\n", msg)
#define TEST_FAIL(msg)                      \
	do {                                \
		printf("[FAIL] %s\n", msg); \
		g_test_failed = true;       \
	} while (0)

/* --- jmalloc Tests --- */
#if defined(__TEST_JMALLOC)
static void jmalloc_basic_allocation()
{
	TEST_PRINT("jmalloc: Basic allocation, alignment, and data integrity");
	size_t size = 100;
	uint8_t *ptr = jmalloc(size);
	if (ptr == NULL) {
		TEST_FAIL("jmalloc returned NULL for a basic allocation.");
		return;
	}

	if (((uintptr_t)ptr % 16) == 0) {
		TEST_PASS("jmalloc: Pointer is 16-byte aligned.");
	} else {
		TEST_FAIL("jmalloc: Pointer is NOT 16-byte aligned.");
	}

	for (size_t i = 0; i < size; i++) {
		ptr[i] = (uint8_t)(i & 0xFF);
	}

	bool match = true;
	for (size_t i = 0; i < size; i++) {
		if (ptr[i] != (uint8_t)(i & 0xFF)) {
			match = false;
			break;
		}
	}

	if (match) {
		TEST_PASS("jmalloc: Data integrity verified.");
	} else {
		TEST_FAIL("jmalloc: Data corruption detected.");
	}

	jfree(ptr);
}

static void jmalloc_zero_allocation()
{
	TEST_PRINT("jmalloc: Zero-size allocation");
	void *ptr = jmalloc(0);
	/* POSIX allows either NULL or a unique pointer. If it returns a pointer, it must be freeable. */
	if (ptr == NULL) {
		TEST_PASS("jmalloc(0) returned NULL as allowed by POSIX.");
	} else {
		jfree(ptr);
		TEST_PASS(
			"jmalloc(0) returned a unique pointer and was freed successfully.");
	}
}

static void jmalloc_huge_allocation()
{
	TEST_PRINT("jmalloc: Huge allocation (jmalloc(SIZE_MAX))");
	void *huge_ptr = jmalloc(SIZE_MAX);
	if (huge_ptr == NULL) {
		TEST_PASS(
			"jmalloc(SIZE_MAX) correctly returned NULL (OOM expected).");
	} else {
		TEST_FAIL(
			"jmalloc(SIZE_MAX) returned a pointer, which is highly suspicious.");
		jfree(huge_ptr);
	}
}

static void jmalloc_multiple_allocations()
{
	TEST_PRINT("jmalloc: Multiple sequential allocations");
	void *p1 = jmalloc(100);
	void *p2 = jmalloc(200);
	void *p3 = jmalloc(300);

	if (p1 && p2 && p3 && (p1 != p2) && (p2 != p3) && (p1 != p3)) {
		TEST_PASS(
			"jmalloc: Multiple allocations returned unique pointers.");
	} else {
		TEST_FAIL("jmalloc: Multiple allocations failed.");
	}

	jfree(p1);
	jfree(p2);
	jfree(p3);
}

void test_jmalloc()
{
	jmalloc_basic_allocation();
	jmalloc_zero_allocation();
	jmalloc_huge_allocation();
	jmalloc_multiple_allocations();
}
#endif

/* --- jfree Tests --- */
#if defined(__TEST_JFREE)
static void jfree_null()
{
	TEST_PRINT("jfree: Freeing a NULL pointer");
	jfree(NULL);
	TEST_PASS("jfree(NULL) was handled gracefully (no-op).");
}

static void jfree_coalescing()
{
	TEST_PRINT("jfree: Coalescing adjacent free blocks");
	void *p1 = jmalloc(128);
	void *p2 = jmalloc(128);
	void *p3 = jmalloc(128);
	if (!p1 || !p2 || !p3) {
		TEST_FAIL("jfree: Coalescing test setup failed.");
		jfree(p1);
		jfree(p2);
		jfree(p3);
		return;
	}

	jfree(p2);
	jfree(p1); // Should coalesce with p2's block

	void *p4 = jmalloc(200); // Should fit in the coalesced block
	if (p4 == p1) {
		TEST_PASS("jfree: Forward coalescing verified (p1 + p2).");
	} else {
		TEST_FAIL("jfree: Forward coalescing failed.");
	}
	jfree(p3);
	jfree(p4);
}

static void jfree_stress()
{
	TEST_PRINT("jfree: Stress test (alloc/free fragmentation)");
	void *ptrs[100];
	for (int i = 0; i < 100; i++) {
		ptrs[i] = jmalloc(i + 1);
		if (ptrs[i])
			memset(ptrs[i], i, i + 1);
	}

	for (int i = 0; i < 100; i += 2) {
		jfree(ptrs[i]);
		ptrs[i] = NULL;
	}

	bool corruption = false;
	for (int i = 1; i < 100; i += 2) {
		if (!ptrs[i])
			continue;
		bool match = true;
		uint8_t *p = (uint8_t *)ptrs[i];
		for (int j = 0; j < i + 1; j++) {
			if (p[j] != (uint8_t)i) {
				match = false;
				break;
			}
		}
		if (!match) {
			corruption = true;
			break;
		}
	}
	if (corruption) {
		TEST_FAIL(
			"jfree: Data corruption detected during stress test.");
	} else {
		TEST_PASS(
			"jfree: Data integrity maintained during stress test.");
	}

	for (int i = 0; i < 100; i++) {
		jfree(ptrs[i]); // Free remaining, including NULL ptrs
	}
	TEST_PASS("jfree: Stress test cleanup successful.");
}

void test_jfree()
{
	jfree_null();
	jfree_coalescing();
	jfree_stress();
}
#endif

#if defined(__TEST_JREALLOC)
void test_jrealloc_basic()
{
	TEST_PRINT("jrealloc basic (NULL and 0)");
	void *p1 = jrealloc(NULL, 100);
	assert(p1 != NULL);
	memset(p1, 0xEE, 100);
	TEST_PASS("jrealloc(NULL, 100) successful.");

	void *p2 = jrealloc(p1, 0);
	assert(p2 == NULL);
	TEST_PASS("jrealloc(p, 0) returned NULL.");
}

void test_jrealloc_shrink()
{
	TEST_PRINT("jrealloc shrink");
	void *p1 = jmalloc(1024);
	memset(p1, 0xDD, 1024);

	void *p2 = jrealloc(p1, 512);
	if (p1 == p2) {
		TEST_PASS("jrealloc shrank block in-place.");
	} else {
		TEST_FAIL(
			"jrealloc relocated block instead of shrinking in-place.");
	}

	bool match = true;
	for (size_t i = 0; i < 512; i++) {
		if (((uint8_t *)p2)[i] != 0xDD) {
			match = false;
			break;
		}
	}
	if (match) {
		TEST_PASS("Data preserved after shrink.");
	} else {
		TEST_FAIL("Data corruption after shrink.");
	}
	jfree(p2);
}

void test_jrealloc_expand_inplace()
{
	TEST_PRINT("jrealloc expand in-place");
	void *p1 = jmalloc(128);
	void *p2 = jmalloc(128);
	jfree(p2);

	void *p3 = jrealloc(p1, 200);
	if (p3 == p1) {
		TEST_PASS(
			"jrealloc expanded block in-place using next free block.");
	} else {
		printf("  p3 (%p) != p1 (%p). In-place expansion failed.\n", p3,
		       p1);
	}
	jfree(p3);
}

void test_jrealloc_relocate()
{
	TEST_PRINT("jrealloc relocate (Path 3)");
	void *p1 = jmalloc(128);
	memset(p1, 0xCC, 128);
	void *p2 = jmalloc(128);

	void *p3 = jrealloc(p1, 512);
	if (p3 == NULL) {
		TEST_FAIL("jrealloc returned NULL during relocation.");
	} else if (p3 != p1) {
		TEST_PASS("jrealloc relocated block correctly.");
		bool match = true;
		for (size_t i = 0; i < 128; i++) {
			if (((uint8_t *)p3)[i] != 0xCC) {
				match = false;
				break;
			}
		}
		if (match) {
			TEST_PASS("Data preserved after relocation.");
		} else {
			TEST_FAIL("Data corruption after relocation.");
		}
	} else {
		TEST_FAIL(
			"jrealloc did not relocate but p2 was supposed to block it.");
	}

	jfree(p3);
	jfree(p2);
}
#endif

void run_jmm_tests()
{
	printf("=== JMM (Custom Malloc) Comprehensive Suite ===\n\n");
#if defined(__TEST_JMALLOC)
	test_jmalloc();
#endif
#if defined(__TEST_JFREE)
	test_jfree();
#endif
#if defined(__TEST_JREALLOC)
	test_jrealloc_basic();
	test_jrealloc_shrink();
	test_jrealloc_expand_inplace();
	test_jrealloc_relocate();
#endif
	printf("\n=== All Malloc Tests Finished ===\n");
}
#endif
