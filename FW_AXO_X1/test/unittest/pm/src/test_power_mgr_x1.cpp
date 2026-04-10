#include <zephyr/ztest.h>
//#include <vector>
//#include <algorithm>

#define private public
#include "pm/power_mgr_x1.h"

#include "stubs.h"

ZTEST_SUITE(tests_pm_power_mgr_x1, NULL, NULL, NULL, NULL, NULL);

// test constructor and destructor
ZTEST(tests_pm_power_mgr_x1, test_pm_power_mgr_x1_constructor)
{
	PowerMgrX1* pPowerMgrX1 = new PowerMgrX1("PowerMgrX1");
	zassert_not_null(pPowerMgrX1, "memory alloc failed");
	zassert_not_null(pPowerMgrX1->pInMbox, "pInMbox = NULL");
	zassert_not_null(pPowerMgrX1->BoostCtrlTimer, "BoostCtrlTimer = NULL");
	zassert_not_null(pPowerMgrX1->LteStartDelayTimer, "LteStartDelayTimer = NULL");
	zassert_not_null(pPowerMgrX1->AdxlWakeupTimer, "AdxlWakeupTimer = NULL");
    zassert_equal(pPowerMgrX1->pInMbox->count(), 0, "pInMbox->count() != 0");
    zassert_equal(pPowerMgrX1->pInMbox, Mbox::getMboxById(Mbox::ePowerManager),
                  "Error getting mailbox by id");

    zassert_equal(pPowerMgrX1->metricProviders[0], Mbox::MboxId::eShtcxTask, "metricProviders[0] != eShtcxTask");
    zassert_equal(pPowerMgrX1->metricProviders[1], Mbox::MboxId::eBattMgr, "metricProviders[1] != eBattMgr");
    zassert_equal(pPowerMgrX1->metricProviders[2], Mbox::MboxId::eHwMetrics, "metricProviders[2] != eHwMetrics");
    zassert_equal(pPowerMgrX1->metricProviders[3], Mbox::MboxId::eDataManager, "metricProviders[3] != eDataManager");				  
    zassert_equal(pPowerMgrX1->metricType[0], msgType::eShtcxReport, "metricType[0] != eShtcxReport");
    zassert_equal(pPowerMgrX1->metricType[1], msgType::eBattReport, "metricType[1] != eBattReport");
    zassert_equal(pPowerMgrX1->metricType[2], msgType::eHwReport, "metricType[2] != eHwReport");
    zassert_equal(pPowerMgrX1->metricType[3], msgType::eLteReport, "metricType[3] != eLteReport");				  

	delete pPowerMgrX1;
}

// MISSING for 100% branch coverage operator new does not deliver NULL
// here code lines 
// BoostCtrlTimer = new Timer(PowerMgrX1::BoostCtrlTimer_cb, this, eLteWakeTmo, 0);
// LteStartDelayTimer = new Timer(PowerMgrX1::LteStartDelayTimer_cb, this, eLteWakeTmo, 0);
// AdxlWakeupTimer = new Timer(PowerMgrX1::AdxlWakeupTimer_cb, this, eLteWakeTmo, 0);
// static PoolElem e1 = {eBoostCtrlTimerExpired, Mbox::MboxId::ePowerManager};
// static PoolElem e1 = {eLteStartDelayTimerExpired, Mbox::MboxId::ePowerManager};
// static PoolElem e1 = {eAdxlWakeupTimerExpired, Mbox::MboxId::ePowerManager};
// are not covered completly.



// test function reqBoostConverter
ZTEST(tests_pm_power_mgr_x1, test_pm_power_mgr_x1_reqBoostConverter)
{
    PoolElem* pElem;

	Mbox* pBoostMbox = new Mbox("BoostMbox");
    Mbox::registerMbox(pBoostMbox, Mbox::eBoostConverter);
	zassert_not_null(pBoostMbox, "pBoostMbox = NULL");

	PowerMgrX1* pPowerMgrX1 = new PowerMgrX1("PowerMgrX1");
	zassert_not_null(pPowerMgrX1, "memory alloc failed");
	zassert_not_null(pPowerMgrX1->pInMbox, "pInMbox = NULL");
	zassert_not_null(pPowerMgrX1->BoostCtrlTimer, "BoostCtrlTimer = NULL");
	zassert_not_null(pPowerMgrX1->LteStartDelayTimer, "LteStartDelayTimer = NULL");
	zassert_not_null(pPowerMgrX1->AdxlWakeupTimer, "AdxlWakeupTimer = NULL");
    zassert_equal(pPowerMgrX1->pInMbox->count(), 0, "pInMbox->count() != 0");
    zassert_equal(pPowerMgrX1->pInMbox, Mbox::getMboxById(Mbox::ePowerManager),
                  "Error getting mailbox by id");

    pPowerMgrX1->reqBoostConverter(true);				  

    pElem = pBoostMbox->read();
    zassert_equal(pElem->mtype, msgType::eResume, "Rcvd mtype != msgType::eResume");
    zassert_equal(pElem->from, Mbox::ePowerManager, "Rcvd sender != Mbox::ePowerManager");

	pElem->free();
	delete pBoostMbox;
	delete pPowerMgrX1;
}

// test function reqBoostConverter
ZTEST(tests_pm_power_mgr_x1, test_pm_power_mgr_x1_reqBoostConverter2)
{
    PoolElem* pElem;

	Mbox* pBoostMbox = new Mbox("BoostMbox");
    Mbox::registerMbox(pBoostMbox, Mbox::eBoostConverter);
	zassert_not_null(pBoostMbox, "pBoostMbox = NULL");

	PowerMgrX1* pPowerMgrX1 = new PowerMgrX1("PowerMgrX1");
	zassert_not_null(pPowerMgrX1, "memory alloc failed");
	zassert_not_null(pPowerMgrX1->pInMbox, "pInMbox = NULL");
	zassert_not_null(pPowerMgrX1->BoostCtrlTimer, "BoostCtrlTimer = NULL");
	zassert_not_null(pPowerMgrX1->LteStartDelayTimer, "LteStartDelayTimer = NULL");
	zassert_not_null(pPowerMgrX1->AdxlWakeupTimer, "AdxlWakeupTimer = NULL");
    zassert_equal(pPowerMgrX1->pInMbox->count(), 0, "pInMbox->count() != 0");
    zassert_equal(pPowerMgrX1->pInMbox, Mbox::getMboxById(Mbox::ePowerManager),
                  "Error getting mailbox by id");

    pPowerMgrX1->reqBoostConverter(false);				  

    pElem = pBoostMbox->read();
    zassert_equal(pElem->mtype, msgType::eSuspend, "Rcvd mtype != msgType::eSuspend");
    zassert_equal(pElem->from, Mbox::ePowerManager, "Rcvd sender != Mbox::ePowerManager");

	pElem->free();
	delete pBoostMbox;
	delete pPowerMgrX1;
}

// test function BoostCtrlTimer_cb and BoostCtrlTimer_expired
ZTEST(tests_pm_power_mgr_x1, test_pm_power_mgr_x1_timer_cb)
{
    PoolElem* pElem;

	PowerMgrX1* pPowerMgrX1 = new PowerMgrX1("PowerMgrX1");
	zassert_not_null(pPowerMgrX1, "memory alloc failed");
	zassert_not_null(pPowerMgrX1->pInMbox, "pInMbox = NULL");
	zassert_not_null(pPowerMgrX1->BoostCtrlTimer, "BoostCtrlTimer = NULL");
	zassert_not_null(pPowerMgrX1->LteStartDelayTimer, "LteStartDelayTimer = NULL");
	zassert_not_null(pPowerMgrX1->AdxlWakeupTimer, "AdxlWakeupTimer = NULL");
    zassert_equal(pPowerMgrX1->pInMbox->count(), 0, "pInMbox->count() != 0");
    zassert_equal(pPowerMgrX1->pInMbox, Mbox::getMboxById(Mbox::ePowerManager),
                  "Error getting mailbox by id");

    pPowerMgrX1->BoostCtrlTimer_cb(reinterpret_cast<k_timer*>(pPowerMgrX1));

    zassert_equal(pPowerMgrX1->pInMbox->count(), 1, "pInMbox->count() != 1");
    pElem = pPowerMgrX1->pInMbox->read();
    zassert_equal(pElem->mtype, msgType::eBoostCtrlTimerExpired, "Rcvd mtype != msgType::eTimerExpired");

    delete pPowerMgrX1;
}    

// test function LteStartDelayTimer_cb and LteStartDelayTimer_expired
ZTEST(tests_pm_power_mgr_x1, test_pm_power_mgr_x1_timer_cb2)
{
    PoolElem* pElem;

	PowerMgrX1* pPowerMgrX1 = new PowerMgrX1("PowerMgrX1");
	zassert_not_null(pPowerMgrX1, "memory alloc failed");
	zassert_not_null(pPowerMgrX1->pInMbox, "pInMbox = NULL");
	zassert_not_null(pPowerMgrX1->LteStartDelayTimer, "LteStartDelayTimer = NULL");
	zassert_not_null(pPowerMgrX1->LteStartDelayTimer, "LteStartDelayTimer = NULL");
	zassert_not_null(pPowerMgrX1->AdxlWakeupTimer, "AdxlWakeupTimer = NULL");
    zassert_equal(pPowerMgrX1->pInMbox->count(), 0, "pInMbox->count() != 0");
    zassert_equal(pPowerMgrX1->pInMbox, Mbox::getMboxById(Mbox::ePowerManager),
                  "Error getting mailbox by id");

    pPowerMgrX1->LteStartDelayTimer_cb(reinterpret_cast<k_timer*>(pPowerMgrX1));

    zassert_equal(pPowerMgrX1->pInMbox->count(), 1, "pInMbox->count() != 1");
    pElem = pPowerMgrX1->pInMbox->read();
    zassert_equal(pElem->mtype, msgType::eLteStartDelayTimerExpired, "Rcvd mtype != msgType::eTimerExpired");

    delete pPowerMgrX1;
}    

// test function AdxlWakeupTimer_cb and AdxlWakeupTimer_expired
ZTEST(tests_pm_power_mgr_x1, test_pm_power_mgr_x1_timer_cb3)
{
    PoolElem* pElem;

	PowerMgrX1* pPowerMgrX1 = new PowerMgrX1("PowerMgrX1");
	zassert_not_null(pPowerMgrX1, "memory alloc failed");
	zassert_not_null(pPowerMgrX1->pInMbox, "pInMbox = NULL");
	zassert_not_null(pPowerMgrX1->AdxlWakeupTimer, "AdxlWakeupTimer = NULL");
	zassert_not_null(pPowerMgrX1->LteStartDelayTimer, "LteStartDelayTimer = NULL");
	zassert_not_null(pPowerMgrX1->AdxlWakeupTimer, "AdxlWakeupTimer = NULL");
    zassert_equal(pPowerMgrX1->pInMbox->count(), 0, "pInMbox->count() != 0");
    zassert_equal(pPowerMgrX1->pInMbox, Mbox::getMboxById(Mbox::ePowerManager),
                  "Error getting mailbox by id");

    pPowerMgrX1->AdxlWakeupTimer_cb(reinterpret_cast<k_timer*>(pPowerMgrX1));

    zassert_equal(pPowerMgrX1->pInMbox->count(), 1, "pInMbox->count() != 1");
    pElem = pPowerMgrX1->pInMbox->read();
    zassert_equal(pElem->mtype, msgType::eAdxlWakeupTimerExpired, "Rcvd mtype != msgType::eTimerExpired");

    delete pPowerMgrX1;
}    

// test function taskInit
ZTEST(tests_pm_power_mgr_x1, test_pm_power_mgr_x1_taskInit)
{
    PoolElem* pElem;

	Mbox* pBoostMbox = new Mbox("BoostMbox");
    Mbox::registerMbox(pBoostMbox, Mbox::eBoostConverter);
	zassert_not_null(pBoostMbox, "pBoostMbox = NULL");

	PowerMgrX1* pPowerMgrX1 = new PowerMgrX1("PowerMgrX1");
	zassert_not_null(pPowerMgrX1, "memory alloc failed");
	zassert_not_null(pPowerMgrX1->pInMbox, "pInMbox = NULL");
	zassert_not_null(pPowerMgrX1->BoostCtrlTimer, "BoostCtrlTimer = NULL");
	zassert_not_null(pPowerMgrX1->LteStartDelayTimer, "LteStartDelayTimer = NULL");
	zassert_not_null(pPowerMgrX1->AdxlWakeupTimer, "AdxlWakeupTimer = NULL");
    zassert_equal(pPowerMgrX1->pInMbox->count(), 0, "pInMbox->count() != 0");
    zassert_equal(pPowerMgrX1->pInMbox, Mbox::getMboxById(Mbox::ePowerManager),
                  "Error getting mailbox by id");

    pPowerMgrX1->taskInit();				  

    pElem = pBoostMbox->read();
    zassert_equal(pElem->mtype, msgType::eResume, "Rcvd mtype != msgType::eResume");
    zassert_equal(pElem->from, Mbox::ePowerManager, "Rcvd sender != Mbox::ePowerManager");

	pElem->free();
	delete pBoostMbox;
	delete pPowerMgrX1;
}

//Test receive msg eMetricsTxTimerExpired 
ZTEST(tests_pm_power_mgr_x1, test_pm_power_mgr_x1_rcv_msg_eMetricsTxTimerExpired)
{
    PoolElem* pElem;

	Mbox* pBoostMbox = new Mbox("BoostMbox");
    Mbox::registerMbox(pBoostMbox, Mbox::eBoostConverter);
	zassert_not_null(pBoostMbox, "pBoostMbox = NULL");

	PowerMgrX1* pPowerMgrX1 = new PowerMgrX1("PowerMgrX1");
	zassert_not_null(pPowerMgrX1, "memory alloc failed");
	zassert_not_null(pPowerMgrX1->pInMbox, "pInMbox = NULL");
	zassert_not_null(pPowerMgrX1->BoostCtrlTimer, "BoostCtrlTimer = NULL");
	zassert_not_null(pPowerMgrX1->LteStartDelayTimer, "LteStartDelayTimer = NULL");
	zassert_not_null(pPowerMgrX1->AdxlWakeupTimer, "AdxlWakeupTimer = NULL");
    zassert_equal(pPowerMgrX1->pInMbox->count(), 0, "pInMbox->count() != 0");
    zassert_equal(pPowerMgrX1->pInMbox, Mbox::getMboxById(Mbox::ePowerManager),
                  "Error getting mailbox by id");

    Mbox::sendMsg(msgType::eMetricsTxTimerExpired, Mbox::ePowerManager, Mbox::ePowerManager);

    zassert_equal(pPowerMgrX1->pInMbox->count(), 1, "pInMbox->count() != 1");

    pPowerMgrX1->task();  
    zassert_equal(pPowerMgrX1->pInMbox->count(), 0, "pInMbox->count() != 0");

    pElem = pBoostMbox->read();
    zassert_equal(pElem->mtype, msgType::eResume, "Rcvd mtype != msgType::eResume");
    zassert_equal(pElem->from, Mbox::ePowerManager, "Rcvd sender != Mbox::ePowerManager");
	pElem->free();

    pElem = pBoostMbox->read();
    zassert_equal(pElem->mtype, msgType::eResume, "Rcvd mtype != msgType::eResume");
    zassert_equal(pElem->from, Mbox::ePowerManager, "Rcvd sender != Mbox::ePowerManager");
	pElem->free();

	delete pBoostMbox;
	delete pPowerMgrX1; 
}

//Test receive msg eBoostCtrlTimerExpired 
ZTEST(tests_pm_power_mgr_x1, test_pm_power_mgr_x1_rcv_msg_eBoostCtrlTimerExpired)
{
    PoolElem* pElem;

	Mbox* pBoostMbox = new Mbox("BoostMbox");
    Mbox::registerMbox(pBoostMbox, Mbox::eBoostConverter);
	zassert_not_null(pBoostMbox, "pBoostMbox = NULL");

	Mbox* pShtcxMbox = new Mbox("ShtcxMbox");
    Mbox::registerMbox(pShtcxMbox, Mbox::eShtcxTask);
	zassert_not_null(pShtcxMbox, "pShtcxMbox = NULL");

	Mbox* pBattMgrMbox = new Mbox("BattMgrMbox");
    Mbox::registerMbox(pBattMgrMbox, Mbox::eBattMgr);
	zassert_not_null(pBattMgrMbox, "pBattMgrMbox = NULL");

	Mbox* pHwMetricsMbox = new Mbox("HwMetricsMbox");
    Mbox::registerMbox(pHwMetricsMbox, Mbox::eHwMetrics);
	zassert_not_null(pHwMetricsMbox, "pHwMetricsMbox = NULL");

	Mbox* pDataMgrMbox = new Mbox("DataMgrMbox");
    Mbox::registerMbox(pDataMgrMbox, Mbox::eDataManager);
	zassert_not_null(pDataMgrMbox, "pDataMgrMbox = NULL");

	PowerMgrX1* pPowerMgrX1 = new PowerMgrX1("PowerMgrX1");
	zassert_not_null(pPowerMgrX1, "memory alloc failed");
	zassert_not_null(pPowerMgrX1->pInMbox, "pInMbox = NULL");
	zassert_not_null(pPowerMgrX1->BoostCtrlTimer, "BoostCtrlTimer = NULL");
	zassert_not_null(pPowerMgrX1->LteStartDelayTimer, "LteStartDelayTimer = NULL");
	zassert_not_null(pPowerMgrX1->AdxlWakeupTimer, "AdxlWakeupTimer = NULL");
    zassert_equal(pPowerMgrX1->pInMbox->count(), 0, "pInMbox->count() != 0");
    zassert_equal(pPowerMgrX1->pInMbox, Mbox::getMboxById(Mbox::ePowerManager),
                  "Error getting mailbox by id");

    Mbox::sendMsg(msgType::eBoostCtrlTimerExpired, Mbox::ePowerManager, Mbox::ePowerManager);

    zassert_equal(pPowerMgrX1->pInMbox->count(), 1, "pInMbox->count() != 1");

    pPowerMgrX1->task();  
    zassert_equal(pPowerMgrX1->pInMbox->count(), 0, "pInMbox->count() != 0");

    pElem = pBoostMbox->read();
    zassert_equal(pElem->mtype, msgType::eResume, "Rcvd mtype != msgType::eResume");
    zassert_equal(pElem->from, Mbox::ePowerManager, "Rcvd sender != Mbox::ePowerManager");
	pElem->free();

    pElem = pShtcxMbox->read();
	//printk("mtype = %d\n", pElem->mtype);
    zassert_equal(pElem->mtype, msgType::eShtcxReport, "Rcvd mtype != msgType::eShtcxReport");
    zassert_equal(pElem->from, Mbox::ePowerManager, "Rcvd sender != Mbox::ePowerManager");
	pElem->free();
	
    pElem = pBattMgrMbox->read();
    zassert_equal(pElem->mtype, msgType::eBattReport, "Rcvd mtype != msgType::eBattReport");
    zassert_equal(pElem->from, Mbox::ePowerManager, "Rcvd sender != Mbox::ePowerManager");
	pElem->free();
	
    pElem = pHwMetricsMbox->read();
    zassert_equal(pElem->mtype, msgType::eHwReport, "Rcvd mtype != msgType::eHwReport");
    zassert_equal(pElem->from, Mbox::ePowerManager, "Rcvd sender != Mbox::ePowerManager");
	pElem->free();
	
    pElem = pDataMgrMbox->read();
    zassert_equal(pElem->mtype, msgType::eLteReport, "Rcvd mtype != msgType::eLteReport");
    zassert_equal(pElem->from, Mbox::ePowerManager, "Rcvd sender != Mbox::ePowerManager");
	pElem->free();
	
	delete pBoostMbox;
    delete pShtcxMbox;
	delete pBattMgrMbox;
	delete pHwMetricsMbox;
	delete pDataMgrMbox;
	delete pPowerMgrX1; 
}

//Test receive msg eLteStartDelayTimerExpired 
ZTEST(tests_pm_power_mgr_x1, test_pm_power_mgr_x1_rcv_msg_eLteStartDelayTimerExpired)
{
    PoolElem* pElem;

	Mbox* pBoostMbox = new Mbox("BoostMbox");
    Mbox::registerMbox(pBoostMbox, Mbox::eBoostConverter);
	zassert_not_null(pBoostMbox, "pBoostMbox = NULL");

	Mbox* pLteMbox = new Mbox("LteMbox");
    Mbox::registerMbox(pLteMbox, Mbox::eLteConnector);
	zassert_not_null(pLteMbox, "pLteMbox = NULL");

	PowerMgrX1* pPowerMgrX1 = new PowerMgrX1("PowerMgrX1");
	zassert_not_null(pPowerMgrX1, "memory alloc failed");
	zassert_not_null(pPowerMgrX1->pInMbox, "pInMbox = NULL");
	zassert_not_null(pPowerMgrX1->BoostCtrlTimer, "BoostCtrlTimer = NULL");
	zassert_not_null(pPowerMgrX1->LteStartDelayTimer, "LteStartDelayTimer = NULL");
	zassert_not_null(pPowerMgrX1->AdxlWakeupTimer, "AdxlWakeupTimer = NULL");
    zassert_equal(pPowerMgrX1->pInMbox->count(), 0, "pInMbox->count() != 0");
    zassert_equal(pPowerMgrX1->pInMbox, Mbox::getMboxById(Mbox::ePowerManager),
                  "Error getting mailbox by id");

    Mbox::sendMsg(msgType::eLteStartDelayTimerExpired, Mbox::ePowerManager, Mbox::ePowerManager);

    zassert_equal(pPowerMgrX1->pInMbox->count(), 1, "pInMbox->count() != 1");

    pPowerMgrX1->task();  
    zassert_equal(pPowerMgrX1->pInMbox->count(), 0, "pInMbox->count() != 0");

    pElem = pBoostMbox->read();
    zassert_equal(pElem->mtype, msgType::eResume, "Rcvd mtype != msgType::eResume");
    zassert_equal(pElem->from, Mbox::ePowerManager, "Rcvd sender != Mbox::ePowerManager");
	pElem->free();
	
    pElem = pLteMbox->read();
    zassert_equal(pElem->mtype, msgType::eResume, "Rcvd mtype != msgType::eResume");
    zassert_equal(pElem->from, Mbox::ePowerManager, "Rcvd sender != Mbox::ePowerManager");
	pElem->free();

	delete pBoostMbox;
	delete pLteMbox;
	delete pPowerMgrX1; 
}

//Test receive msg eAdxlWakeupTimerExpired 
ZTEST(tests_pm_power_mgr_x1, test_pm_power_mgr_x1_rcv_msg_eAdxlWakeupTimerExpired)
{
    PoolElem* pElem;

	Mbox* pBoostMbox = new Mbox("BoostMbox");
    Mbox::registerMbox(pBoostMbox, Mbox::eBoostConverter);
	zassert_not_null(pBoostMbox, "pBoostMbox = NULL");

	Mbox* pRmsStoreMbox = new Mbox("RmsStoreMbox");
    Mbox::registerMbox(pRmsStoreMbox, Mbox::eRmsStore);
	zassert_not_null(pRmsStoreMbox, "pRmsStoreMbox = NULL");

	PowerMgrX1* pPowerMgrX1 = new PowerMgrX1("PowerMgrX1");
	zassert_not_null(pPowerMgrX1, "memory alloc failed");
	zassert_not_null(pPowerMgrX1->pInMbox, "pInMbox = NULL");
	zassert_not_null(pPowerMgrX1->BoostCtrlTimer, "BoostCtrlTimer = NULL");
	zassert_not_null(pPowerMgrX1->LteStartDelayTimer, "LteStartDelayTimer = NULL");
	zassert_not_null(pPowerMgrX1->AdxlWakeupTimer, "AdxlWakeupTimer = NULL");
    zassert_equal(pPowerMgrX1->pInMbox->count(), 0, "pInMbox->count() != 0");
    zassert_equal(pPowerMgrX1->pInMbox, Mbox::getMboxById(Mbox::ePowerManager),
                  "Error getting mailbox by id");

    Mbox::sendMsg(msgType::eAdxlWakeupTimerExpired, Mbox::ePowerManager, Mbox::ePowerManager);

    zassert_equal(pPowerMgrX1->pInMbox->count(), 1, "pInMbox->count() != 1");

    pPowerMgrX1->task();  
    zassert_equal(pPowerMgrX1->pInMbox->count(), 0, "pInMbox->count() != 0");

    pElem = pBoostMbox->read();
    zassert_equal(pElem->mtype, msgType::eResume, "Rcvd mtype != msgType::eResume");
    zassert_equal(pElem->from, Mbox::ePowerManager, "Rcvd sender != Mbox::ePowerManager");
	pElem->free();
	
    pElem = pRmsStoreMbox->read();
    zassert_equal(pElem->mtype, msgType::eRmsReport, "Rcvd mtype != msgType::eRmsReport");
    zassert_equal(pElem->from, Mbox::ePowerManager, "Rcvd sender != Mbox::ePowerManager");
	pElem->free();

	delete pBoostMbox;
	delete pRmsStoreMbox;
	delete pPowerMgrX1; 
}

//Test receive msg eLteSleepEnter 
ZTEST(tests_pm_power_mgr_x1, test_pm_power_mgr_x1_rcv_msg_eLteSleepEnter)
{
    PoolElem* pElem;

	Mbox* pBoostMbox = new Mbox("BoostMbox");
    Mbox::registerMbox(pBoostMbox, Mbox::eBoostConverter);
	zassert_not_null(pBoostMbox, "pBoostMbox = NULL");

	PowerMgrX1* pPowerMgrX1 = new PowerMgrX1("PowerMgrX1");
	zassert_not_null(pPowerMgrX1, "memory alloc failed");
	zassert_not_null(pPowerMgrX1->pInMbox, "pInMbox = NULL");
	zassert_not_null(pPowerMgrX1->BoostCtrlTimer, "BoostCtrlTimer = NULL");
	zassert_not_null(pPowerMgrX1->LteStartDelayTimer, "LteStartDelayTimer = NULL");
	zassert_not_null(pPowerMgrX1->AdxlWakeupTimer, "AdxlWakeupTimer = NULL");
    zassert_equal(pPowerMgrX1->pInMbox->count(), 0, "pInMbox->count() != 0");
    zassert_equal(pPowerMgrX1->pInMbox, Mbox::getMboxById(Mbox::ePowerManager),
                  "Error getting mailbox by id");

    Mbox::sendMsg(msgType::eLteSleepEnter, Mbox::ePowerManager, Mbox::ePowerManager);

    zassert_equal(pPowerMgrX1->pInMbox->count(), 1, "pInMbox->count() != 1");

    gIntVal = 31000;
    pPowerMgrX1->task();  
    zassert_equal(pPowerMgrX1->pInMbox->count(), 0, "pInMbox->count() != 0");

    pElem = pBoostMbox->read();
    zassert_equal(pElem->mtype, msgType::eResume, "Rcvd mtype != msgType::eResume");
    zassert_equal(pElem->from, Mbox::ePowerManager, "Rcvd sender != Mbox::ePowerManager");
	pElem->free();
    
    pElem = pBoostMbox->read();
    zassert_equal(pElem->mtype, msgType::eSuspend, "Rcvd mtype != msgType::eSuspend");
    zassert_equal(pElem->from, Mbox::ePowerManager, "Rcvd sender != Mbox::ePowerManager");
	pElem->free();
    
	delete pBoostMbox;
	delete pPowerMgrX1; 
}

//Test receive msg eLteSleepEnter 
ZTEST(tests_pm_power_mgr_x1, test_pm_power_mgr_x1_rcv_msg_eLteSleepEnter2)
{
    PoolElem* pElem;

	Mbox* pBoostMbox = new Mbox("BoostMbox");
    Mbox::registerMbox(pBoostMbox, Mbox::eBoostConverter);
	zassert_not_null(pBoostMbox, "pBoostMbox = NULL");

	PowerMgrX1* pPowerMgrX1 = new PowerMgrX1("PowerMgrX1");
	zassert_not_null(pPowerMgrX1, "memory alloc failed");
	zassert_not_null(pPowerMgrX1->pInMbox, "pInMbox = NULL");
	zassert_not_null(pPowerMgrX1->BoostCtrlTimer, "BoostCtrlTimer = NULL");
	zassert_not_null(pPowerMgrX1->LteStartDelayTimer, "LteStartDelayTimer = NULL");
	zassert_not_null(pPowerMgrX1->AdxlWakeupTimer, "AdxlWakeupTimer = NULL");
    zassert_equal(pPowerMgrX1->pInMbox->count(), 0, "pInMbox->count() != 0");
    zassert_equal(pPowerMgrX1->pInMbox, Mbox::getMboxById(Mbox::ePowerManager),
                  "Error getting mailbox by id");

    Mbox::sendMsg(msgType::eLteSleepEnter, Mbox::ePowerManager, Mbox::ePowerManager);

    zassert_equal(pPowerMgrX1->pInMbox->count(), 1, "pInMbox->count() != 1");

    gIntVal = 1000;
    pPowerMgrX1->task();  
	//printk("count = %d\n", pPowerMgrX1->pInMbox->count());
    zassert_equal(pPowerMgrX1->pInMbox->count(), 0, "pInMbox->count() != 0");

    pElem = pBoostMbox->read();
    zassert_equal(pElem->mtype, msgType::eResume, "Rcvd mtype != msgType::eResume");
    zassert_equal(pElem->from, Mbox::ePowerManager, "Rcvd sender != Mbox::ePowerManager");
	pElem->free();
	
	delete pBoostMbox;
	delete pPowerMgrX1; 
}

//Test receive msg eLteSleepExit 
ZTEST(tests_pm_power_mgr_x1, test_pm_power_mgr_x1_rcv_msg_eLteSleepExit)
{
    PoolElem* pElem;

	Mbox* pBoostMbox = new Mbox("BoostMbox");
    Mbox::registerMbox(pBoostMbox, Mbox::eBoostConverter);
	zassert_not_null(pBoostMbox, "pBoostMbox = NULL");

	PowerMgrX1* pPowerMgrX1 = new PowerMgrX1("PowerMgrX1");
	zassert_not_null(pPowerMgrX1, "memory alloc failed");
	zassert_not_null(pPowerMgrX1->pInMbox, "pInMbox = NULL");
	zassert_not_null(pPowerMgrX1->BoostCtrlTimer, "BoostCtrlTimer = NULL");
	zassert_not_null(pPowerMgrX1->LteStartDelayTimer, "LteStartDelayTimer = NULL");
	zassert_not_null(pPowerMgrX1->AdxlWakeupTimer, "AdxlWakeupTimer = NULL");
    zassert_equal(pPowerMgrX1->pInMbox->count(), 0, "pInMbox->count() != 0");
    zassert_equal(pPowerMgrX1->pInMbox, Mbox::getMboxById(Mbox::ePowerManager),
                  "Error getting mailbox by id");

    Mbox::sendMsg(msgType::eLteSleepExit, Mbox::ePowerManager, Mbox::ePowerManager);

    zassert_equal(pPowerMgrX1->pInMbox->count(), 1, "pInMbox->count() != 1");

    pPowerMgrX1->task();  
    zassert_equal(pPowerMgrX1->pInMbox->count(), 0, "pInMbox->count() != 0");

    pElem = pBoostMbox->read();
    zassert_equal(pElem->mtype, msgType::eResume, "Rcvd mtype != msgType::eResume");
    zassert_equal(pElem->from, Mbox::ePowerManager, "Rcvd sender != Mbox::ePowerManager");
	pElem->free();
    
    pElem = pBoostMbox->read();
    zassert_equal(pElem->mtype, msgType::eResume, "Rcvd mtype != msgType::eResume");
    zassert_equal(pElem->from, Mbox::ePowerManager, "Rcvd sender != Mbox::ePowerManager");
	pElem->free();
    
	delete pBoostMbox;
	delete pPowerMgrX1; 
}

//Test receive msg eAdxl37xWake 
ZTEST(tests_pm_power_mgr_x1, test_pm_power_mgr_x1_rcv_msg_eAdxl37xWake)
{
    PoolElem* pElem;

	Mbox* pBoostMbox = new Mbox("BoostMbox");
    Mbox::registerMbox(pBoostMbox, Mbox::eBoostConverter);
	zassert_not_null(pBoostMbox, "pBoostMbox = NULL");

	PowerMgrX1* pPowerMgrX1 = new PowerMgrX1("PowerMgrX1");
	zassert_not_null(pPowerMgrX1, "memory alloc failed");
	zassert_not_null(pPowerMgrX1->pInMbox, "pInMbox = NULL");
	zassert_not_null(pPowerMgrX1->BoostCtrlTimer, "BoostCtrlTimer = NULL");
	zassert_not_null(pPowerMgrX1->LteStartDelayTimer, "LteStartDelayTimer = NULL");
	zassert_not_null(pPowerMgrX1->AdxlWakeupTimer, "AdxlWakeupTimer = NULL");
    zassert_equal(pPowerMgrX1->pInMbox->count(), 0, "pInMbox->count() != 0");
    zassert_equal(pPowerMgrX1->pInMbox, Mbox::getMboxById(Mbox::ePowerManager),
                  "Error getting mailbox by id");

    Mbox::sendMsg(msgType::eAdxl37xWake, Mbox::ePowerManager, Mbox::ePowerManager);

    zassert_equal(pPowerMgrX1->pInMbox->count(), 1, "pInMbox->count() != 1");

    pPowerMgrX1->task();  
    zassert_equal(pPowerMgrX1->pInMbox->count(), 0, "pInMbox->count() != 0");

    pElem = pBoostMbox->read();
    zassert_equal(pElem->mtype, msgType::eResume, "Rcvd mtype != msgType::eResume");
    zassert_equal(pElem->from, Mbox::ePowerManager, "Rcvd sender != Mbox::ePowerManager");
	pElem->free();
    
    pElem = pBoostMbox->read();
    zassert_equal(pElem->mtype, msgType::eResume, "Rcvd mtype != msgType::eResume");
    zassert_equal(pElem->from, Mbox::ePowerManager, "Rcvd sender != Mbox::ePowerManager");
	pElem->free();
    
	delete pBoostMbox;
	delete pPowerMgrX1; 
}

//Test receive msg eLteConnected 
ZTEST(tests_pm_power_mgr_x1, test_pm_power_mgr_x1_rcv_msg_eLteConnected)
{
    PoolElem* pElem;

	Mbox* pBoostMbox = new Mbox("BoostMbox");
    Mbox::registerMbox(pBoostMbox, Mbox::eBoostConverter);
	zassert_not_null(pBoostMbox, "pBoostMbox = NULL");

	PowerMgrX1* pPowerMgrX1 = new PowerMgrX1("PowerMgrX1");
	zassert_not_null(pPowerMgrX1, "memory alloc failed");
	zassert_not_null(pPowerMgrX1->pInMbox, "pInMbox = NULL");
	zassert_not_null(pPowerMgrX1->BoostCtrlTimer, "BoostCtrlTimer = NULL");
	zassert_not_null(pPowerMgrX1->LteStartDelayTimer, "LteStartDelayTimer = NULL");
	zassert_not_null(pPowerMgrX1->AdxlWakeupTimer, "AdxlWakeupTimer = NULL");
    zassert_equal(pPowerMgrX1->pInMbox->count(), 0, "pInMbox->count() != 0");
    zassert_equal(pPowerMgrX1->pInMbox, Mbox::getMboxById(Mbox::ePowerManager),
                  "Error getting mailbox by id");

    Mbox::sendMsg(msgType::eLteConnected, Mbox::ePowerManager, Mbox::ePowerManager);

    zassert_equal(pPowerMgrX1->pInMbox->count(), 1, "pInMbox->count() != 1");

    pPowerMgrX1->task();  
    zassert_equal(pPowerMgrX1->pInMbox->count(), 0, "pInMbox->count() != 0");
    zassert_true(pPowerMgrX1->lte_connected, "lte_connected != true");

    pElem = pBoostMbox->read();
    zassert_equal(pElem->mtype, msgType::eResume, "Rcvd mtype != msgType::eResume");
    zassert_equal(pElem->from, Mbox::ePowerManager, "Rcvd sender != Mbox::ePowerManager");
	pElem->free();
	
	delete pBoostMbox;
	delete pPowerMgrX1; 
}

//Test receive msg eLteDisconnected 
ZTEST(tests_pm_power_mgr_x1, test_pm_power_mgr_x1_rcv_msg_eLteDisconnected)
{
    PoolElem* pElem;

	Mbox* pBoostMbox = new Mbox("BoostMbox");
    Mbox::registerMbox(pBoostMbox, Mbox::eBoostConverter);
	zassert_not_null(pBoostMbox, "pBoostMbox = NULL");

	PowerMgrX1* pPowerMgrX1 = new PowerMgrX1("PowerMgrX1");
	zassert_not_null(pPowerMgrX1, "memory alloc failed");
	zassert_not_null(pPowerMgrX1->pInMbox, "pInMbox = NULL");
	zassert_not_null(pPowerMgrX1->BoostCtrlTimer, "BoostCtrlTimer = NULL");
	zassert_not_null(pPowerMgrX1->LteStartDelayTimer, "LteStartDelayTimer = NULL");
	zassert_not_null(pPowerMgrX1->AdxlWakeupTimer, "AdxlWakeupTimer = NULL");
    zassert_equal(pPowerMgrX1->pInMbox->count(), 0, "pInMbox->count() != 0");
    zassert_equal(pPowerMgrX1->pInMbox, Mbox::getMboxById(Mbox::ePowerManager),
                  "Error getting mailbox by id");

    Mbox::sendMsg(msgType::eLteDisconnected, Mbox::ePowerManager, Mbox::ePowerManager);

    zassert_equal(pPowerMgrX1->pInMbox->count(), 1, "pInMbox->count() != 1");

    pPowerMgrX1->task();  
    zassert_equal(pPowerMgrX1->pInMbox->count(), 0, "pInMbox->count() != 0");
    zassert_false(pPowerMgrX1->lte_connected, "lte_connected != false");

    pElem = pBoostMbox->read();
    zassert_equal(pElem->mtype, msgType::eResume, "Rcvd mtype != msgType::eResume");
    zassert_equal(pElem->from, Mbox::ePowerManager, "Rcvd sender != Mbox::ePowerManager");
	pElem->free();
	
	delete pBoostMbox;
	delete pPowerMgrX1; 
}

//Test receive msg eMqttConnected 
ZTEST(tests_pm_power_mgr_x1, test_pm_power_mgr_x1_rcv_msg_eMqttConnected)
{
    PoolElem* pElem;
	std::string s;

	Mbox* pBoostMbox = new Mbox("BoostMbox");
    Mbox::registerMbox(pBoostMbox, Mbox::eBoostConverter);
	zassert_not_null(pBoostMbox, "pBoostMbox = NULL");

	Mbox* pShtcxMbox = new Mbox("ShtcxMbox");
    Mbox::registerMbox(pShtcxMbox, Mbox::eShtcxTask);
	zassert_not_null(pShtcxMbox, "pShtcxMbox = NULL");

	Mbox* pBattMgrMbox = new Mbox("BattMgrMbox");
    Mbox::registerMbox(pBattMgrMbox, Mbox::eBattMgr);
	zassert_not_null(pBattMgrMbox, "pBattMgrMbox = NULL");

	Mbox* pHwMetricsMbox = new Mbox("HwMetricsMbox");
    Mbox::registerMbox(pHwMetricsMbox, Mbox::eHwMetrics);
	zassert_not_null(pHwMetricsMbox, "pHwMetricsMbox = NULL");

	Mbox* pDataMgrMbox = new Mbox("DataMgrMbox");
    Mbox::registerMbox(pDataMgrMbox, Mbox::eDataManager);
	zassert_not_null(pDataMgrMbox, "pDataMgrMbox = NULL");

	PowerMgrX1* pPowerMgrX1 = new PowerMgrX1("PowerMgrX1");
	zassert_not_null(pPowerMgrX1, "memory alloc failed");
	zassert_not_null(pPowerMgrX1->pInMbox, "pInMbox = NULL");
	zassert_not_null(pPowerMgrX1->BoostCtrlTimer, "BoostCtrlTimer = NULL");
	zassert_not_null(pPowerMgrX1->LteStartDelayTimer, "LteStartDelayTimer = NULL");
	zassert_not_null(pPowerMgrX1->AdxlWakeupTimer, "AdxlWakeupTimer = NULL");
    zassert_equal(pPowerMgrX1->pInMbox->count(), 0, "pInMbox->count() != 0");
    zassert_equal(pPowerMgrX1->pInMbox, Mbox::getMboxById(Mbox::ePowerManager),
                  "Error getting mailbox by id");

    zassert_equal(pPowerMgrX1->metricType[0], msgType::eShtcxReport, "metricType[0] != eShtcxReport");
    zassert_equal(pPowerMgrX1->metricType[1], msgType::eBattReport, "metricType[1] != eBattReport");
    zassert_equal(pPowerMgrX1->metricType[2], msgType::eHwReport, "metricType[2] != eHwReport");
    zassert_equal(pPowerMgrX1->metricType[3], msgType::eLteReport, "metricType[3] != eLteReport");				  
	//pPowerMgrX1->boot_cnt = 17;
	//pPowerMgrX1->reason_str = "QWERTZ";
    Mbox::sendMsg(msgType::eMqttConnected, Mbox::ePowerManager, Mbox::ePowerManager);

    zassert_equal(pPowerMgrX1->pInMbox->count(), 1, "pInMbox->count() != 1");

    pPowerMgrX1->task();  
    zassert_equal(pPowerMgrX1->pInMbox->count(), 0, "pInMbox->count() != 0");

    pElem = pBoostMbox->read();
    zassert_equal(pElem->mtype, msgType::eResume, "Rcvd mtype != msgType::eResume");
    zassert_equal(pElem->from, Mbox::ePowerManager, "Rcvd sender != Mbox::ePowerManager");
	pElem->free();

    pElem = pDataMgrMbox->read();
	//printk("pDataMgrMbox mtype = %d, %x\n", pElem->mtype, (unsigned int)pElem);
	s = std::string(reinterpret_cast<char*>(pElem->getBuffer()));
    zassert_equal(pElem->mtype, msgType::eResetCause, "Rcvd mtype != msgType::eResetCause");
    zassert_equal(pElem->from, Mbox::ePowerManager, "Rcvd sender != Mbox::ePowerManager");
	zassert_equal(strcmp(s.c_str(), "POR"), 0, "Wrong rcvd string");
	pElem->free();
	
    pElem = pDataMgrMbox->read();
	//printk("pDataMgrMbox mtype = %d, %x\n", pElem->mtype, (unsigned int)pElem);
    int32_t cnt = *reinterpret_cast<int32_t*>(pElem->getBuffer());
    zassert_equal(pElem->mtype, msgType::eBootCnt, "Rcvd mtype != msgType::eBootCnt");
    zassert_equal(pElem->from, Mbox::ePowerManager, "Rcvd sender != Mbox::ePowerManager");
	zassert_equal(cnt, 1, "Wrong boot count");
	pElem->free();

    pElem = pShtcxMbox->read();
	//printk("pShtcxMbox mtype = %d, %x\n", pElem->mtype, (unsigned int)pElem);
    zassert_equal(pElem->mtype, msgType::eShtcxReport, "Rcvd mtype != msgType::eShtcxReport");
    zassert_equal(pElem->from, Mbox::ePowerManager, "Rcvd sender != Mbox::ePowerManager");
	pElem->free();
	
    pElem = pBattMgrMbox->read();
	//printk("pBattMgrMbox mtype = %d, %x\n", pElem->mtype, (unsigned int)pElem);
    zassert_equal(pElem->mtype, msgType::eBattReport, "Rcvd mtype != msgType::eBattReport");
    zassert_equal(pElem->from, Mbox::ePowerManager, "Rcvd sender != Mbox::ePowerManager");
	pElem->free();
	
    pElem = pHwMetricsMbox->read();
	//printk("pHwMetricsMbox mtype = %d, %x\n", pElem->mtype, (unsigned int)pElem);
    zassert_equal(pElem->mtype, msgType::eHwReport, "Rcvd mtype != msgType::eHwReport");
    zassert_equal(pElem->from, Mbox::ePowerManager, "Rcvd sender != Mbox::ePowerManager");
	pElem->free();
	
    pElem = pDataMgrMbox->read();
	//printk("pDataMgrMbox mtype = %d, %x\n", pElem->mtype, (unsigned int)pElem);
    zassert_equal(pElem->mtype, msgType::eLteReport, "Rcvd mtype != msgType::eLteReport");
    zassert_equal(pElem->from, Mbox::ePowerManager, "Rcvd sender != Mbox::ePowerManager");
	pElem->free();
	
    delete pBoostMbox;
	delete pShtcxMbox;
	delete pBattMgrMbox;
	delete pHwMetricsMbox;
	delete pDataMgrMbox;
	delete pPowerMgrX1; 
}

//Test receive unexpected msg 
ZTEST(tests_pm_power_mgr_x1, test_pm_power_mgr_x1_rcv_msg_unexpected)
{
    PoolElem* pElem;

	Mbox* pBoostMbox = new Mbox("BoostMbox");
    Mbox::registerMbox(pBoostMbox, Mbox::eBoostConverter);
	zassert_not_null(pBoostMbox, "pBoostMbox = NULL");

	PowerMgrX1* pPowerMgrX1 = new PowerMgrX1("PowerMgrX1");
	zassert_not_null(pPowerMgrX1, "memory alloc failed");
	zassert_not_null(pPowerMgrX1->pInMbox, "pInMbox = NULL");
	zassert_not_null(pPowerMgrX1->BoostCtrlTimer, "BoostCtrlTimer = NULL");
	zassert_not_null(pPowerMgrX1->LteStartDelayTimer, "LteStartDelayTimer = NULL");
	zassert_not_null(pPowerMgrX1->AdxlWakeupTimer, "AdxlWakeupTimer = NULL");
    zassert_equal(pPowerMgrX1->pInMbox->count(), 0, "pInMbox->count() != 0");
    zassert_equal(pPowerMgrX1->pInMbox, Mbox::getMboxById(Mbox::ePowerManager),
                  "Error getting mailbox by id");

    Mbox::sendMsg(msgType::eStoreSResult, Mbox::ePowerManager, Mbox::ePowerManager);

    zassert_equal(pPowerMgrX1->pInMbox->count(), 1, "pInMbox->count() != 1");

    pPowerMgrX1->task();  
    zassert_equal(pPowerMgrX1->pInMbox->count(), 0, "pInMbox->count() != 0");

    pElem = pBoostMbox->read();
    zassert_equal(pElem->mtype, msgType::eResume, "Rcvd mtype != msgType::eResume");
    zassert_equal(pElem->from, Mbox::ePowerManager, "Rcvd sender != Mbox::ePowerManager");
	pElem->free();
	
	delete pBoostMbox;
	delete pPowerMgrX1; 
}

//Test receive no msg 
ZTEST(tests_pm_power_mgr_x1, test_pm_power_mgr_x1_rcv_msg_none)
{
    PoolElem* pElem;

	Mbox* pBoostMbox = new Mbox("BoostMbox");
    Mbox::registerMbox(pBoostMbox, Mbox::eBoostConverter);
	zassert_not_null(pBoostMbox, "pBoostMbox = NULL");

	PowerMgrX1* pPowerMgrX1 = new PowerMgrX1("PowerMgrX1");
	zassert_not_null(pPowerMgrX1, "memory alloc failed");
	zassert_not_null(pPowerMgrX1->pInMbox, "pInMbox = NULL");
	zassert_not_null(pPowerMgrX1->BoostCtrlTimer, "BoostCtrlTimer = NULL");
	zassert_not_null(pPowerMgrX1->LteStartDelayTimer, "LteStartDelayTimer = NULL");
	zassert_not_null(pPowerMgrX1->AdxlWakeupTimer, "AdxlWakeupTimer = NULL");
    zassert_equal(pPowerMgrX1->pInMbox->count(), 0, "pInMbox->count() != 0");
    zassert_equal(pPowerMgrX1->pInMbox, Mbox::getMboxById(Mbox::ePowerManager),
                  "Error getting mailbox by id");

    pPowerMgrX1->pInMbox->write(NULL);
    
    pPowerMgrX1->task();  
    zassert_equal(pPowerMgrX1->pInMbox->count(), 0, "pInMbox->count() != 0");

    pElem = pBoostMbox->read();
    zassert_equal(pElem->mtype, msgType::eResume, "Rcvd mtype != msgType::eResume");
    zassert_equal(pElem->from, Mbox::ePowerManager, "Rcvd sender != Mbox::ePowerManager");
	pElem->free();
	
	delete pBoostMbox;
	delete pPowerMgrX1; 
}


