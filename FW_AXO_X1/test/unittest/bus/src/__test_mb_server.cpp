#include <zephyr/ztest.h>

#define private public
#include "bus/mb/modbus_server.h"

#include "stubs.h"

ZTEST_SUITE(tests_bus_mb_server, NULL, NULL, NULL, NULL, NULL);

// test constructor and destructor
ZTEST(tests_bus_mb_server, test_bus_mb_server_constructor)
{
	I2cBus* pI2cBus = new I2cBus();

	zassert_not_null(pI2cBus, "memory alloc failed");

	delete pI2cBus;
}

/*/ test init
ZTEST(tests_bus_mb_server, test_bus_mb_server_init)
{
	I2cBus::inst()->init();
	zassert_true(I2cBus::inst()->init_done, "init_done != true");
}

// test init
ZTEST(tests_bus_mb_server, test_bus_mb_server_init2)
{
	I2cBus::inst()->init_done = true;
	I2cBus::inst()->init();
	zassert_true(I2cBus::inst()->init_done, "init_done != true");
}

// test init
ZTEST(tests_bus_mb_server, test_bus_mb_server_init3)
{
	I2cBus::inst()->init_done = false;
	gbRetVal = true;
	gbReady = true;
	I2cBus::inst()->init();
	zassert_true(I2cBus::inst()->init_done, "init_done != true");
}

// test init
ZTEST(tests_bus_mb_server, test_bus_mb_server_init4)
{
	I2cBus::inst()->init_done = false;
	gbRetVal = true;
	gbReady = false;
	I2cBus::inst()->init();
	zassert_true(I2cBus::inst()->init_done, "init_done != true");
}

// test init
ZTEST(tests_bus_mb_server, test_bus_mb_server_init5)
{
	I2cBus::inst()->init_done = false;
	gbRetVal = false;
	gbReady = true;
	I2cBus::inst()->init();
	zassert_true(I2cBus::inst()->init_done, "init_done != true");
}

// test init
ZTEST(tests_bus_mb_server, test_bus_mb_server_init6)
{
	I2cBus::inst()->init_done = false;
	gbRetVal = false;
	gbReady = false;
	I2cBus::inst()->init();
	zassert_true(I2cBus::inst()->init_done, "init_done != true");
}
*/

