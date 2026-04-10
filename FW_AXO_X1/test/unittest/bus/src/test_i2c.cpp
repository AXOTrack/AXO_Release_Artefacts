#include <zephyr/ztest.h>

#define private public
#include "bus/i2c/i2cbus.h"

#include "stubs.h"

ZTEST_SUITE(tests_bus_i2c, NULL, NULL, NULL, NULL, NULL);

// test constructor and destructor
ZTEST(tests_bus_i2c, test_bus_i2c_constructor)
{
	I2cBus* pI2cBus = new I2cBus();

	zassert_not_null(pI2cBus, "memory alloc failed");

	delete pI2cBus;
}

// MISSING for 100% branch coverage static instantiation does not deliver NULL
// here code line 
// static I2cBus myInstance;
// are not covered completly.


// test init
ZTEST(tests_bus_i2c, test_bus_i2c_init)
{
	I2cBus::inst()->init();
	zassert_true(I2cBus::inst()->init_done, "init_done != true");
}

// test init
ZTEST(tests_bus_i2c, test_bus_i2c_init2)
{
	I2cBus::inst()->init_done = true;
	I2cBus::inst()->init();
	zassert_true(I2cBus::inst()->init_done, "init_done != true");
}

// test init
ZTEST(tests_bus_i2c, test_bus_i2c_init3)
{
	I2cBus::inst()->init_done = false;
	gbRetVal = true;
	gbReady = true;
	I2cBus::inst()->init();
	zassert_true(I2cBus::inst()->init_done, "init_done != true");
}

// test init
ZTEST(tests_bus_i2c, test_bus_i2c_init4)
{
	I2cBus::inst()->init_done = false;
	gbRetVal = true;
	gbReady = false;
	I2cBus::inst()->init();
	zassert_true(I2cBus::inst()->init_done, "init_done != true");
}

// test init
ZTEST(tests_bus_i2c, test_bus_i2c_init5)
{
	I2cBus::inst()->init_done = false;
	gbRetVal = false;
	gbReady = true;
	I2cBus::inst()->init();
	zassert_true(I2cBus::inst()->init_done, "init_done != true");
}

// test init
ZTEST(tests_bus_i2c, test_bus_i2c_init6)
{
	I2cBus::inst()->init_done = false;
	gbRetVal = false;
	gbReady = false;
	I2cBus::inst()->init();
	zassert_true(I2cBus::inst()->init_done, "init_done != true");
}


