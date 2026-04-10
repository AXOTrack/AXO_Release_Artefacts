#include <zephyr/ztest.h>

extern "C" void qemu_exit(int status);
extern "C" void gcov_coverage_dump(void);

static void final_teardown(void* fixture) {
#if defined(CONFIG_COVERAGE)
    gcov_coverage_dump();
#endif
    qemu_exit(0);
}

ZTEST_SUITE(ztest_exit, NULL, /* predicate */
            NULL,             /* setup */
            NULL,             /* before */
            NULL,             /* after */
            final_teardown    /* teardown */
);
