#include <zephyr/ztest.h>

#define private public
#include "drv/adc/batt_mgr.h"
#include "drv/adc/adconverter.h"

#include "stubs.h"

ZTEST_SUITE(tests_drv_BatteryMgr, NULL, NULL, NULL, NULL, NULL);

// test constructor and destructor
ZTEST(tests_drv_BatteryMgr, test_drv_BatteryMgr_constructor)
{
	BatteryMgr* pBattMgr = new BatteryMgr("battmgr");
	zassert_not_null(pBattMgr, "memory alloc failed");
	zassert_not_null(pBattMgr->pInMbox, "pInMbox = NULL");
    zassert_equal(pBattMgr->pInMbox->count(), 0, "pInMbox->count() != 0");
    zassert_equal(pBattMgr->pInMbox, Mbox::getMboxById(Mbox::eBattMgr),
                  "Error getting mailbox by id");

	//printk("%x\n", pBattMgr->adcChannels);
	zassert_equal(pBattMgr->adcChannels, 0x8E, "pBattMgr->adcChannels != 0x47");
 
	delete pBattMgr;
}

// MISSING for 100% branch coverage static instantiation does not deliver NULL
// here code line 
// pInMbox = new Mbox(name);
// are not covered completly.

// test function reqAdcReport
ZTEST(tests_drv_BatteryMgr, test_drv_BatteryMgr_reqAdcReport)
{
    PoolElem* pElem;

	Mbox* pAdcMbox = new Mbox("AdcMbox");
    Mbox::registerMbox(pAdcMbox, Mbox::eADConverter);
	zassert_not_null(pAdcMbox, "pAdcMbox = NULL");

	BatteryMgr* pBattMgr = new BatteryMgr("battmgr");
	zassert_not_null(pBattMgr, "memory alloc failed");
	zassert_not_null(pBattMgr->pInMbox, "pInMbox = NULL");
    zassert_equal(pBattMgr->pInMbox->count(), 0, "pInMbox->count() != 0");
    zassert_equal(pBattMgr->pInMbox, Mbox::getMboxById(Mbox::eBattMgr),
                  "Error getting mailbox by id");

	pBattMgr->reqAdcReport(0xFF);
 
    pElem = pAdcMbox->read();
    zassert_equal(pElem->mtype, msgType::eAdcReport, "Rcvd mtype != msgType::eAdcReport");
    zassert_equal(pElem->from, Mbox::eBattMgr, "Rcvd sender != Mbox::eBattMgr");

    pElem->free();
    delete pAdcMbox;
	delete pBattMgr;
}

//Test receive msg eTimerExpired
ZTEST(tests_drv_BatteryMgr, test_drv_BatteryMgr_rcv_msg_eTimerExpired)
{
	BatteryMgr* pBattMgr = new BatteryMgr("battmgr");
	zassert_not_null(pBattMgr, "memory alloc failed");
	zassert_not_null(pBattMgr->pInMbox, "pInMbox = NULL");
    zassert_equal(pBattMgr->pInMbox->count(), 0, "pInMbox->count() != 0");
    zassert_equal(pBattMgr->pInMbox, Mbox::getMboxById(Mbox::eBattMgr),
                  "Error getting mailbox by id");

    Mbox::sendMsg(msgType::eTimerExpired, Mbox::eBattMgr, Mbox::eBattMgr);

    zassert_equal(pBattMgr->pInMbox->count(), 1, "pInMbox->count() != 1");

    pBattMgr->task();  

    zassert_equal(pBattMgr->pInMbox->count(), 0, "pInMbox->count() != 0");

	delete pBattMgr; 
}

//Test receive msg eAdcData
ZTEST(tests_drv_BatteryMgr, test_drv_BatteryMgr_rcv_msg_eAdcData)
{
    PoolElem* pElem;

	Mbox* pDataMgrMbox = new Mbox("DataMgrMbox");
    Mbox::registerMbox(pDataMgrMbox, Mbox::eDataManager);
	zassert_not_null(pDataMgrMbox, "pDataMgrMbox = NULL");

	BatteryMgr* pBattMgr = new BatteryMgr("battmgr");
	zassert_not_null(pBattMgr, "memory alloc failed");
	zassert_not_null(pBattMgr->pInMbox, "pInMbox = NULL");
    zassert_equal(pBattMgr->pInMbox->count(), 0, "pInMbox->count() != 0");
    zassert_equal(pBattMgr->pInMbox, Mbox::getMboxById(Mbox::eBattMgr),
                  "Error getting mailbox by id");

    struct ADInput ad_in = {7, 17, 27};
	Mbox::sendMsg(msgType::eAdcData, Mbox::eBattMgr, Mbox::eBattMgr, (uint8_t*)&ad_in, sizeof(ad_in));

    zassert_equal(pBattMgr->pInMbox->count(), 1, "pInMbox->count() != 1");

    pBattMgr->task();  

    zassert_equal(pBattMgr->pInMbox->count(), 0, "pInMbox->count() != 0");

	pElem = pDataMgrMbox->read(); 
    zassert_equal(pElem->mtype, msgType::eAdcData, "Rcvd mtype != msgType::eAdcData");
    zassert_equal(pElem->from, Mbox::eBattMgr, "Rcvd sender != Mbox::eBattMgr");

	struct ADInput ad_in2 =  *reinterpret_cast<struct ADInput*>(pElem->getBuffer());
    zassert_equal(ad_in2.channel, 7, "channel != 7");
    zassert_equal(ad_in2.raw, 17, "raw != 17");
    zassert_equal(ad_in2.mv, 27, "mv != 27");

	pElem->free();
	delete pDataMgrMbox;
	delete pBattMgr; 
}

//Test receive msg eBattReport
ZTEST(tests_drv_BatteryMgr, test_drv_BatteryMgr_rcv_msg_eBattReport)
{
    PoolElem* pElem;

	Mbox* pAdcMbox = new Mbox("AdcMbox");
    Mbox::registerMbox(pAdcMbox, Mbox::eADConverter);
	zassert_not_null(pAdcMbox, "pAdcMbox = NULL");

	BatteryMgr* pBattMgr = new BatteryMgr("battmgr");
	zassert_not_null(pBattMgr, "memory alloc failed");
	zassert_not_null(pBattMgr->pInMbox, "pInMbox = NULL");
    zassert_equal(pBattMgr->pInMbox->count(), 0, "pInMbox->count() != 0");
    zassert_equal(pBattMgr->pInMbox, Mbox::getMboxById(Mbox::eBattMgr),
                  "Error getting mailbox by id");

	pBattMgr->adcChannels = 0xFF;
	Mbox::sendMsg(msgType::eBattReport, Mbox::eBattMgr, Mbox::eBattMgr);

    zassert_equal(pBattMgr->pInMbox->count(), 1, "pInMbox->count() != 1");

    pBattMgr->task();  

    zassert_equal(pBattMgr->pInMbox->count(), 0, "pInMbox->count() != 0");

	pElem = pAdcMbox->read(); 
    zassert_equal(pElem->mtype, msgType::eAdcReport, "Rcvd mtype != msgType::eAdcReport");
    zassert_equal(pElem->from, Mbox::eBattMgr, "Rcvd sender != Mbox::eBattMgr");

    pElem->free();
    delete pAdcMbox;
	delete pBattMgr;
}



//Test receive unexpected msg 
ZTEST(tests_drv_BatteryMgr, test_drv_BatteryMgr_rcv_msg_unexpected)
{
	BatteryMgr* pBattMgr = new BatteryMgr("battmgr");
	zassert_not_null(pBattMgr, "memory alloc failed");
	zassert_not_null(pBattMgr->pInMbox, "pInMbox = NULL");
    zassert_equal(pBattMgr->pInMbox->count(), 0, "pInMbox->count() != 0");
    zassert_equal(pBattMgr->pInMbox, Mbox::getMboxById(Mbox::eBattMgr),
                  "Error getting mailbox by id");

    Mbox::sendMsg(msgType::eResetCause, Mbox::eBattMgr, Mbox::eBattMgr);

    zassert_equal(pBattMgr->pInMbox->count(), 1, "pInMbox->count() != 1");

    pBattMgr->task();  

    zassert_equal(pBattMgr->pInMbox->count(), 0, "pInMbox->count() != 0");
	delete pBattMgr; 
}

//Test receive no msg 
ZTEST(tests_drv_BatteryMgr, test_drv_BatteryMgr_rcv_msg_none)
{
	BatteryMgr* pBattMgr = new BatteryMgr("battmgr");
	zassert_not_null(pBattMgr, "memory alloc failed");
	zassert_not_null(pBattMgr->pInMbox, "pInMbox = NULL");
    zassert_equal(pBattMgr->pInMbox->count(), 0, "pInMbox->count() != 0");
    zassert_equal(pBattMgr->pInMbox, Mbox::getMboxById(Mbox::eBattMgr),
                  "Error getting mailbox by id");

    pBattMgr->pInMbox->write(NULL);
    
    pBattMgr->task();  

    zassert_equal(pBattMgr->pInMbox->count(), 0, "pInMbox->count() != 0");
	delete pBattMgr; 
}



