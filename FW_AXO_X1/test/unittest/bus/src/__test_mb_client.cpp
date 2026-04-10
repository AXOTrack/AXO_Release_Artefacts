#include <zephyr/ztest.h>

#define private public
#include "bus/mb/modbus_client.h"

#include "stubs.h"

ZTEST_SUITE(tests_bus_mb_client, NULL, NULL, NULL, NULL, NULL);

// test constructor and destructor
ZTEST(tests_bus_mb_client, test_bus_mb_client_constructor)
{
	ModbusRtuClient* pModBus = new ModbusRtuClient();

	zassert_not_null(pModBus, "memory alloc failed");

	delete pModBus;
}

/*/ test init
ZTEST(tests_bus_mb_client, test_bus_mb_client_init)
{
	I2cBus::inst()->init();
	zassert_true(I2cBus::inst()->init_done, "init_done != true");
}

// test init
ZTEST(tests_bus_mb_client, test_bus_mb_client_init2)
{
	I2cBus::inst()->init_done = true;
	I2cBus::inst()->init();
	zassert_true(I2cBus::inst()->init_done, "init_done != true");
}

// test init
ZTEST(tests_bus_mb_client, test_bus_mb_client_init3)
{
	I2cBus::inst()->init_done = false;
	gbRetVal = true;
	gbReady = true;
	I2cBus::inst()->init();
	zassert_true(I2cBus::inst()->init_done, "init_done != true");
}

// test init
ZTEST(tests_bus_mb_client, test_bus_mb_client_init4)
{
	I2cBus::inst()->init_done = false;
	gbRetVal = true;
	gbReady = false;
	I2cBus::inst()->init();
	zassert_true(I2cBus::inst()->init_done, "init_done != true");
}

// test init
ZTEST(tests_bus_mb_client, test_bus_mb_client_init5)
{
	I2cBus::inst()->init_done = false;
	gbRetVal = false;
	gbReady = true;
	I2cBus::inst()->init();
	zassert_true(I2cBus::inst()->init_done, "init_done != true");
}

// test init
ZTEST(tests_bus_mb_client, test_bus_mb_client_init6)
{
	I2cBus::inst()->init_done = false;
	gbRetVal = false;
	gbReady = false;
	I2cBus::inst()->init();
	zassert_true(I2cBus::inst()->init_done, "init_done != true");
}
*/

