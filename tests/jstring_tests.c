#if defined(__JSTR_DEBUG)
/* tests/jstring_tests.c - JString Implementation Tests
Copyright (C) 2026  Emir Baha Yıldırım */

#include "jstring.h"
#include <stdio.h>
#include <string.h>
#include <assert.h>
#include <stdlib.h>
#include <stdbool.h>

extern bool g_test_failed;

#define TEST_PRINT(msg) printf("[TEST] %s\n", msg)
#define TEST_PASS(msg) printf("[PASS] %s\n", msg)
#define TEST_FAIL(msg)                      \
	do {                                \
		printf("[FAIL] %s\n", msg); \
		g_test_failed = true;       \
	} while (0)

#if defined(__TEST_MEMCPY)
void test_jmemcpy()
{
	TEST_PRINT("jmemcpy");
	/* 1. Basic copy */
	char src[] = "Hello, World!";
	char dest[20];
	memset(dest, 'X', 20);
	jmemcpy(dest, src, sizeof(src));
	if (memcmp(dest, src, sizeof(src)) == 0) {
		TEST_PASS("jmemcpy copied data correctly.");
	} else {
		TEST_FAIL("jmemcpy failed to copy data correctly.");
	}

	/* 2. n=0 copy */
	char dest0[5] = "AAAAA";
	jmemcpy(dest0, src, 0);
	if (memcmp(dest0, "AAAAA", 5) == 0) {
		TEST_PASS("jmemcpy with n=0 did not modify dest.");
	} else {
		TEST_FAIL("jmemcpy with n=0 modified dest.");
	}
}
#endif

#if defined(__TEST_MEMMOVE)
void test_jmemmove()
{
	TEST_PRINT("jmemmove");
	/* 1. Overlap: src < dst */
	char data1[] = "1234567890";
	jmemmove(data1 + 2, data1, 5);
	if (memcmp(data1, "1212345890", 10) == 0) {
		TEST_PASS(
			"jmemmove handled overlapping memory (src < dst) correctly.");
	} else {
		TEST_FAIL("jmemmove failed overlapping copy (src < dst).");
	}

	/* 2. Overlap: src > dst */
	char data2[] = "1234567890";
	jmemmove(data2, data2 + 2, 5);
	if (memcmp(data2, "3456767890", 10) == 0) {
		TEST_PASS(
			"jmemmove handled overlapping memory (src > dst) correctly.");
	} else {
		TEST_FAIL("jmemmove failed overlapping copy (src > dst).");
	}

	/* 3. n=0 copy */
	char data3[] = "AAAAA";
	jmemmove(data3, data1, 0);
	if (memcmp(data3, "AAAAA", 5) == 0) {
		TEST_PASS("jmemmove with n=0 did not modify dest.");
	} else {
		TEST_FAIL("jmemmove with n=0 modified dest.");
	}
}
#endif

#if defined(__TEST_MEMSET)
void test_jmemset()
{
	TEST_PRINT("jmemset");
	/* 1. Basic set */
	char buffer[10];
	jmemset(buffer, 'A', 10);
	bool ok = true;
	for (int i = 0; i < 10; i++) {
		if (buffer[i] != 'A') {
			ok = false;
			break;
		}
	}
	if (ok) {
		TEST_PASS("jmemset filled buffer correctly.");
	} else {
		TEST_FAIL("jmemset failed to fill buffer.");
	}

	/* 2. n=0 set */
	char buffer0[5] = "BBBBB";
	jmemset(buffer0, 'A', 0);
	if (memcmp(buffer0, "BBBBB", 5) == 0) {
		TEST_PASS("jmemset with n=0 did not modify buffer.");
	} else {
		TEST_FAIL("jmemset with n=0 modified buffer.");
	}
}
#endif

#if defined(__TEST_MEMCMP)
void test_jmemcmp()
{
	TEST_PRINT("jmemcmp");
	char b1[] = "ABCDE";
	char b2[] = "ABCDE";
	char b3[] = "ABCZE";
	/* 1. Basic comparisons */
	if (jmemcmp(b1, b2, 5) == 0 && jmemcmp(b1, b3, 5) < 0 &&
	    jmemcmp(b3, b1, 5) > 0) {
		TEST_PASS("jmemcmp basic comparison verified.");
	} else {
		TEST_FAIL("jmemcmp failed basic comparison.");
	}

	/* 2. n=0 comparison */
	if (jmemcmp(b1, b3, 0) == 0) {
		TEST_PASS("jmemcmp with n=0 returned 0.");
	} else {
		TEST_FAIL("jmemcmp with n=0 failed to return 0.");
	}
}
#endif

#if defined(__TEST_STRLEN)
void test_jstrlen()
{
	TEST_PRINT("jstrlen");
	if (jstrlen("Hello") == 5 && jstrlen("") == 0) {
		TEST_PASS("jstrlen correctly calculated length.");
	} else {
		TEST_FAIL("jstrlen returned incorrect length.");
	}
}
#endif

#if defined(__TEST_STRCPY)
void test_jstrcpy()
{
	TEST_PRINT("jstrcpy");
	/* 1. Basic jstrcpy */
	char dest[20];
	char *ret = jstrcpy(dest, "Hello");
	if (ret == dest && strcmp(dest, "Hello") == 0) {
		TEST_PASS("jstrcpy basic copy verified.");
	} else {
		TEST_FAIL("jstrcpy failed.");
	}

	/* 2. jstrcpy empty string */
	char dest_empty[10] = "XXXXX";
	jstrcpy(dest_empty, "");
	if (dest_empty[0] == '\0') {
		TEST_PASS("jstrcpy empty string works.");
	} else {
		TEST_FAIL("jstrcpy empty string failed.");
	}
}
#endif

#if defined(__TEST_STPCPY)
void test_jstpcpy()
{
	TEST_PRINT("jstpcpy");
	/* 1. Basic jstpcpy */
	char dest2[20];
	char *ret2 = jstpcpy(dest2, "World");
	if (ret2 == dest2 + 5 && strcmp(dest2, "World") == 0) {
		TEST_PASS("jstpcpy basic copy and pointer return verified.");
	} else {
		TEST_FAIL("jstpcpy failed.");
	}

	/* 2. jstpcpy empty string */
	char dest2_empty[10] = "YYYYY";
	char *ret2_empty = jstpcpy(dest2_empty, "");
	if (ret2_empty == dest2_empty && dest2_empty[0] == '\0') {
		TEST_PASS("jstpcpy empty string works.");
	} else {
		TEST_FAIL("jstpcpy empty string failed.");
	}
}
#endif

#if defined(__TEST_STRCAT)
void test_jstrcat()
{
	TEST_PRINT("jstrcat");
	/* 1. Basic jstrcat */
	char dest[20] = "Hello";
	jstrcat(dest, " World");
	if (strcmp(dest, "Hello World") == 0) {
		TEST_PASS("jstrcat concatenated strings correctly.");
	} else {
		TEST_FAIL("jstrcat failed.");
	}

	/* 2. jstrcat to empty dest */
	char dest_empty[20] = "";
	jstrcat(dest_empty, "Start");
	if (strcmp(dest_empty, "Start") == 0) {
		TEST_PASS("jstrcat to empty string works.");
	} else {
		TEST_FAIL("jstrcat to empty string failed.");
	}

	/* 3. jstrcat empty src */
	char dest_full[20] = "Full";
	jstrcat(dest_full, "");
	if (strcmp(dest_full, "Full") == 0) {
		TEST_PASS("jstrcat with empty source works.");
	} else {
		TEST_FAIL("jstrcat with empty source failed.");
	}
}
#endif

#if defined(__TEST_STRNCPY)
void test_jstrncpy()
{
	TEST_PRINT("jstrncpy");

	/* 1. Basic copy with padding (src < n) */
	char dest1[10];
	memset(dest1, 'A', 10);
	char *ret1 = jstrncpy(dest1, "Hello", 10);
	if (ret1 == dest1 && memcmp(dest1, "Hello\0\0\0\0\0", 10) == 0) {
		TEST_PASS("jstrncpy: padding works and returns dest.");
	} else {
		TEST_FAIL("jstrncpy: padding failed.");
	}

	/* 2. Truncation (src > n) */
	char dest2[6];
	memset(dest2, 'A', 6);
	char *ret2 = jstrncpy(dest2, "123456789", 5);
	/* dest2 should be "12345A" since n=5 and dest2[5] was 'A' */
	if (ret2 == dest2 && memcmp(dest2, "12345A", 6) == 0) {
		TEST_PASS("jstrncpy: truncation works (no overflow at n+1).");
	} else {
		TEST_FAIL("jstrncpy: truncation failed or overflowed.");
	}

	/* 3. Empty string with n > 0 */
	char dest3[5];
	memset(dest3, 'B', 5);
	jstrncpy(dest3, "", 3);
	/* dest3 should be "\0\0\0BB" */
	if (memcmp(dest3, "\0\0\0BB", 5) == 0) {
		TEST_PASS("jstrncpy: empty string padding works.");
	} else {
		TEST_FAIL("jstrncpy: empty string padding failed.");
	}

	/* 4. n = 0 */
	char dest4[5];
	memset(dest4, 'C', 5);
	jstrncpy(dest4, "Hello", 0);
	if (memcmp(dest4, "CCCCC", 5) == 0) {
		TEST_PASS("jstrncpy: n=0 leaves dest unchanged.");
	} else {
		TEST_FAIL("jstrncpy: n=0 modified dest.");
	}
}
#endif

#if defined(__TEST_STPNCPY)
void test_jstpncpy()
{
	TEST_PRINT("jstpncpy");

	/* 1. Basic copy within limits */
	char dest1[10];
	memset(dest1, 'A', 10);
	char *ret1 = jstpncpy(dest1, "Hello", 10);
	if (ret1 == dest1 + 5 && memcmp(dest1, "Hello\0\0\0\0\0", 10) == 0) {
		TEST_PASS(
			"jstpncpy: basic copy with padding works and returns correct pointer.");
	} else {
		TEST_FAIL("jstpncpy: basic copy with padding failed.");
	}

	/* 2. Copy exactly n bytes */
	char dest2[6];
	memset(dest2, 'A', 6);
	char *ret2 = jstpncpy(dest2, "12345", 5);
	if (ret2 == dest2 + 5 && memcmp(dest2, "12345A", 6) == 0) {
		TEST_PASS("jstpncpy: copy exactly n bytes works.");
	} else {
		TEST_FAIL("jstpncpy: copy exactly n bytes failed.");
	}

	/* 3. Padding check beyond first null */
	char dest3[20];
	memset(dest3, 'A', 20);
	char *ret3 = jstpncpy(dest3, "pad", 15);
	bool padded = true;
	for (int i = 3; i < 15; i++) {
		if (dest3[i] != '\0')
			padded = false;
	}
	if (ret3 == dest3 + 3 && padded && dest3[15] == 'A') {
		TEST_PASS(
			"jstpncpy: large padding and pointer return verified.");
	} else {
		TEST_FAIL("jstpncpy: padding or pointer return failed.");
	}
}
#endif

#if defined(__TEST_STRCMP)
void test_jstrcmp()
{
	TEST_PRINT("jstrcmp");
	/* 1. jstrcmp basic */
	if (jstrcmp("abc", "abc") == 0 && jstrcmp("abc", "abd") < 0 &&
	    jstrcmp("abd", "abc") > 0) {
		TEST_PASS("jstrcmp basic comparison verified.");
	} else {
		TEST_FAIL("jstrcmp basic failed.");
	}

	/* 2. jstrcmp empty cases */
	if (jstrcmp("", "") == 0 && jstrcmp("", "a") < 0 &&
	    jstrcmp("a", "") > 0) {
		TEST_PASS("jstrcmp empty string cases verified.");
	} else {
		TEST_FAIL("jstrcmp empty cases failed.");
	}
}
#endif

#if defined(__TEST_STRNCMP)
void test_jstrncmp()
{
	TEST_PRINT("jstrncmp");
	/* 1. jstrncmp basic */
	if (jstrncmp("abcde", "abczz", 3) == 0 &&
	    jstrncmp("abcde", "abczz", 4) < 0 &&
	    jstrncmp("abczz", "abcde", 4) > 0) {
		TEST_PASS("jstrncmp basic comparison verified.");
	} else {
		TEST_FAIL("jstrncmp basic failed.");
	}

	/* 2. jstrncmp n=0 */
	if (jstrncmp("abc", "def", 0) == 0) {
		TEST_PASS("jstrncmp with n=0 returned 0.");
	} else {
		TEST_FAIL("jstrncmp with n=0 failed to return 0.");
	}

	/* 3. jstrncmp differ after n */
	if (jstrncmp("abcde", "abcfg", 3) == 0) {
		TEST_PASS("jstrncmp properly ignored difference after n.");
	} else {
		TEST_FAIL("jstrncmp did not ignore difference after n.");
	}

	/* 4. jstrncmp doesn't differ, extra byte */
	if (jstrncmp("a", "a", 10) == 0) {
		TEST_PASS("jstrncmp properly handled n > strlen.");
	} else {
		TEST_FAIL("jstrncmp failed when n > strlen.");
	}
}
#endif

#if defined(__TEST_STRCHR)
void test_jstrchr()
{
	TEST_PRINT("jstrchr");
	const char *s = "Hello World";
	/* 1. Basic jstrchr */
	if (jstrchr(s, 'o') == s + 4 && jstrchr(s, 'z') == NULL) {
		TEST_PASS("jstrchr found correct character.");
	} else {
		TEST_FAIL("jstrchr failed.");
	}

	/* 2. jstrchr finding null terminator */
	if (jstrchr(s, '\0') == s + 11) {
		TEST_PASS("jstrchr found null terminator.");
	} else {
		TEST_FAIL("jstrchr failed to find null terminator.");
	}
}
#endif

#if defined(__TEST_STRRCHR)
void test_jstrrchr()
{
	TEST_PRINT("jstrrchr");
	const char *s = "Hello World";
	/* 1. Basic jstrrchr */
	if (jstrrchr(s, 'o') == s + 7) {
		TEST_PASS("jstrrchr found last character.");
	} else {
		TEST_FAIL("jstrrchr failed to find last character.");
	}

	/* 2. jstrrchr finding null terminator */
	if (jstrrchr(s, '\0') == s + 11) {
		TEST_PASS("jstrrchr found null terminator.");
	} else {
		TEST_FAIL("jstrrchr failed to find null terminator.");
	}

	/* 3. jstrrchr missing character (should return NULL) */
	if (jstrrchr(s, 'z') == NULL) {
		TEST_PASS("jstrrchr returned NULL for missing character.");
	} else {
		TEST_FAIL(
			"jstrrchr failed to return NULL for missing character.");
	}

	/* 4. jstrrchr finding the very first character */
	if (jstrrchr(s, 'H') == s) {
		TEST_PASS("jstrrchr found the first character.");
	} else {
		TEST_FAIL("jstrrchr failed to find the first character.");
	}
}
#endif

#if defined(__TEST_STRCHRNUL)
void test_jstrchrnul()
{
	TEST_PRINT("jstrchrnul");
	const char *s = "Hello World";
	/* 1. Basic jstrchrnul */
	if (jstrchrnul(s, 'z') == s + 11) {
		TEST_PASS(
			"jstrchrnul returned end of string for missing char.");
	} else {
		TEST_FAIL("jstrchrnul failed.");
	}

	/* 2. jstrchrnul finding an existing character */
	if (jstrchrnul(s, 'o') == s + 4) {
		TEST_PASS("jstrchrnul found existing character.");
	} else {
		TEST_FAIL("jstrchrnul failed to find existing character.");
	}

	/* 3. jstrchrnul finding null terminator explicitly */
	if (jstrchrnul(s, '\0') == s + 11) {
		TEST_PASS("jstrchrnul handled explicit null search.");
	} else {
		TEST_FAIL("jstrchrnul failed explicit null search.");
	}
}
#endif

#if defined(__TEST_STRSEP)
void test_jstrsep()
{
	TEST_PRINT("jstrsep");
	/* 1. Basic parsing */
	char input1[] = "abc,def,ghi";
	char *tmp1 = input1;
	char *token;

	token = jstrsep(&tmp1, ",");
	bool ok = (strcmp(token, "abc") == 0 && tmp1 != NULL);
	token = jstrsep(&tmp1, ",");
	ok = ok && (strcmp(token, "def") == 0 && tmp1 != NULL);
	token = jstrsep(&tmp1, ",");
	ok = ok && (strcmp(token, "ghi") == 0 && tmp1 == NULL);

	if (ok) {
		TEST_PASS("jstrsep basic parsing works.");
	} else {
		TEST_FAIL("jstrsep basic parsing failed.");
	}

	/* 2. Multiple and consecutive delimiters */
	char input2[] = ",,a,,b,";
	char *tmp2 = input2;
	ok = (strcmp(jstrsep(&tmp2, ","), "") == 0);
	ok = ok && (strcmp(jstrsep(&tmp2, ","), "") == 0);
	ok = ok && (strcmp(jstrsep(&tmp2, ","), "a") == 0);
	ok = ok && (strcmp(jstrsep(&tmp2, ","), "") == 0);
	ok = ok && (strcmp(jstrsep(&tmp2, ","), "b") == 0);
	ok = ok && (strcmp(jstrsep(&tmp2, ","), "") == 0);
	ok = ok && (jstrsep(&tmp2, ",") == NULL);

	if (ok) {
		TEST_PASS("jstrsep consecutive and start/end delimiters work.");
	} else {
		TEST_FAIL("jstrsep consecutive delimiters failed.");
	}

	/* 3. No delimiter found */
	char input3[] = "no_delim";
	char *tmp3 = input3;
	token = jstrsep(&tmp3, ",");
	if (strcmp(token, "no_delim") == 0 && tmp3 == NULL) {
		TEST_PASS("jstrsep works when no delimiter found.");
	} else {
		TEST_FAIL("jstrsep failed when no delimiter found.");
	}
}
#endif

#if defined(__TEST_STRDUP)
#include "jmm.h"
void test_jstrdup()
{
	TEST_PRINT("jstrdup");
	/* 1. Basic jstrdup */
	const char *orig = "Duplicate me";
	char *dup = jstrdup(orig);
	if (dup != NULL && dup != orig && strcmp(dup, orig) == 0) {
		TEST_PASS("jstrdup correctly duplicated string.");
	} else {
		TEST_FAIL("jstrdup failed.");
	}
	jfree(dup);

	/* 2. jstrdup empty string */
	const char *orig_empty = "";
	char *dup_empty = jstrdup(orig_empty);
	if (dup_empty != NULL && dup_empty != orig_empty &&
	    dup_empty[0] == '\0') {
		TEST_PASS("jstrdup duplicated empty string.");
	} else {
		TEST_FAIL("jstrdup failed empty string.");
	}
	jfree(dup_empty);
}
#endif

#if defined(__TEST_STRNDUP)
#include "jmm.h"
void test_jstrndup()
{
	TEST_PRINT("jstrndup");
	/* 1. jstrndup basic truncation */
	const char *orig1 = "Hello World";
	char *ndup1 = jstrndup(orig1, 5);
	if (ndup1 != NULL && ndup1 != orig1 && memcmp(ndup1, "Hello", 5) == 0 &&
	    ndup1[5] == '\0') {
		TEST_PASS(
			"jstrndup basic truncation and null-termination work.");
	} else {
		TEST_FAIL(
			"jstrndup basic truncation or null-termination failed.");
	}
	jfree(ndup1);

	/* 2. jstrndup n > strlen(src) */
	const char *orig2 = "Hello";
	char *ndup2 = jstrndup(orig2, 10);
	if (ndup2 != NULL && ndup2 != orig2 && strcmp(ndup2, "Hello") == 0 &&
	    ndup2[5] == '\0') {
		TEST_PASS(
			"jstrndup n > strlen works and is correctly terminated.");
	} else {
		TEST_FAIL("jstrndup n > strlen or null-termination failed.");
	}
	jfree(ndup2);

	/* 3. jstrndup n = 0 */
	const char *orig3 = "Hello";
	char *ndup3 = jstrndup(orig3, 0);
	if (ndup3 != NULL && ndup3 != orig3 && ndup3[0] == '\0') {
		TEST_PASS("jstrndup with n=0 works.");
	} else {
		TEST_FAIL("jstrndup with n=0 failed.");
	}
	jfree(ndup3);

	/* 4. jstrndup with empty string */
	const char *orig4 = "";
	char *ndup4 = jstrndup(orig4, 5);
	if (ndup4 != NULL && ndup4 != orig4 && strcmp(ndup4, "") == 0 &&
	    ndup4[0] == '\0') {
		TEST_PASS("jstrndup with empty string works.");
	} else {
		TEST_FAIL("jstrndup with empty string failed.");
	}
	jfree(ndup4);
}
#endif

void run_jstring_tests()
{
	printf("=== JString Implementation Test Suite ===\n\n");
#if defined(__TEST_MEMCPY)
	test_jmemcpy();
#endif
#if defined(__TEST_MEMMOVE)
	test_jmemmove();
#endif
#if defined(__TEST_MEMSET)
	test_jmemset();
#endif
#if defined(__TEST_MEMCMP)
	test_jmemcmp();
#endif
#if defined(__TEST_STRLEN)
	test_jstrlen();
#endif
#if defined(__TEST_STRCPY)
	test_jstrcpy();
#endif
#if defined(__TEST_STPCPY)
	test_jstpcpy();
#endif
#if defined(__TEST_STRCAT)
	test_jstrcat();
#endif
#if defined(__TEST_STRNCPY)
	test_jstrncpy();
#endif
#if defined(__TEST_STPNCPY)
	test_jstpncpy();
#endif
#if defined(__TEST_STRCMP)
	test_jstrcmp();
#endif
#if defined(__TEST_STRNCMP)
	test_jstrncmp();
#endif
#if defined(__TEST_STRCHR)
	test_jstrchr();
#endif
#if defined(__TEST_STRRCHR)
	test_jstrrchr();
#endif
#if defined(__TEST_STRCHRNUL)
	test_jstrchrnul();
#endif
#if defined(__TEST_STRSEP)
	test_jstrsep();
#endif
#if defined(__TEST_STRDUP)
	test_jstrdup();
#endif
#if defined(__TEST_STRNDUP)
	test_jstrndup();
#endif
	printf("\n=== All String Tests Finished ===\n");
}
#endif
