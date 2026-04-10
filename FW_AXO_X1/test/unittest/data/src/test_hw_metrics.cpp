#include <zephyr/ztest.h>
#include <zephyr/kernel.h>
#include <zephyr/sys/printk.h>
#include <vector>
#include <algorithm>
#include <memory>

#define private public
#include "rtos/mbox.h"
#include "rtos/mpelem.h"
#include "rtos/mpool.h"
#include "rtos/mqueue.h"
#include "rtos/task.h"

#include "data/hw_metrics.h"

// Stubs

ZTEST_SUITE(tests_hw_metrics, NULL, NULL, NULL, NULL, NULL);

// test constructor and destructor
ZTEST(tests_hw_metrics, test_hw_metrics_constructor)
{
	HwMetrics* pHwMetrics = new HwMetrics("hw_metrics");

	zassert_not_null(pHwMetrics, "memory alloc failed");
	zassert_not_null(pHwMetrics->pInMbox, "pInMbox = NULL");
    zassert_equal(pHwMetrics->pInMbox->count(), 0, "pInMbox->count() != 0");
    zassert_equal(pHwMetrics->pInMbox, Mbox::getMboxById(Mbox::eHwMetrics),
                  "Error getting mailbox by id");

	delete pHwMetrics; 
}

// MISSING for 100% branch coverage static instantiation does not deliver NULL
// here code line 
// pInMbox = new Mbox(name); 
// are not covered completly.


//Test receive msg eHwReport
ZTEST(tests_hw_metrics, test_hw_metrics_rcv_msg_eHwReport)
{
    PoolElem* pElem;

	Mbox* pAdxl37xMbox = new Mbox("Adxl37xMbox");
    Mbox::registerMbox(pAdxl37xMbox, Mbox::eAdxl37xTask);
	zassert_not_null(pAdxl37xMbox, "pAdxl37xMbox = NULL");

    HwMetrics* pHwMetrics = new HwMetrics("hw_metrics");
	zassert_not_null(pHwMetrics, "memory alloc failed");
	zassert_not_null(pHwMetrics->pInMbox, "pInMbox = NULL");
    zassert_equal(pHwMetrics->pInMbox->count(), 0, "pInMbox->count() != 0");
    zassert_equal(pHwMetrics->pInMbox, Mbox::getMboxById(Mbox::eHwMetrics),
                  "Error getting mailbox by id");

    Mbox::sendMsg(msgType::eHwReport, Mbox::eHwMetrics, Mbox::eHwMetrics);

    zassert_equal(pHwMetrics->pInMbox->count(), 1, "pInMbox->count() != 1");

    pHwMetrics->task();  

    zassert_equal(pHwMetrics->pInMbox->count(), 0, "pInMbox->count() != 0");
    zassert_equal(pAdxl37xMbox->count(), 1, "pAdxl37xMbox->count() != 1");

    pElem = pAdxl37xMbox->read();
    zassert_equal(pElem->mtype, msgType::eHwReport, "Rcvd mtype != msgType::eHwReport");
    zassert_equal(pElem->from, Mbox::eHwMetrics, "Rcvd sender != Mbox::eHwMetrics");

    delete pElem;
    delete pAdxl37xMbox;
	delete pHwMetrics; 
}

//Test receive msg eHwStatusAdxl37x with eHwOK
ZTEST(tests_hw_metrics, test_hw_metrics_rcv_msg_eHwStatusAdxl37x)
{
    PoolElem* pElem;
    struct hw_metrics hwM = {77, eHwOK};

	Mbox* pDataMgrMbox = new Mbox("DataMgrMbox");
    Mbox::registerMbox(pDataMgrMbox, Mbox::eDataManager);
	zassert_not_null(pDataMgrMbox, "pDataMgrMbox = NULL");

    HwMetrics* pHwMetrics = new HwMetrics("hw_metrics");
	zassert_not_null(pHwMetrics, "memory alloc failed");
	zassert_not_null(pHwMetrics->pInMbox, "pInMbox = NULL");
    zassert_equal(pHwMetrics->pInMbox->count(), 0, "pInMbox->count() != 0");
    zassert_equal(pHwMetrics->pInMbox, Mbox::getMboxById(Mbox::eHwMetrics),
                  "Error getting mailbox by id");

    Mbox::sendMsg(msgType::eHwStatusAdxl37x, Mbox::eHwMetrics, Mbox::eHwMetrics, (uint8_t*)&hwM, sizeof(hwM));

    zassert_equal(pHwMetrics->pInMbox->count(), 1, "pInMbox->count() != 1");

    pHwMetrics->task();  

    zassert_equal(pHwMetrics->pInMbox->count(), 0, "pInMbox->count() != 0");
    zassert_equal(pDataMgrMbox->count(), 1, "pAdxl37xMbox->count() != 1");

    pElem = pDataMgrMbox->read();
    zassert_equal(pElem->mtype, msgType::eHwMetrics, "Rcvd mtype != msgType::eHwMetrics");
    zassert_equal(pElem->from, Mbox::eHwMetrics, "Rcvd sender != Mbox::eHwMetrics");

    struct hw_metrics* hw_m = (struct hw_metrics*)pElem->getBuffer();
    zassert_equal(hw_m->num_errors, 77, "num_errors != 77");
    zassert_equal(hw_m->hw_status, eHwOK, "hw_status != eHwOK");

    pElem->free();
    delete pDataMgrMbox;
	delete pHwMetrics; 
}

//Test receive msg eHwStatusAdxl37x with eHwFailure
ZTEST(tests_hw_metrics, test_hw_metrics_rcv_msg_eHwStatusAdxl37x_2)
{
    PoolElem* pElem;
    struct hw_metrics hwM = {0, eHwFailure};

	Mbox* pDataMgrMbox = new Mbox("DataMgrMbox");
    Mbox::registerMbox(pDataMgrMbox, Mbox::eDataManager);
	zassert_not_null(pDataMgrMbox, "pDataMgrMbox = NULL");

    HwMetrics* pHwMetrics = new HwMetrics("hw_metrics");
	zassert_not_null(pHwMetrics, "memory alloc failed");
	zassert_not_null(pHwMetrics->pInMbox, "pInMbox = NULL");
    zassert_equal(pHwMetrics->pInMbox->count(), 0, "pInMbox->count() != 0");
    zassert_equal(pHwMetrics->pInMbox, Mbox::getMboxById(Mbox::eHwMetrics),
                  "Error getting mailbox by id");

    Mbox::sendMsg(msgType::eHwStatusAdxl37x, Mbox::eHwMetrics, Mbox::eHwMetrics, (uint8_t*)&hwM, sizeof(hwM));

    zassert_equal(pHwMetrics->pInMbox->count(), 1, "pInMbox->count() != 1");

    pHwMetrics->task();  

    zassert_equal(pHwMetrics->pInMbox->count(), 0, "pInMbox->count() != 0");
    zassert_equal(pDataMgrMbox->count(), 1, "pAdxl37xMbox->count() != 1");

    pElem = pDataMgrMbox->read();
    zassert_equal(pElem->mtype, msgType::eHwMetrics, "Rcvd mtype != msgType::eHwMetrics");
    zassert_equal(pElem->from, Mbox::eHwMetrics, "Rcvd sender != Mbox::eHwMetrics");

    struct hw_metrics* hw_m = (struct hw_metrics*)pElem->getBuffer();
    zassert_equal(hw_m->num_errors, 1, "num_errors != 1");
    zassert_equal(hw_m->hw_status, eHwFailure, "hw_status != eHwFailure");

    pElem->free();
    delete pDataMgrMbox;
	delete pHwMetrics; 
}

//Test receive msg eHwStatusAdxl37x with eHwSelftestFailed
ZTEST(tests_hw_metrics, test_hw_metrics_rcv_msg_eHwStatusAdxl37x_3)
{
    PoolElem* pElem;
    struct hw_metrics hwM = {2, eHwSelftestFailed};

	Mbox* pDataMgrMbox = new Mbox("DataMgrMbox");
    Mbox::registerMbox(pDataMgrMbox, Mbox::eDataManager);
	zassert_not_null(pDataMgrMbox, "pDataMgrMbox = NULL");

    HwMetrics* pHwMetrics = new HwMetrics("hw_metrics");
	zassert_not_null(pHwMetrics, "memory alloc failed");
	zassert_not_null(pHwMetrics->pInMbox, "pInMbox = NULL");
    zassert_equal(pHwMetrics->pInMbox->count(), 0, "pInMbox->count() != 0");
    zassert_equal(pHwMetrics->pInMbox, Mbox::getMboxById(Mbox::eHwMetrics),
                  "Error getting mailbox by id");

    Mbox::sendMsg(msgType::eHwStatusAdxl37x, Mbox::eHwMetrics, Mbox::eHwMetrics, (uint8_t*)&hwM, sizeof(hwM));

    zassert_equal(pHwMetrics->pInMbox->count(), 1, "pInMbox->count() != 1");

    pHwMetrics->task();  

    zassert_equal(pHwMetrics->pInMbox->count(), 0, "pInMbox->count() != 0");
    zassert_equal(pDataMgrMbox->count(), 1, "pAdxl37xMbox->count() != 1");

    pElem = pDataMgrMbox->read();
    zassert_equal(pElem->mtype, msgType::eHwMetrics, "Rcvd mtype != msgType::eHwMetrics");
    zassert_equal(pElem->from, Mbox::eHwMetrics, "Rcvd sender != Mbox::eHwMetrics");

    struct hw_metrics* hw_m = (struct hw_metrics*)pElem->getBuffer();
    zassert_equal(hw_m->num_errors, 2, "num_errors != 2");
    zassert_equal(hw_m->hw_status, eHwSelftestFailed, "hw_status != eHwSelftestFailed");

    pElem->free();
    delete pDataMgrMbox;
	delete pHwMetrics; 
}

//Test receive unexpected msg 
ZTEST(tests_hw_metrics, test_hw_metrics_rcv_msg_eHwStatusAdxl37x_4)
{
    HwMetrics* pHwMetrics = new HwMetrics("hw_metrics");
	zassert_not_null(pHwMetrics, "memory alloc failed");
	zassert_not_null(pHwMetrics->pInMbox, "pInMbox = NULL");
    zassert_equal(pHwMetrics->pInMbox->count(), 0, "pInMbox->count() != 0");
    zassert_equal(pHwMetrics->pInMbox, Mbox::getMboxById(Mbox::eHwMetrics),
                  "Error getting mailbox by id");

    Mbox::sendMsg(msgType::eResetCause, Mbox::eHwMetrics, Mbox::eHwMetrics);

    zassert_equal(pHwMetrics->pInMbox->count(), 1, "pInMbox->count() != 1");

    pHwMetrics->task();  

    zassert_equal(pHwMetrics->pInMbox->count(), 0, "pInMbox->count() != 0");
	delete pHwMetrics; 
}

//Test receive no msg 
ZTEST(tests_hw_metrics, test_hw_metrics_rcv_msg_eHwStatusAdxl37x_5)
{
    HwMetrics* pHwMetrics = new HwMetrics("hw_metrics");
	zassert_not_null(pHwMetrics, "memory alloc failed");
	zassert_not_null(pHwMetrics->pInMbox, "pInMbox = NULL");
    zassert_equal(pHwMetrics->pInMbox->count(), 0, "pInMbox->count() != 0");
    zassert_equal(pHwMetrics->pInMbox, Mbox::getMboxById(Mbox::eHwMetrics),
                  "Error getting mailbox by id");

    pHwMetrics->pInMbox->write(NULL);
    
    pHwMetrics->task();  

    zassert_equal(pHwMetrics->pInMbox->count(), 0, "pInMbox->count() != 0");
	delete pHwMetrics; 
}
