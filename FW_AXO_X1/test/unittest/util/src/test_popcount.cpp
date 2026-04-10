#include <zephyr/ztest.h>

#define private public
#include "util/popcount.h"

#include "stubs.h"

ZTEST_SUITE(tests_util_popcount, NULL, NULL, NULL, NULL, NULL);

// test constructor and destructor
ZTEST(tests_util_popcount, test_util_popcount)
{
    int iRet;
    
    iRet = popcount32(0);
    zassert_equal(iRet, 0, "iRet != 0");

    iRet = popcount32(2);
    zassert_equal(iRet, 1, "iRet != 1");

    iRet = popcount32(5);
    zassert_equal(iRet, 2, "iRet != 2");

    iRet = popcount32(0xF);
    zassert_equal(iRet, 4, "iRet != 4");

    iRet = popcount32(0xFF);
    zassert_equal(iRet, 8, "iRet != 8");
}
