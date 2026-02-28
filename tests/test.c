/* tests/test.c - Unified Test Dispatcher for nstdlib
Copyright (C) 2026  Emir Baha Yıldırım */

#include <stdio.h>
#include <stdbool.h>

/* Global failure flag */
bool g_test_failed = false;

/* Forward declarations of test runners */
#if defined(__JMM_DEBUG)
extern void run_jmm_tests(void);
#endif

#if defined(__JSTR_DEBUG)
extern void run_jstring_tests(void);
#endif

int main(void)
{
#if defined(__JMM_DEBUG)
	run_jmm_tests();
#endif

#if defined(__JSTR_DEBUG)
	run_jstring_tests();
#endif

	if (g_test_failed) {
		printf("\n*** Some tests FAILED! Check the output above. ***\n");
		return 1;
	} else {
		printf("\nAll tests PASSED successfully.\n");
		return 0;
	}
}
