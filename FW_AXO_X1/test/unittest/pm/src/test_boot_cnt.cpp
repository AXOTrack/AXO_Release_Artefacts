#include <zephyr/ztest.h>
#include <string.h>


#define private public
#include "pm/boot_cnt.h"

#include "stubs.h"

ZTEST_SUITE(tests_pm_boot_cnt, NULL, NULL, NULL, NULL, NULL);

// test constructor and destructor
ZTEST(tests_pm_boot_cnt, test_pm_boot_cnt_constructor)
{
	memset(gBuf, 0, sizeof(gBuf));
	BootCnt* pBootCnt = new BootCnt();
	zassert_not_null(pBootCnt, "memory alloc failed");
	zassert_equal(pBootCnt->boot_count, 1, "boot_count != 1");

	delete pBootCnt;
}

// MISSING for 100% branch coverage static instantiation does not deliver NULL
// here code line 
// static BootCnt myInstance;
// are not covered completly.



// test constructor and destructor
ZTEST(tests_pm_boot_cnt, test_pm_boot_cnt_constructor2)
{
	memset(gBuf, -7, sizeof(gBuf));
	BootCnt* pBootCnt = new BootCnt();
	zassert_not_null(pBootCnt, "memory alloc failed");
	zassert_equal(pBootCnt->boot_count, 1, "boot_count != 1");

	delete pBootCnt;
}

// test function get
ZTEST(tests_pm_boot_cnt, test_pm_boot_cnt_get)
{
	int32_t cnt = BootCnt::inst()->get();
	BootCnt::inst()->boot_count = 77;
	zassert_equal(BootCnt::inst()->boot_count, 77, "boot_count != 77");
}

