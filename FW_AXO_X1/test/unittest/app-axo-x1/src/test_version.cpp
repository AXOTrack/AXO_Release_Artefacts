#include <zephyr/ztest.h>

//#define private public
#include "../../../app-axo-x1/src/version.h"

//#include "stubs.h"

ZTEST_SUITE(tests_app_axo_x1_version, NULL, NULL, NULL, NULL, NULL);

// test function getFwVersion
ZTEST(tests_app_axo_x1_version, test_app_axo_x1_version_getFwVersion)
{
	const char* pVer = getFwVersion();

	zassert(strcmp(pVer, "2.35")==0, 
                   "version %s not correct", pVer);
}

// test function displayFwVersion
ZTEST(tests_app_axo_x1_version, test_app_axo_x1_version_displayFwVersion)
{
	displayFwVersion();
}

