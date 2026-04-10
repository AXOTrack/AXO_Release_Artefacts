#include <zephyr/ztest.h>
//#include <vector>
//#include <algorithm>

#define private public
#include "pm/reset_reason.h"

#include "stubs.h"

ZTEST_SUITE(tests_pm_reset_reason, NULL, NULL, NULL, NULL, NULL);

// test constructor and destructor
ZTEST(tests_pm_reset_reason, test_pm_reset_reason_constructor)
{
	ResetReason* pResetReason = new ResetReason();

	zassert_not_null(pResetReason, "memory alloc failed");

	delete pResetReason;
}

// MISSING for 100% branch coverage static instantiation does not deliver NULL
// here code line 
// static ResetReason myInstance;
// are not covered completly.



// test function get
ZTEST(tests_pm_reset_reason, test_pm_reset_reason_get)
{
	std::string r;
	unsigned int m;

	gu32Val = 0;
	gIntVal = 1;
	ResetReason::inst()->get(&m, &r);
	zassert(strcmp(r.c_str(), "POR")==0, "reset reason not POR");

	gu32Val = NRFX_RESET_REASON_RESETPIN_MASK;
	ResetReason::inst()->get(&m, &r);
	zassert(strcmp(r.c_str(), "PIN")==0, "reset reason not PIN");

	gu32Val = NRFX_RESET_REASON_DOG_MASK;
	ResetReason::inst()->get(&m, &r);
	zassert(strcmp(r.c_str(), "WDT")==0, "reset reason not WDT");

	gu32Val = NRFX_RESET_REASON_SREQ_MASK;
	ResetReason::inst()->get(&m, &r);
	zassert(strcmp(r.c_str(), "SWR")==0, "reset reason not SWR");

	gu32Val = NRFX_RESET_REASON_LOCKUP_MASK;
	ResetReason::inst()->get(&m, &r);
	zassert(strcmp(r.c_str(), "LCK")==0, "reset reason not LCK");

	gu32Val = NRFX_RESET_REASON_OFF_MASK;
	ResetReason::inst()->get(&m, &r);
	zassert(strcmp(r.c_str(), "OFF")==0, "reset reason not OFF");

	gu32Val = NRFX_RESET_REASON_DIF_MASK;
	ResetReason::inst()->get(&m, &r);
	zassert(strcmp(r.c_str(), "DBG")==0, "reset reason not DBG");

	gu32Val = 0x10000000;
	ResetReason::inst()->get(&m, &r);
	zassert(strcmp(r.c_str(), "???")==0, "reset reason not ???");

	gIntVal = 2;
	gu32Val = NRFX_RESET_REASON_RESETPIN_MASK | NRFX_RESET_REASON_DOG_MASK;
	ResetReason::inst()->get(&m, &r);
	zassert(strcmp(r.c_str(), "PINWDT")==0, "reset reason not PINWDT");
}

