#include <zephyr/ztest.h>
//#include <vector>
//#include <algorithm>

#define private public
#define protected public
#include "pm/power_mgr.h"
#include "conf/config.h"
#include "rtos/timer.h"

#include "stubs.h"

class PowerMgrTest : public PowerMgr {
 public:
    explicit PowerMgrTest(const char* name = "??");
    virtual ~PowerMgrTest();
	virtual void taskInit(void);
	virtual PoolElem* message_handler(PoolElem* pElem);

 private:
};

PowerMgrTest::PowerMgrTest(const char* name) : PowerMgr(name) { }
PowerMgrTest::~PowerMgrTest() { }
void PowerMgrTest::taskInit(void) { }

PoolElem* PowerMgrTest::message_handler(PoolElem* pElem) { 
	if (gbRetVal) {
		return pElem;
	}
	return NULL;
}

ZTEST_SUITE(tests_pm_power_mgr, NULL, NULL, NULL, NULL, NULL);

// test constructor and destructor
ZTEST(tests_pm_power_mgr, test_pm_power_mgr_constructor)
{
	PowerMgrTest* pPowerMgr = new PowerMgrTest("PowerMgr");
	zassert_not_null(pPowerMgr, "memory alloc failed");
	zassert_not_null(pPowerMgr->pInMbox, "pInMbox = NULL");
	zassert_not_null(pPowerMgr->MetricsTxTimer, "MetricsTxTimer = NULL");
    zassert_equal(pPowerMgr->pInMbox->count(), 0, "pInMbox->count() != 0");
    zassert_equal(pPowerMgr->pInMbox, Mbox::getMboxById(Mbox::ePowerManager),
                  "Error getting mailbox by id");

	delete pPowerMgr;
}

// MISSING for 100% branch coverage operator new does not deliver NULL
// here code lines 
// pInMbox = new Mbox(name); 
// MetricsTxTimer = new Timer(PowerMgr::MetricsTxTimer_cb, this, eMetricsTmo, eMetricsTmo);
// static PoolElem e1 = {eMetricsTxTimerExpired, Mbox::MboxId::ePowerManager, 'p', 'm'};
// are not covered completly.



// test function uart0_off
ZTEST(tests_pm_power_mgr, test_pm_power_mgr_uart0_off)
{
	PowerMgrTest* pPowerMgr = new PowerMgrTest("PowerMgr");
	zassert_not_null(pPowerMgr, "memory alloc failed");
	zassert_not_null(pPowerMgr->pInMbox, "pInMbox = NULL");
	zassert_not_null(pPowerMgr->MetricsTxTimer, "MetricsTxTimer = NULL");
    zassert_equal(pPowerMgr->pInMbox->count(), 0, "pInMbox->count() != 0");
    zassert_equal(pPowerMgr->pInMbox, Mbox::getMboxById(Mbox::ePowerManager),
                  "Error getting mailbox by id");

    pPowerMgr->uart0_off();				  

	delete pPowerMgr;
}

// test function uart0_set_enable
ZTEST(tests_pm_power_mgr, test_pm_power_mgr_uart0_set_enable)
{
	PowerMgrTest* pPowerMgr = new PowerMgrTest("PowerMgr");
	zassert_not_null(pPowerMgr, "memory alloc failed");
	zassert_not_null(pPowerMgr->pInMbox, "pInMbox = NULL");
	zassert_not_null(pPowerMgr->MetricsTxTimer, "MetricsTxTimer = NULL");
    zassert_equal(pPowerMgr->pInMbox->count(), 0, "pInMbox->count() != 0");
    zassert_equal(pPowerMgr->pInMbox, Mbox::getMboxById(Mbox::ePowerManager),
                  "Error getting mailbox by id");

    pPowerMgr->uart0_set_enable(true);				  
    pPowerMgr->uart0_set_enable(false);				  

	delete pPowerMgr;
}

// test function reqRmsReport
ZTEST(tests_pm_power_mgr, test_pm_power_mgr_reqRmsReport)
{
    PoolElem* pElem;

	Mbox* pRmsStoreMbox = new Mbox("RmsStoreMbox");
    Mbox::registerMbox(pRmsStoreMbox, Mbox::eRmsStore);
	zassert_not_null(pRmsStoreMbox, "pRmsStoreMbox = NULL");

	PowerMgrTest* pPowerMgr = new PowerMgrTest("PowerMgr");
	zassert_not_null(pPowerMgr, "memory alloc failed");
	zassert_not_null(pPowerMgr->pInMbox, "pInMbox = NULL");
	zassert_not_null(pPowerMgr->MetricsTxTimer, "MetricsTxTimer = NULL");
    zassert_equal(pPowerMgr->pInMbox->count(), 0, "pInMbox->count() != 0");
    zassert_equal(pPowerMgr->pInMbox, Mbox::getMboxById(Mbox::ePowerManager),
                  "Error getting mailbox by id");

    pPowerMgr->reqRmsReport();				  

    pElem = pRmsStoreMbox->read();
    zassert_equal(pElem->mtype, msgType::eRmsReport, "Rcvd mtype != msgType::eRmsReport");
    zassert_equal(pElem->from, Mbox::ePowerManager, "Rcvd sender != Mbox::ePowerManager");

	pElem->free();
	delete pRmsStoreMbox;
	delete pPowerMgr;
}


// test function reportResetCause
ZTEST(tests_pm_power_mgr, test_pm_power_mgr_reportResetCause)
{
    PoolElem* pElem;
	std::string s;

	Mbox* pDataMgrMbox = new Mbox("DataMgrMbox");
    Mbox::registerMbox(pDataMgrMbox, Mbox::eDataManager);
	zassert_not_null(pDataMgrMbox, "pDataMgrMbox = NULL");

	PowerMgrTest* pPowerMgr = new PowerMgrTest("PowerMgr");
	zassert_not_null(pPowerMgr, "memory alloc failed");
	zassert_not_null(pPowerMgr->pInMbox, "pInMbox = NULL");
	zassert_not_null(pPowerMgr->MetricsTxTimer, "MetricsTxTimer = NULL");
    zassert_equal(pPowerMgr->pInMbox->count(), 0, "pInMbox->count() != 0");
    zassert_equal(pPowerMgr->pInMbox, Mbox::getMboxById(Mbox::ePowerManager),
                  "Error getting mailbox by id");

	pPowerMgr->reason_str = "QWERTZ";
    pPowerMgr->reportResetCause();				  

    pElem = pDataMgrMbox->read();
	s = std::string(reinterpret_cast<char*>(pElem->getBuffer()));
    zassert_equal(pElem->mtype, msgType::eResetCause, "Rcvd mtype != msgType::eResetCause");
    zassert_equal(pElem->from, Mbox::ePowerManager, "Rcvd sender != Mbox::ePowerManager");
	zassert_equal(strcmp(s.c_str(), "QWERTZ"), 0, "Wrong rcvd string");

	pElem->free();
	delete pDataMgrMbox;
	delete pPowerMgr;
}

// test function reportBootCnt
ZTEST(tests_pm_power_mgr, test_pm_power_mgr_reportBootCnt)
{
    PoolElem* pElem;

	Mbox* pDataMgrMbox = new Mbox("DataMgrMbox");
    Mbox::registerMbox(pDataMgrMbox, Mbox::eDataManager);
	zassert_not_null(pDataMgrMbox, "pDataMgrMbox = NULL");

	PowerMgrTest* pPowerMgr = new PowerMgrTest("PowerMgr");
	zassert_not_null(pPowerMgr, "memory alloc failed");
	zassert_not_null(pPowerMgr->pInMbox, "pInMbox = NULL");
	zassert_not_null(pPowerMgr->MetricsTxTimer, "MetricsTxTimer = NULL");
    zassert_equal(pPowerMgr->pInMbox->count(), 0, "pInMbox->count() != 0");
    zassert_equal(pPowerMgr->pInMbox, Mbox::getMboxById(Mbox::ePowerManager),
                  "Error getting mailbox by id");

	pPowerMgr->boot_cnt = 17;
    pPowerMgr->reportBootCnt();				  

    pElem = pDataMgrMbox->read();
    int32_t cnt = *reinterpret_cast<int32_t*>(pElem->getBuffer());
    zassert_equal(pElem->mtype, msgType::eBootCnt, "Rcvd mtype != msgType::eBootCnt");
    zassert_equal(pElem->from, Mbox::ePowerManager, "Rcvd sender != Mbox::ePowerManager");
	zassert_equal(cnt, 17, "Wrong boot count");

	pElem->free();
	delete pDataMgrMbox;
	delete pPowerMgr;
}

// test function MetricsTxTimer_cb and MetricsTxTimer_expired
ZTEST(tests_pm_power_mgr, test_pm_power_mgr_timer_cb)
{
    PoolElem* pElem;

	PowerMgrTest* pPowerMgr = new PowerMgrTest("PowerMgr");
	zassert_not_null(pPowerMgr, "memory alloc failed");
	zassert_not_null(pPowerMgr->pInMbox, "pInMbox = NULL");
	zassert_not_null(pPowerMgr->MetricsTxTimer, "MetricsTxTimer = NULL");
    zassert_equal(pPowerMgr->pInMbox->count(), 0, "pInMbox->count() != 0");
    zassert_equal(pPowerMgr->pInMbox, Mbox::getMboxById(Mbox::ePowerManager),
                  "Error getting mailbox by id");

    pPowerMgr->MetricsTxTimer_cb(reinterpret_cast<k_timer*>(pPowerMgr));

    zassert_equal(pPowerMgr->pInMbox->count(), 1, "pInMbox->count() != 1");
    pElem = pPowerMgr->pInMbox->read();
    zassert_equal(pElem->mtype, msgType::eMetricsTxTimerExpired, "Rcvd mtype != msgType::eMetricsTxTimerExpired");

    delete pPowerMgr;
}    

// test function registerMetricProvider, metricProviderNotify and metricProviderNotifyAll
ZTEST(tests_pm_power_mgr, test_pm_power_mgr_metricProvider)
{
    PoolElem* pElem;

	Mbox* pDataMgrMbox = new Mbox("DataMgrMbox");
    Mbox::registerMbox(pDataMgrMbox, Mbox::eDataManager);
	zassert_not_null(pDataMgrMbox, "pDataMgrMbox = NULL");

	Mbox* pRmsStoreMbox = new Mbox("RmsStoreMbox");
    Mbox::registerMbox(pRmsStoreMbox, Mbox::eRmsStore);
	zassert_not_null(pRmsStoreMbox, "pRmsStoreMbox = NULL");

	PowerMgrTest* pPowerMgr = new PowerMgrTest("PowerMgr");
	zassert_not_null(pPowerMgr, "memory alloc failed");
	zassert_not_null(pPowerMgr->pInMbox, "pInMbox = NULL");
	zassert_not_null(pPowerMgr->MetricsTxTimer, "MetricsTxTimer = NULL");
    zassert_equal(pPowerMgr->pInMbox->count(), 0, "pInMbox->count() != 0");
    zassert_equal(pPowerMgr->pInMbox, Mbox::getMboxById(Mbox::ePowerManager),
                  "Error getting mailbox by id");

    pPowerMgr->registerMetricProvider(Mbox::eDataManager, msgType::eBattReport);				  
    pPowerMgr->registerMetricProvider(Mbox::eRmsStore, msgType::eBattReport);	

	pPowerMgr->metricProviderNotifyAll();			  

    pElem = pDataMgrMbox->read();
    zassert_equal(pElem->mtype, msgType::eBattReport, "Rcvd mtype != msgType::eBattReport");
    zassert_equal(pElem->from, Mbox::ePowerManager, "Rcvd sender != Mbox::ePowerManager");
	pElem->free();

    pElem = pRmsStoreMbox->read();
    zassert_equal(pElem->mtype, msgType::eBattReport, "Rcvd mtype != msgType::eBattReport");
    zassert_equal(pElem->from, Mbox::ePowerManager, "Rcvd sender != Mbox::ePowerManager");

	pElem->free();
	delete pDataMgrMbox;
	delete pPowerMgr;
}

// test function task
ZTEST(tests_pm_power_mgr, test_pm_power_mgr_task)
{
	PowerMgrTest* pPowerMgr = new PowerMgrTest("PowerMgr");
	zassert_not_null(pPowerMgr, "memory alloc failed");
	zassert_not_null(pPowerMgr->pInMbox, "pInMbox = NULL");
	zassert_not_null(pPowerMgr->MetricsTxTimer, "MetricsTxTimer = NULL");
    zassert_equal(pPowerMgr->pInMbox->count(), 0, "pInMbox->count() != 0");
    zassert_equal(pPowerMgr->pInMbox, Mbox::getMboxById(Mbox::ePowerManager),
                  "Error getting mailbox by id");

    //gbRetVal = false;
	pPowerMgr->pInMbox->write(NULL);
    pPowerMgr->task();

    delete pPowerMgr;
}    

// test function task
ZTEST(tests_pm_power_mgr, test_pm_power_mgr_task2)
{
    PoolElem* e1 = new PoolElem(eTimerExpired, Mbox::MboxId::eDemoTask);
    e1->buf = NULL;
    e1->bufLen = 0;

	PowerMgrTest* pPowerMgr = new PowerMgrTest("PowerMgr");
	zassert_not_null(pPowerMgr, "memory alloc failed");
	zassert_not_null(pPowerMgr->pInMbox, "pInMbox = NULL");
	zassert_not_null(pPowerMgr->MetricsTxTimer, "MetricsTxTimer = NULL");
    zassert_equal(pPowerMgr->pInMbox->count(), 0, "pInMbox->count() != 0");
    zassert_equal(pPowerMgr->pInMbox, Mbox::getMboxById(Mbox::ePowerManager),
                  "Error getting mailbox by id");

    gbRetVal = true;
	pPowerMgr->pInMbox->write(e1);
    zassert_equal(pPowerMgr->pInMbox->count(), 1, "pInMbox->count() != 1");

    pPowerMgr->task();
    zassert_equal(pPowerMgr->pInMbox->count(), 0, "pInMbox->count() != 0");

    delete pPowerMgr;
}    

// test function task
ZTEST(tests_pm_power_mgr, test_pm_power_mgr_task3)
{
    PoolElem* e1 = new PoolElem(eTimerExpired, Mbox::MboxId::eDemoTask);
    e1->buf = NULL;
    e1->bufLen = 0;

	PowerMgrTest* pPowerMgr = new PowerMgrTest("PowerMgr");
	zassert_not_null(pPowerMgr, "memory alloc failed");
	zassert_not_null(pPowerMgr->pInMbox, "pInMbox = NULL");
	zassert_not_null(pPowerMgr->MetricsTxTimer, "MetricsTxTimer = NULL");
    zassert_equal(pPowerMgr->pInMbox->count(), 0, "pInMbox->count() != 0");
    zassert_equal(pPowerMgr->pInMbox, Mbox::getMboxById(Mbox::ePowerManager),
                  "Error getting mailbox by id");

    gbRetVal = false;
	pPowerMgr->pInMbox->write(e1);
    zassert_equal(pPowerMgr->pInMbox->count(), 1, "pInMbox->count() != 1");

    pPowerMgr->task();
    zassert_equal(pPowerMgr->pInMbox->count(), 0, "pInMbox->count() != 0");

    delete pPowerMgr;
}    

// test function configure
ZTEST(tests_pm_power_mgr, test_pm_power_mgr_configure)
{
	PowerMgrTest* pPowerMgr = new PowerMgrTest("PowerMgr");
	zassert_not_null(pPowerMgr, "memory alloc failed");
	zassert_not_null(pPowerMgr->pInMbox, "pInMbox = NULL");
	zassert_not_null(pPowerMgr->MetricsTxTimer, "MetricsTxTimer = NULL");
    zassert_equal(pPowerMgr->pInMbox->count(), 0, "pInMbox->count() != 0");
    zassert_equal(pPowerMgr->pInMbox, Mbox::getMboxById(Mbox::ePowerManager),
                  "Error getting mailbox by id");

	Config::inst()->cfgMetrics.interval = "10";

    pPowerMgr->configure();
    zassert_equal(pPowerMgr->metricsTmo, 600, "metricsTmo != 600");
    zassert_equal(pPowerMgr->MetricsTxTimer->duration, 600, "duration != 600");
    zassert_equal(pPowerMgr->MetricsTxTimer->period, 600, "period != 600");

    delete pPowerMgr;
}    

