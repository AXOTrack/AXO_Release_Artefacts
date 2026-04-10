#include <zephyr/ztest.h>
//#include <vector>
//#include <algorithm>

#define private public
#include "disp/disptask.h"
#include "data/adxl_store.h"
#include "rtos/mbox.h"

#include "stubs.h"

ZTEST_SUITE(tests_disp_disptask, NULL, NULL, NULL, NULL, NULL);

// test constructor and destructor
ZTEST(tests_disp_disptask, test_disp_disptask_constructor)
{
	DispTask* pDisp = new DispTask("disp", true);
	zassert_not_null(pDisp, "memory alloc failed");
	zassert_not_null(pDisp->pInMbox, "pInMbox = NULL");   
    zassert_equal(pDisp->pInMbox->count(), 0, "pInMbox->count() != 0");
    zassert_equal(pDisp->pInMbox, Mbox::getMboxById(Mbox::eDispTask),
                  "Error getting mailbox by id");
    zassert_equal(pDisp->bIdle, true, "idle != true");

	delete pDisp;
}

// MISSING for 100% branch coverage static instantiation does not deliver NULL
// here code line 
// pInMbox = new Mbox(name); 
// are not covered completly.

// test function configure
ZTEST(tests_disp_disptask, test_disp_disptask_configure)
{
	DispTask* pDisp = new DispTask("disp", true);
	zassert_not_null(pDisp, "memory alloc failed");
	zassert_not_null(pDisp->pInMbox, "pInMbox = NULL");   
    zassert_equal(pDisp->pInMbox->count(), 0, "pInMbox->count() != 0");
    zassert_equal(pDisp->pInMbox, Mbox::getMboxById(Mbox::eDispTask),
                  "Error getting mailbox by id");

	gIntVal = 0;
    pDisp->configure();
    printk("%f\n\n", pDisp->b1);
    zassert_equal(pDisp->b1, 0.7, "b1 incorrect");

	gIntVal = 2;
    errno = ERANGE;
    pDisp->configure();
    printk("%f\n\n", pDisp->b1);
    zassert_equal(pDisp->b1, 1.0, "b1 incorrect");

	delete pDisp;
}

// test function detrendCalcSums
ZTEST(tests_disp_disptask, test_disp_disptask_detrendCalcSums)
{
	DispTask* pDisp = new DispTask("disp", true);
	zassert_not_null(pDisp, "memory alloc failed");
	zassert_not_null(pDisp->pInMbox, "pInMbox = NULL");   
    zassert_equal(pDisp->pInMbox->count(), 0, "pInMbox->count() != 0");
    zassert_equal(pDisp->pInMbox, Mbox::getMboxById(Mbox::eDispTask),
                  "Error getting mailbox by id");

    stDetrendVar d = {0,0,0,0,0,0};				  

	pDisp->detrendCalcSums(2, 7.0, d);
    zassert_equal(d.sumX, 2.0, "sumX incorrect");
    zassert_equal(d.sumY, 7.0, "sumY incorrect");
    zassert_equal(d.sumXY, 14.0, "sumXY incorrect");
    zassert_equal(d.sumX2, 4.0, "sumX2 incorrect");

	delete pDisp;
}

// test function detrendCalcSlopeIntercept
ZTEST(tests_disp_disptask, test_disp_disptask_detrendCalcSlopeIntercept)
{
	DispTask* pDisp = new DispTask("disp", true);
	zassert_not_null(pDisp, "memory alloc failed");
	zassert_not_null(pDisp->pInMbox, "pInMbox = NULL");   
    zassert_equal(pDisp->pInMbox->count(), 0, "pInMbox->count() != 0");
    zassert_equal(pDisp->pInMbox, Mbox::getMboxById(Mbox::eDispTask),
                  "Error getting mailbox by id");

    stDetrendVar d = {1.0,2.0,2.0,1.0,0,0};				  

	pDisp->detrendCalcSlopeIntercept(10, d);
    zassert_equal(d.slope, 2.0, "slope incorrect");
    zassert_equal(d.intercept, 0.0, "intercept incorrect");

	delete pDisp;
}

// test function detrendRemoveTrend
ZTEST(tests_disp_disptask, test_disp_disptask_detrendRemoveTrend)
{
	DispTask* pDisp = new DispTask("disp", true);
	zassert_not_null(pDisp, "memory alloc failed");
	zassert_not_null(pDisp->pInMbox, "pInMbox = NULL");   
    zassert_equal(pDisp->pInMbox->count(), 0, "pInMbox->count() != 0");
    zassert_equal(pDisp->pInMbox, Mbox::getMboxById(Mbox::eDispTask),
                  "Error getting mailbox by id");

    stDetrendVar d = {1.0,2.0,2.0,1.0,5.0,2.0};	
	double out;			  

	pDisp->detrendRemoveTrend(2, 10.0, out, d);
    zassert_equal(out, -2.0, "out incorrect");

	delete pDisp;
}

// test function HandleCalcDisplacementReq
ZTEST(tests_disp_disptask, test_disp_disptask_HandleCalcDisplacementReq)
{
    PoolElem* pElem = NULL;

	Mbox* pAdxlStoreMbox = new Mbox("AdxlStoreMbox");
    Mbox::registerMbox(pAdxlStoreMbox, Mbox::eAdxlStore);
	zassert_not_null(pAdxlStoreMbox, "pAdxlStoreMbox = NULL");

	DispTask* pDisp = new DispTask("disp", true);
	zassert_not_null(pDisp, "memory alloc failed");
	zassert_not_null(pDisp->pInMbox, "pInMbox = NULL");   
    zassert_equal(pDisp->pInMbox->count(), 0, "pInMbox->count() != 0");
    zassert_equal(pDisp->pInMbox, Mbox::getMboxById(Mbox::eDispTask),
                  "Error getting mailbox by id");

    zassert_equal(pDisp->bIdle, true, "idle != true");
	pDisp->HandleCalcDisplacementReq(pElem);
    zassert_equal(pDisp->bIdle, false, "idle != false");

    pElem = pAdxlStoreMbox->read();
    zassert_equal(pElem->mtype, msgType::eRawDataStatus, "Rcvd mtype != msgType::eRawDataStatus");
    zassert_equal(pElem->from, Mbox::eDispTask, "Rcvd sender != Mbox::eDispTask");

    pElem->free();
    delete pAdxlStoreMbox;
	delete pDisp;
}

// Test function reqFwFiltData
ZTEST(tests_disp_disptask, test_disp_disptask_reqFwFiltData)
{
    PoolElem* pElem;
    struct AdxlRawDataProgress progress;

	Mbox* pAdxlStoreMbox = new Mbox("AdxlStoreMbox");
    Mbox::registerMbox(pAdxlStoreMbox, Mbox::eAdxlStore);
	zassert_not_null(pAdxlStoreMbox, "pAdxlStoreMbox = NULL");

	DispTask* pDisp = new DispTask("disp", true);
	zassert_not_null(pDisp, "memory alloc failed");
	zassert_not_null(pDisp->pInMbox, "pInMbox = NULL");   
    zassert_equal(pDisp->pInMbox->count(), 0, "pInMbox->count() != 0");
    zassert_equal(pDisp->pInMbox, Mbox::getMboxById(Mbox::eDispTask),
                  "Error getting mailbox by id");

    pDisp->totalNumEntries = 1010;
    pDisp->rcvdEntries     = 1000;
    pDisp->requestedEntries = 100;
    pDisp->startReadPos = 0;

    pDisp->reqFwFiltData();  
    zassert_equal(pDisp->rcvdEntries, 1000, "rcvdEntries != 1000");
    
    pElem = pAdxlStoreMbox->read();
    //printk("pElem->mtype = %d\n\n", pElem->mtype);
    zassert_equal(pElem->mtype, msgType::eFwFiltData, "Rcvd mtype != msgType::eFwFiltData");
    zassert_equal(pElem->from, Mbox::eDispTask, "Rcvd sender != Mbox::eDispTask");
    
    progress = *reinterpret_cast<struct AdxlRawDataProgress*>(pElem->getBuffer());
    //printk("progress.startRd = %d\n\n", progress.startRd);
    zassert_equal(progress.startRd, 0, "progress.startRd != 0");
    zassert_equal(progress.rcvdEntries, 1000, "progress.rcvdEntries != 1000");
    zassert_equal(progress.requestedEntries, 10, "progress.requestedEntries != 100");

    pElem->free();
    delete pAdxlStoreMbox;
	delete pDisp; 
}

// Test function reqBwFiltData
ZTEST(tests_disp_disptask, test_disp_disptask_reqBwFiltData)
{
    PoolElem* pElem;
    struct AdxlRawDataProgress progress;

	Mbox* pAdxlStoreMbox = new Mbox("AdxlStoreMbox");
    Mbox::registerMbox(pAdxlStoreMbox, Mbox::eAdxlStore);
	zassert_not_null(pAdxlStoreMbox, "pAdxlStoreMbox = NULL");

	DispTask* pDisp = new DispTask("disp", true);
	zassert_not_null(pDisp, "memory alloc failed");
	zassert_not_null(pDisp->pInMbox, "pInMbox = NULL");   
    zassert_equal(pDisp->pInMbox->count(), 0, "pInMbox->count() != 0");
    zassert_equal(pDisp->pInMbox, Mbox::getMboxById(Mbox::eDispTask),
                  "Error getting mailbox by id");

    pDisp->totalNumEntries = 1010;
    pDisp->rcvdEntries     = 1000;
    pDisp->requestedEntries = 100;
    pDisp->startReadPos = 0;

    pDisp->reqBwFiltData();  
    zassert_equal(pDisp->rcvdEntries, 1000, "rcvdEntries != 1000");
    
    pElem = pAdxlStoreMbox->read();
    //printk("pElem->mtype = %d\n\n", pElem->mtype);
    zassert_equal(pElem->mtype, msgType::eBwFiltData, "Rcvd mtype != msgType::eBwFiltData");
    zassert_equal(pElem->from, Mbox::eDispTask, "Rcvd sender != Mbox::eDispTask");
    
    progress = *reinterpret_cast<struct AdxlRawDataProgress*>(pElem->getBuffer());
    //printk("progress.startRd = %d\n\n", progress.startRd);
    zassert_equal(progress.startRd, 0, "progress.startRd != 0");
    zassert_equal(progress.rcvdEntries, 1000, "progress.rcvdEntries != 1000");
    zassert_equal(progress.requestedEntries, 10, "progress.requestedEntries != 10");

    pElem->free();
    delete pAdxlStoreMbox;
	delete pDisp; 
}

// Test function reqSResultData
ZTEST(tests_disp_disptask, test_disp_disptask_reqSResultData)
{
    PoolElem* pElem;
    struct AdxlRawDataProgress progress;

	Mbox* pAdxlStoreMbox = new Mbox("AdxlStoreMbox");
    Mbox::registerMbox(pAdxlStoreMbox, Mbox::eAdxlStore);
	zassert_not_null(pAdxlStoreMbox, "pAdxlStoreMbox = NULL");

	DispTask* pDisp = new DispTask("disp", true);
	zassert_not_null(pDisp, "memory alloc failed");
	zassert_not_null(pDisp->pInMbox, "pInMbox = NULL");   
    zassert_equal(pDisp->pInMbox->count(), 0, "pInMbox->count() != 0");
    zassert_equal(pDisp->pInMbox, Mbox::getMboxById(Mbox::eDispTask),
                  "Error getting mailbox by id");

    pDisp->totalNumEntries = 1010;
    pDisp->rcvdEntries     = 1000;
    pDisp->requestedEntries = 100;
    pDisp->startReadPos = 0;

    pDisp->reqSResultData();  
    zassert_equal(pDisp->rcvdEntries, 1000, "rcvdEntries != 1000");
    
    pElem = pAdxlStoreMbox->read();
    //printk("pElem->mtype = %d\n\n", pElem->mtype);
    zassert_equal(pElem->mtype, msgType::eSResultData, "Rcvd mtype != msgType::eSResultData");
    zassert_equal(pElem->from, Mbox::eDispTask, "Rcvd sender != Mbox::eDispTask");
    
    progress = *reinterpret_cast<struct AdxlRawDataProgress*>(pElem->getBuffer());
    //printk("progress.startRd = %d\n\n", progress.startRd);
    zassert_equal(progress.startRd, 0, "progress.startRd != 0");
    zassert_equal(progress.rcvdEntries, 1000, "progress.rcvdEntries != 1000");
    zassert_equal(progress.requestedEntries, 10, "progress.requestedEntries != 10");

    pElem->free();
    delete pAdxlStoreMbox;
	delete pDisp; 
}

// Test function reqSResultWrite
ZTEST(tests_disp_disptask, test_disp_disptask_reqSResultWrite)
{
    PoolElem* pElem;
    struct AdxlFilterDataProgress progress;

	Mbox* pAdxlStoreMbox = new Mbox("AdxlStoreMbox");
    Mbox::registerMbox(pAdxlStoreMbox, Mbox::eAdxlStore);
	zassert_not_null(pAdxlStoreMbox, "pAdxlStoreMbox = NULL");

	DispTask* pDisp = new DispTask("disp", true);
	zassert_not_null(pDisp, "memory alloc failed");
	zassert_not_null(pDisp->pInMbox, "pInMbox = NULL");   
    zassert_equal(pDisp->pInMbox->count(), 0, "pInMbox->count() != 0");
    zassert_equal(pDisp->pInMbox, Mbox::getMboxById(Mbox::eDispTask),
                  "Error getting mailbox by id");

    pDisp->totalNumEntries = 1010;
    pDisp->sentEntries     = 1000;
    pDisp->requestedWrEntries = 100;
    pDisp->startReadPos = 0;

    vectord s = {1,2,3,4,5,6,7,8,9,0};

    pDisp->reqSResultWrite(s);  
    //printk("pDisp->sentEntries = %d\n\n", pDisp->sentEntries);
    zassert_equal(pDisp->sentEntries, 1010, "sentEntries != 1010");
    
    pElem = pAdxlStoreMbox->read();
    //printk("pElem->mtype = %d\n\n", pElem->mtype);
    zassert_equal(pElem->mtype, msgType::eStoreSResult, "Rcvd mtype != msgType::eStoreSResult");
    zassert_equal(pElem->from, Mbox::eDispTask, "Rcvd sender != Mbox::eDispTask");
    
    progress = *reinterpret_cast<struct AdxlFilterDataProgress*>(pElem->getBuffer());
    //printk("progress.startWr = %d, %d, %d\n\n", progress.startWr, progress.sentEntries, progress.requestedEntries);
    zassert_equal(progress.startWr, 142000, "progress.startWr != 142000");
    zassert_equal(progress.sentEntries, 1000, "progress.sentEntries != 1000");
    zassert_equal(progress.requestedEntries, 10, "progress.requestedEntries != 10");

    pElem->free();
    delete pAdxlStoreMbox;
	delete pDisp; 
}

// Test function reqDResultWrite
ZTEST(tests_disp_disptask, test_disp_disptask_reqDResultWrite)
{
    PoolElem* pElem;
    struct AdxlFilterDataProgress progress;

	Mbox* pAdxlStoreMbox = new Mbox("AdxlStoreMbox");
    Mbox::registerMbox(pAdxlStoreMbox, Mbox::eAdxlStore);
	zassert_not_null(pAdxlStoreMbox, "pAdxlStoreMbox = NULL");

	DispTask* pDisp = new DispTask("disp", true);
	zassert_not_null(pDisp, "memory alloc failed");
	zassert_not_null(pDisp->pInMbox, "pInMbox = NULL");   
    zassert_equal(pDisp->pInMbox->count(), 0, "pInMbox->count() != 0");
    zassert_equal(pDisp->pInMbox, Mbox::getMboxById(Mbox::eDispTask),
                  "Error getting mailbox by id");

    pDisp->totalNumEntries = 1010;
    pDisp->sentEntries     = 1000;
    pDisp->requestedWrEntries = 100;
    pDisp->startReadPos = 0;

    vectord s = {1,2,3,4,5,6,7,8,9,0};

    pDisp->reqDResultWrite(s);  
    //printk("pDisp->sentEntries = %d\n\n", pDisp->sentEntries);
    zassert_equal(pDisp->sentEntries, 1010, "sentEntries != 1010");
    
    pElem = pAdxlStoreMbox->read();
    //printk("pElem->mtype = %d\n\n", pElem->mtype);
    zassert_equal(pElem->mtype, msgType::eStoreDResult, "Rcvd mtype != msgType::eStoreDResult");
    zassert_equal(pElem->from, Mbox::eDispTask, "Rcvd sender != Mbox::eDispTask");
    
    progress = *reinterpret_cast<struct AdxlFilterDataProgress*>(pElem->getBuffer());
    //printk("progress.startWr = %d, %d, %d\n\n", progress.startWr, progress.sentEntries, progress.requestedEntries);
    zassert_equal(progress.startWr, 142000, "progress.startWr != 142000");
    zassert_equal(progress.sentEntries, 1000, "progress.sentEntries != 1000");
    zassert_equal(progress.requestedEntries, 10, "progress.requestedEntries != 10");

    pElem->free();
    delete pAdxlStoreMbox;
	delete pDisp; 
}

// Test receive msg eCalcDisplacementReq
ZTEST(tests_disp_disptask, test_disp_disptask_rcv_msg_eCalcDisplacementReq)
{
    PoolElem* pElem;

	Mbox* pAdxlStoreMbox = new Mbox("AdxlStoreMbox");
    Mbox::registerMbox(pAdxlStoreMbox, Mbox::eAdxlStore);
	zassert_not_null(pAdxlStoreMbox, "pAdxlStoreMbox = NULL");

	DispTask* pDisp = new DispTask("disp", true);
	zassert_not_null(pDisp, "memory alloc failed");
	zassert_not_null(pDisp->pInMbox, "pInMbox = NULL");   
    zassert_equal(pDisp->pInMbox->count(), 0, "pInMbox->count() != 0");
    zassert_equal(pDisp->pInMbox, Mbox::getMboxById(Mbox::eDispTask),
                  "Error getting mailbox by id");

    Mbox::sendMsg(msgType::eCalcDisplacementReq, Mbox::eDispTask, Mbox::eDispTask);

    zassert_equal(pDisp->pInMbox->count(), 1, "pInMbox->count() != 1");

    pDisp->task();  

    zassert_equal(pDisp->pInMbox->count(), 0, "pInMbox->count() != 0");
    zassert_equal(pAdxlStoreMbox->count(), 1, "pMqttConnMbox->count() != 1");

    pElem = pAdxlStoreMbox->read();
    zassert_equal(pElem->mtype, msgType::eRawDataStatus, "Rcvd mtype != msgType::eRawDataStatus");
    zassert_equal(pElem->from, Mbox::eDispTask, "Rcvd sender != Mbox::eDispTask");

    pElem->free();
    delete pAdxlStoreMbox;
	delete pDisp; 
}

// Test receive msg eRawDataStatus
ZTEST(tests_disp_disptask, test_disp_disptask_rcv_msg_eRawDataStatus)
{
	DispTask* pDisp = new DispTask("disp", true);
	zassert_not_null(pDisp, "memory alloc failed");
	zassert_not_null(pDisp->pInMbox, "pInMbox = NULL");   
    zassert_equal(pDisp->pInMbox->count(), 0, "pInMbox->count() != 0");
    zassert_equal(pDisp->pInMbox, Mbox::getMboxById(Mbox::eDispTask),
                  "Error getting mailbox by id");

    //uint32_t startRd;  //!< Start read position
    //uint32_t entries;  //!< Total number of entries in FRAM
    struct AdxlRawDataStatus stat = {0, 0};
    pDisp->bIdle = false;
    Mbox::sendMsg(msgType::eRawDataStatus, Mbox::eDispTask, Mbox::eDispTask, (uint8_t*)&stat, sizeof(stat));

    zassert_equal(pDisp->pInMbox->count(), 1, "pInMbox->count() != 1");

    pDisp->task();  
    zassert_equal(pDisp->pInMbox->count(), 0, "pInMbox->count() != 0");
    zassert_equal(pDisp->rcvdEntries, 0, "rcvdEntries != 0");
    zassert_true(pDisp->bIdle, "bIdle != true");

	delete pDisp; 
}

// Test receive msg eRawDataStatus
ZTEST(tests_disp_disptask, test_disp_disptask_rcv_msg_eRawDataStatus2)
{
    PoolElem* pElem;
    struct AdxlRawDataProgress progress;

	Mbox* pAdxlStoreMbox = new Mbox("AdxlStoreMbox");
    Mbox::registerMbox(pAdxlStoreMbox, Mbox::eAdxlStore);
	zassert_not_null(pAdxlStoreMbox, "pAdxlStoreMbox = NULL");

	DispTask* pDisp = new DispTask("disp", true);
	zassert_not_null(pDisp, "memory alloc failed");
	zassert_not_null(pDisp->pInMbox, "pInMbox = NULL");   
    zassert_equal(pDisp->pInMbox->count(), 0, "pInMbox->count() != 0");
    zassert_equal(pDisp->pInMbox, Mbox::getMboxById(Mbox::eDispTask),
                  "Error getting mailbox by id");

    //uint32_t startRd;  //!< Start read position
    //uint32_t entries;  //!< Total number of entries in FRAM
    struct AdxlRawDataStatus stat = {0, 1000};
    pDisp->bIdle = false;
    pDisp->nfact = 9;
    Mbox::sendMsg(msgType::eRawDataStatus, Mbox::eDispTask, Mbox::eDispTask, (uint8_t*)&stat, sizeof(stat));

    zassert_equal(pDisp->pInMbox->count(), 1, "pInMbox->count() != 1");

    pDisp->task();  
    zassert_equal(pDisp->pInMbox->count(), 0, "pInMbox->count() != 0");
    zassert_equal(pDisp->rcvdEntries, 0, "rcvdEntries != 0");
    zassert_false(pDisp->bIdle, "bIdle != false");

    pElem = pAdxlStoreMbox->read();
    zassert_equal(pElem->mtype, msgType::eDispFirstEntries, "Rcvd mtype != msgType::eDispFirstEntries");
    zassert_equal(pElem->from, Mbox::eDispTask, "Rcvd sender != Mbox::eDispTask");
    progress = *reinterpret_cast<struct AdxlRawDataProgress*>(pElem->getBuffer());;
    zassert_equal(progress.startRd, 0, "progress.startRd != 0");
    zassert_equal(progress.rcvdEntries, 0, "progress.rcvdEntries != 0");
    zassert_equal(progress.requestedEntries, 11, "progress.requestedEntries != 11");

    pElem->free();
    delete pAdxlStoreMbox;
	delete pDisp; 
}

// Test receive msg eDispFirstEntries
ZTEST(tests_disp_disptask, test_disp_disptask_rcv_msg_eDispFirstEntries)
{
    PoolElem* pElem;
    struct AdxlRawDataProgress progress;

	Mbox* pAdxlStoreMbox = new Mbox("AdxlStoreMbox");
    Mbox::registerMbox(pAdxlStoreMbox, Mbox::eAdxlStore);
	zassert_not_null(pAdxlStoreMbox, "pAdxlStoreMbox = NULL");

	DispTask* pDisp = new DispTask("disp", true);
	zassert_not_null(pDisp, "memory alloc failed");
	zassert_not_null(pDisp->pInMbox, "pInMbox = NULL");   
    zassert_equal(pDisp->pInMbox->count(), 0, "pInMbox->count() != 0");
    zassert_equal(pDisp->pInMbox, Mbox::getMboxById(Mbox::eDispTask),
                  "Error getting mailbox by id");

    // int16_t x;  //!< measured x value of the acceleration data
    // int16_t y;  //!< measured y value of the acceleration data
    // int16_t z;  //!< measured z value of the acceleration data
    struct adxl372_xyz_accel_data data[11] = {  {1, 1, 1},
                                            {2, 2, 2},
                                            {-2, -2, -2},
                                            {-1, -1, -1},
                                            {1, 1, 1},
                                            {2, 2, 2},
                                            {-2, -2, -2},
                                            {-1, -1, -1},
                                            {1, 1, 1},
                                            {2, 2, 2},
                                            {-2, -2, -2} }; 
    pDisp->bIdle = false;
    pDisp->nfact = 9;
    pDisp->totalNumEntries = 1000;

    Mbox::sendMsg(msgType::eDispFirstEntries, Mbox::eDispTask, Mbox::eDispTask, (uint8_t*)&data, sizeof(data));

    zassert_equal(pDisp->pInMbox->count(), 1, "pInMbox->count() != 1");

    pDisp->task();  
    zassert_equal(pDisp->pInMbox->count(), 0, "pInMbox->count() != 0");
    zassert_equal(pDisp->rcvdEntries, 0, "rcvdEntries != 0");

    pElem = pAdxlStoreMbox->read();
    zassert_equal(pElem->mtype, msgType::eDispLastEntries, "Rcvd mtype != msgType::eDispLasttEntries");
    zassert_equal(pElem->from, Mbox::eDispTask, "Rcvd sender != Mbox::eDispTask");
    progress = *reinterpret_cast<struct AdxlRawDataProgress*>(pElem->getBuffer());;
    zassert_equal(progress.startRd, 0, "progress.startRd != 0");
    zassert_equal(progress.rcvdEntries, 990, "progress.rcvdEntries != 990");
    zassert_equal(progress.requestedEntries, 10, "progress.requestedEntries != 10");

    pElem->free();
    delete pAdxlStoreMbox;
	delete pDisp; 
}

// Test receive msg eDispLastEntries
ZTEST(tests_disp_disptask, test_disp_disptask_rcv_msg_eDispLastEntries)
{
    PoolElem* pElem;
    struct AdxlRawDataProgress progress;

	Mbox* pAdxlStoreMbox = new Mbox("AdxlStoreMbox");
    Mbox::registerMbox(pAdxlStoreMbox, Mbox::eAdxlStore);
	zassert_not_null(pAdxlStoreMbox, "pAdxlStoreMbox = NULL");

	DispTask* pDisp = new DispTask("disp", true);
	zassert_not_null(pDisp, "memory alloc failed");
	zassert_not_null(pDisp->pInMbox, "pInMbox = NULL");   
    zassert_equal(pDisp->pInMbox->count(), 0, "pInMbox->count() != 0");
    zassert_equal(pDisp->pInMbox, Mbox::getMboxById(Mbox::eDispTask),
                  "Error getting mailbox by id");

    // int16_t x;  //!< measured x value of the acceleration data
    // int16_t y;  //!< measured y value of the acceleration data
    // int16_t z;  //!< measured z value of the acceleration data
    struct adxl372_xyz_accel_data data[11] = {  {1, 1, 1},
                                            {2, 2, 2},
                                            {-2, -2, -2},
                                            {-1, -1, -1},
                                            {1, 1, 1},
                                            {2, 2, 2},
                                            {-2, -2, -2},
                                            {-1, -1, -1},
                                            {1, 1, 1},
                                            {2, 2, 2} }; 
    pDisp->bIdle = false;
    pDisp->nfact = 9;
    pDisp->totalNumEntries = 1000;
    pDisp->rcvdEntries     = 100;
    pDisp->startReadPos = 0;
    Mbox::sendMsg(msgType::eDispLastEntries, Mbox::eDispTask, Mbox::eDispTask, (uint8_t*)&data, sizeof(data));

    zassert_equal(pDisp->pInMbox->count(), 1, "pInMbox->count() != 1");

    pDisp->task();  
    zassert_equal(pDisp->pInMbox->count(), 0, "pInMbox->count() != 0");
    zassert_equal(pDisp->rcvdEntries, 100, "rcvdEntries != 100");

    pElem = pAdxlStoreMbox->read();
    zassert_equal(pElem->mtype, msgType::eRawData, "Rcvd mtype != msgType::eRawData");
    zassert_equal(pElem->from, Mbox::eDispTask, "Rcvd sender != Mbox::eDispTask");
    zassert_equal(pDisp->sentEntries, 0, "sentEntries != 0");
    
    progress = *reinterpret_cast<struct AdxlRawDataProgress*>(pElem->getBuffer());;
    zassert_equal(progress.startRd, 0, "progress.startRd != 0");
    zassert_equal(progress.rcvdEntries, 100, "progress.rcvdEntries != 100");
    zassert_equal(progress.requestedEntries, 100, "progress.requestedEntries != 100");

    pElem->free();
    delete pAdxlStoreMbox;
	delete pDisp; 
}

// Test receive msg eDispLastEntries
ZTEST(tests_disp_disptask, test_disp_disptask_rcv_msg_eDispLastEntries2)
{
    PoolElem* pElem;
    struct AdxlRawDataProgress progress;

	Mbox* pAdxlStoreMbox = new Mbox("AdxlStoreMbox");
    Mbox::registerMbox(pAdxlStoreMbox, Mbox::eAdxlStore);
	zassert_not_null(pAdxlStoreMbox, "pAdxlStoreMbox = NULL");

	DispTask* pDisp = new DispTask("disp", true);
	zassert_not_null(pDisp, "memory alloc failed");
	zassert_not_null(pDisp->pInMbox, "pInMbox = NULL");   
    zassert_equal(pDisp->pInMbox->count(), 0, "pInMbox->count() != 0");
    zassert_equal(pDisp->pInMbox, Mbox::getMboxById(Mbox::eDispTask),
                  "Error getting mailbox by id");

    // int16_t x;  //!< measured x value of the acceleration data
    // int16_t y;  //!< measured y value of the acceleration data
    // int16_t z;  //!< measured z value of the acceleration data
    struct adxl372_xyz_accel_data data[11] = {  {1, 1, 1},
                                            {2, 2, 2},
                                            {-2, -2, -2},
                                            {-1, -1, -1},
                                            {1, 1, 1},
                                            {2, 2, 2},
                                            {-2, -2, -2},
                                            {-1, -1, -1},
                                            {1, 1, 1},
                                            {2, 2, 2} }; 
    pDisp->bIdle = false;
    pDisp->nfact = 9;
    pDisp->totalNumEntries = 1000;
    pDisp->rcvdEntries     = 980;
    Mbox::sendMsg(msgType::eDispLastEntries, Mbox::eDispTask, Mbox::eDispTask, (uint8_t*)&data, sizeof(data));

    zassert_equal(pDisp->pInMbox->count(), 1, "pInMbox->count() != 1");

    pDisp->task();  
    zassert_equal(pDisp->pInMbox->count(), 0, "pInMbox->count() != 0");
    zassert_equal(pDisp->rcvdEntries, 980, "rcvdEntries != 980");

    pElem = pAdxlStoreMbox->read();
    zassert_equal(pElem->mtype, msgType::eRawData, "Rcvd mtype != msgType::eRawData");
    zassert_equal(pElem->from, Mbox::eDispTask, "Rcvd sender != Mbox::eDispTask");
    zassert_equal(pDisp->sentEntries, 0, "sentEntries != 0");
    
    progress = *reinterpret_cast<struct AdxlRawDataProgress*>(pElem->getBuffer());;
    zassert_equal(progress.startRd, 0, "progress.startRd != 0");
    zassert_equal(progress.rcvdEntries, 980, "progress.rcvdEntries != 980");
    zassert_equal(progress.requestedEntries, 20, "progress.requestedEntries != 20");

    pElem->free();
    delete pAdxlStoreMbox;
	delete pDisp; 
}

// Test receive msg eRawData
ZTEST(tests_disp_disptask, test_disp_disptask_rcv_msg_eRawData)
{
    PoolElem* pElem;
    struct AdxlRawDataProgress progress;

	Mbox* pAdxlStoreMbox = new Mbox("AdxlStoreMbox");
    Mbox::registerMbox(pAdxlStoreMbox, Mbox::eAdxlStore);
	zassert_not_null(pAdxlStoreMbox, "pAdxlStoreMbox = NULL");

	DispTask* pDisp = new DispTask("disp", true);
	zassert_not_null(pDisp, "memory alloc failed");
	zassert_not_null(pDisp->pInMbox, "pInMbox = NULL");   
    zassert_equal(pDisp->pInMbox->count(), 0, "pInMbox->count() != 0");
    zassert_equal(pDisp->pInMbox, Mbox::getMboxById(Mbox::eDispTask),
                  "Error getting mailbox by id");

    // int16_t x;  //!< measured x value of the acceleration data
    // int16_t y;  //!< measured y value of the acceleration data
    // int16_t z;  //!< measured z value of the acceleration data
    struct adxl372_xyz_accel_data data[11] = {  {1, 1, 1},
                                            {2, 2, 2},
                                            {-2, -2, -2},
                                            {-1, -1, -1},
                                            {1, 1, 1},
                                            {2, 2, 2},
                                            {-2, -2, -2},
                                            {-1, -1, -1},
                                            {1, 1, 1},
                                            {2, 2, 2} }; 
    pDisp->bIdle = false;
    pDisp->nfact = 9;
    pDisp->totalNumEntries = 1000;
    pDisp->rcvdEntries     = 100;
    pDisp->requestedEntries = 10;
    pDisp->startReadPos = 0;
    pDisp->nfilt = 4;
    pDisp->nfact = 9;
    Mbox::sendMsg(msgType::eRawData, Mbox::eDispTask, Mbox::eDispTask, (uint8_t*)&data, sizeof(data));

    zassert_equal(pDisp->pInMbox->count(), 1, "pInMbox->count() != 1");

    vectord b_coeff = { 0.95806, -2.87418,  2.87418, -0.95806 };
	vectord a_coeff = { 1.00000, -2.91432,  2.83228, -0.91788 };
	vectord zi = { -0.95806, 1.91612, -0.95806, 0.0 };
    vectord x = { 1,-.2,.3,1,-.5,1,-.7,.8,-.9,.5 };
	vectord y = {  };

	CFilter::inst()->a = &a_coeff[0];
	CFilter::inst()->b = &b_coeff[0];
	CFilter::inst()->z = &zi[0];
	CFilter::inst()->X_old = {-0.70000,  0.80000,  -0.90000,  0.50000};
	CFilter::inst()->Y_old = {-1.37864,  0.18662,  -1.44279,  0.03703};
	CFilter::inst()->filter_order = 4;

    pDisp->task();  
    zassert_equal(pDisp->pInMbox->count(), 0, "pInMbox->count() != 0");
    zassert_equal(pDisp->rcvdEntries, 200, "rcvdEntries != 200");
    
    pElem = pAdxlStoreMbox->read();
    //printk("pElem->mtype = %d\n\n", pElem->mtype);
    zassert_equal(pElem->mtype, msgType::eStoreFwFiltResult, "Rcvd mtype != msgType::eStoreFwFiltResult");
    zassert_equal(pElem->from, Mbox::eDispTask, "Rcvd sender != Mbox::eDispTask");

    pElem = pAdxlStoreMbox->read();
    //printk("pElem->mtype = %d\n\n", pElem->mtype);
    zassert_equal(pElem->mtype, msgType::eRawData, "Rcvd mtype != msgType::eRawData");
    zassert_equal(pElem->from, Mbox::eDispTask, "Rcvd sender != Mbox::eDispTask");
    
    progress = *reinterpret_cast<struct AdxlRawDataProgress*>(pElem->getBuffer());;
    //printk("progress.startRd = %d\n\n", progress.startRd);
    zassert_equal(progress.startRd, 0, "progress.startRd != 0");
    zassert_equal(progress.rcvdEntries, 200, "progress.rcvdEntries != 200");
    zassert_equal(progress.requestedEntries, 100, "progress.requestedEntries != 100");

    pElem->free();
    delete pAdxlStoreMbox;
	delete pDisp; 
}

// Test receive msg eRawData
ZTEST(tests_disp_disptask, test_disp_disptask_rcv_msg_eRawData2)
{
    PoolElem* pElem;

	Mbox* pAdxlStoreMbox = new Mbox("AdxlStoreMbox");
    Mbox::registerMbox(pAdxlStoreMbox, Mbox::eAdxlStore);
	zassert_not_null(pAdxlStoreMbox, "pAdxlStoreMbox = NULL");

	DispTask* pDisp = new DispTask("disp", true);
	zassert_not_null(pDisp, "memory alloc failed");
	zassert_not_null(pDisp->pInMbox, "pInMbox = NULL");   
    zassert_equal(pDisp->pInMbox->count(), 0, "pInMbox->count() != 0");
    zassert_equal(pDisp->pInMbox, Mbox::getMboxById(Mbox::eDispTask),
                  "Error getting mailbox by id");

    // int16_t x;  //!< measured x value of the acceleration data
    // int16_t y;  //!< measured y value of the acceleration data
    // int16_t z;  //!< measured z value of the acceleration data
    struct adxl372_xyz_accel_data data[11] = {  {1, 1, 1},
                                            {2, 2, 2},
                                            {-2, -2, -2},
                                            {-1, -1, -1},
                                            {1, 1, 1},
                                            {2, 2, 2},
                                            {-2, -2, -2},
                                            {-1, -1, -1},
                                            {1, 1, 1},
                                            {2, 2, 2} }; 
    pDisp->bIdle = false;
    pDisp->nfact = 9;
    pDisp->totalNumEntries = 1000;
    pDisp->rcvdEntries     = 990;
    pDisp->requestedEntries = 10;
    pDisp->startReadPos = 0;
    pDisp->nfilt = 4;
    pDisp->nfact = 9;
    Mbox::sendMsg(msgType::eRawData, Mbox::eDispTask, Mbox::eDispTask, (uint8_t*)&data, sizeof(data));

    zassert_equal(pDisp->pInMbox->count(), 1, "pInMbox->count() != 1");

    vectord b_coeff = { 0.95806, -2.87418,  2.87418, -0.95806 };
	vectord a_coeff = { 1.00000, -2.91432,  2.83228, -0.91788 };
	vectord zi = { -0.95806, 1.91612, -0.95806, 0.0 };
    vectord x = { 1,-.2,.3,1,-.5,1,-.7,.8,-.9,.5 };
	vectord y = {  };

	CFilter::inst()->a = &a_coeff[0];
	CFilter::inst()->b = &b_coeff[0];
	CFilter::inst()->z = &zi[0];
	CFilter::inst()->X_old = {-0.70000,  0.80000,  -0.90000,  0.50000};
	CFilter::inst()->Y_old = {-1.37864,  0.18662,  -1.44279,  0.03703};
	CFilter::inst()->filter_order = 4;

    pDisp->task();  
    zassert_equal(pDisp->pInMbox->count(), 0, "pInMbox->count() != 0");
    zassert_equal(pDisp->rcvdEntries, 1000, "rcvdEntries != 1000");

    pElem = pAdxlStoreMbox->read();
    //printk("pElem->mtype = %d\n\n", pElem->mtype);
    zassert_equal(pElem->mtype, msgType::eStoreFwFiltResult, "Rcvd mtype != msgType::eStoreFwFiltResult");
    zassert_equal(pElem->from, Mbox::eDispTask, "Rcvd sender != Mbox::eDispTask");

    pElem->free();
    delete pAdxlStoreMbox;
	delete pDisp; 
}

// Test receive msg eStoredFwFiltVals
ZTEST(tests_disp_disptask, test_disp_disptask_rcv_msg_eStoredFwFiltVals)
{
    PoolElem* pElem;
    struct AdxlRawDataProgress progress;

	Mbox* pAdxlStoreMbox = new Mbox("AdxlStoreMbox");
    Mbox::registerMbox(pAdxlStoreMbox, Mbox::eAdxlStore);
	zassert_not_null(pAdxlStoreMbox, "pAdxlStoreMbox = NULL");

	DispTask* pDisp = new DispTask("disp", true);
	zassert_not_null(pDisp, "memory alloc failed");
	zassert_not_null(pDisp->pInMbox, "pInMbox = NULL");   
    zassert_equal(pDisp->pInMbox->count(), 0, "pInMbox->count() != 0");
    zassert_equal(pDisp->pInMbox, Mbox::getMboxById(Mbox::eDispTask),
                  "Error getting mailbox by id");

    // int16_t x;  //!< measured x value of the acceleration data
    // int16_t y;  //!< measured y value of the acceleration data
    // int16_t z;  //!< measured z value of the acceleration data
    struct adxl372_xyz_accel_data data[11] = {  {1, 1, 1},
                                            {2, 2, 2},
                                            {-2, -2, -2},
                                            {-1, -1, -1},
                                            {1, 1, 1},
                                            {2, 2, 2},
                                            {-2, -2, -2},
                                            {-1, -1, -1},
                                            {1, 1, 1},
                                            {2, 2, 2} }; 
    pDisp->leftpadFilt = {1,1,1,1,1,1,1,1,1,1,1};
    pDisp->rightpadFilt = {1,1,1,1,1,1,1,1,1,1};
    pDisp->bIdle = false;
    pDisp->nfilt = 4;
    pDisp->nfact = 9;
    pDisp->totalNumEntries = 1000;
    pDisp->rcvdEntries     = 0;
    pDisp->startReadPos = 0;
    pDisp->sentEntries = 0;
    pDisp->sentEntries = 0;
    Mbox::sendMsg(msgType::eStoredFwFiltVals, Mbox::eDispTask, Mbox::eDispTask);

    zassert_equal(pDisp->pInMbox->count(), 1, "pInMbox->count() != 1");

    vectord b_coeff = { 0.95806, -2.87418,  2.87418, -0.95806 };
	vectord a_coeff = { 1.00000, -2.91432,  2.83228, -0.91788 };
	vectord zi = { -0.95806, 1.91612, -0.95806, 0.0 };
    vectord x = { 1,-.2,.3,1,-.5,1,-.7,.8,-.9,.5 };
	vectord y = {  };

	CFilter::inst()->a = &a_coeff[0];
	CFilter::inst()->b = &b_coeff[0];
	CFilter::inst()->z = &zi[0];
	CFilter::inst()->X_old = {-0.70000,  0.80000,  -0.90000,  0.50000};
	CFilter::inst()->Y_old = {-1.37864,  0.18662,  -1.44279,  0.03703};
	CFilter::inst()->filter_order = 4;

    pDisp->task();  
    zassert_equal(pDisp->pInMbox->count(), 0, "pInMbox->count() != 0");
    zassert_equal(pDisp->rcvdEntries, 0, "rcvdEntries != 0");
    zassert_equal(pDisp->startReadPos, 0, "startReadPos != 0");
    zassert_equal(pDisp->sentEntries, 0, "sentEntries != 0");

    pElem = pAdxlStoreMbox->read();
    //printk("pElem->mtype = %d\n\n", pElem->mtype);
    zassert_equal(pElem->mtype, msgType::eFwFiltData, "Rcvd mtype != msgType::eFwFiltData");
    zassert_equal(pElem->from, Mbox::eDispTask, "Rcvd sender != Mbox::eDispTask");

    progress = *reinterpret_cast<struct AdxlRawDataProgress*>(pElem->getBuffer());;
    zassert_equal(progress.startRd, 0, "progress.startRd != 0");
    zassert_equal(progress.rcvdEntries, 0, "progress.rcvdEntries != 0");
    zassert_equal(progress.requestedEntries, 100, "progress.requestedEntries != 100");

    pElem->free();
    delete pAdxlStoreMbox;
	delete pDisp; 
}

// Test receive msg eFwFiltData
// NOTE: throws exception only if called in context of other test cases, if called 
// without other test cases function passes!!
ZTEST(tests_disp_disptask, test_disp_disptask_rcv_msg_eFwFiltData)
{
    PoolElem* pElem;
    struct AdxlRawDataProgress progress;

	Mbox* pAdxlStoreMbox = new Mbox("AdxlStoreMbox");
    Mbox::registerMbox(pAdxlStoreMbox, Mbox::eAdxlStore);
	zassert_not_null(pAdxlStoreMbox, "pAdxlStoreMbox = NULL");

	DispTask* pDisp = new DispTask("disp", true);
	zassert_not_null(pDisp, "memory alloc failed");
	zassert_not_null(pDisp->pInMbox, "pInMbox = NULL");   
    zassert_equal(pDisp->pInMbox->count(), 0, "pInMbox->count() != 0");
    zassert_equal(pDisp->pInMbox, Mbox::getMboxById(Mbox::eDispTask),
                  "Error getting mailbox by id");

    // int16_t x;  //!< measured x value of the acceleration data
    // int16_t y;  //!< measured y value of the acceleration data
    // int16_t z;  //!< measured z value of the acceleration data
    struct adxl372_xyz_accel_data data[11] = {  {1, 1, 1},
                                            {2, 2, 2},
                                            {-2, -2, -2},
                                            {-1, -1, -1},
                                            {1, 1, 1},
                                            {2, 2, 2},
                                            {-2, -2, -2},
                                            {-1, -1, -1},
                                            {1, 1, 1},
                                            {2, 2, 2} }; 
    pDisp->bIdle = false;
    pDisp->nfact = 9;
    pDisp->totalNumEntries = 1000;
    pDisp->rcvdEntries     = 100;
    pDisp->requestedEntries = 10;
    pDisp->sentEntries = 990;
    pDisp->startReadPos = 0;
    pDisp->nfilt = 4;
    pDisp->nfact = 9;
    Mbox::sendMsg(msgType::eFwFiltData, Mbox::eDispTask, Mbox::eDispTask, (uint8_t*)&data, sizeof(data));

    zassert_equal(pDisp->pInMbox->count(), 1, "pInMbox->count() != 1");

    vectord b_coeff = { 0.95806, -2.87418,  2.87418, -0.95806 };
	vectord a_coeff = { 1.00000, -2.91432,  2.83228, -0.91788 };
	vectord zi = { -0.95806, 1.91612, -0.95806, 0.0 };
    vectord x = { 1,-.2,.3,1,-.5,1,-.7,.8,-.9,.5 };
	vectord y = {  };

	CFilter::inst()->a = &a_coeff[0];
	CFilter::inst()->b = &b_coeff[0];
	CFilter::inst()->z = &zi[0];
	CFilter::inst()->X_old = {-0.70000,  0.80000,  -0.90000,  0.50000};
	CFilter::inst()->Y_old = {-1.37864,  0.18662,  -1.44279,  0.03703};
	CFilter::inst()->filter_order = 4;
	CFilter::inst()->nfilt = 4;
	CFilter::inst()->nfact = 9;

    pDisp->task();  // +++
    zassert_equal(pDisp->pInMbox->count(), 0, "pInMbox->count() != 0");
    zassert_equal(pDisp->rcvdEntries, 200, "rcvdEntries != 200");
    
    pElem = pAdxlStoreMbox->read();
    //printk("pElem->mtype = %d\n\n", pElem->mtype);
    zassert_equal(pElem->mtype, msgType::eStoreBwFiltResult, "Rcvd mtype != msgType::eStoreBwFiltResult");
    zassert_equal(pElem->from, Mbox::eDispTask, "Rcvd sender != Mbox::eDispTask");

    pElem = pAdxlStoreMbox->read();
    //printk("pElem->mtype = %d\n\n", pElem->mtype);
    zassert_equal(pElem->mtype, msgType::eFwFiltData, "Rcvd mtype != msgType::eFwFiltData");
    zassert_equal(pElem->from, Mbox::eDispTask, "Rcvd sender != Mbox::eDispTask");
    
    progress = *reinterpret_cast<struct AdxlRawDataProgress*>(pElem->getBuffer());
    //printk("progress.startRd = %d\n\n", progress.startRd);
    zassert_equal(progress.startRd, 0, "progress.startRd != 0");
    zassert_equal(progress.rcvdEntries, 200, "progress.rcvdEntries != 200");
    zassert_equal(progress.requestedEntries, 100, "progress.requestedEntries != 100");

    pElem->free();
    delete pAdxlStoreMbox;
	delete pDisp; 
}

// Test receive msg eFwFiltData
// NOTE: throws exception only if callen in contecxt of other test cases, if called 
// without other test cases function passes!!
ZTEST(tests_disp_disptask, test_disp_disptask_rcv_msg_eFwFiltData2)
{
    PoolElem* pElem;

	Mbox* pAdxlStoreMbox = new Mbox("AdxlStoreMbox");
    Mbox::registerMbox(pAdxlStoreMbox, Mbox::eAdxlStore);
	zassert_not_null(pAdxlStoreMbox, "pAdxlStoreMbox = NULL");

	DispTask* pDisp = new DispTask("disp", true);
	zassert_not_null(pDisp, "memory alloc failed");
	zassert_not_null(pDisp->pInMbox, "pInMbox = NULL");   
    zassert_equal(pDisp->pInMbox->count(), 0, "pInMbox->count() != 0");
    zassert_equal(pDisp->pInMbox, Mbox::getMboxById(Mbox::eDispTask),
                  "Error getting mailbox by id");

    // int16_t x;  //!< measured x value of the acceleration data
    // int16_t y;  //!< measured y value of the acceleration data
    // int16_t z;  //!< measured z value of the acceleration data
    struct adxl372_xyz_accel_data data[11] = {  {1, 1, 1},
                                            {2, 2, 2},
                                            {-2, -2, -2},
                                            {-1, -1, -1},
                                            {1, 1, 1},
                                            {2, 2, 2},
                                            {-2, -2, -2},
                                            {-1, -1, -1},
                                            {1, 1, 1},
                                            {2, 2, 2} }; 
    pDisp->bIdle = false;
    pDisp->nfact = 9;
    pDisp->totalNumEntries = 1000;
    pDisp->rcvdEntries     = 990;
    pDisp->requestedEntries = 10;
    pDisp->sentEntries = 10;
    pDisp->startReadPos = 0;
    pDisp->nfilt = 4;
    pDisp->nfact = 9;
    Mbox::sendMsg(msgType::eFwFiltData, Mbox::eDispTask, Mbox::eDispTask, (uint8_t*)&data, sizeof(data));

    zassert_equal(pDisp->pInMbox->count(), 1, "pInMbox->count() != 1");

    vectord b_coeff = { 0.95806, -2.87418,  2.87418, -0.95806 };
	vectord a_coeff = { 1.00000, -2.91432,  2.83228, -0.91788 };
	vectord zi = { -0.95806, 1.91612, -0.95806, 0.0 };
    vectord x = { 1,-.2,.3,1,-.5,1,-.7,.8,-.9,.5 };
	vectord y = {  };

	CFilter::inst()->a = &a_coeff[0];
	CFilter::inst()->b = &b_coeff[0];
	CFilter::inst()->z = &zi[0];
	CFilter::inst()->X_old = {-0.70000,  0.80000,  -0.90000,  0.50000};
	CFilter::inst()->Y_old = {-1.37864,  0.18662,  -1.44279,  0.03703};
	CFilter::inst()->filter_order = 4;
	CFilter::inst()->nfilt = 4;
	CFilter::inst()->nfact = 9;

    pDisp->task();  
    zassert_equal(pDisp->pInMbox->count(), 0, "pInMbox->count() != 0");
    zassert_equal(pDisp->rcvdEntries, 1000, "rcvdEntries != 1000");

    pElem = pAdxlStoreMbox->read();
    //printk("pElem->mtype = %d\n\n", pElem->mtype);
    zassert_equal(pElem->mtype, msgType::eStoreBwFiltResult, "Rcvd mtype != msgType::eStoreBwFiltResult");
    zassert_equal(pElem->from, Mbox::eDispTask, "Rcvd sender != Mbox::eDispTask");

    pElem->free();
    delete pAdxlStoreMbox;
	delete pDisp; 
}

// Test receive msg eStoredBwFiltVals
ZTEST(tests_disp_disptask, test_disp_disptask_rcv_msg_eStoredBwFiltVals)
{
    PoolElem* pElem;
    struct AdxlRawDataProgress progress;

	Mbox* pAdxlStoreMbox = new Mbox("AdxlStoreMbox");
    Mbox::registerMbox(pAdxlStoreMbox, Mbox::eAdxlStore);
	zassert_not_null(pAdxlStoreMbox, "pAdxlStoreMbox = NULL");

	DispTask* pDisp = new DispTask("disp", true);
	zassert_not_null(pDisp, "memory alloc failed");
	zassert_not_null(pDisp->pInMbox, "pInMbox = NULL");   
    zassert_equal(pDisp->pInMbox->count(), 0, "pInMbox->count() != 0");
    zassert_equal(pDisp->pInMbox, Mbox::getMboxById(Mbox::eDispTask),
                  "Error getting mailbox by id");

    // int16_t x;  //!< measured x value of the acceleration data
    // int16_t y;  //!< measured y value of the acceleration data
    // int16_t z;  //!< measured z value of the acceleration data
    pDisp->bIdle = false;
    pDisp->nfact = 9;
    pDisp->totalNumEntries = 1000;
    pDisp->rcvdEntries     = 0;
    pDisp->startReadPos = 0;
    Mbox::sendMsg(msgType::eStoredBwFiltVals, Mbox::eDispTask, Mbox::eDispTask);

    zassert_equal(pDisp->pInMbox->count(), 1, "pInMbox->count() != 1");

    pDisp->task();  
    zassert_equal(pDisp->pInMbox->count(), 0, "pInMbox->count() != 0");
    zassert_equal(pDisp->rcvdEntries, 0, "rcvdEntries != 0");
    zassert_equal(pDisp->startReadPos, 0, "startReadPos != 0");
    zassert_equal(pDisp->startWritePos, 0, "startWritePos != 0");
    zassert_equal(pDisp->sentEntries, 0, "sentEntries != 0");

    zassert_equal(pDisp->detrendVars.sumX, 0.0, "detrendVars.sumX != 0");
    zassert_equal(pDisp->detrendVars.sumY, 0.0, "detrendVars.sumY != 0");
    zassert_equal(pDisp->detrendVars.sumXY, 0.0, "detrendVars.sumXY != 0");
    zassert_equal(pDisp->detrendVars.sumX2, 0.0, "detrendVars.sumX2 != 0");

    zassert_equal(pDisp->dVIdx, 1, "dVIdx != 0");
    zassert_equal(pDisp->V_last, 0.0, "V_last != 0");
    zassert_equal(pDisp->S_last, 0.0, "S_last != 0");
    zassert_equal(pDisp->IN_last, 0.0, "IN_last != 0");

    pElem = pAdxlStoreMbox->read();
    //printk("pElem->mtype = %d\n\n", pElem->mtype);
    zassert_equal(pElem->mtype, msgType::eBwFiltData, "Rcvd mtype != msgType::eBwFiltData");
    zassert_equal(pElem->from, Mbox::eDispTask, "Rcvd sender != Mbox::eDispTask");

    progress = *reinterpret_cast<struct AdxlRawDataProgress*>(pElem->getBuffer());;
    zassert_equal(progress.startRd, 0, "progress.startRd != 0");
    zassert_equal(progress.rcvdEntries, 0, "progress.rcvdEntries != 0");
    zassert_equal(progress.requestedEntries, 100, "progress.requestedEntries != 100");

    pElem->free();
    delete pAdxlStoreMbox;
	delete pDisp; 
}

// Test receive msg eBwFiltData
ZTEST(tests_disp_disptask, test_disp_disptask_rcv_msg_eBwFiltData)
{
    PoolElem* pElem;
    struct AdxlRawDataProgress progress;

	Mbox* pAdxlStoreMbox = new Mbox("AdxlStoreMbox");
    Mbox::registerMbox(pAdxlStoreMbox, Mbox::eAdxlStore);
	zassert_not_null(pAdxlStoreMbox, "pAdxlStoreMbox = NULL");

	DispTask* pDisp = new DispTask("disp", true);
	zassert_not_null(pDisp, "memory alloc failed");
	zassert_not_null(pDisp->pInMbox, "pInMbox = NULL");   
    zassert_equal(pDisp->pInMbox->count(), 0, "pInMbox->count() != 0");
    zassert_equal(pDisp->pInMbox, Mbox::getMboxById(Mbox::eDispTask),
                  "Error getting mailbox by id");

    // int16_t x;  //!< measured x value of the acceleration data
    // int16_t y;  //!< measured y value of the acceleration data
    // int16_t z;  //!< measured z value of the acceleration data
    struct adxl372_xyz_accel_data data[11] = {  {1, 1, 1},
                                            {2, 2, 2},
                                            {-2, -2, -2},
                                            {-1, -1, -1},
                                            {1, 1, 1},
                                            {2, 2, 2},
                                            {-2, -2, -2},
                                            {-1, -1, -1},
                                            {1, 1, 1},
                                            {2, 2, 2} }; 
    pDisp->bIdle = false;
    pDisp->nfact = 9;
    pDisp->totalNumEntries = 1000;
    pDisp->rcvdEntries     = 100;
    pDisp->requestedEntries = 10;
    pDisp->startReadPos = 0;
    Mbox::sendMsg(msgType::eBwFiltData, Mbox::eDispTask, Mbox::eDispTask, (uint8_t*)&data, sizeof(data));

    zassert_equal(pDisp->pInMbox->count(), 1, "pInMbox->count() != 1");

    vectord b_coeff = { 0.95806, -2.87418,  2.87418, -0.95806 };
	vectord a_coeff = { 1.00000, -2.91432,  2.83228, -0.91788 };
	vectord zi = { -0.95806, 1.91612, -0.95806, 0.0 };
    vectord x = { 1,-.2,.3,1,-.5,1,-.7,.8,-.9,.5 };
	vectord y = {  };

	CFilter::inst()->a = &a_coeff[0];
	CFilter::inst()->b = &b_coeff[0];
	CFilter::inst()->z = &zi[0];
	CFilter::inst()->X_old = {-0.70000,  0.80000,  -0.90000,  0.50000};
	CFilter::inst()->Y_old = {-1.37864,  0.18662,  -1.44279,  0.03703};
	CFilter::inst()->filter_order = 4;

    pDisp->task();  
    zassert_equal(pDisp->pInMbox->count(), 0, "pInMbox->count() != 0");
    zassert_equal(pDisp->rcvdEntries, 110, "rcvdEntries != 110");
    
    pElem = pAdxlStoreMbox->read();
    //printk("pElem->mtype = %d\n\n", pElem->mtype);
    zassert_equal(pElem->mtype, msgType::eStoreSResult, "Rcvd mtype != msgType::eStoreSResult");
    zassert_equal(pElem->from, Mbox::eDispTask, "Rcvd sender != Mbox::eDispTask");

    pElem = pAdxlStoreMbox->read();
    //printk("pElem->mtype = %d\n\n", pElem->mtype);
    zassert_equal(pElem->mtype, msgType::eBwFiltData, "Rcvd mtype != msgType::eBwFiltData");
    zassert_equal(pElem->from, Mbox::eDispTask, "Rcvd sender != Mbox::eDispTask");
    
    progress = *reinterpret_cast<struct AdxlRawDataProgress*>(pElem->getBuffer());;
    zassert_equal(progress.startRd, 0, "progress.startRd != 0");
    zassert_equal(progress.rcvdEntries, 110, "progress.rcvdEntries != 110");
    zassert_equal(progress.requestedEntries, 100, "progress.requestedEntries != 100");

    pElem->free();
    delete pAdxlStoreMbox;
	delete pDisp; 
}

// Test receive msg eBwFiltData
ZTEST(tests_disp_disptask, test_disp_disptask_rcv_msg_eBwFiltData2)
{
    PoolElem* pElem;
    struct AdxlRawDataProgress progress;

	Mbox* pAdxlStoreMbox = new Mbox("AdxlStoreMbox");
    Mbox::registerMbox(pAdxlStoreMbox, Mbox::eAdxlStore);
	zassert_not_null(pAdxlStoreMbox, "pAdxlStoreMbox = NULL");

	DispTask* pDisp = new DispTask("disp", true);
	zassert_not_null(pDisp, "memory alloc failed");
	zassert_not_null(pDisp->pInMbox, "pInMbox = NULL");   
    zassert_equal(pDisp->pInMbox->count(), 0, "pInMbox->count() != 0");
    zassert_equal(pDisp->pInMbox, Mbox::getMboxById(Mbox::eDispTask),
                  "Error getting mailbox by id");

    // int16_t x;  //!< measured x value of the acceleration data
    // int16_t y;  //!< measured y value of the acceleration data
    // int16_t z;  //!< measured z value of the acceleration data
    struct adxl372_xyz_accel_data data[11] = {  {1, 1, 1},
                                            {2, 2, 2},
                                            {-2, -2, -2},
                                            {-1, -1, -1},
                                            {1, 1, 1},
                                            {2, 2, 2},
                                            {-2, -2, -2},
                                            {-1, -1, -1},
                                            {1, 1, 1},
                                            {2, 2, 2} }; 
    pDisp->bIdle = false;
    pDisp->nfact = 9;
    pDisp->totalNumEntries = 1000;
    pDisp->rcvdEntries     = 990;
    pDisp->requestedEntries = 10;
    pDisp->startReadPos = 0;
    Mbox::sendMsg(msgType::eBwFiltData, Mbox::eDispTask, Mbox::eDispTask, (uint8_t*)&data, sizeof(data));

    zassert_equal(pDisp->pInMbox->count(), 1, "pInMbox->count() != 1");

    vectord b_coeff = { 0.95806, -2.87418,  2.87418, -0.95806 };
	vectord a_coeff = { 1.00000, -2.91432,  2.83228, -0.91788 };
	vectord zi = { -0.95806, 1.91612, -0.95806, 0.0 };
    vectord x = { 1,-.2,.3,1,-.5,1,-.7,.8,-.9,.5 };
	vectord y = {  };

	CFilter::inst()->a = &a_coeff[0];
	CFilter::inst()->b = &b_coeff[0];
	CFilter::inst()->z = &zi[0];
	CFilter::inst()->X_old = {-0.70000,  0.80000,  -0.90000,  0.50000};
	CFilter::inst()->Y_old = {-1.37864,  0.18662,  -1.44279,  0.03703};
	CFilter::inst()->filter_order = 4;

    pDisp->task();  
    zassert_equal(pDisp->pInMbox->count(), 0, "pInMbox->count() != 0");
    zassert_equal(pDisp->rcvdEntries, 110, "rcvdEntries != 110");
    
    pElem = pAdxlStoreMbox->read();
    //printk("pElem->mtype = %d\n\n", pElem->mtype);
    zassert_equal(pElem->mtype, msgType::eStoreSResult, "Rcvd mtype != msgType::eStoreSResult");
    zassert_equal(pElem->from, Mbox::eDispTask, "Rcvd sender != Mbox::eDispTask");

    pElem = pAdxlStoreMbox->read();
    //printk("pElem->mtype = %d\n\n", pElem->mtype);
    zassert_equal(pElem->mtype, msgType::eBwFiltData, "Rcvd mtype != msgType::eBwFiltData");
    zassert_equal(pElem->from, Mbox::eDispTask, "Rcvd sender != Mbox::eDispTask");
    
    progress = *reinterpret_cast<struct AdxlRawDataProgress*>(pElem->getBuffer());;
    zassert_equal(progress.startRd, 0, "progress.startRd != 0");
    zassert_equal(progress.rcvdEntries, 110, "progress.rcvdEntries != 110");
    zassert_equal(progress.requestedEntries, 100, "progress.requestedEntries != 100");

    pElem->free();
    delete pAdxlStoreMbox;
	delete pDisp; 
}

// Test function calcDisplacement
ZTEST(tests_disp_disptask, test_disp_disptask_rcv_msg_calcDisplacement)
{
	DispTask* pDisp = new DispTask("disp", true);
	zassert_not_null(pDisp, "memory alloc failed");
	zassert_not_null(pDisp->pInMbox, "pInMbox = NULL");   
    zassert_equal(pDisp->pInMbox->count(), 0, "pInMbox->count() != 0");
    zassert_equal(pDisp->pInMbox, Mbox::getMboxById(Mbox::eDispTask),
                  "Error getting mailbox by id");

    vectord data = {1,2,3,4,5,6,7,8,9,0,1,2,3,4,5,6,7,8,9,0}; 
    pDisp->bIdle = false;
    pDisp->nfact = 9;
    pDisp->totalNumEntries = 2000;
    pDisp->rcvdEntries     = 1000;
    pDisp->requestedEntries = 10;
    pDisp->startReadPos = 0;
    Mbox::sendMsg(msgType::eBwFiltData, Mbox::eDispTask, Mbox::eDispTask, (uint8_t*)&data, sizeof(data));

    zassert_equal(pDisp->pInMbox->count(), 1, "pInMbox->count() != 1");

    vectord b_coeff = { 0.95806, -2.87418,  2.87418, -0.95806 };
	vectord a_coeff = { 1.00000, -2.91432,  2.83228, -0.91788 };
	vectord zi = { -0.95806, 1.91612, -0.95806, 0.0 };
    vectord x = { 1,-.2,.3,1,-.5,1,-.7,.8,-.9,.5 };
	vectord y = {  };

	CFilter::inst()->a = &a_coeff[0];
	CFilter::inst()->b = &b_coeff[0];
	CFilter::inst()->z = &zi[0];
	CFilter::inst()->X_old = {-0.70000,  0.80000,  -0.90000,  0.50000};
	CFilter::inst()->Y_old = {-1.37864,  0.18662,  -1.44279,  0.03703};
	CFilter::inst()->filter_order = 4;

    vectord vout(20), sout(20);

    pDisp->calcDisplacement(data, vout, sout);  
    //zassert_equal(pDisp->pInMbox->count(), 0, "pInMbox->count() != 0");
    //zassert_equal(pDisp->rcvdEntries, 110, "rcvdEntries != 110");
    
	delete pDisp; 
}

// Test receive msg eStoredSResultVals
ZTEST(tests_disp_disptask, test_disp_disptask_rcv_msg_eStoredSResultVals)
{
    PoolElem* pElem;
    struct AdxlRawDataProgress progress;

	Mbox* pAdxlStoreMbox = new Mbox("AdxlStoreMbox");
    Mbox::registerMbox(pAdxlStoreMbox, Mbox::eAdxlStore);
	zassert_not_null(pAdxlStoreMbox, "pAdxlStoreMbox = NULL");

	DispTask* pDisp = new DispTask("disp", true);
	zassert_not_null(pDisp, "memory alloc failed");
	zassert_not_null(pDisp->pInMbox, "pInMbox = NULL");   
    zassert_equal(pDisp->pInMbox->count(), 0, "pInMbox->count() != 0");
    zassert_equal(pDisp->pInMbox, Mbox::getMboxById(Mbox::eDispTask),
                  "Error getting mailbox by id");

    pDisp->bIdle = false;
    pDisp->nfact = 9;
    pDisp->totalNumEntries = 1000;
    pDisp->rcvdEntries     = 0;
    pDisp->startReadPos = 0;
    Mbox::sendMsg(msgType::eStoredSResultVals, Mbox::eDispTask, Mbox::eDispTask);

    zassert_equal(pDisp->pInMbox->count(), 1, "pInMbox->count() != 1");

    pDisp->task();  
    zassert_equal(pDisp->pInMbox->count(), 0, "pInMbox->count() != 0");
    zassert_equal(pDisp->rcvdEntries, 0, "rcvdEntries != 0");
    zassert_equal(pDisp->startReadPos, 0, "startReadPos != 0");
    zassert_equal(pDisp->startWritePos, 0, "startWritePos != 0");
    zassert_equal(pDisp->sentEntries, 0, "sentEntries != 0");

    zassert_equal(pDisp->detrendIdx, 0, "detrendIdx != 0");
    zassert_equal(pDisp->dispMin, 0.0, "dispMin != 0");
    zassert_equal(pDisp->dispMax, 0.0, "dispMax != 0");
    zassert_equal(pDisp->dispP2P, 0.0, "dispP2P != 0");

    pElem = pAdxlStoreMbox->read();
    //printk("pElem->mtype = %d\n\n", pElem->mtype);
    zassert_equal(pElem->mtype, msgType::eSResultData, "Rcvd mtype != msgType::eSResultData");
    zassert_equal(pElem->from, Mbox::eDispTask, "Rcvd sender != Mbox::eDispTask");

    progress = *reinterpret_cast<struct AdxlRawDataProgress*>(pElem->getBuffer());;
    zassert_equal(progress.startRd, 0, "progress.startRd != 0");
    zassert_equal(progress.rcvdEntries, 0, "progress.rcvdEntries != 0");
    zassert_equal(progress.requestedEntries, 100, "progress.requestedEntries != 100");

    pElem->free();
    delete pAdxlStoreMbox;
	delete pDisp; 
}

// Test receive msg eSResultData
ZTEST(tests_disp_disptask, test_disp_disptask_rcv_msg_eSResultData)
{
    PoolElem* pElem;
    struct AdxlRawDataProgress progress;

	Mbox* pAdxlStoreMbox = new Mbox("AdxlStoreMbox");
    Mbox::registerMbox(pAdxlStoreMbox, Mbox::eAdxlStore);
	zassert_not_null(pAdxlStoreMbox, "pAdxlStoreMbox = NULL");

	DispTask* pDisp = new DispTask("disp", true);
	zassert_not_null(pDisp, "memory alloc failed");
	zassert_not_null(pDisp->pInMbox, "pInMbox = NULL");   
    zassert_equal(pDisp->pInMbox->count(), 0, "pInMbox->count() != 0");
    zassert_equal(pDisp->pInMbox, Mbox::getMboxById(Mbox::eDispTask),
                  "Error getting mailbox by id");

    // int16_t x;  //!< measured x value of the acceleration data
    // int16_t y;  //!< measured y value of the acceleration data
    // int16_t z;  //!< measured z value of the acceleration data
    struct adxl372_xyz_accel_data data[11] = {  {1, 1, 1},
                                            {2, 2, 2},
                                            {-2, -2, -2},
                                            {-1, -1, -1},
                                            {1, 1, 1},
                                            {2, 2, 2},
                                            {-2, -2, -2},
                                            {-1, -1, -1},
                                            {1, 1, 1},
                                            {2, 2, 2} }; 
    pDisp->bIdle = false;
    pDisp->nfact = 9;
    pDisp->totalNumEntries = 1000;
    pDisp->rcvdEntries     = 100;
    pDisp->requestedEntries = 10;
    pDisp->startReadPos = 0;
    Mbox::sendMsg(msgType::eSResultData, Mbox::eDispTask, Mbox::eDispTask, (uint8_t*)&data, sizeof(data));

    zassert_equal(pDisp->pInMbox->count(), 1, "pInMbox->count() != 1");

    vectord b_coeff = { 0.95806, -2.87418,  2.87418, -0.95806 };
	vectord a_coeff = { 1.00000, -2.91432,  2.83228, -0.91788 };
	vectord zi = { -0.95806, 1.91612, -0.95806, 0.0 };
    vectord x = { 1,-.2,.3,1,-.5,1,-.7,.8,-.9,.5 };
	vectord y = {  };

	CFilter::inst()->a = &a_coeff[0];
	CFilter::inst()->b = &b_coeff[0];
	CFilter::inst()->z = &zi[0];
	CFilter::inst()->X_old = {-0.70000,  0.80000,  -0.90000,  0.50000};
	CFilter::inst()->Y_old = {-1.37864,  0.18662,  -1.44279,  0.03703};
	CFilter::inst()->filter_order = 4;

    pDisp->task();  
    zassert_equal(pDisp->pInMbox->count(), 0, "pInMbox->count() != 0");
    zassert_equal(pDisp->rcvdEntries, 110, "rcvdEntries != 110");
    
    pElem = pAdxlStoreMbox->read();
    //printk("pElem->mtype = %d\n\n", pElem->mtype);
    zassert_equal(pElem->mtype, msgType::eStoreDResult, "Rcvd mtype != msgType::eStoreDResult");
    zassert_equal(pElem->from, Mbox::eDispTask, "Rcvd sender != Mbox::eDispTask");

    pElem = pAdxlStoreMbox->read();
    //printk("pElem->mtype = %d\n\n", pElem->mtype);
    zassert_equal(pElem->mtype, msgType::eSResultData, "Rcvd mtype != msgType::eSResultData");
    zassert_equal(pElem->from, Mbox::eDispTask, "Rcvd sender != Mbox::eDispTask");
    
    progress = *reinterpret_cast<struct AdxlRawDataProgress*>(pElem->getBuffer());;
    zassert_equal(progress.startRd, 0, "progress.startRd != 0");
    zassert_equal(progress.rcvdEntries, 110, "progress.rcvdEntries != 110");
    zassert_equal(progress.requestedEntries, 100, "progress.requestedEntries != 100");

    pElem->free();
    delete pAdxlStoreMbox;
	delete pDisp; 
}

// Test receive msg eSResultData
ZTEST(tests_disp_disptask, test_disp_disptask_rcv_msg_eSResultData2)
{
    PoolElem* pElem;
    stDispStatistics stat;

	Mbox* pAdxlStoreMbox = new Mbox("AdxlStoreMbox");
    Mbox::registerMbox(pAdxlStoreMbox, Mbox::eAdxlStore);
	zassert_not_null(pAdxlStoreMbox, "pAdxlStoreMbox = NULL");

	Mbox* pDataMgrMbox = new Mbox("DataMgrMbox");
    Mbox::registerMbox(pDataMgrMbox, Mbox::eDataManager);
	zassert_not_null(pDataMgrMbox, "pDataMgrMbox = NULL");

	DispTask* pDisp = new DispTask("disp", true);
	zassert_not_null(pDisp, "memory alloc failed");
	zassert_not_null(pDisp->pInMbox, "pInMbox = NULL");   
    zassert_equal(pDisp->pInMbox->count(), 0, "pInMbox->count() != 0");
    zassert_equal(pDisp->pInMbox, Mbox::getMboxById(Mbox::eDispTask),
                  "Error getting mailbox by id");

    // int16_t x;  //!< measured x value of the acceleration data
    // int16_t y;  //!< measured y value of the acceleration data
    // int16_t z;  //!< measured z value of the acceleration data
    struct adxl372_xyz_accel_data data[11] = {  {1, 1, 1},
                                            {2, 2, 2},
                                            {-2, -2, -2},
                                            {-1, -1, -1},
                                            {1, 1, 1},
                                            {2, 2, 2},
                                            {-2, -2, -2},
                                            {-1, -1, -1},
                                            {1, 1, 1},
                                            {2, 2, 2} }; 
    pDisp->bIdle = false;
    pDisp->nfact = 9;
    pDisp->totalNumEntries = 1000;
    pDisp->rcvdEntries     = 990;
    pDisp->requestedEntries = 10;
    pDisp->startReadPos = 0;
    Mbox::sendMsg(msgType::eSResultData, Mbox::eDispTask, Mbox::eDispTask, (uint8_t*)&data, sizeof(data));

    zassert_equal(pDisp->pInMbox->count(), 1, "pInMbox->count() != 1");

    vectord b_coeff = { 0.95806, -2.87418,  2.87418, -0.95806 };
	vectord a_coeff = { 1.00000, -2.91432,  2.83228, -0.91788 };
	vectord zi = { -0.95806, 1.91612, -0.95806, 0.0 };
    vectord x = { 1,-.2,.3,1,-.5,1,-.7,.8,-.9,.5 };
	vectord y = {  };

	CFilter::inst()->a = &a_coeff[0];
	CFilter::inst()->b = &b_coeff[0];
	CFilter::inst()->z = &zi[0];
	CFilter::inst()->X_old = {-0.70000,  0.80000,  -0.90000,  0.50000};
	CFilter::inst()->Y_old = {-1.37864,  0.18662,  -1.44279,  0.03703};
	CFilter::inst()->filter_order = 4;

    pDisp->b1 = 1.0;
    pDisp->dispMin = 0.001;
    pDisp->dispMax = -0.001;
    pDisp->dispP2P = 0;

    pDisp->task();  
    zassert_equal(pDisp->pInMbox->count(), 0, "pInMbox->count() != 0");
    zassert_equal(pDisp->rcvdEntries, 1000, "rcvdEntries != 1000");
    
    pElem = pAdxlStoreMbox->read();
    //printk("pElem->mtype = %d\n\n", pElem->mtype);
    zassert_equal(pElem->mtype, msgType::eStoreDResult, "Rcvd mtype != msgType::eStoreDResult");
    zassert_equal(pElem->from, Mbox::eDispTask, "Rcvd sender != Mbox::eDispTask");

    pElem = pDataMgrMbox->read();
    //printk("pElem->mtype = %d\n\n", pElem->mtype);
    zassert_equal(pElem->mtype, msgType::eDispStatistics, "Rcvd mtype != msgType::eDispStatistics");
    zassert_equal(pElem->from, Mbox::eDispTask, "Rcvd sender != Mbox::eDispTask");
    
    stat = *reinterpret_cast<stDispStatistics*>(pElem->getBuffer());
    //printk("stat = %f, %f, %f\n\n", stat.min, stat.max, stat.p2p);
    zassert_equal(stat.min, 1.0, "stat.min != 1");
    zassert_equal(stat.max, -1.0, "stat.max != -1");
    zassert_equal(stat.p2p, 0.0, "stat.p2p != 0");

    pElem->free();
    delete pAdxlStoreMbox;
    delete pDataMgrMbox;
	delete pDisp; 
}

// Test receive msg eSResultData
ZTEST(tests_disp_disptask, test_disp_disptask_rcv_msg_eSResultData3)
{
    PoolElem* pElem;
    stDispStatistics stat;
 
	Mbox* pAdxlStoreMbox = new Mbox("AdxlStoreMbox");
    Mbox::registerMbox(pAdxlStoreMbox, Mbox::eAdxlStore);
	zassert_not_null(pAdxlStoreMbox, "pAdxlStoreMbox = NULL");

	Mbox* pDataMgrMbox = new Mbox("DataMgrMbox");
    Mbox::registerMbox(pDataMgrMbox, Mbox::eDataManager);
	zassert_not_null(pDataMgrMbox, "pDataMgrMbox = NULL");

	DispTask* pDisp = new DispTask("disp", true);
	zassert_not_null(pDisp, "memory alloc failed");
	zassert_not_null(pDisp->pInMbox, "pInMbox = NULL");   
    zassert_equal(pDisp->pInMbox->count(), 0, "pInMbox->count() != 0");
    zassert_equal(pDisp->pInMbox, Mbox::getMboxById(Mbox::eDispTask),
                  "Error getting mailbox by id");

    // int16_t x;  //!< measured x value of the acceleration data
    // int16_t y;  //!< measured y value of the acceleration data
    // int16_t z;  //!< measured z value of the acceleration data
    struct adxl372_xyz_accel_data data[11] = {  {1, 1, 1},
                                            {2, 2, 2},
                                            {-2, -2, -2},
                                            {-1, -1, -1},
                                            {1, 1, 1},
                                            {2, 2, 2},
                                            {-2, -2, -2},
                                            {-1, -1, -1},
                                            {1, 1, 1},
                                            {2, 2, 2} }; 
    pDisp->bIdle = false;
    pDisp->nfact = 9;
    pDisp->totalNumEntries = 2000;
    pDisp->rcvdEntries     = 1000;
    pDisp->requestedEntries = 10;
    pDisp->startReadPos = 0;
    pDisp->detrendIdx = 1000;
    Mbox::sendMsg(msgType::eSResultData, Mbox::eDispTask, Mbox::eDispTask, (uint8_t*)&data, sizeof(data));

    zassert_equal(pDisp->pInMbox->count(), 1, "pInMbox->count() != 1");

    vectord b_coeff = { 0.95806, -2.87418,  2.87418, -0.95806 };
	vectord a_coeff = { 1.00000, -2.91432,  2.83228, -0.91788 };
	vectord zi = { -0.95806, 1.91612, -0.95806, 0.0 };
    vectord x = { 1,-.2,.3,1,-.5,1,-.7,.8,-.9,.5 };
	vectord y = {  };

	CFilter::inst()->a = &a_coeff[0];
	CFilter::inst()->b = &b_coeff[0];
	CFilter::inst()->z = &zi[0];
	CFilter::inst()->X_old = {-0.70000,  0.80000,  -0.90000,  0.50000};
	CFilter::inst()->Y_old = {-1.37864,  0.18662,  -1.44279,  0.03703};
	CFilter::inst()->filter_order = 4;

    pDisp->b1 = 1.0;
    pDisp->dispMin = 0.001;
    pDisp->dispMax = -0.001;
    pDisp->dispP2P = 0;

    pDisp->task();  
    zassert_equal(pDisp->pInMbox->count(), 0, "pInMbox->count() != 0");
    zassert_equal(pDisp->rcvdEntries, 1000, "rcvdEntries != 1000");
    
    pElem = pAdxlStoreMbox->read();
    //printk("pElem->mtype = %d\n\n", pElem->mtype);
    zassert_equal(pElem->mtype, msgType::eStoreDResult, "Rcvd mtype != msgType::eStoreDResult");
    zassert_equal(pElem->from, Mbox::eDispTask, "Rcvd sender != Mbox::eDispTask");

    pElem = pDataMgrMbox->read();
    //printk("pElem->mtype = %d\n\n", pElem->mtype);
    zassert_equal(pElem->mtype, msgType::eDispStatistics, "Rcvd mtype != msgType::eDispStatistics");
    zassert_equal(pElem->from, Mbox::eDispTask, "Rcvd sender != Mbox::eDispTask");
    
    stat = *reinterpret_cast<stDispStatistics*>(pElem->getBuffer());
    //printk("stat = %f, %f, %f\n\n", stat.min, stat.max, stat.p2p);
    zassert_equal(stat.min, 1.0, "stat.min != 1");
    zassert_equal(stat.max, -1.0, "stat.max != -1");
    zassert_equal(stat.p2p, 0.0, "stat.p2p != 0");

    pElem->free();
    delete pAdxlStoreMbox;
    delete pDataMgrMbox;
	delete pDisp; 
}

// Test receive msg eSResultData
ZTEST(tests_disp_disptask, test_disp_disptask_rcv_msg_eSResultData4)
{
    PoolElem* pElem;
    stDispStatistics stat;
 
	Mbox* pAdxlStoreMbox = new Mbox("AdxlStoreMbox");
    Mbox::registerMbox(pAdxlStoreMbox, Mbox::eAdxlStore);
	zassert_not_null(pAdxlStoreMbox, "pAdxlStoreMbox = NULL");

	Mbox* pDataMgrMbox = new Mbox("DataMgrMbox");
    Mbox::registerMbox(pDataMgrMbox, Mbox::eDataManager);
	zassert_not_null(pDataMgrMbox, "pDataMgrMbox = NULL");

	DispTask* pDisp = new DispTask("disp", true);
	zassert_not_null(pDisp, "memory alloc failed");
	zassert_not_null(pDisp->pInMbox, "pInMbox = NULL");   
    zassert_equal(pDisp->pInMbox->count(), 0, "pInMbox->count() != 0");
    zassert_equal(pDisp->pInMbox, Mbox::getMboxById(Mbox::eDispTask),
                  "Error getting mailbox by id");

    // int16_t x;  //!< measured x value of the acceleration data
    // int16_t y;  //!< measured y value of the acceleration data
    // int16_t z;  //!< measured z value of the acceleration data
    struct adxl372_xyz_accel_data data[11] = {  {1, 1, 1},
                                            {2, 2, 2},
                                            {-2, -2, -2},
                                            {-1, -1, -1},
                                            {1, 1, 1},
                                            {2, 2, 2},
                                            {-2, -2, -2},
                                            {-1, -1, -1},
                                            {1, 1, 1},
                                            {2, 2, 2} }; 
    pDisp->bIdle = false;
    pDisp->nfact = 9;
    pDisp->totalNumEntries = 2000;
    pDisp->rcvdEntries     = 1000;
    pDisp->requestedEntries = 10;
    pDisp->startReadPos = 0;
    pDisp->detrendIdx = 100;
    Mbox::sendMsg(msgType::eSResultData, Mbox::eDispTask, Mbox::eDispTask, (uint8_t*)&data, sizeof(data));

    zassert_equal(pDisp->pInMbox->count(), 1, "pInMbox->count() != 1");

    vectord b_coeff = { 0.95806, -2.87418,  2.87418, -0.95806 };
	vectord a_coeff = { 1.00000, -2.91432,  2.83228, -0.91788 };
	vectord zi = { -0.95806, 1.91612, -0.95806, 0.0 };
    vectord x = { 1,-.2,.3,1,-.5,1,-.7,.8,-.9,.5 };
	vectord y = {  };

	CFilter::inst()->a = &a_coeff[0];
	CFilter::inst()->b = &b_coeff[0];
	CFilter::inst()->z = &zi[0];
	CFilter::inst()->X_old = {-0.70000,  0.80000,  -0.90000,  0.50000};
	CFilter::inst()->Y_old = {-1.37864,  0.18662,  -1.44279,  0.03703};
	CFilter::inst()->filter_order = 4;

    pDisp->b1 = 1.0;
    pDisp->dispMin = 0.001;
    pDisp->dispMax = -0.001;
    pDisp->dispP2P = 0;

    pDisp->task();  
    zassert_equal(pDisp->pInMbox->count(), 0, "pInMbox->count() != 0");
    zassert_equal(pDisp->rcvdEntries, 1000, "rcvdEntries != 1000");
    
    pElem = pAdxlStoreMbox->read();
    //printk("pElem->mtype = %d\n\n", pElem->mtype);
    zassert_equal(pElem->mtype, msgType::eStoreDResult, "Rcvd mtype != msgType::eStoreDResult");
    zassert_equal(pElem->from, Mbox::eDispTask, "Rcvd sender != Mbox::eDispTask");

    pElem = pDataMgrMbox->read();
    //printk("pElem->mtype = %d\n\n", pElem->mtype);
    zassert_equal(pElem->mtype, msgType::eDispStatistics, "Rcvd mtype != msgType::eDispStatistics");
    zassert_equal(pElem->from, Mbox::eDispTask, "Rcvd sender != Mbox::eDispTask");
    
    stat = *reinterpret_cast<stDispStatistics*>(pElem->getBuffer());
    //printk("stat = %f, %f, %f\n\n", stat.min, stat.max, stat.p2p);
    zassert_equal(stat.min, 1.0, "stat.min != 1");
    zassert_equal(stat.max, -1.0, "stat.max != -1");
    zassert_equal(stat.p2p, 0.0, "stat.p2p != 0");

    pElem->free();
    delete pAdxlStoreMbox;
    delete pDataMgrMbox;
	delete pDisp; 
}

// Test receive msg eStoredDResultVals
ZTEST(tests_disp_disptask, test_disp_disptask_rcv_msg_eStoredDResultVals)
{
    PoolElem* pElem;

	Mbox* pHttpMbox = new Mbox("HttpMbox");
    Mbox::registerMbox(pHttpMbox, Mbox::eHttpConnector);
	zassert_not_null(pHttpMbox, "pHttpMbox = NULL");

	DispTask* pDisp = new DispTask("disp", true);
	zassert_not_null(pDisp, "memory alloc failed");
	zassert_not_null(pDisp->pInMbox, "pInMbox = NULL");   
    zassert_equal(pDisp->pInMbox->count(), 0, "pInMbox->count() != 0");
    zassert_equal(pDisp->pInMbox, Mbox::getMboxById(Mbox::eDispTask),
                  "Error getting mailbox by id");

    pDisp->bIdle = false;
    pDisp->have_http_connector = true;
    Mbox::sendMsg(msgType::eStoredDResultVals, Mbox::eDispTask, Mbox::eDispTask);

    zassert_equal(pDisp->pInMbox->count(), 1, "pInMbox->count() != 1");

    pDisp->task();  
    zassert_equal(pDisp->pInMbox->count(), 0, "pInMbox->count() != 0");
    zassert_true(pDisp->bIdle, "bIdle != true");
    
    pElem = pHttpMbox->read();
    //printk("pElem->mtype = %d\n\n", pElem->mtype);
    zassert_equal(pElem->mtype, msgType::eSendDispDataReq, "Rcvd mtype != msgType::eSendDispDataReq");
    zassert_equal(pElem->from, Mbox::eDispTask, "Rcvd sender != Mbox::eDispTask");
    
    pElem->free();
    delete pHttpMbox;
	delete pDisp; 
}

// Test receive msg eStoredDResultVals
ZTEST(tests_disp_disptask, test_disp_disptask_rcv_msg_eStoredDResultVals2)
{
	DispTask* pDisp = new DispTask("disp", true);
	zassert_not_null(pDisp, "memory alloc failed");
	zassert_not_null(pDisp->pInMbox, "pInMbox = NULL");   
    zassert_equal(pDisp->pInMbox->count(), 0, "pInMbox->count() != 0");
    zassert_equal(pDisp->pInMbox, Mbox::getMboxById(Mbox::eDispTask),
                  "Error getting mailbox by id");

    pDisp->bIdle = false;
    pDisp->have_http_connector = false;
    Mbox::sendMsg(msgType::eStoredDResultVals, Mbox::eDispTask, Mbox::eDispTask);

    zassert_equal(pDisp->pInMbox->count(), 1, "pInMbox->count() != 1");

    pDisp->task();  
    zassert_equal(pDisp->pInMbox->count(), 0, "pInMbox->count() != 0");
    zassert_true(pDisp->bIdle, "bIdle != true");
    
	delete pDisp; 
}

//Test receive unexpected msg 
ZTEST(tests_disp_disptask, test_disp_disptask_rcv_msg_unexpected)
{
	DispTask* pDisp = new DispTask("disp", true);
	zassert_not_null(pDisp, "memory alloc failed");
	zassert_not_null(pDisp->pInMbox, "pInMbox = NULL");   
    zassert_equal(pDisp->pInMbox->count(), 0, "pInMbox->count() != 0");
    zassert_equal(pDisp->pInMbox, Mbox::getMboxById(Mbox::eDispTask),
                  "Error getting mailbox by id");

    Mbox::sendMsg(msgType::eRmsReport, Mbox::eDispTask, Mbox::eDispTask);

    zassert_equal(pDisp->pInMbox->count(), 1, "pInMbox->count() != 1");

    pDisp->task();  

    zassert_equal(pDisp->pInMbox->count(), 0, "pInMbox->count() != 0");

	delete pDisp; 
}

//Test receive no msg 
ZTEST(tests_disp_disptask, test_disp_disptask_rcv_msg_none)
{
	DispTask* pDisp = new DispTask("disp", true);
	zassert_not_null(pDisp, "memory alloc failed");
	zassert_not_null(pDisp->pInMbox, "pInMbox = NULL");   
    zassert_equal(pDisp->pInMbox->count(), 0, "pInMbox->count() != 0");
    zassert_equal(pDisp->pInMbox, Mbox::getMboxById(Mbox::eDispTask),
                  "Error getting mailbox by id");

    pDisp->pInMbox->write(NULL);
    
    pDisp->task();  

    zassert_equal(pDisp->pInMbox->count(), 0, "pInMbox->count() != 0");
	delete pDisp; 
}

// Test function filt_dump
ZTEST(tests_disp_disptask, test_disp_disptask_filt_dump)
{
    PoolElem* pElem;

	DispTask* pDisp = new DispTask("disp", true);
	zassert_not_null(pDisp, "memory alloc failed");
	zassert_not_null(pDisp->pInMbox, "pInMbox = NULL");   
    zassert_equal(pDisp->pInMbox->count(), 0, "pInMbox->count() != 0");
    zassert_equal(pDisp->pInMbox, Mbox::getMboxById(Mbox::eDispTask),
                  "Error getting mailbox by id");

    pDisp->filt_dump(NULL, 0, NULL);  
    zassert_equal(pDisp->pInMbox->count(), 1, "pInMbox->count() != 1");
    
    pElem = pDisp->pInMbox->read();
    zassert_equal(pDisp->pInMbox->count(), 0, "pInMbox->count() != 0");
    //printk("pElem->mtype = %d\n\n", pElem->mtype);
    zassert_equal(pElem->mtype, msgType::eCalcDisplacementReq, "Rcvd mtype != msgType::eCalcDisplacementReq");
    zassert_equal(pElem->from, Mbox::eDispTask, "Rcvd sender != Mbox::eDispTask");
    
    pElem->free();
	delete pDisp; 
}

// Test function reqFiltResultWrite
ZTEST(tests_disp_disptask, test_disp_disptask_reqFiltResultWrite)
{
	DispTask* pDisp = new DispTask("disp", true);
	zassert_not_null(pDisp, "memory alloc failed");
	zassert_not_null(pDisp->pInMbox, "pInMbox = NULL");   
    zassert_equal(pDisp->pInMbox->count(), 0, "pInMbox->count() != 0");
    zassert_equal(pDisp->pInMbox, Mbox::getMboxById(Mbox::eDispTask),
                  "Error getting mailbox by id");

    vectord filterOutput = {1.0, 2.0, 3.0, 1.0, 5.0};
    pDisp->totalNumEntries = 100;
    pDisp->sentEntries     = 90;

    pDisp->reqFiltResultWrite(filterOutput, msgType::eCalcDisplacementReq);  
    zassert_equal(pDisp->requestedWrEntries, 10, "requestedWrEntries != 10");
    
	delete pDisp; 
}

