#include <zephyr/ztest.h>

#define private public
#include "drv/spi/adxl37x.h"

#include "stubs.h"

ZTEST_SUITE(tests_drv_Adxl37x, NULL, NULL, NULL, NULL, NULL);

// test constructor and destructor
ZTEST(tests_drv_Adxl37x, test_drv_Adxl37x_constructor)
{
	gbReady  = true;
	gbRetVal = true;

	Adxl37x* pAdxl37x = new Adxl37x("adxl");
	zassert_not_null(pAdxl37x, "memory alloc failed");
	zassert_not_null(pAdxl37x->pInMbox, "pInMbox = NULL");
	zassert_not_null(pAdxl37x->myTimer, "myTimer = NULL");
    zassert_equal(pAdxl37x->pInMbox->count(), 0, "pInMbox->count() != 0");
    zassert_equal(pAdxl37x->pInMbox, Mbox::getMboxById(Mbox::eAdxl37xTask),
                  "Error getting mailbox by id");

    zassert_true(pAdxl37x->device_good, "device_good != true");

	delete pAdxl37x;
}

// test constructor and destructor
ZTEST(tests_drv_Adxl37x, test_drv_Adxl37x_constructor2)
{
	gbReady  = false;
	gbRetVal = false;

	Adxl37x* pAdxl37x = new Adxl37x("adxl");
	zassert_not_null(pAdxl37x, "memory alloc failed");
	zassert_not_null(pAdxl37x->pInMbox, "pInMbox = NULL");
	zassert_not_null(pAdxl37x->myTimer, "myTimer = NULL");
    zassert_equal(pAdxl37x->pInMbox->count(), 0, "pInMbox->count() != 0");
    zassert_equal(pAdxl37x->pInMbox, Mbox::getMboxById(Mbox::eAdxl37xTask),
                  "Error getting mailbox by id");

    zassert_false(pAdxl37x->device_good, "device_good != false");

	delete pAdxl37x;
}

// MISSING for 100% branch coverage static instantiation does not deliver NULL
// here code line 
// pInMbox = new Mbox(name);
// myTimer = new Timer(Adxl37x::timer_cb, this, suspendTimeout);
// static PoolElem e1 = {eTimerExpired, Mbox::MboxId::eAdxl37xTask, 'a', 'x'};
// static PoolElem e1 = {eAdxl37xFifo, Mbox::eAdxl37xTask, 't', 'h'};
// are not covered completly.

// test function timer_cb and timer1_expired
ZTEST(tests_drv_Adxl37x, test_drv_Adxl37x_timer_cb)
{
    PoolElem* pElem;

	Adxl37x* pAdxl37x = new Adxl37x("adxl");
	zassert_not_null(pAdxl37x, "memory alloc failed");
	zassert_not_null(pAdxl37x->pInMbox, "pInMbox = NULL");
	zassert_not_null(pAdxl37x->myTimer, "myTimer = NULL");
    zassert_equal(pAdxl37x->pInMbox->count(), 0, "pInMbox->count() != 0");
    zassert_equal(pAdxl37x->pInMbox, Mbox::getMboxById(Mbox::eAdxl37xTask),
                  "Error getting mailbox by id");

    pAdxl37x->timer_cb(reinterpret_cast<k_timer*>(pAdxl37x));

    zassert_equal(pAdxl37x->pInMbox->count(), 1, "pInMbox->count() != 1");
    pElem = pAdxl37x->pInMbox->read();
    zassert_equal(pElem->mtype, msgType::eTimerExpired, "Rcvd mtype != msgType::eTimerExpired");

    delete pAdxl37x;
}   

// test function convert
ZTEST(tests_drv_Adxl37x, test_drv_Adxl37x_convert)
{
	Adxl37x* pAdxl37x = new Adxl37x("adxl");
	zassert_not_null(pAdxl37x, "memory alloc failed");
	zassert_not_null(pAdxl37x->pInMbox, "pInMbox = NULL");
	zassert_not_null(pAdxl37x->myTimer, "myTimer = NULL");
    zassert_equal(pAdxl37x->pInMbox->count(), 0, "pInMbox->count() != 0");
    zassert_equal(pAdxl37x->pInMbox, Mbox::getMboxById(Mbox::eAdxl37xTask),
                  "Error getting mailbox by id");

	struct sensor_value v = {17, 17};
    pAdxl37x->convert(&v, 700);
	//printk("%i, %i\n", v.val1, v.val2);
	zassert_equal(v.val1, 42, "val1 != 42");
	zassert_equal(v.val2, 903700, "val2 != 903700");

    delete pAdxl37x;
}    

// test function print
ZTEST(tests_drv_Adxl37x, test_drv_Adxl37x_print)
{
	Adxl37x* pAdxl37x = new Adxl37x("adxl");
	zassert_not_null(pAdxl37x, "memory alloc failed");
	zassert_not_null(pAdxl37x->pInMbox, "pInMbox = NULL");
	zassert_not_null(pAdxl37x->myTimer, "myTimer = NULL");
    zassert_equal(pAdxl37x->pInMbox->count(), 0, "pInMbox->count() != 0");
    zassert_equal(pAdxl37x->pInMbox, Mbox::getMboxById(Mbox::eAdxl37xTask),
                  "Error getting mailbox by id");

    pAdxl37x->print();

    delete pAdxl37x;
}    

// test function getHwStatus
ZTEST(tests_drv_Adxl37x, test_drv_Adxl37x_getHwStatus)
{
	enum hw_stat state; 

	Adxl37x* pAdxl37x = new Adxl37x("adxl");
	zassert_not_null(pAdxl37x, "memory alloc failed");
	zassert_not_null(pAdxl37x->pInMbox, "pInMbox = NULL");
	zassert_not_null(pAdxl37x->myTimer, "myTimer = NULL");
    zassert_equal(pAdxl37x->pInMbox->count(), 0, "pInMbox->count() != 0");
    zassert_equal(pAdxl37x->pInMbox, Mbox::getMboxById(Mbox::eAdxl37xTask),
                  "Error getting mailbox by id");

	gIntVal = 0;
	pAdxl37x->bSelftestPassed = false;

    state = pAdxl37x->getHwStatus();
	zassert_equal(state, eHwSelftestFailed, "state != eHwSelftestFailed");

    delete pAdxl37x;
}    

// test function getHwStatus
ZTEST(tests_drv_Adxl37x, test_drv_Adxl37x_getHwStatus2)
{
	enum hw_stat state; 

	Adxl37x* pAdxl37x = new Adxl37x("adxl");
	zassert_not_null(pAdxl37x, "memory alloc failed");
	zassert_not_null(pAdxl37x->pInMbox, "pInMbox = NULL");
	zassert_not_null(pAdxl37x->myTimer, "myTimer = NULL");
    zassert_equal(pAdxl37x->pInMbox->count(), 0, "pInMbox->count() != 0");
    zassert_equal(pAdxl37x->pInMbox, Mbox::getMboxById(Mbox::eAdxl37xTask),
                  "Error getting mailbox by id");

	gIntVal = 17;
	pAdxl37x->bSelftestPassed = true;

    state = pAdxl37x->getHwStatus();
	zassert_equal(state, eHwFailure, "state != eHwFailure");

    delete pAdxl37x;
}    

// test function getHwStatus
ZTEST(tests_drv_Adxl37x, test_drv_Adxl37x_getHwStatus3)
{
	enum hw_stat state; 

	Adxl37x* pAdxl37x = new Adxl37x("adxl");
	zassert_not_null(pAdxl37x, "memory alloc failed");
	zassert_not_null(pAdxl37x->pInMbox, "pInMbox = NULL");
	zassert_not_null(pAdxl37x->myTimer, "myTimer = NULL");
    zassert_equal(pAdxl37x->pInMbox->count(), 0, "pInMbox->count() != 0");
    zassert_equal(pAdxl37x->pInMbox, Mbox::getMboxById(Mbox::eAdxl37xTask),
                  "Error getting mailbox by id");

	gbRetVal = 0;
	pAdxl37x->bSelftestPassed = true;
	pAdxl37x->adxl372Status.status1 = 0xFF;
	pAdxl37x->adxl372Status.status2 = 0xFF;
	pAdxl37x->adxl372Status.fifo_entries = 0xFFFF;

    state = pAdxl37x->getHwStatus();
	zassert_equal(state, eHwFailure, "state != eHwFailure");

    delete pAdxl37x;
}    

// test function getHwStatus
ZTEST(tests_drv_Adxl37x, test_drv_Adxl37x_getHwStatus4)
{
	enum hw_stat state; 

	Adxl37x* pAdxl37x = new Adxl37x("adxl");
	zassert_not_null(pAdxl37x, "memory alloc failed");
	zassert_not_null(pAdxl37x->pInMbox, "pInMbox = NULL");
	zassert_not_null(pAdxl37x->myTimer, "myTimer = NULL");
    zassert_equal(pAdxl37x->pInMbox->count(), 0, "pInMbox->count() != 0");
    zassert_equal(pAdxl37x->pInMbox, Mbox::getMboxById(Mbox::eAdxl37xTask),
                  "Error getting mailbox by id");

	gbRetVal = 0;
	pAdxl37x->bSelftestPassed = true;
	pAdxl37x->adxl372Status.status1 = 0;
	pAdxl37x->adxl372Status.status2 = 0;
	pAdxl37x->adxl372Status.fifo_entries = 2;
	pAdxl37x->device_good = false;

    state = pAdxl37x->getHwStatus();
	zassert_equal(state, eHwFailure, "state != eHwFailure");

    delete pAdxl37x;
}    

// test function getHwStatus
ZTEST(tests_drv_Adxl37x, test_drv_Adxl37x_getHwStatus5)
{
	enum hw_stat state; 

	Adxl37x* pAdxl37x = new Adxl37x("adxl");
	zassert_not_null(pAdxl37x, "memory alloc failed");
	zassert_not_null(pAdxl37x->pInMbox, "pInMbox = NULL");
	zassert_not_null(pAdxl37x->myTimer, "myTimer = NULL");
    zassert_equal(pAdxl37x->pInMbox->count(), 0, "pInMbox->count() != 0");
    zassert_equal(pAdxl37x->pInMbox, Mbox::getMboxById(Mbox::eAdxl37xTask),
                  "Error getting mailbox by id");

	gbRetVal = 0;
	pAdxl37x->bSelftestPassed = true;
	pAdxl37x->adxl372Status.status1 = 0;
	pAdxl37x->adxl372Status.status2 = 0;
	pAdxl37x->adxl372Status.fifo_entries = 2;
	pAdxl37x->device_good = true;

    state = pAdxl37x->getHwStatus();
	zassert_equal(state, eHwOK, "state != eHwFailure");

    delete pAdxl37x;
}    

// test function getHwStatus
ZTEST(tests_drv_Adxl37x, test_drv_Adxl37x_getHwStatus6)
{
	enum hw_stat state; 

	Adxl37x* pAdxl37x = new Adxl37x("adxl");
	zassert_not_null(pAdxl37x, "memory alloc failed");
	zassert_not_null(pAdxl37x->pInMbox, "pInMbox = NULL");
	zassert_not_null(pAdxl37x->myTimer, "myTimer = NULL");
    zassert_equal(pAdxl37x->pInMbox->count(), 0, "pInMbox->count() != 0");
    zassert_equal(pAdxl37x->pInMbox, Mbox::getMboxById(Mbox::eAdxl37xTask),
                  "Error getting mailbox by id");

	gbRetVal = 0;
	pAdxl37x->bSelftestPassed = true;
	pAdxl37x->adxl372Status.status1 = 0xFF;
	pAdxl37x->adxl372Status.status2 = 0;
	pAdxl37x->adxl372Status.fifo_entries = 2;
	pAdxl37x->device_good = false;

    state = pAdxl37x->getHwStatus();
	zassert_equal(state, eHwFailure, "state != eHwFailure");

    delete pAdxl37x;
}    

// test function getHwStatus
ZTEST(tests_drv_Adxl37x, test_drv_Adxl37x_getHwStatus7)
{
	enum hw_stat state; 

	Adxl37x* pAdxl37x = new Adxl37x("adxl");
	zassert_not_null(pAdxl37x, "memory alloc failed");
	zassert_not_null(pAdxl37x->pInMbox, "pInMbox = NULL");
	zassert_not_null(pAdxl37x->myTimer, "myTimer = NULL");
    zassert_equal(pAdxl37x->pInMbox->count(), 0, "pInMbox->count() != 0");
    zassert_equal(pAdxl37x->pInMbox, Mbox::getMboxById(Mbox::eAdxl37xTask),
                  "Error getting mailbox by id");

	gbRetVal = 0;
	pAdxl37x->bSelftestPassed = true;
	pAdxl37x->adxl372Status.status1 = 0xFF;
	pAdxl37x->adxl372Status.status2 = 0xFF;
	pAdxl37x->adxl372Status.fifo_entries = 2;
	pAdxl37x->device_good = false;

    state = pAdxl37x->getHwStatus();
	zassert_equal(state, eHwFailure, "state != eHwFailure");

    delete pAdxl37x;
}    

// test function getHwStatus
ZTEST(tests_drv_Adxl37x, test_drv_Adxl37x_getHwStatus8)
{
	enum hw_stat state; 

	Adxl37x* pAdxl37x = new Adxl37x("adxl");
	zassert_not_null(pAdxl37x, "memory alloc failed");
	zassert_not_null(pAdxl37x->pInMbox, "pInMbox = NULL");
	zassert_not_null(pAdxl37x->myTimer, "myTimer = NULL");
    zassert_equal(pAdxl37x->pInMbox->count(), 0, "pInMbox->count() != 0");
    zassert_equal(pAdxl37x->pInMbox, Mbox::getMboxById(Mbox::eAdxl37xTask),
                  "Error getting mailbox by id");

	gbRetVal = 0;
	pAdxl37x->bSelftestPassed = true;
	pAdxl37x->adxl372Status.status1 = 0xFF;
	pAdxl37x->adxl372Status.status2 = 0;
	pAdxl37x->adxl372Status.fifo_entries = 0xFFFF;
	pAdxl37x->device_good = false;

    state = pAdxl37x->getHwStatus();
	zassert_equal(state, eHwFailure, "state != eHwFailure");

    delete pAdxl37x;
}    

// test function setExtSync
ZTEST(tests_drv_Adxl37x, test_drv_Adxl37x_setExtSync)
{
	Adxl37x* pAdxl37x = new Adxl37x("adxl");
	zassert_not_null(pAdxl37x, "memory alloc failed");
	zassert_not_null(pAdxl37x->pInMbox, "pInMbox = NULL");
	zassert_not_null(pAdxl37x->myTimer, "myTimer = NULL");
    zassert_equal(pAdxl37x->pInMbox->count(), 0, "pInMbox->count() != 0");
    zassert_equal(pAdxl37x->pInMbox, Mbox::getMboxById(Mbox::eAdxl37xTask),
                  "Error getting mailbox by id");

    pAdxl37x->extSync = true;
	gbRetVal = true;

	pAdxl37x->setExtSync(true);
	zassert_true(pAdxl37x->extSync, "pAdxl37x->extSync != true");

    delete pAdxl37x;
}    

// test function setExtSync
ZTEST(tests_drv_Adxl37x, test_drv_Adxl37x_setExtSync2)
{
	Adxl37x* pAdxl37x = new Adxl37x("adxl");
	zassert_not_null(pAdxl37x, "memory alloc failed");
	zassert_not_null(pAdxl37x->pInMbox, "pInMbox = NULL");
	zassert_not_null(pAdxl37x->myTimer, "myTimer = NULL");
    zassert_equal(pAdxl37x->pInMbox->count(), 0, "pInMbox->count() != 0");
    zassert_equal(pAdxl37x->pInMbox, Mbox::getMboxById(Mbox::eAdxl37xTask),
                  "Error getting mailbox by id");

    pAdxl37x->extSync = false;
	gbRetVal = true;

	pAdxl37x->setExtSync(true);
	zassert_true(pAdxl37x->extSync, "pAdxl37x->extSync != true");

    delete pAdxl37x;
}    

// test function setExtSync
ZTEST(tests_drv_Adxl37x, test_drv_Adxl37x_setExtSync3)
{
	Adxl37x* pAdxl37x = new Adxl37x("adxl");
	zassert_not_null(pAdxl37x, "memory alloc failed");
	zassert_not_null(pAdxl37x->pInMbox, "pInMbox = NULL");
	zassert_not_null(pAdxl37x->myTimer, "myTimer = NULL");
    zassert_equal(pAdxl37x->pInMbox->count(), 0, "pInMbox->count() != 0");
    zassert_equal(pAdxl37x->pInMbox, Mbox::getMboxById(Mbox::eAdxl37xTask),
                  "Error getting mailbox by id");

    pAdxl37x->extSync = false;
	gbRetVal = false;

	pAdxl37x->setExtSync(true);
	zassert_true(pAdxl37x->extSync, "pAdxl37x->extSync != true");

    delete pAdxl37x;
}    

// test function setExtSync
ZTEST(tests_drv_Adxl37x, test_drv_Adxl37x_setExtSync4)
{
	Adxl37x* pAdxl37x = new Adxl37x("adxl");
	zassert_not_null(pAdxl37x, "memory alloc failed");
	zassert_not_null(pAdxl37x->pInMbox, "pInMbox = NULL");
	zassert_not_null(pAdxl37x->myTimer, "myTimer = NULL");
    zassert_equal(pAdxl37x->pInMbox->count(), 0, "pInMbox->count() != 0");
    zassert_equal(pAdxl37x->pInMbox, Mbox::getMboxById(Mbox::eAdxl37xTask),
                  "Error getting mailbox by id");

    pAdxl37x->extSync = true;
	gbRetVal = false;

	pAdxl37x->setExtSync(false);
	zassert_false(pAdxl37x->extSync, "pAdxl37x->extSync != false");

    delete pAdxl37x;
}    

// test function setExtSyncClk
ZTEST(tests_drv_Adxl37x, test_drv_Adxl37x_setExtSyncClk)
{
	Adxl37x* pAdxl37x = new Adxl37x("adxl");
	zassert_not_null(pAdxl37x, "memory alloc failed");
	zassert_not_null(pAdxl37x->pInMbox, "pInMbox = NULL");
	zassert_not_null(pAdxl37x->myTimer, "myTimer = NULL");
    zassert_equal(pAdxl37x->pInMbox->count(), 0, "pInMbox->count() != 0");
    zassert_equal(pAdxl37x->pInMbox, Mbox::getMboxById(Mbox::eAdxl37xTask),
                  "Error getting mailbox by id");

	pAdxl37x->setExtSyncClk(true);

    delete pAdxl37x;
}    

// test function setExtSyncClk
ZTEST(tests_drv_Adxl37x, test_drv_Adxl37x_setExtSyncClk2)
{
	Adxl37x* pAdxl37x = new Adxl37x("adxl");
	zassert_not_null(pAdxl37x, "memory alloc failed");
	zassert_not_null(pAdxl37x->pInMbox, "pInMbox = NULL");
	zassert_not_null(pAdxl37x->myTimer, "myTimer = NULL");
    zassert_equal(pAdxl37x->pInMbox->count(), 0, "pInMbox->count() != 0");
    zassert_equal(pAdxl37x->pInMbox, Mbox::getMboxById(Mbox::eAdxl37xTask),
                  "Error getting mailbox by id");

	pAdxl37x->setExtSyncClk(false);

    delete pAdxl37x;
}    

// test function dump
ZTEST(tests_drv_Adxl37x, test_drv_Adxl37x_dump)
{
	Adxl37x* pAdxl37x = new Adxl37x("adxl");
	zassert_not_null(pAdxl37x, "memory alloc failed");
	zassert_not_null(pAdxl37x->pInMbox, "pInMbox = NULL");
	zassert_not_null(pAdxl37x->myTimer, "myTimer = NULL");
    zassert_equal(pAdxl37x->pInMbox->count(), 0, "pInMbox->count() != 0");
    zassert_equal(pAdxl37x->pInMbox, Mbox::getMboxById(Mbox::eAdxl37xTask),
                  "Error getting mailbox by id");

    adxl372_xyz_accel_data d[10] = {{1,1,1},{2,2,2},{3,3,3},{4,4,4},{5,5,5},{6,6,6},{7,7,7},{8,8,8},{9,9,9},{0,0,0}};
	pAdxl37x->dump(d);

    delete pAdxl37x;
}    

// test function fifo_handler
ZTEST(tests_drv_Adxl37x, test_drv_Adxl37x_fifo_handler)
{
    PoolElem* pElem;

	Adxl37x* pAdxl37x = new Adxl37x("adxl");
	zassert_not_null(pAdxl37x, "memory alloc failed");
	zassert_not_null(pAdxl37x->pInMbox, "pInMbox = NULL");
	zassert_not_null(pAdxl37x->myTimer, "myTimer = NULL");
    zassert_equal(pAdxl37x->pInMbox->count(), 0, "pInMbox->count() != 0");
    zassert_equal(pAdxl37x->pInMbox, Mbox::getMboxById(Mbox::eAdxl37xTask),
                  "Error getting mailbox by id");

	gbRetVal = true;
    pAdxl37x->extSync = true;
	pAdxl37x->cur_buf = 0;

	pAdxl37x->fifo_handler(NULL, NULL);
	zassert_equal(pAdxl37x->sample_cnt, 2, "sample_cnt != 2");
	zassert_equal(pAdxl37x->cur_buf, 1, "sample_cnt != 1");
	zassert_true(pAdxl37x->extSync, "pAdxl37x->extSync != true");

	pElem = pAdxl37x->pInMbox->read(); 
    zassert_equal(pElem->mtype, msgType::eAdxl37xFifo, "Rcvd mtype != msgType::eAdxl37xFifo");
    zassert_equal(pElem->from, Mbox::eAdxl37xTask, "Rcvd sender != Mbox::eAdxl37xTask");

    delete pAdxl37x;
}    

// test function fifo_handler
ZTEST(tests_drv_Adxl37x, test_drv_Adxl37x_fifo_handler2)
{
    PoolElem* pElem;

	Adxl37x* pAdxl37x = new Adxl37x("adxl");
	zassert_not_null(pAdxl37x, "memory alloc failed");
	zassert_not_null(pAdxl37x->pInMbox, "pInMbox = NULL");
	zassert_not_null(pAdxl37x->myTimer, "myTimer = NULL");
    zassert_equal(pAdxl37x->pInMbox->count(), 0, "pInMbox->count() != 0");
    zassert_equal(pAdxl37x->pInMbox, Mbox::getMboxById(Mbox::eAdxl37xTask),
                  "Error getting mailbox by id");

	gbRetVal = true;
    pAdxl37x->extSync = false;
	pAdxl37x->cur_buf = 0;

	pAdxl37x->fifo_handler(NULL, NULL);
	zassert_equal(pAdxl37x->sample_cnt, 2, "sample_cnt != 2");
	zassert_equal(pAdxl37x->cur_buf, 1, "sample_cnt != 1");
	zassert_true(pAdxl37x->extSync, "pAdxl37x->extSync != true");

	pElem = pAdxl37x->pInMbox->read(); 
    zassert_equal(pElem->mtype, msgType::eAdxl37xFifo, "Rcvd mtype != msgType::eAdxl37xFifo");
    zassert_equal(pElem->from, Mbox::eAdxl37xTask, "Rcvd sender != Mbox::eAdxl37xTask");

    delete pAdxl37x;
}    

// test function configure
ZTEST(tests_drv_Adxl37x, test_drv_Adxl37x_configure)
{
	Adxl37x* pAdxl37x = new Adxl37x("adxl");
	zassert_not_null(pAdxl37x, "memory alloc failed");
	zassert_not_null(pAdxl37x->pInMbox, "pInMbox = NULL");
	zassert_not_null(pAdxl37x->myTimer, "myTimer = NULL");
    zassert_equal(pAdxl37x->pInMbox->count(), 0, "pInMbox->count() != 0");
    zassert_equal(pAdxl37x->pInMbox, Mbox::getMboxById(Mbox::eAdxl37xTask),
                  "Error getting mailbox by id");

    pAdxl37x->configure();

    delete pAdxl37x;
}    

//Test receive msg eTimerExpired
ZTEST(tests_drv_Adxl37x, test_drv_Adxl37x_rcv_msg_eTimerExpired)
{
	Adxl37x* pAdxl37x = new Adxl37x("adxl");
	zassert_not_null(pAdxl37x, "memory alloc failed");
	zassert_not_null(pAdxl37x->pInMbox, "pInMbox = NULL");
	zassert_not_null(pAdxl37x->myTimer, "myTimer = NULL");
    zassert_equal(pAdxl37x->pInMbox->count(), 0, "pInMbox->count() != 0");
    zassert_equal(pAdxl37x->pInMbox, Mbox::getMboxById(Mbox::eAdxl37xTask),
                  "Error getting mailbox by id");
    
    Mbox::sendMsg(msgType::eTimerExpired, Mbox::eAdxl37xTask, Mbox::eAdxl37xTask);

    zassert_equal(pAdxl37x->pInMbox->count(), 1, "pInMbox->count() != 1");

    pAdxl37x->task();  
    zassert_equal(pAdxl37x->pInMbox->count(), 0, "pInMbox->count() != 0");
    zassert_equal(gIntVal, (int)ADXL372_WAKE_UP, "op_mode != ADXL372_WAKE_UP");
    zassert_false(pAdxl37x->dirty[0], "dirty[0] != false");
    zassert_false(pAdxl37x->dirty[1], "dirty[1] != false");	

	delete pAdxl37x; 
}

//Test receive msg eAdxl37xFifo
ZTEST(tests_drv_Adxl37x, test_drv_Adxl37x_rcv_msg_eAdxl37xFifo)
{
    PoolElem* pElem;

	Mbox* pAdxlStoreMbox = new Mbox("AdxlStoreMbox");
    Mbox::registerMbox(pAdxlStoreMbox, Mbox::eAdxlStore);
	zassert_not_null(pAdxlStoreMbox, "pAdxlStoreMbox = NULL");

	Adxl37x* pAdxl37x = new Adxl37x("adxl");
	zassert_not_null(pAdxl37x, "memory alloc failed");
	zassert_not_null(pAdxl37x->pInMbox, "pInMbox = NULL");
	zassert_not_null(pAdxl37x->myTimer, "myTimer = NULL");
    zassert_equal(pAdxl37x->pInMbox->count(), 0, "pInMbox->count() != 0");
    zassert_equal(pAdxl37x->pInMbox, Mbox::getMboxById(Mbox::eAdxl37xTask),
                  "Error getting mailbox by id");
    
    pAdxl37x->dirty[0] = false;
    pAdxl37x->dirty[1] = false;
	pAdxl37x->cur_buf = 0;
	pAdxl37x->sample_cnt = 7;
	
	Mbox::sendMsg(msgType::eAdxl37xFifo, Mbox::eAdxl37xTask, Mbox::eAdxl37xTask);

    zassert_equal(pAdxl37x->pInMbox->count(), 1, "pInMbox->count() != 1");

    pAdxl37x->task(); 

	pElem = pAdxlStoreMbox->read(); 
    zassert_equal(pElem->mtype, msgType::eAdxl37xSensorData, "Rcvd mtype != msgType::eAdxl37xSensorData");
    zassert_equal(pElem->from, Mbox::eAdxl37xTask, "Rcvd sender != Mbox::eAdxl37xTask");
	//printk("%d, %d\n", (int)pAdxl37x->dirty[0], (int)pAdxl37x->dirty[1]);
	struct adxl37x_sensor_value val =
        *reinterpret_cast<struct adxl37x_sensor_value*>(pElem->getBuffer());
    zassert_equal(val.cnt, 7, "val.cnt != 7");
    zassert_true(pAdxl37x->dirty[0], "dirty[0] != true");
    zassert_false(pAdxl37x->dirty[1], "dirty[1] != false");	

    pElem->free();
    delete pAdxlStoreMbox;
	delete pAdxl37x; 
}

//Test receive msg eAdxl37xIdle
ZTEST(tests_drv_Adxl37x, test_drv_Adxl37x_rcv_msg_eAdxl37xIdle)
{
    PoolElem* pElem;

	Mbox* pAdxlStoreMbox = new Mbox("AdxlStoreMbox");
    Mbox::registerMbox(pAdxlStoreMbox, Mbox::eAdxlStore);
	zassert_not_null(pAdxlStoreMbox, "pAdxlStoreMbox = NULL");

	Adxl37x* pAdxl37x = new Adxl37x("adxl");
	zassert_not_null(pAdxl37x, "memory alloc failed");
	zassert_not_null(pAdxl37x->pInMbox, "pInMbox = NULL");
	zassert_not_null(pAdxl37x->myTimer, "myTimer = NULL");
    zassert_equal(pAdxl37x->pInMbox->count(), 0, "pInMbox->count() != 0");
    zassert_equal(pAdxl37x->pInMbox, Mbox::getMboxById(Mbox::eAdxl37xTask),
                  "Error getting mailbox by id");
    
	Mbox::sendMsg(msgType::eAdxl37xIdle, Mbox::eAdxl37xTask, Mbox::eAdxl37xTask);

    zassert_equal(pAdxl37x->pInMbox->count(), 1, "pInMbox->count() != 1");

    pAdxl37x->task(); 
    zassert_equal(gIntVal, (int)ADXL372_STANDBY, "op_mode != ADXL372_STANDBY");

	pElem = pAdxlStoreMbox->read(); 
    zassert_equal(pElem->mtype, msgType::eAdxl37xIdle, "Rcvd mtype != msgType::eAdxl37xIdle");
    zassert_equal(pElem->from, Mbox::eAdxl37xTask, "Rcvd sender != Mbox::eAdxl37xTask");

    pElem->free();
    delete pAdxlStoreMbox;
	delete pAdxl37x; 
}

//Test receive msg eAdxlStorageDone
ZTEST(tests_drv_Adxl37x, test_drv_Adxl37x_rcv_msg_eAdxlStorageDone)
{
	Adxl37x* pAdxl37x = new Adxl37x("adxl");
	zassert_not_null(pAdxl37x, "memory alloc failed");
	zassert_not_null(pAdxl37x->pInMbox, "pInMbox = NULL");
	zassert_not_null(pAdxl37x->myTimer, "myTimer = NULL");
    zassert_equal(pAdxl37x->pInMbox->count(), 0, "pInMbox->count() != 0");
    zassert_equal(pAdxl37x->pInMbox, Mbox::getMboxById(Mbox::eAdxl37xTask),
                  "Error getting mailbox by id");
    
    pAdxl37x->dirty[0] = true;
    pAdxl37x->dirty[1] = true;
	Mbox::sendMsg(msgType::eAdxlStorageDone, Mbox::eAdxl37xTask, Mbox::eAdxl37xTask);

    zassert_equal(pAdxl37x->pInMbox->count(), 1, "pInMbox->count() != 1");

    pAdxl37x->task(); 
	//printk("%d, %d\n", (int)pAdxl37x->dirty[0], (int)pAdxl37x->dirty[1]);
    zassert_false(pAdxl37x->dirty[0], "dirty[0] != false");
    zassert_true(pAdxl37x->dirty[1], "dirty[1] != true");	

	delete pAdxl37x; 
}

//Test receive msg eAdxlStorageFull (suspend)
ZTEST(tests_drv_Adxl37x, test_drv_Adxl37x_rcv_msg_eAdxlStorageFull)
{
	Adxl37x* pAdxl37x = new Adxl37x("adxl");
	zassert_not_null(pAdxl37x, "memory alloc failed");
	zassert_not_null(pAdxl37x->pInMbox, "pInMbox = NULL");
	zassert_not_null(pAdxl37x->myTimer, "myTimer = NULL");
    zassert_equal(pAdxl37x->pInMbox->count(), 0, "pInMbox->count() != 0");
    zassert_equal(pAdxl37x->pInMbox, Mbox::getMboxById(Mbox::eAdxl37xTask),
                  "Error getting mailbox by id");
    
	Mbox::sendMsg(msgType::eAdxlStorageFull, Mbox::eAdxl37xTask, Mbox::eAdxl37xTask);

    zassert_equal(pAdxl37x->pInMbox->count(), 1, "pInMbox->count() != 1");

    pAdxl37x->task(); 
    zassert_equal(gIntVal, (int)ADXL372_STANDBY, "op_mode != ADXL372_STANDBY");

	delete pAdxl37x; 
}

//Test receive msg eAdxlResumeMeas (resume)
ZTEST(tests_drv_Adxl37x, test_drv_Adxl37x_rcv_msg_eAdxlResumeMeas)
{
	Adxl37x* pAdxl37x = new Adxl37x("adxl");
	zassert_not_null(pAdxl37x, "memory alloc failed");
	zassert_not_null(pAdxl37x->pInMbox, "pInMbox = NULL");
	zassert_not_null(pAdxl37x->myTimer, "myTimer = NULL");
    zassert_equal(pAdxl37x->pInMbox->count(), 0, "pInMbox->count() != 0");
    zassert_equal(pAdxl37x->pInMbox, Mbox::getMboxById(Mbox::eAdxl37xTask),
                  "Error getting mailbox by id");
    
    pAdxl37x->dirty[0] = true;
    pAdxl37x->dirty[1] = true;
	Mbox::sendMsg(msgType::eAdxlResumeMeas, Mbox::eAdxl37xTask, Mbox::eAdxl37xTask);

    zassert_equal(pAdxl37x->pInMbox->count(), 1, "pInMbox->count() != 1");

    pAdxl37x->task(); 
    zassert_equal(gIntVal, (int)ADXL372_WAKE_UP, "op_mode != ADXL372_STANDBY");
    zassert_false(pAdxl37x->dirty[0], "dirty[0] != false");
    zassert_false(pAdxl37x->dirty[1], "dirty[1] != false");	

	delete pAdxl37x; 
}

//Test receive msg eFwUpdateActive (suspend)
ZTEST(tests_drv_Adxl37x, test_drv_Adxl37x_rcv_msg_eFwUpdateActive)
{
	Adxl37x* pAdxl37x = new Adxl37x("adxl");
	zassert_not_null(pAdxl37x, "memory alloc failed");
	zassert_not_null(pAdxl37x->pInMbox, "pInMbox = NULL");
	zassert_not_null(pAdxl37x->myTimer, "myTimer = NULL");
    zassert_equal(pAdxl37x->pInMbox->count(), 0, "pInMbox->count() != 0");
    zassert_equal(pAdxl37x->pInMbox, Mbox::getMboxById(Mbox::eAdxl37xTask),
                  "Error getting mailbox by id");
    
	Mbox::sendMsg(msgType::eFwUpdateActive, Mbox::eAdxl37xTask, Mbox::eAdxl37xTask);

    zassert_equal(pAdxl37x->pInMbox->count(), 1, "pInMbox->count() != 1");

    pAdxl37x->task(); 
    zassert_equal(gIntVal, (int)ADXL372_STANDBY, "op_mode != ADXL372_STANDBY");

	delete pAdxl37x; 
}

//Test receive msg eRawDataTransferActive
ZTEST(tests_drv_Adxl37x, test_drv_Adxl37x_rcv_msg_eRawDataTransferActive)
{
	Adxl37x* pAdxl37x = new Adxl37x("adxl");
	zassert_not_null(pAdxl37x, "memory alloc failed");
	zassert_not_null(pAdxl37x->pInMbox, "pInMbox = NULL");
	zassert_not_null(pAdxl37x->myTimer, "myTimer = NULL");
    zassert_equal(pAdxl37x->pInMbox->count(), 0, "pInMbox->count() != 0");
    zassert_equal(pAdxl37x->pInMbox, Mbox::getMboxById(Mbox::eAdxl37xTask),
                  "Error getting mailbox by id");
    
	Mbox::sendMsg(msgType::eRawDataTransferActive, Mbox::eAdxl37xTask, Mbox::eAdxl37xTask);

    zassert_equal(pAdxl37x->pInMbox->count(), 1, "pInMbox->count() != 1");

    pAdxl37x->task(); 
    zassert_equal(pAdxl37x->pInMbox->count(), 0, "pInMbox->count() != 0");

	delete pAdxl37x; 
}

//Test receive msg eRawDataTransferComplete
ZTEST(tests_drv_Adxl37x, test_drv_Adxl37x_rcv_msg_eRawDataTransferComplete)
{
	Adxl37x* pAdxl37x = new Adxl37x("adxl");
	zassert_not_null(pAdxl37x, "memory alloc failed");
	zassert_not_null(pAdxl37x->pInMbox, "pInMbox = NULL");
	zassert_not_null(pAdxl37x->myTimer, "myTimer = NULL");
    zassert_equal(pAdxl37x->pInMbox->count(), 0, "pInMbox->count() != 0");
    zassert_equal(pAdxl37x->pInMbox, Mbox::getMboxById(Mbox::eAdxl37xTask),
                  "Error getting mailbox by id");
    
	Mbox::sendMsg(msgType::eRawDataTransferComplete, Mbox::eAdxl37xTask, Mbox::eAdxl37xTask);

    zassert_equal(pAdxl37x->pInMbox->count(), 1, "pInMbox->count() != 1");

    pAdxl37x->task(); 
    zassert_equal(pAdxl37x->pInMbox->count(), 0, "pInMbox->count() != 0");

	delete pAdxl37x; 
}

//Test receive msg eDispDataTransferActive
ZTEST(tests_drv_Adxl37x, test_drv_Adxl37x_rcv_msg_eDispDataTransferActive)
{
	Adxl37x* pAdxl37x = new Adxl37x("adxl");
	zassert_not_null(pAdxl37x, "memory alloc failed");
	zassert_not_null(pAdxl37x->pInMbox, "pInMbox = NULL");
	zassert_not_null(pAdxl37x->myTimer, "myTimer = NULL");
    zassert_equal(pAdxl37x->pInMbox->count(), 0, "pInMbox->count() != 0");
    zassert_equal(pAdxl37x->pInMbox, Mbox::getMboxById(Mbox::eAdxl37xTask),
                  "Error getting mailbox by id");
    
	Mbox::sendMsg(msgType::eDispDataTransferActive, Mbox::eAdxl37xTask, Mbox::eAdxl37xTask);

    zassert_equal(pAdxl37x->pInMbox->count(), 1, "pInMbox->count() != 1");

    pAdxl37x->task(); 
    zassert_equal(pAdxl37x->pInMbox->count(), 0, "pInMbox->count() != 0");

	delete pAdxl37x; 
}

//Test receive msg eDispDataTransferComplete
ZTEST(tests_drv_Adxl37x, test_drv_Adxl37x_rcv_msg_eDispDataTransferComplete)
{
	Adxl37x* pAdxl37x = new Adxl37x("adxl");
	zassert_not_null(pAdxl37x, "memory alloc failed");
	zassert_not_null(pAdxl37x->pInMbox, "pInMbox = NULL");
	zassert_not_null(pAdxl37x->myTimer, "myTimer = NULL");
    zassert_equal(pAdxl37x->pInMbox->count(), 0, "pInMbox->count() != 0");
    zassert_equal(pAdxl37x->pInMbox, Mbox::getMboxById(Mbox::eAdxl37xTask),
                  "Error getting mailbox by id");
    
	Mbox::sendMsg(msgType::eDispDataTransferComplete, Mbox::eAdxl37xTask, Mbox::eAdxl37xTask);

    zassert_equal(pAdxl37x->pInMbox->count(), 1, "pInMbox->count() != 1");

    pAdxl37x->task(); 
    zassert_equal(pAdxl37x->pInMbox->count(), 0, "pInMbox->count() != 0");

	delete pAdxl37x; 
}

//Test receive msg eAdxlStorageDumpActive (suspend)
ZTEST(tests_drv_Adxl37x, test_drv_Adxl37x_rcv_msg_eAdxlStorageDumpActive)
{
	Adxl37x* pAdxl37x = new Adxl37x("adxl");
	zassert_not_null(pAdxl37x, "memory alloc failed");
	zassert_not_null(pAdxl37x->pInMbox, "pInMbox = NULL");
	zassert_not_null(pAdxl37x->myTimer, "myTimer = NULL");
    zassert_equal(pAdxl37x->pInMbox->count(), 0, "pInMbox->count() != 0");
    zassert_equal(pAdxl37x->pInMbox, Mbox::getMboxById(Mbox::eAdxl37xTask),
                  "Error getting mailbox by id");
    
	Mbox::sendMsg(msgType::eAdxlStorageDumpActive, Mbox::eAdxl37xTask, Mbox::eAdxl37xTask);

    zassert_equal(pAdxl37x->pInMbox->count(), 1, "pInMbox->count() != 1");

    pAdxl37x->task(); 
    zassert_equal(gIntVal, (int)ADXL372_STANDBY, "op_mode != ADXL372_STANDBY");

	delete pAdxl37x; 
}

//Test receive msg eAdxlStorageDumpComplete (resume)
ZTEST(tests_drv_Adxl37x, test_drv_Adxl37x_rcv_msg_eAdxlStorageDumpComplete)
{
	Adxl37x* pAdxl37x = new Adxl37x("adxl");
	zassert_not_null(pAdxl37x, "memory alloc failed");
	zassert_not_null(pAdxl37x->pInMbox, "pInMbox = NULL");
	zassert_not_null(pAdxl37x->myTimer, "myTimer = NULL");
    zassert_equal(pAdxl37x->pInMbox->count(), 0, "pInMbox->count() != 0");
    zassert_equal(pAdxl37x->pInMbox, Mbox::getMboxById(Mbox::eAdxl37xTask),
                  "Error getting mailbox by id");
    
    pAdxl37x->dirty[0] = true;
    pAdxl37x->dirty[1] = true;
	Mbox::sendMsg(msgType::eAdxlStorageDumpComplete, Mbox::eAdxl37xTask, Mbox::eAdxl37xTask);

    zassert_equal(pAdxl37x->pInMbox->count(), 1, "pInMbox->count() != 1");

    pAdxl37x->task(); 
    zassert_equal(gIntVal, (int)ADXL372_WAKE_UP, "op_mode != ADXL372_STANDBY");
    zassert_false(pAdxl37x->dirty[0], "dirty[0] != false");
    zassert_false(pAdxl37x->dirty[1], "dirty[1] != false");	

	delete pAdxl37x; 
}

//Test receive msg eDisableADXL (suspend)
ZTEST(tests_drv_Adxl37x, test_drv_Adxl37x_rcv_msg_eDisableADXL)
{
	Adxl37x* pAdxl37x = new Adxl37x("adxl");
	zassert_not_null(pAdxl37x, "memory alloc failed");
	zassert_not_null(pAdxl37x->pInMbox, "pInMbox = NULL");
	zassert_not_null(pAdxl37x->myTimer, "myTimer = NULL");
    zassert_equal(pAdxl37x->pInMbox->count(), 0, "pInMbox->count() != 0");
    zassert_equal(pAdxl37x->pInMbox, Mbox::getMboxById(Mbox::eAdxl37xTask),
                  "Error getting mailbox by id");
    
	Mbox::sendMsg(msgType::eDisableADXL, Mbox::eAdxl37xTask, Mbox::eAdxl37xTask);

    zassert_equal(pAdxl37x->pInMbox->count(), 1, "pInMbox->count() != 1");

    pAdxl37x->task(); 
    zassert_equal(gIntVal, (int)ADXL372_STANDBY, "op_mode != ADXL372_STANDBY");

	delete pAdxl37x; 
}

//Test receive msg eEnableADXL (resume)
ZTEST(tests_drv_Adxl37x, test_drv_Adxl37x_rcv_msg_eEnableADXL)
{
	Adxl37x* pAdxl37x = new Adxl37x("adxl");
	zassert_not_null(pAdxl37x, "memory alloc failed");
	zassert_not_null(pAdxl37x->pInMbox, "pInMbox = NULL");
	zassert_not_null(pAdxl37x->myTimer, "myTimer = NULL");
    zassert_equal(pAdxl37x->pInMbox->count(), 0, "pInMbox->count() != 0");
    zassert_equal(pAdxl37x->pInMbox, Mbox::getMboxById(Mbox::eAdxl37xTask),
                  "Error getting mailbox by id");
    
    pAdxl37x->dirty[0] = true;
    pAdxl37x->dirty[1] = true;
	Mbox::sendMsg(msgType::eEnableADXL, Mbox::eAdxl37xTask, Mbox::eAdxl37xTask);

    zassert_equal(pAdxl37x->pInMbox->count(), 1, "pInMbox->count() != 1");

    pAdxl37x->task(); 
    zassert_equal(gIntVal, (int)ADXL372_WAKE_UP, "op_mode != ADXL372_STANDBY");
    zassert_false(pAdxl37x->dirty[0], "dirty[0] != false");
    zassert_false(pAdxl37x->dirty[1], "dirty[1] != false");	

	delete pAdxl37x; 
}

//Test receive msg eHwReport
ZTEST(tests_drv_Adxl37x, test_drv_Adxl37x_rcv_msg_eHwReport)
{
    PoolElem* pElem;

	Mbox* pHwMetricsMbox = new Mbox("HwMetricsMbox");
    Mbox::registerMbox(pHwMetricsMbox, Mbox::eHwMetrics);
	zassert_not_null(pHwMetricsMbox, "pHwMetricsMbox = NULL");

	Adxl37x* pAdxl37x = new Adxl37x("adxl");
	zassert_not_null(pAdxl37x, "memory alloc failed");
	zassert_not_null(pAdxl37x->pInMbox, "pInMbox = NULL");
	zassert_not_null(pAdxl37x->myTimer, "myTimer = NULL");
    zassert_equal(pAdxl37x->pInMbox->count(), 0, "pInMbox->count() != 0");
    zassert_equal(pAdxl37x->pInMbox, Mbox::getMboxById(Mbox::eAdxl37xTask),
                  "Error getting mailbox by id");
    
	pAdxl37x->bSelftestPassed = true;
	Mbox::sendMsg(msgType::eHwReport, Mbox::eAdxl37xTask, Mbox::eAdxl37xTask);

    zassert_equal(pAdxl37x->pInMbox->count(), 1, "pInMbox->count() != 1");

    pAdxl37x->task(); 
    zassert_equal(gIntVal, (int)ADXL372_STANDBY, "op_mode != ADXL372_STANDBY");

	pElem = pHwMetricsMbox->read(); 
    zassert_equal(pElem->mtype, msgType::eHwStatusAdxl37x, "Rcvd mtype != msgType::eAdxl37xIdle");
    zassert_equal(pElem->from, Mbox::eAdxl37xTask, "Rcvd sender != Mbox::eAdxl37xTask");
	struct hw_metrics val =
        *reinterpret_cast<struct hw_metrics*>(pElem->getBuffer());
    zassert_equal(val.num_errors, 0, "num_errors != 0");

    pElem->free();
    delete pHwMetricsMbox;
	delete pAdxl37x; 
}

//Test receive msg eHwReport
ZTEST(tests_drv_Adxl37x, test_drv_Adxl37x_rcv_msg_eHwReport2)
{
    PoolElem* pElem;

	Mbox* pHwMetricsMbox = new Mbox("HwMetricsMbox");
    Mbox::registerMbox(pHwMetricsMbox, Mbox::eHwMetrics);
	zassert_not_null(pHwMetricsMbox, "pHwMetricsMbox = NULL");

	Adxl37x* pAdxl37x = new Adxl37x("adxl");
	zassert_not_null(pAdxl37x, "memory alloc failed");
	zassert_not_null(pAdxl37x->pInMbox, "pInMbox = NULL");
	zassert_not_null(pAdxl37x->myTimer, "myTimer = NULL");
    zassert_equal(pAdxl37x->pInMbox->count(), 0, "pInMbox->count() != 0");
    zassert_equal(pAdxl37x->pInMbox, Mbox::getMboxById(Mbox::eAdxl37xTask),
                  "Error getting mailbox by id");
    
	pAdxl37x->bSelftestPassed = false;
	Mbox::sendMsg(msgType::eHwReport, Mbox::eAdxl37xTask, Mbox::eAdxl37xTask);

    zassert_equal(pAdxl37x->pInMbox->count(), 1, "pInMbox->count() != 1");

    pAdxl37x->task(); 
    zassert_equal(gIntVal, (int)ADXL372_STANDBY, "op_mode != ADXL372_STANDBY");

	pElem = pHwMetricsMbox->read(); 
    zassert_equal(pElem->mtype, msgType::eHwStatusAdxl37x, "Rcvd mtype != msgType::eAdxl37xIdle");
    zassert_equal(pElem->from, Mbox::eAdxl37xTask, "Rcvd sender != Mbox::eAdxl37xTask");
	struct hw_metrics val =
        *reinterpret_cast<struct hw_metrics*>(pElem->getBuffer());
    zassert_equal(val.num_errors, 1, "num_errors != 1");

    pElem->free();
    delete pHwMetricsMbox;
	delete pAdxl37x; 
}

//Test receive unexpected msg 
ZTEST(tests_drv_Adxl37x, test_drv_Adxl37x_rcv_msg_unexpected)
{
	Adxl37x* pAdxl37x = new Adxl37x("adxl");
	zassert_not_null(pAdxl37x, "memory alloc failed");
	zassert_not_null(pAdxl37x->pInMbox, "pInMbox = NULL");
	zassert_not_null(pAdxl37x->myTimer, "myTimer = NULL");
    zassert_equal(pAdxl37x->pInMbox->count(), 0, "pInMbox->count() != 0");
    zassert_equal(pAdxl37x->pInMbox, Mbox::getMboxById(Mbox::eAdxl37xTask),
                  "Error getting mailbox by id");

    Mbox::sendMsg(msgType::eResetCause, Mbox::eAdxl37xTask, Mbox::eAdxl37xTask);

    zassert_equal(pAdxl37x->pInMbox->count(), 1, "pInMbox->count() != 1");

    pAdxl37x->task();  

    zassert_equal(pAdxl37x->pInMbox->count(), 0, "pInMbox->count() != 0");
	delete pAdxl37x; 
}

//Test receive no msg 
ZTEST(tests_drv_Adxl37x, test_drv_Adxl37x_rcv_msg_none)
{
	Adxl37x* pAdxl37x = new Adxl37x("adxl");
	zassert_not_null(pAdxl37x, "memory alloc failed");
	zassert_not_null(pAdxl37x->pInMbox, "pInMbox = NULL");
	zassert_not_null(pAdxl37x->myTimer, "myTimer = NULL");
    zassert_equal(pAdxl37x->pInMbox->count(), 0, "pInMbox->count() != 0");
    zassert_equal(pAdxl37x->pInMbox, Mbox::getMboxById(Mbox::eAdxl37xTask),
                  "Error getting mailbox by id");
    
    pAdxl37x->pInMbox->write(NULL);
    
    pAdxl37x->task();  

    zassert_equal(pAdxl37x->pInMbox->count(), 0, "pInMbox->count() != 0");
	delete pAdxl37x; 
}
