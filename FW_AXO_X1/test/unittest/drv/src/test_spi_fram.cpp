#include <zephyr/ztest.h>

#define private public
#include "drv/spi/fram.h"

#include "stubs.h"

ZTEST_SUITE(tests_drv_spifram, NULL, NULL, NULL, NULL, NULL);

// test constructor and destructor
ZTEST(tests_drv_spifram, test_drv_spifram_constructor)
{
	spifram* pSpifram = new spifram();
	zassert_not_null(pSpifram, "memory alloc failed");
	zassert_false(pSpifram->dpd_active, "dpd_active != false");

	delete pSpifram;
}

// MISSING for 100% branch coverage static instantiation does not deliver NULL
// here code line 
// static spifram myInstance;
// are not covered completly.

// test function init
ZTEST(tests_drv_spifram, test_drv_spifram_init)
{
	spifram::inst()->init();
}

// test function init
ZTEST(tests_drv_spifram, test_drv_spifram_init2)
{
	spifram::inst()->id[0] = 0x04;
	spifram::inst()->id[1] = 0x7F;
	spifram::inst()->id[2] = 0x49;
	spifram::inst()->id[3] = 0x0B;

	gIntVal = 60;
	spifram::inst()->init();
}

// test function readDeviceId
ZTEST(tests_drv_spifram, test_drv_spifram_readDeviceId)
{
	gIntVal = 0;
	int iRet = spifram::inst()->readDeviceId();
	zassert_equal(iRet, 0, "iRet != 0");

	gIntVal = -17;
	iRet = spifram::inst()->readDeviceId();
	zassert_equal(iRet, -17, "iRet != -17");
}

// test function erase
ZTEST(tests_drv_spifram, test_drv_spifram_erase)
{
	spifram::inst()->erase();
}

// test function dump
ZTEST(tests_drv_spifram, test_drv_spifram_dump)
{
	spifram::inst()->dump(0,2);
}

// test function getDeviceId
ZTEST(tests_drv_spifram, test_drv_spifram_getDeviceId)
{
	uint8_t* pId;

	spifram::inst()->id[0] = 0x01;
	spifram::inst()->id[1] = 0x02;
	spifram::inst()->id[2] = 0x03;
	spifram::inst()->id[3] = 0x04;
	pId = spifram::inst()->getDeviceId();
	zassert_equal(pId[0], 0x01, "pId[0] != 0x01");
	zassert_equal(pId[1], 0x02, "pId[1] != 0x02");
	zassert_equal(pId[2], 0x03, "pId[2] != 0x03");
	zassert_equal(pId[3], 0x04, "pId[3] != 0x04");
}

// test function sleep
ZTEST(tests_drv_spifram, test_drv_spifram_sleep)
{
	int iRet = spifram::inst()->sleep();
	printk("%i\n", iRet);
	zassert_equal(iRet, 0, "iRet != 0");
}

// test function wakeup
ZTEST(tests_drv_spifram, test_drv_spifram_wakeup)
{
	int iRet = spifram::inst()->wakeup();
	printk("%i\n", iRet);
	zassert_equal(iRet, 0, "iRet != 0");
}

// test function writeBytes
ZTEST(tests_drv_spifram, test_drv_spifram_writeBytes)
{
	uint8_t buf[4] = {1,2,3,4};
	int iRet = spifram::inst()->writeBytes(0, buf, 4);
	printk("%i\n", iRet);
	zassert_equal(iRet, 0, "iRet != 0");
}

// test function readBytes
ZTEST(tests_drv_spifram, test_drv_spifram_readBytes)
{
	uint8_t buf[4];
	int iRet = spifram::inst()->readBytes(0, buf, 4);
	printk("%i\n", iRet);
	zassert_equal(iRet, 0, "iRet != 0");
}

// test function read_bytes
ZTEST(tests_drv_spifram, test_drv_spifram_read_bytes)
{
	gIntVal = 0;
	uint8_t buf[4];
	int iRet = spifram::inst()->read_bytes(NULL, NULL, 0, buf, 4);
	//printk("%i\n", iRet);
	zassert_equal(iRet, 0, "iRet != 0");
}

// test function write_bytes
ZTEST(tests_drv_spifram, test_drv_spifram_write_bytes)
{
	int iRet;
	uint8_t buf[4] = {1,2,3,4};
	
	gIntVal = 0;
	iRet = spifram::inst()->write_bytes(NULL, NULL, 0, buf, 4);
	zassert_equal(iRet, 0, "iRet != 0");

	
	gIntVal = 17;
	iRet = spifram::inst()->write_bytes(NULL, NULL, 0, buf, 4);
	zassert_equal(iRet, -5, "iRet != -5");

 	gIntVal = 27;
	iRet = spifram::inst()->write_bytes(NULL, NULL, 0, buf, 4);
	zassert_equal(iRet, -5, "iRet != -5");
}

// test function dpd_enter
ZTEST(tests_drv_spifram, test_drv_spifram_dpd_enter)
{
	gIntVal = 17;
	int iRet = spifram::inst()->dpd_enter(NULL, NULL);
	zassert_equal(iRet, -5, "iRet != -5");
}

// test function dpd_enter
ZTEST(tests_drv_spifram, test_drv_spifram_dpd_enter2)
{
	gIntVal = 0;
	int iRet = spifram::inst()->dpd_enter(NULL, NULL);
	zassert_equal(iRet, 0, "iRet != 0");
	zassert_true(spifram::inst()->dpd_active, "dpd_active != true");
}

// test function dpd_exit
ZTEST(tests_drv_spifram, test_drv_spifram_dpd_exit)
{
	int iRet;
	gIntVal = 17;
	struct gpio_dt_spec dt = {NULL, 1, 27};
	struct spi_cs_control cs = {dt, 2};
	struct spi_config conf = {0,0,0,&cs};

	iRet = spifram::inst()->dpd_exit(NULL, &conf);
	zassert_equal(iRet, -5, "iRet != -5");

	gIntVal = 27;
	iRet = spifram::inst()->dpd_exit(NULL, &conf);
	zassert_equal(iRet, -5, "iRet != -5");
}

// test function selftest
ZTEST(tests_drv_spifram, test_drv_spifram_selftest)
{
	spifram::inst()->id[0] = 0x04;
	spifram::inst()->id[1] = 0x7F;
	spifram::inst()->id[2] = 0x49;
	spifram::inst()->id[3] = 0x0B;

	gIntVal = 0;
	bool bRet = spifram::inst()->selftest();
	zassert_false(bRet, "bRet != false");
	//zassert_true(bRet, "bRet != true");
}

// test function selftest
ZTEST(tests_drv_spifram, test_drv_spifram_selftest2)
{
	spifram::inst()->id[0] = 0x00;
	spifram::inst()->id[1] = 0x7F;
	spifram::inst()->id[2] = 0x49;
	spifram::inst()->id[3] = 0x0B;

	gIntVal = 0;
	bool bRet = spifram::inst()->selftest();
	zassert_false(bRet, "bRet != false");
	//zassert_true(bRet, "bRet != true");
}

// test function selftest
ZTEST(tests_drv_spifram, test_drv_spifram_selftest3)
{
	spifram::inst()->id[0] = 0x04;
	spifram::inst()->id[1] = 0x00;
	spifram::inst()->id[2] = 0x49;
	spifram::inst()->id[3] = 0x0B;

	gIntVal = 0;
	bool bRet = spifram::inst()->selftest();
	zassert_false(bRet, "bRet != false");
	//zassert_true(bRet, "bRet != true");
}

// test function selftest
ZTEST(tests_drv_spifram, test_drv_spifram_selftest4)
{
	spifram::inst()->id[0] = 0x04;
	spifram::inst()->id[1] = 0x7F;
	spifram::inst()->id[2] = 0x00;
	spifram::inst()->id[3] = 0x0B;

	gIntVal = 0;
	bool bRet = spifram::inst()->selftest();
	zassert_false(bRet, "bRet != false");
	//zassert_true(bRet, "bRet != true");
}

// test function selftest
ZTEST(tests_drv_spifram, test_drv_spifram_selftest5)
{
	spifram::inst()->id[0] = 0x04;
	spifram::inst()->id[1] = 0x7F;
	spifram::inst()->id[2] = 0x49;
	spifram::inst()->id[3] = 0x00;

	gIntVal = 0;
	bool bRet = spifram::inst()->selftest();
	zassert_false(bRet, "bRet != false");
	//zassert_true(bRet, "bRet != true");
}

// test function selftest
ZTEST(tests_drv_spifram, test_drv_spifram_selftest6)
{
	spifram::inst()->id[0] = 0x00;
	spifram::inst()->id[1] = 0x00;
	spifram::inst()->id[2] = 0x00;
	spifram::inst()->id[3] = 0x00;

	gIntVal = 0;
	bool bRet = spifram::inst()->selftest();
	zassert_false(bRet, "bRet != false");
	//zassert_true(bRet, "bRet != true");
}

// test function selftest
ZTEST(tests_drv_spifram, test_drv_spifram_selftest10)
{	
	uint8_t buf[2];
	int iRet = -1;

	spifram::inst()->id[0] = 0x04;
	spifram::inst()->id[1] = 0x7F;
	spifram::inst()->id[2] = 0x49;
	spifram::inst()->id[3] = 0x0B;

	gIntVal = 57;
	bool bRet = spifram::inst()->selftest();
	zassert_false(bRet, "bRet != false");

	gIntVal = 58;
	bRet = spifram::inst()->selftest();
	zassert_false(bRet, "bRet != false");

	gIntVal = 59;
	bRet = spifram::inst()->selftest();
	zassert_false(bRet, "bRet != false");

	gIntVal = 60;
	bRet = spifram::inst()->selftest();
	zassert_true(bRet, "bRet != true");

	gIntVal = 61;
	bRet = spifram::inst()->selftest();
	zassert_false(bRet, "bRet != false");

	gIntVal = 62;
	bRet = spifram::inst()->selftest();
	zassert_false(bRet, "bRet != false");

	gIntVal = 60;
	iRet = spifram::inst()->readBytes(0x7FFFF, &buf[0], 2);
	zassert_true(iRet, "iRet != 0");

	gIntVal = 61;
	iRet = spifram::inst()->readBytes(0x7FFFF, &buf[0], 2);
	zassert_true(iRet, "iRet != 0");

	gIntVal = 62;
	iRet = spifram::inst()->readBytes(0x7FFFF, &buf[0], 2);
	zassert_true(iRet, "iRet != 0");
}


