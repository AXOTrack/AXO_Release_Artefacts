#include <zephyr/ztest.h>

#define private public
#include "../../../app-axo-x1/src/version.h"

#include "stubs.h"

extern int maintest();

ZTEST_SUITE(tests_app_axo_x1_main, NULL, NULL, NULL, NULL, NULL);

// test constructor and destructor
ZTEST(tests_app_axo_x1_main, test_app_axo_x1_main)
{
	maintest();
}

