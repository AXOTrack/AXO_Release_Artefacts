#include <zephyr/ztest.h>

#define private public
#include "rtos/mbox.h"
#include "rtos/mpelem.h"
#include "rtos/mpool.h"
#include "rtos/mqueue.h"
#include "rtos/task.h"
#include "rtos/timer.h"

#include "drv/i2c/shtcx.h"

#include "stubs.h"

ZTEST_SUITE(tests_drv_Shtcx, NULL, NULL, NULL, NULL, NULL);

// test constructor and destructor
ZTEST(tests_drv_Shtcx, test_drv_Shtcx_constructor)
{
	Shtcx* pShtcx = new Shtcx("shctx");
	zassert_not_null(pShtcx, "memory alloc failed");
	zassert_not_null(pShtcx->pInMbox, "pInMbox = NULL");
	zassert_not_null(pShtcx->myTimer, "myTimer = NULL");
    zassert_equal(pShtcx->pInMbox->count(), 0, "pInMbox->count() != 0");
    zassert_equal(pShtcx->pInMbox, Mbox::getMboxById(Mbox::eShtcxTask),
                  "Error getting mailbox by id");

	delete pShtcx;
}

// MISSING for 100% branch coverage static instantiation does not deliver NULL
// here code lines
// static PoolElem e1 = {eTimerExpired, Mbox::MboxId::eShtcxTask, 's', 'h'};
// pInMbox = new Mbox(name);
// myTimer = new Timer(Shtcx::timer_cb, this, 5, 5);
// are not covered completly.

// test function initDevice
ZTEST(tests_drv_Shtcx, test_drv_Shtcx_initDevice)
{
	Shtcx* pShtcx = new Shtcx("shctx");
	zassert_not_null(pShtcx, "memory alloc failed");
	zassert_not_null(pShtcx->pInMbox, "pInMbox = NULL");
	zassert_not_null(pShtcx->myTimer, "myTimer = NULL");
    zassert_equal(pShtcx->pInMbox->count(), 0, "pInMbox->count() != 0");
    zassert_equal(pShtcx->pInMbox, Mbox::getMboxById(Mbox::eShtcxTask),
                  "Error getting mailbox by id");

	gbReady = true;
	pShtcx->initDevice();
	zassert_true(pShtcx->device_good, "device_good != true");

	delete pShtcx;
}

// test function initDevice
ZTEST(tests_drv_Shtcx, test_drv_Shtcx_initDevice2)
{
	Shtcx* pShtcx = new Shtcx("shctx");
	zassert_not_null(pShtcx, "memory alloc failed");
	zassert_not_null(pShtcx->pInMbox, "pInMbox = NULL");
	zassert_not_null(pShtcx->myTimer, "myTimer = NULL");
    zassert_equal(pShtcx->pInMbox->count(), 0, "pInMbox->count() != 0");
    zassert_equal(pShtcx->pInMbox, Mbox::getMboxById(Mbox::eShtcxTask),
                  "Error getting mailbox by id");

	gbReady = false;
	pShtcx->initDevice();
	zassert_false(pShtcx->device_good, "device_good != false");

	delete pShtcx;
}

// test function timer_cb and timer1_expired
ZTEST(tests_drv_Shtcx, test_drv_Shtcx_timer_cb)
{
    PoolElem* pElem;

	Shtcx* pShtcx = new Shtcx("shctx");
	zassert_not_null(pShtcx, "memory alloc failed");
	zassert_not_null(pShtcx->pInMbox, "pInMbox = NULL");
	zassert_not_null(pShtcx->myTimer, "myTimer = NULL");
    zassert_equal(pShtcx->pInMbox->count(), 0, "pInMbox->count() != 0");
    zassert_equal(pShtcx->pInMbox, Mbox::getMboxById(Mbox::eShtcxTask),
                  "Error getting mailbox by id");

    pShtcx->timer_cb(reinterpret_cast<k_timer*>(pShtcx));

    zassert_equal(pShtcx->pInMbox->count(), 1, "pInMbox->count() != 1");
    pElem = pShtcx->pInMbox->read();
    zassert_equal(pElem->mtype, msgType::eTimerExpired, "Rcvd mtype != msgType::eTimerExpired");

    delete pShtcx;
}    

// test function sample
ZTEST(tests_drv_Shtcx, test_drv_Shtcx_sample)
{
	Shtcx* pShtcx = new Shtcx("shctx");
	zassert_not_null(pShtcx, "memory alloc failed");
	zassert_not_null(pShtcx->pInMbox, "pInMbox = NULL");
	zassert_not_null(pShtcx->myTimer, "myTimer = NULL");
    zassert_equal(pShtcx->pInMbox->count(), 0, "pInMbox->count() != 0");
    zassert_equal(pShtcx->pInMbox, Mbox::getMboxById(Mbox::eShtcxTask),
                  "Error getting mailbox by id");

    gbReady = false;
	pShtcx->sample();

	delete pShtcx;
}

// test function dump
ZTEST(tests_drv_Shtcx, test_drv_Shtcx_dump)
{
	Shtcx* pShtcx = new Shtcx("shctx");
	zassert_not_null(pShtcx, "memory alloc failed");
	zassert_not_null(pShtcx->pInMbox, "pInMbox = NULL");
	zassert_not_null(pShtcx->myTimer, "myTimer = NULL");
    zassert_equal(pShtcx->pInMbox->count(), 0, "pInMbox->count() != 0");
    zassert_equal(pShtcx->pInMbox, Mbox::getMboxById(Mbox::eShtcxTask),
                  "Error getting mailbox by id");

	pShtcx->dump();

	delete pShtcx;
}

// test function readTemperature
ZTEST(tests_drv_Shtcx, test_drv_Shtcx_readTemperature)
{
	Shtcx* pShtcx = new Shtcx("shctx");
	zassert_not_null(pShtcx, "memory alloc failed");
	zassert_not_null(pShtcx->pInMbox, "pInMbox = NULL");
	zassert_not_null(pShtcx->myTimer, "myTimer = NULL");
    zassert_equal(pShtcx->pInMbox->count(), 0, "pInMbox->count() != 0");
    zassert_equal(pShtcx->pInMbox, Mbox::getMboxById(Mbox::eShtcxTask),
                  "Error getting mailbox by id");

	pShtcx->device_good = false;
	pShtcx->readTemperature();

	delete pShtcx;
}

// test function readHumidity
ZTEST(tests_drv_Shtcx, test_drv_Shtcx_readHumidity)
{
	Shtcx* pShtcx = new Shtcx("shctx");
	zassert_not_null(pShtcx, "memory alloc failed");
	zassert_not_null(pShtcx->pInMbox, "pInMbox = NULL");
	zassert_not_null(pShtcx->myTimer, "myTimer = NULL");
    zassert_equal(pShtcx->pInMbox->count(), 0, "pInMbox->count() != 0");
    zassert_equal(pShtcx->pInMbox, Mbox::getMboxById(Mbox::eShtcxTask),
                  "Error getting mailbox by id");

	pShtcx->device_good = false;
	pShtcx->readHumidity();

	delete pShtcx;
}

//Test receive msg eTimerExpired
ZTEST(tests_drv_Shtcx, test_drv_Shtcx_rcv_msg_eTimerExpired)
{
	Shtcx* pShtcx = new Shtcx("shctx");
	zassert_not_null(pShtcx, "memory alloc failed");
	zassert_not_null(pShtcx->pInMbox, "pInMbox = NULL");
	zassert_not_null(pShtcx->myTimer, "myTimer = NULL");
    zassert_equal(pShtcx->pInMbox->count(), 0, "pInMbox->count() != 0");
    zassert_equal(pShtcx->pInMbox, Mbox::getMboxById(Mbox::eShtcxTask),
                  "Error getting mailbox by id");

    gbReady = true;
	pShtcx->device_good = false;
    Mbox::sendMsg(msgType::eTimerExpired, Mbox::eShtcxTask, Mbox::eShtcxTask);

    zassert_equal(pShtcx->pInMbox->count(), 1, "pInMbox->count() != 1");

    pShtcx->task();  

    zassert_equal(pShtcx->pInMbox->count(), 0, "pInMbox->count() != 0");

	delete pShtcx; 
}

//Test receive msg eShtcxReport
ZTEST(tests_drv_Shtcx, test_drv_Shtcx_rcv_msg_eShtcxReport)
{
    PoolElem* pElem;

	Mbox* pDataMgrMbox = new Mbox("DataMgrMbox");
    Mbox::registerMbox(pDataMgrMbox, Mbox::eDataManager);
	zassert_not_null(pDataMgrMbox, "pDataMgrMbox = NULL");

	Shtcx* pShtcx = new Shtcx("shctx");
	zassert_not_null(pShtcx, "memory alloc failed");
	zassert_not_null(pShtcx->pInMbox, "pInMbox = NULL");
	zassert_not_null(pShtcx->myTimer, "myTimer = NULL");
    zassert_equal(pShtcx->pInMbox->count(), 0, "pInMbox->count() != 0");
    zassert_equal(pShtcx->pInMbox, Mbox::getMboxById(Mbox::eShtcxTask),
                  "Error getting mailbox by id");

    gbReady = true;
	pShtcx->device_good = true;
	Mbox::sendMsg(msgType::eShtcxReport, Mbox::eShtcxTask, Mbox::eShtcxTask);

    zassert_equal(pShtcx->pInMbox->count(), 1, "pInMbox->count() != 1");

    pShtcx->task();  

    zassert_equal(pShtcx->pInMbox->count(), 0, "pInMbox->count() != 0");

	pElem = pDataMgrMbox->read(); 
    zassert_equal(pElem->mtype, msgType::eShtcxTemperature, "Rcvd mtype != msgType::eShtcxTemperature");
    zassert_equal(pElem->from, Mbox::eShtcxTask, "Rcvd sender != Mbox::eShtcxTask");
	pElem->free();

	pElem = pDataMgrMbox->read(); 
    zassert_equal(pElem->mtype, msgType::eShtcxHumidity, "Rcvd mtype != msgType::eShtcxHumidity");
    zassert_equal(pElem->from, Mbox::eShtcxTask, "Rcvd sender != Mbox::eShtcxTask");

    pElem->free();
    delete pDataMgrMbox;
	delete pShtcx;
}

//Test receive unexpected msg 
ZTEST(tests_drv_Shtcx, test_drv_Shtcx_rcv_msg_unexpected)
{
	Shtcx* pShtcx = new Shtcx("shctx");
	zassert_not_null(pShtcx, "memory alloc failed");
	zassert_not_null(pShtcx->pInMbox, "pInMbox = NULL");
	zassert_not_null(pShtcx->myTimer, "myTimer = NULL");
    zassert_equal(pShtcx->pInMbox->count(), 0, "pInMbox->count() != 0");
    zassert_equal(pShtcx->pInMbox, Mbox::getMboxById(Mbox::eShtcxTask),
                  "Error getting mailbox by id");

    Mbox::sendMsg(msgType::eResetCause, Mbox::eShtcxTask, Mbox::eShtcxTask);

    zassert_equal(pShtcx->pInMbox->count(), 1, "pInMbox->count() != 1");

    pShtcx->task();  

    zassert_equal(pShtcx->pInMbox->count(), 0, "pInMbox->count() != 0");
	delete pShtcx; 
}

//Test receive no msg 
ZTEST(tests_drv_Shtcx, test_drv_Shtcx_rcv_msg_none)
{
	Shtcx* pShtcx = new Shtcx("shctx");
	zassert_not_null(pShtcx, "memory alloc failed");
	zassert_not_null(pShtcx->pInMbox, "pInMbox = NULL");
	zassert_not_null(pShtcx->myTimer, "myTimer = NULL");
    zassert_equal(pShtcx->pInMbox->count(), 0, "pInMbox->count() != 0");
    zassert_equal(pShtcx->pInMbox, Mbox::getMboxById(Mbox::eShtcxTask),
                  "Error getting mailbox by id");

    pShtcx->pInMbox->write(NULL);
    
    pShtcx->task();  

    zassert_equal(pShtcx->pInMbox->count(), 0, "pInMbox->count() != 0");
	delete pShtcx; 
}



