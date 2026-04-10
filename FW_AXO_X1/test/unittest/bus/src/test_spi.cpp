#include <zephyr/ztest.h>

#define private public
#include "bus/spi/spibus.h"

#include "stubs.h"

ZTEST_SUITE(tests_bus_spi, NULL, NULL, NULL, NULL, NULL);

// test constructor and destructor
ZTEST(tests_bus_spi, test_bus_spi_constructor)
{
	spibus* pSpibus = new spibus();

	zassert_not_null(pSpibus, "memory alloc failed");

	delete pSpibus;
}

// MISSING for 100% branch coverage static instantiation does not deliver NULL
// here code line 
// static spibus myInstance;
// are not covered completly.

// test init
ZTEST(tests_bus_spi, test_bus_spi_init)
{
	spibus::inst()->init();
	zassert_true(spibus::inst()->init_done, "init_done != true");
	zassert_equal(spibus::inst()->spi_cfg.frequency, 8000000U, "spi_cfg.frequency != 8000000U");
	zassert_equal(spibus::inst()->spi_cfg.operation, 8, "spi_cfg.operation != 8");
    //printk("%d\n\n", spibus::inst()->spi_cfg.operation);
}

// test init
ZTEST(tests_bus_spi, test_bus_spi_init2)
{
	spibus::inst()->init_done = true;
	spibus::inst()->init();
	zassert_true(spibus::inst()->init_done, "init_done != true");
}

// test init
ZTEST(tests_bus_spi, test_bus_spi_init3)
{
	spibus::inst()->init_done = false;
	gbReady = true;
	spibus::inst()->init();
	zassert_true(spibus::inst()->init_done, "init_done != true");
}

// test init
ZTEST(tests_bus_spi, test_bus_spi_init4)
{
	spibus::inst()->init_done = false;
	gbReady = false;
	spibus::inst()->init();
	zassert_true(spibus::inst()->init_done, "init_done != true");
}



