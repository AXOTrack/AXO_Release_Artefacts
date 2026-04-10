#include <zephyr/ztest.h>

#define private public
#include "drv/pwm/pwm.h"

#include "stubs.h"

ZTEST_SUITE(tests_drv_Pwm, NULL, NULL, NULL, NULL, NULL);

// test constructor and destructor
ZTEST(tests_drv_Pwm, test_drv_Pwm_constructor)
{
	Pwm* pPwm = new Pwm();

	zassert_not_null(pPwm, "memory alloc failed");
	zassert_false(pPwm->polarity_inverted, "polarity_inverted != false");
	zassert_equal(pPwm->period, 50000, "period != 50000");
	zassert_equal(pPwm->pulse, 25000, "pulse != 25000");

	delete pPwm;
}

// MISSING for 100% branch coverage static instantiation does not deliver NULL
// here code line 
// static Pwm myInstance;
// are not covered completly.

// test function init
ZTEST(tests_drv_Pwm, test_drv_Pwm_init)
{
	Pwm* pPwm = new Pwm();
	zassert_not_null(pPwm, "memory alloc failed");

	gbReady  = true;
	pPwm->init_done = true;

	pPwm->init();
	zassert_true(pPwm->init_done, "init_done != true");

	delete pPwm;
}

// test function init
ZTEST(tests_drv_Pwm, test_drv_Pwm_init2)
{
	Pwm* pPwm = new Pwm();
	zassert_not_null(pPwm, "memory alloc failed");

	gbReady  = true;
	pPwm->init_done = false;

	pPwm->init();
	zassert_true(pPwm->init_done, "init_done != true");

	delete pPwm;
}

// test function init
ZTEST(tests_drv_Pwm, test_drv_Pwm_init3)
{
	Pwm* pPwm = new Pwm();
	zassert_not_null(pPwm, "memory alloc failed");

	gbReady  = false;
	pPwm->init_done = false;

	pPwm->init();
	zassert_true(pPwm->init_done, "init_done != true");

	delete pPwm;
}

// test function setPeriod, setPulseWidth, sync, start, stop
ZTEST(tests_drv_Pwm, test_drv_Pwm_setPeriod)
{
	Pwm::inst()->setPeriod(17);
	zassert_equal(Pwm::inst()->period, 17, "period != 17");

	Pwm::inst()->setPulseWidth(27);
	zassert_equal(Pwm::inst()->pulse, 27, "pulse != 27");

	gbRetVal = true;
	Pwm::inst()->sync();
	Pwm::inst()->start();
	Pwm::inst()->stop();
	
	gbRetVal = false;
	Pwm::inst()->sync();
	Pwm::inst()->start();
	Pwm::inst()->stop();
	
}



