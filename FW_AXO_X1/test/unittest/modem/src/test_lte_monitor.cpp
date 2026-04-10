#include <zephyr/ztest.h>
//#include <vector>
//#include <algorithm>

#define private public
#define protected public
#include "modem/lte_monitor.h"
#include "rtos/timer.h"

#include "stubs.h"

ZTEST_SUITE(tests_lte_monitor, NULL, NULL, NULL, NULL, NULL);

// test constructor and destructor
ZTEST(tests_lte_monitor, test_lte_monitor_constructor)
{
	LteMonitor* pLteMonitor = new LteMonitor("LTE");
	zassert_not_null(pLteMonitor, "memory alloc failed");
	zassert_not_null(pLteMonitor->pInMbox, "pInMbox = NULL");
    zassert_not_null(pLteMonitor->myTimer, "myTimer = NULL");

	delete pLteMonitor;
}

// MISSING for 100% branch coverage operator new does not deliver NULL
// here code lines 
// pInMbox = new Mbox(name); 
// myTimer = new Timer(RmsTask::timer_cb, this, 5, 0);
// static PoolElem e1 = {eTimerExpired, Mbox::MboxId::eLteConnector, 'l', 'm'};
// are not covered completly.


// test function timer_cb and timer1_expired
ZTEST(tests_lte_monitor, test_lte_monitor_timer_cb)
{
    PoolElem* pElem;

    LteMonitor* pLteMonitor = new LteMonitor("LTE");
	zassert_not_null(pLteMonitor, "memory alloc failed");
	zassert_not_null(pLteMonitor->pInMbox, "pInMbox = NULL");
    zassert_not_null(pLteMonitor->myTimer, "myTimer = NULL");
    zassert_equal(pLteMonitor->pInMbox->count(), 0, "pInMbox->count() != 0");
    zassert_equal(pLteMonitor->pInMbox, Mbox::getMboxById(Mbox::eLteMonitor),
                  "Error getting mailbox by id");

    pLteMonitor->timer_cb(reinterpret_cast<k_timer*>(pLteMonitor));

    zassert_equal(pLteMonitor->pInMbox->count(), 1, "pInMbox->count() != 1");
    pElem = pLteMonitor->pInMbox->read();
    zassert_equal(pElem->mtype, msgType::eTimerExpired, "Rcvd mtype != msgType::eTimerExpired");

    delete pLteMonitor;
}  


//Test receive msg eTimerExpired 
ZTEST(tests_lte_monitor, test_lte_monitor_rcv_msg_eTimerExpired)
{
    PoolElem* pElem;

	Mbox* pHttpConnMbox = new Mbox("HttpConnMbox");
    Mbox::registerMbox(pHttpConnMbox, Mbox::eHttpConnector);
	zassert_not_null(pHttpConnMbox, "pHttpConnMbox = NULL");

    LteMonitor* pLteMonitor = new LteMonitor("LTE");
	zassert_not_null(pLteMonitor, "memory alloc failed");
	zassert_not_null(pLteMonitor->pInMbox, "pInMbox = NULL");
    zassert_equal(pLteMonitor->pInMbox, Mbox::getMboxById(Mbox::eLteMonitor),
                  "Error getting mailbox by id");

    Mbox::sendMsg(msgType::eTimerExpired, Mbox::eLteMonitor, Mbox::eLteMonitor);

    zassert_equal(pLteMonitor->pInMbox->count(), 1, "pInMbox->count() != 1");

	gbU8 = 0xFF;
    pLteMonitor->task();  

    zassert_equal(pLteMonitor->pInMbox->count(), 0, "pInMbox->count() != 0");
    zassert_equal(pHttpConnMbox->count(), 1, "pHttpConnMbox->count() != 1");

	zassert(strcmp(pLteMonitor->imsi, "123456789012345")==0, "IMSI %s not correct", pLteMonitor->imsi);
	zassert(strcmp(pLteMonitor->imei, "098765432109876")==0, "IMEI %s not correct", pLteMonitor->imei);

    pElem = pHttpConnMbox->read();
    zassert_equal(pElem->mtype, msgType::eLteIMEI, "Rcvd mtype != msgType::eLteIMEI");
    zassert_equal(pElem->from, Mbox::eLteMonitor, "Rcvd sender != Mbox::eAdxlRms");

    pElem->free();
    delete pHttpConnMbox;
	delete pLteMonitor; 
}

//Test receive msg eLteLinkQuality 
ZTEST(tests_lte_monitor, test_lte_monitor_rcv_msg_eLteLinkQuality)
{
    LteMonitor* pLteMonitor = new LteMonitor("LTE");
	zassert_not_null(pLteMonitor, "memory alloc failed");
	zassert_not_null(pLteMonitor->pInMbox, "pInMbox = NULL");
    zassert_equal(pLteMonitor->pInMbox, Mbox::getMboxById(Mbox::eLteMonitor),
                  "Error getting mailbox by id");

    Mbox::sendMsg(msgType::eLteLinkQuality, Mbox::eLteMonitor, Mbox::eLteMonitor, (int8_t)-7);

    zassert_equal(pLteMonitor->pInMbox->count(), 1, "pInMbox->count() != 1");

    pLteMonitor->task();  

    zassert_equal(pLteMonitor->pInMbox->count(), 0, "pInMbox->count() != 0");
    zassert_equal(pLteMonitor->linkQuality, -7, "linkQuality != -7");

	delete pLteMonitor; 
}

//Test receive msg eLteReport 
ZTEST(tests_lte_monitor, test_lte_monitor_rcv_msg_eLteReport)
{
    PoolElem* pElem;

	Mbox* pDataMgrMbox = new Mbox("DataMgrMbox");
    Mbox::registerMbox(pDataMgrMbox, Mbox::eDataManager);
	zassert_not_null(pDataMgrMbox, "pDataMgrMbox = NULL");

    LteMonitor* pLteMonitor = new LteMonitor("LTE");
	zassert_not_null(pLteMonitor, "memory alloc failed");
	zassert_not_null(pLteMonitor->pInMbox, "pInMbox = NULL");
    zassert_equal(pLteMonitor->pInMbox, Mbox::getMboxById(Mbox::eLteMonitor),
                  "Error getting mailbox by id");

    Mbox::sendMsg(msgType::eLteReport, Mbox::eDataManager, Mbox::eLteMonitor);

    zassert_equal(pLteMonitor->pInMbox->count(), 1, "pInMbox->count() != 1");

    pLteMonitor->task();  

    zassert_equal(pLteMonitor->pInMbox->count(), 0, "pInMbox->count() != 0");
    zassert_equal(pDataMgrMbox->count(), 5, "pHttpConnMbox->count() != 5");
	//printk("pDataMgrMbox->count() = %d\n, pDataMgrMbox->count());

	zassert(strcmp(pLteMonitor->imsi, "123456789012345")==0, "IMSI %s not correct", pLteMonitor->imsi);
	zassert_equal(pLteMonitor->battVoltage, 20, "battVoltage != 20");
	zassert_equal(pLteMonitor->temperature, 25, "temperature != 25");

    pElem = pDataMgrMbox->read();
   	//printk("pElem->mtype = %d\n", pElem->mtype);
	zassert_equal(pElem->mtype, msgType::eLteLinkQuality, "Rcvd mtype != msgType::eLteLinkQuality");
    zassert_equal(pElem->from, Mbox::eLteMonitor, "Rcvd sender != Mbox::eLteMonitor");
    pElem->free();

    pElem = pDataMgrMbox->read();
   	//printk("pElem->mtype = %d\n", pElem->mtype);
	zassert_equal(pElem->mtype, msgType::eLteIMSI, "Rcvd mtype != msgType::eLteIMSI");
    zassert_equal(pElem->from, Mbox::eLteMonitor, "Rcvd sender != Mbox::eLteMonitor");
    pElem->free();

    pElem = pDataMgrMbox->read();
   	//printk("pElem->mtype = %d\n", pElem->mtype);
	zassert_equal(pElem->mtype, msgType::eLtePsmAct, "Rcvd mtype != msgType::eLtePsmAct");
    zassert_equal(pElem->from, Mbox::eLteMonitor, "Rcvd sender != Mbox::eLteMonitor");
    pElem->free();

    pElem = pDataMgrMbox->read();
   	//printk("pElem->mtype = %d\n", pElem->mtype);
	zassert_equal(pElem->mtype, msgType::eLteBattVoltage, "Rcvd mtype != msgType::eLteBattVoltage");
    zassert_equal(pElem->from, Mbox::eLteMonitor, "Rcvd sender != Mbox::eLteMonitor");
    pElem->free();

    pElem = pDataMgrMbox->read();
   	//printk("pElem->mtype = %d\n", pElem->mtype);
	zassert_equal(pElem->mtype, msgType::eLteTemperature, "Rcvd mtype != msgType::eLteTemperature");
    zassert_equal(pElem->from, Mbox::eLteMonitor, "Rcvd sender != Mbox::eLteMonitor");
    pElem->free();

    delete pDataMgrMbox;
	delete pLteMonitor; 
}

//Test receive unexpected msg 
ZTEST(tests_lte_monitor, test_lte_monitor_rcv_msg_unexpected)
{
    LteMonitor* pLteMonitor = new LteMonitor("LTE");
	zassert_not_null(pLteMonitor, "memory alloc failed");
	zassert_not_null(pLteMonitor->pInMbox, "pInMbox = NULL");
    zassert_equal(pLteMonitor->pInMbox->count(), 0, "pInMbox->count() != 0");
    zassert_equal(pLteMonitor->pInMbox, Mbox::getMboxById(Mbox::eLteMonitor),
                  "Error getting mailbox by id");

    Mbox::sendMsg(msgType::eResetCause, Mbox::eLteMonitor, Mbox::eLteMonitor);

    zassert_equal(pLteMonitor->pInMbox->count(), 1, "pInMbox->count() != 1");

    pLteMonitor->task();  

    zassert_equal(pLteMonitor->pInMbox->count(), 0, "pInMbox->count() != 0");
	delete pLteMonitor; 
}

//Test receive no msg 
ZTEST(tests_lte_monitor, test_lte_monitor_rcv_msg_none)
{
    LteMonitor* pLteMonitor = new LteMonitor("LTE");
	zassert_not_null(pLteMonitor, "memory alloc failed");
	zassert_not_null(pLteMonitor->pInMbox, "pInMbox = NULL");
    zassert_equal(pLteMonitor->pInMbox->count(), 0, "pInMbox->count() != 0");
    zassert_equal(pLteMonitor->pInMbox, Mbox::getMboxById(Mbox::eLteMonitor),
                  "Error getting mailbox by id");

    pLteMonitor->pInMbox->write(NULL);
    
    pLteMonitor->task();  

    zassert_equal(pLteMonitor->pInMbox->count(), 0, "pInMbox->count() != 0");
	delete pLteMonitor; 
}

//Test function resume_all_notifications, ...
ZTEST(tests_lte_monitor, test_lte_monitor_resume_all_notifications)
{
	char buf[32];
	const char* pBuf = &buf[0];
	int rc;

    LteMonitor* pLteMonitor = new LteMonitor("LTE");
	zassert_not_null(pLteMonitor, "memory alloc failed");
	zassert_not_null(pLteMonitor->pInMbox, "pInMbox = NULL");

	pLteMonitor->resume_all_notifications();
	zassert_equal(pLteMonitor->catch_all.filter, NULL, "catch_all.filter != NULL");
	zassert_equal(pLteMonitor->catch_all.flags.direct, 1, "catch_all.flags.direct != 1");
	zassert_equal(pLteMonitor->catch_all.flags.paused, 1, "catch_all.flags.paused != 1");

	pLteMonitor->at_notif_handler(NULL);

	pLteMonitor->register_link_quality_notifications();

	pLteMonitor->resume_link_quality_notifications();
	zassert_equal(pLteMonitor->link_quality.filter, NULL, "link_quality.filter != NULL");
	zassert_equal(pLteMonitor->link_quality.flags.direct, 1, "link_quality.flags.direct != 1");
	zassert_equal(pLteMonitor->link_quality.flags.paused, 1, "link_quality.flags.paused != 1");

	pLteMonitor->register_network_notifications();

	pLteMonitor->resume_network_notifications();
	zassert_equal(pLteMonitor->network.filter, NULL, "network.filter != NULL");
	zassert_equal(pLteMonitor->network.flags.direct, 1, "network.flags.direct != 1");
	zassert_equal(pLteMonitor->network.flags.paused, 1, "network.flags.paused != 1");

	pBuf = pLteMonitor->cereg_str_get(NO_NETWORK);
	zassert(strcmp(pBuf, "no network")==0, "ret0 %s not correct", pBuf);
	pBuf = pLteMonitor->cereg_str_get(HOME);
	zassert(strcmp(pBuf, "home")==0, "ret1 %s not correct", pBuf);
	pBuf = pLteMonitor->cereg_str_get(SEARCHING);
	zassert(strcmp(pBuf, "searching")==0, "ret2 %s not correct", pBuf);
	pBuf = pLteMonitor->cereg_str_get(DENIED);
	zassert(strcmp(pBuf, "denied")==0, "ret3 %s not correct", pBuf);
	pBuf = pLteMonitor->cereg_str_get(UNKNOWN);
	zassert(strcmp(pBuf, "unknown")==0, "ret4 %s not correct", pBuf);
	pBuf = pLteMonitor->cereg_str_get(ROAMING);
	zassert(strcmp(pBuf, "roaming")==0, "ret5 %s not correct", pBuf);
	pBuf = pLteMonitor->cereg_str_get(EMERGENCY);
	zassert(strcmp(pBuf, "emergency")==0, "ret6 %s not correct", pBuf);
	pBuf = pLteMonitor->cereg_str_get(UICC_FAILURE);
	zassert(strcmp(pBuf, "UICC failure")==0, "ret7 %s not correct", pBuf);
	pBuf = pLteMonitor->cereg_str_get((enum cereg_status)99);
	zassert(strcmp(pBuf, "unexpected")==0, "ret8 %s not correct", pBuf);

	pLteMonitor->at_cereg_handler("1");

    gbU8 = 0;
	pLteMonitor->psm_read();
    gbU8 = 1;
	pLteMonitor->psm_read();
    gbU8 = 2;
	pLteMonitor->psm_read();
    gbU8 = 3;
	pLteMonitor->psm_read();
    gbU8 = 0xFF;
	pLteMonitor->psm_read();

	gbU8 = 0;
	rc = pLteMonitor->getIMEI();
	zassert_equal(rc, -5, "rc != -5");

	rc = pLteMonitor->getIMSI();
	zassert_equal(rc, -5, "rc != -5");

	delete pLteMonitor; 
}

//Test function at_ecsq_handler
ZTEST(tests_lte_monitor, test_lte_monitor_at_ecsq_handler)
{
    LteMonitor* pLteMonitor = new LteMonitor("LTE");
	zassert_not_null(pLteMonitor, "memory alloc failed");
	zassert_not_null(pLteMonitor->pInMbox, "pInMbox = NULL");
    zassert_equal(pLteMonitor->pInMbox->count(), 0, "pInMbox->count() != 0");
    zassert_equal(pLteMonitor->pInMbox, Mbox::getMboxById(Mbox::eLteMonitor),
                  "Error getting mailbox by id");

	gbU8 = 0;
	pLteMonitor->at_ecsq_handler("22");
    zassert_equal(pLteMonitor->pInMbox->count(), 1, "pInMbox->count() != 1");

	pLteMonitor->task(); 
    zassert_equal(pLteMonitor->pInMbox->count(), 0, "pInMbox->count() != 0");

	gbU8 = 10;
	pLteMonitor->at_ecsq_handler("22");
    zassert_equal(pLteMonitor->pInMbox->count(), 1, "pInMbox->count() != 1");

	pLteMonitor->task(); 
    zassert_equal(pLteMonitor->pInMbox->count(), 0, "pInMbox->count() != 0");
	delete pLteMonitor; 
}

//Test function reportFwVersion
ZTEST(tests_lte_monitor, test_lte_monitor_reportFwVersion)
{
    PoolElem* pElem;

	Mbox* pDataMgrMbox = new Mbox("DataMgrMbox");
    Mbox::registerMbox(pDataMgrMbox, Mbox::eDataManager);
	zassert_not_null(pDataMgrMbox, "pDataMgrMbox = NULL");

    LteMonitor* pLteMonitor = new LteMonitor("LTE");
	zassert_not_null(pLteMonitor, "memory alloc failed");
	zassert_not_null(pLteMonitor->pInMbox, "pInMbox = NULL");
    zassert_equal(pLteMonitor->pInMbox->count(), 0, "pInMbox->count() != 0");
    zassert_equal(pLteMonitor->pInMbox, Mbox::getMboxById(Mbox::eLteMonitor),
                  "Error getting mailbox by id");

	pLteMonitor->reportFwVersion(Mbox::eDataManager);
    pElem = pDataMgrMbox->read();
   	//printk("pElem->mtype = %d\n", pElem->mtype);
	zassert_equal(pElem->mtype, msgType::eLteFwVersion, "Rcvd mtype != msgType::eLteFwVersion");
    zassert_equal(pElem->from, Mbox::eLteMonitor, "Rcvd sender != Mbox::eLteMonitor");
    pElem->free();

    delete pDataMgrMbox;
	delete pLteMonitor; 
}

