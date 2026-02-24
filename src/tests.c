/* allocs - Custom memory allocators written as a study exercise.
Copyright (C) 2026  Emir Baha Yıldırım

This program is free software: you can redistribute it and/or modify
it under the terms of the GNU General Public License as published by
the Free Software Foundation, either version 3 of the License, or
(at your option) any later version.

This program is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with this program.  If not, see <https://www.gnu.org/licenses/>. */

/* Assisted by: Gemini:gemini-3.1-pro-preview */

#include <stdio.h>
#include <string.h>
#include <stdint.h>
#include <stdbool.h>
#include "jmalloc.h"

// Simple testing macros for cleaner output
#define TEST_PRINT(msg) printf("[TEST] %s\n", msg)
#define TEST_PASS(msg) printf("[PASS] %s\n", msg)
#define TEST_FAIL(msg) printf("[FAIL] %s\n", msg)

int main(void)
{
	printf("=== jmalloc/jfree Comprehensive Test Suite ===\n\n");

	// Phase 1: Basic Allocation & Data Integrity
	TEST_PRINT("Phase 1: Basic Allocation & Data Integrity");
	char *str = (char *)jmalloc(14);
	if (str != NULL) {
		strcpy(str, "Hello, World!");
		printf("  Data read: %s\n", str);
		TEST_PASS("Basic allocation and memory write/read successful.");
		jfree(str);
	} else {
		TEST_FAIL("Basic allocation returned NULL.");
	}

	// Phase 2: Edge Cases - Zero & Huge Allocations
	TEST_PRINT("\nPhase 2: Size Edge Cases");

	void *zero_ptr = jmalloc(0);
	printf("  jmalloc(0) returned: %p\n", zero_ptr);
	// Note: The C standard allows malloc(0) to return NULL or a unique pointer that can be safely freed.
	jfree(zero_ptr);

	void *huge_ptr = jmalloc(SIZE_MAX);
	if (huge_ptr == NULL) {
		TEST_PASS(
			"jmalloc(SIZE_MAX) correctly returned NULL (Out of Memory expected).");
	} else {
		TEST_FAIL(
			"jmalloc(SIZE_MAX) returned a pointer. This is highly suspicious.");
		jfree(huge_ptr);
	}

	// Phase 3: Freeing Edge Cases
	TEST_PRINT("\nPhase 3: Freeing Edge Cases");
	TEST_PRINT("  Calling jfree(NULL)...");
	jfree(NULL); // A robust free() implementation must treat NULL as a no-op and not crash.
	TEST_PASS("jfree(NULL) handled gracefully without segfaulting.");

	// Phase 4: Fragmentation & Block Reuse Simulation
	TEST_PRINT("\nPhase 4: Fragmentation & Block Reuse");
	void *a = jmalloc(128);
	void *b = jmalloc(256);
	void *c = jmalloc(128);

	printf("  Allocated Block A: %p (128 bytes)\n", a);
	printf("  Allocated Block B: %p (256 bytes)\n", b);
	printf("  Allocated Block C: %p (128 bytes)\n", c);

	jfree(b);
	TEST_PRINT("  Freed Block B (creating a 256-byte hole).");

	void *d = jmalloc(200);
	printf("  Allocated Block D: %p (200 bytes)\n", d);

	if (d == b) {
		TEST_PASS(
			"Perfect! Block B's space was successfully reused for Block D.");
	} else {
		printf("  [NOTE] Block D was placed elsewhere. (Check your block-splitting or first-fit/best-fit logic if you expected reuse here).\n");
	}

	// Clean up the remaining blocks
	jfree(a);
	jfree(c);
	jfree(d);

	printf("\n=== Test Suite Complete ===\n");
	return 0;
}

// /* TESTS */
// #include "jmalloc.h"
// #include <stdio.h>
// #include <string.h>
// #include <assert.h>
//
// int main() {
//     printf("Starting custom malloc tests...\n\n");
//
//     // 1. Basic Allocation & Write Test
//     printf("Test 1: Basic allocation and write...\n");
//     int *single_int = (int *)jmalloc(sizeof(int));
//     assert(single_int != NULL);
//     *single_int = 42;
//     assert(*single_int == 42);
//     printf("  -> Passed. Value retained: %d\n", *single_int);
//     jfree(single_int);
//
//     // 2. Multiple Allocations Test (Checking for overlap)
//     printf("Test 2: Multiple allocations (no overlap)...\n");
//     int *arr1 = (int *)jmalloc(100 * sizeof(int));
//     int *arr2 = (int *)jmalloc(100 * sizeof(int));
//     assert(arr1 != NULL && arr2 != NULL);
//     assert(arr1 != arr2); // Pointers must be distinct
//
//     // Fill them to ensure they don't corrupt each other's memory blocks
//     for (int i = 0; i < 100; i++) {
//         arr1[i] = 1;
//         arr2[i] = 2;
//     }
//     for (int i = 0; i < 100; i++) {
//         assert(arr1[i] == 1);
//         assert(arr2[i] == 2);
//     }
//     printf("  -> Passed. Memory blocks are isolated.\n");
//     jfree(arr1);
//     jfree(arr2);
//
//     // 3. Edge Case: malloc(0)
//     printf("Test 3: Edge case - jmalloc(0)...\n");
//     void *zero_ptr = jmalloc(0);
//     // Behavior of malloc(0) is implementation-defined (can be NULL or a unique pointer).
//     // The main goal here is to ensure your allocator doesn't segfault or hang.
//     printf("  -> Passed. Returned: %p\n", zero_ptr);
//     // if (zero_ptr) free(zero_ptr);
//
//     // 4. Large Allocation Test
//     printf("Test 4: Large allocation...\n");
//     size_t large_size = 1024 * 1024 * 10; // 10 MB
//     char *large_chunk = (char *)jmalloc(large_size);
//     if (large_chunk != NULL) {
//         memset(large_chunk, 'A', large_size);
//         assert(large_chunk[large_size - 1] == 'A');
//         printf("  -> Passed. 10MB allocated and successfully written.\n");
//         jfree(large_chunk);
//     } else {
//         printf("  -> Passed. Handled gracefully (returned NULL for large request).\n");
//     }
//
//     printf("\nAll basic tests completed successfully!\n");
//     return 0;
// }
