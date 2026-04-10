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
#include "rtos/timer.h"

#include "stubs.h"

#include "data/pdata.h"


ZTEST_SUITE(tests_pdata, NULL, NULL, NULL, NULL, NULL);

// test constructor and destructor
ZTEST(tests_pdata, test_pdata_constructor)
{
	DataManager* pDataManager = new DataManager("data_mgr", true);

	zassert_not_null(pDataManager, "memory alloc failed");
	zassert_not_null(pDataManager->pInMbox, "pInMbox = NULL");   
    zassert_not_null(pDataManager->myTimer, "myTimer = NULL");
    zassert_equal(pDataManager->pInMbox->count(), 0, "pInMbox->count() != 0");
    zassert_equal(pDataManager->pInMbox, Mbox::getMboxById(Mbox::eDataManager),
                  "Error getting mailbox by id");

    zassert_equal(pDataManager->have_mqtt_connector, true, "have_mqtt_connector != true");

	delete pDataManager; 
}

// MISSING for 100% branch coverage static instantiation does not deliver NULL
// here code line 
// pInMbox = new Mbox(name); 
// myTimer = new Timer(DataManager::timer_cb, this, 10, 20);
// static PoolElem e1 = {eTimerExpired, Mbox::MboxId::eDataManager, 'd', 'd'}; 
// are not covered completly.

// test function timer_cb and timer1_expired
ZTEST(tests_pdata, test_pdata_timer_cb)
{
    PoolElem* pElem;

    DataManager* pDataManager = new DataManager("data_mgr", true);
	zassert_not_null(pDataManager, "memory alloc failed");
	zassert_not_null(pDataManager->pInMbox, "pInMbox = NULL");
    zassert_equal(pDataManager->pInMbox->count(), 0, "pInMbox->count() != 0");
    zassert_equal(pDataManager->pInMbox, Mbox::getMboxById(Mbox::eDataManager),
                  "Error getting mailbox by id");

    pDataManager->timer_cb(reinterpret_cast<k_timer*>(pDataManager));

    zassert_equal(pDataManager->pInMbox->count(), 1, "pInMbox->count() != 1");
    pElem = pDataManager->pInMbox->read();
    zassert_equal(pElem->mtype, msgType::eTimerExpired, "Rcvd mtype != msgType::eTimerExpired");

	delete pDataManager; 
}  

//Test function configure
ZTEST(tests_pdata, test_pdata_configure)
{
    DataManager* pDataManager = new DataManager("data_mgr", true);
	zassert_not_null(pDataManager, "memory alloc failed");
	zassert_not_null(pDataManager->pInMbox, "pInMbox = NULL");
    zassert_equal(pDataManager->pInMbox->count(), 0, "pInMbox->count() != 0");
    zassert_equal(pDataManager->pInMbox, Mbox::getMboxById(Mbox::eDataManager),
                  "Error getting mailbox by id");

    gbRetVal = true;
    pDataManager->configure();  
    zassert_true(pDataManager->bDispModeOff, "bDispModeOff != true");
    zassert(strcmp(pDataManager->metrics.pubkey.c_str(), "000##LF##12345##LF##67890##LF##")==0, "pubkey incorrect");  
    //printk("Key = %s\n\n", pDataManager->metrics.pubkey.c_str());  
    
	delete pDataManager; 
}
ZTEST(tests_pdata, test_pdata_configure2)
{
    DataManager* pDataManager = new DataManager("data_mgr", true);
	zassert_not_null(pDataManager, "memory alloc failed");
	zassert_not_null(pDataManager->pInMbox, "pInMbox = NULL");
    zassert_equal(pDataManager->pInMbox->count(), 0, "pInMbox->count() != 0");
    zassert_equal(pDataManager->pInMbox, Mbox::getMboxById(Mbox::eDataManager),
                  "Error getting mailbox by id");

    gbRetVal = false;
    pDataManager->configure();  
    zassert_false(pDataManager->bDispModeOff, "bDispModeOff != false");
    zassert(strcmp(pDataManager->metrics.pubkey.c_str(), "000##LF##12345##LF##67890##LF##")==0, "pubkey incorrect");   
    //printk("Key = %s\n\n", pDataManager->metrics.pubkey.c_str()); 
    
	delete pDataManager; 
}

//Test function publishMeasurements with have_mqtt_connector = false;
ZTEST(tests_pdata, test_pdata_publishMeasurements)
{
    DataManager* pDataManager = new DataManager("data_mgr", true);
	zassert_not_null(pDataManager, "memory alloc failed");
	zassert_not_null(pDataManager->pInMbox, "pInMbox = NULL");
    zassert_equal(pDataManager->pInMbox->count(), 0, "pInMbox->count() != 0");
    zassert_equal(pDataManager->pInMbox, Mbox::getMboxById(Mbox::eDataManager),
                  "Error getting mailbox by id");

    gbRetVal = true;
    pDataManager->have_mqtt_connector = false;
    pDataManager->publishMeasurements();  
	delete pDataManager; 
}

//Test receive msg eTimerExpired
ZTEST(tests_pdata, test_pdata_rcv_msg_eTimerExpired)
{
    DataManager* pDataManager = new DataManager("data_mgr", true);
	zassert_not_null(pDataManager, "memory alloc failed");
	zassert_not_null(pDataManager->pInMbox, "pInMbox = NULL");
    zassert_equal(pDataManager->pInMbox->count(), 0, "pInMbox->count() != 0");
    zassert_equal(pDataManager->pInMbox, Mbox::getMboxById(Mbox::eDataManager),
                  "Error getting mailbox by id");

    Mbox::sendMsg(msgType::eTimerExpired, Mbox::eDataManager, Mbox::eDataManager);
    zassert_equal(pDataManager->pInMbox->count(), 1, "pInMbox->count() != 1");
    pDataManager->task();  
    zassert_equal(pDataManager->pInMbox->count(), 0, "pInMbox->count() != 0");
	delete pDataManager; 
}

//Test receive msg eLteReport 
ZTEST(tests_pdata, test_pdata_rcv_msg_eLteReport)
{
    PoolElem* pElem;

	Mbox* pLteMonMbox = new Mbox("LteMonMbox");
    Mbox::registerMbox(pLteMonMbox, Mbox::eLteMonitor);
	zassert_not_null(pLteMonMbox, "pLteMonMbox = NULL");

    DataManager* pDataManager = new DataManager("data_mgr", true);
	zassert_not_null(pDataManager, "memory alloc failed");
	zassert_not_null(pDataManager->pInMbox, "pInMbox = NULL");
    zassert_equal(pDataManager->pInMbox->count(), 0, "pInMbox->count() != 0");
    zassert_equal(pDataManager->pInMbox, Mbox::getMboxById(Mbox::eDataManager),
                  "Error getting mailbox by id");

    Mbox::sendMsg(msgType::eLteReport, Mbox::eDataManager, Mbox::eDataManager);

    zassert_equal(pDataManager->pInMbox->count(), 1, "pInMbox->count() != 1");

    pDataManager->task();  

    zassert_equal(pDataManager->pInMbox->count(), 0, "pInMbox->count() != 0");
    zassert_equal(pLteMonMbox->count(), 1, "pLteMonMbox->count() != 1");

    pElem = pLteMonMbox->read();
    zassert_equal(pElem->mtype, msgType::eLteReport, "Rcvd mtype != msgType::eLteReport");
    zassert_equal(pElem->from, Mbox::eDataManager, "Rcvd sender != Mbox::eDataManager");

    pElem->free();
    delete pLteMonMbox;
	delete pDataManager; 
}

//Test receive msg eLteLinkQuality with isComplete() = true
ZTEST(tests_pdata, test_pdata_rcv_msg_eLteLinkQuality)
{
    PoolElem* pElem;
    char str[64];

	Mbox* pMqttConnMbox = new Mbox("MqttConnectorMbox");
    Mbox::registerMbox(pMqttConnMbox, Mbox::eMqttConnector);
	zassert_not_null(pMqttConnMbox, "pMqttConnMbox = NULL");

    DataManager* pDataManager = new DataManager("data_mgr", true);
	zassert_not_null(pDataManager, "memory alloc failed");
	zassert_not_null(pDataManager->pInMbox, "pInMbox = NULL");
    zassert_equal(pDataManager->pInMbox->count(), 0, "pInMbox->count() != 0");
    zassert_equal(pDataManager->pInMbox, Mbox::getMboxById(Mbox::eDataManager),
                  "Error getting mailbox by id");

    Mbox::sendMsg(msgType::eLteLinkQuality, Mbox::eDataManager, Mbox::eDataManager);

    zassert_equal(pDataManager->pInMbox->count(), 1, "pInMbox->count() != 1");

    gbRetVal = true;
    
    pDataManager->task();  

    zassert_equal(pDataManager->pInMbox->count(), 0, "pInMbox->count() != 0");
    zassert_equal(pMqttConnMbox->count(), 1, "pMqttConnMbox->count() != 1");

    pElem = pMqttConnMbox->read();
    zassert_equal(pElem->mtype, msgType::eMetrics, "Rcvd mtype != msgType::eMetrics");
    zassert_equal(pElem->from, Mbox::eDataManager, "Rcvd sender != Mbox::eDataManager");
    snprintf(str, sizeof(str), "%s", (char*)pElem->getBuffer());
    //printk("payload = %s\n\n", str);
    zassert(strcmp(str, "{12345}")==0, "payload incorrect");

    pElem->free();
    delete pMqttConnMbox;
	delete pDataManager; 
}

//Test receive msg eLteBattVoltage with isComplete() = false
ZTEST(tests_pdata, test_pdata_rcv_msg_eLteBattVoltage)
{
    DataManager* pDataManager = new DataManager("data_mgr", true);
	zassert_not_null(pDataManager, "memory alloc failed");
	zassert_not_null(pDataManager->pInMbox, "pInMbox = NULL");
    zassert_equal(pDataManager->pInMbox->count(), 0, "pInMbox->count() != 0");
    zassert_equal(pDataManager->pInMbox, Mbox::getMboxById(Mbox::eDataManager),
                  "Error getting mailbox by id");

    Mbox::sendMsg(msgType::eLteBattVoltage, Mbox::eDataManager, Mbox::eDataManager);
    zassert_equal(pDataManager->pInMbox->count(), 1, "pInMbox->count() != 1");
    gbRetVal = false; 
    pDataManager->task();  
    zassert_equal(pDataManager->pInMbox->count(), 0, "pInMbox->count() != 0");
	delete pDataManager; 
}

//Test receive msg eLteTemperature with isComplete() = false
ZTEST(tests_pdata, test_pdata_rcv_msg_eLteTemperature)
{
    DataManager* pDataManager = new DataManager("data_mgr", true);
	zassert_not_null(pDataManager, "memory alloc failed");
	zassert_not_null(pDataManager->pInMbox, "pInMbox = NULL");
    zassert_equal(pDataManager->pInMbox->count(), 0, "pInMbox->count() != 0");
    zassert_equal(pDataManager->pInMbox, Mbox::getMboxById(Mbox::eDataManager),
                  "Error getting mailbox by id");

    Mbox::sendMsg(msgType::eLteTemperature, Mbox::eDataManager, Mbox::eDataManager);
    zassert_equal(pDataManager->pInMbox->count(), 1, "pInMbox->count() != 1");
    gbRetVal = false; 
    pDataManager->task();  
    zassert_equal(pDataManager->pInMbox->count(), 0, "pInMbox->count() != 0");
	delete pDataManager; 
}

//Test receive msg eLtePsmAct with isComplete() = false
ZTEST(tests_pdata, test_pdata_rcv_msg_eLtePsmAct)
{
    DataManager* pDataManager = new DataManager("data_mgr", true);
	zassert_not_null(pDataManager, "memory alloc failed");
	zassert_not_null(pDataManager->pInMbox, "pInMbox = NULL");
    zassert_equal(pDataManager->pInMbox->count(), 0, "pInMbox->count() != 0");
    zassert_equal(pDataManager->pInMbox, Mbox::getMboxById(Mbox::eDataManager),
                  "Error getting mailbox by id");

    Mbox::sendMsg(msgType::eLtePsmAct, Mbox::eDataManager, Mbox::eDataManager);
    zassert_equal(pDataManager->pInMbox->count(), 1, "pInMbox->count() != 1");
    gbRetVal = false; 
    pDataManager->task();  
    zassert_equal(pDataManager->pInMbox->count(), 0, "pInMbox->count() != 0");
	delete pDataManager; 
}

//Test receive msg eResetCause with isComplete() = false
ZTEST(tests_pdata, test_pdata_rcv_msg_eResetCause)
{
    DataManager* pDataManager = new DataManager("data_mgr", true);
	zassert_not_null(pDataManager, "memory alloc failed");
	zassert_not_null(pDataManager->pInMbox, "pInMbox = NULL");
    zassert_equal(pDataManager->pInMbox->count(), 0, "pInMbox->count() != 0");
    zassert_equal(pDataManager->pInMbox, Mbox::getMboxById(Mbox::eDataManager),
                  "Error getting mailbox by id");

    Mbox::sendMsg(msgType::eResetCause, Mbox::eDataManager, Mbox::eDataManager);
    zassert_equal(pDataManager->pInMbox->count(), 1, "pInMbox->count() != 1");
    gbRetVal = false; 
    pDataManager->task();  
    zassert_equal(pDataManager->pInMbox->count(), 0, "pInMbox->count() != 0");
	delete pDataManager; 
}

//Test receive msg eBootCnt with isComplete() = false
ZTEST(tests_pdata, test_pdata_rcv_msg_eBootCnt)
{
    DataManager* pDataManager = new DataManager("data_mgr", true);
	zassert_not_null(pDataManager, "memory alloc failed");
	zassert_not_null(pDataManager->pInMbox, "pInMbox = NULL");
    zassert_equal(pDataManager->pInMbox->count(), 0, "pInMbox->count() != 0");
    zassert_equal(pDataManager->pInMbox, Mbox::getMboxById(Mbox::eDataManager),
                  "Error getting mailbox by id");

    Mbox::sendMsg(msgType::eBootCnt, Mbox::eDataManager, Mbox::eDataManager);
    zassert_equal(pDataManager->pInMbox->count(), 1, "pInMbox->count() != 1");
    gbRetVal = false; 
    pDataManager->task();  
    zassert_equal(pDataManager->pInMbox->count(), 0, "pInMbox->count() != 0");
	delete pDataManager; 
}

//Test receive msg eAdcData with isComplete() = false
ZTEST(tests_pdata, test_pdata_rcv_msg_eAdcData)
{
    DataManager* pDataManager = new DataManager("data_mgr", true);
	zassert_not_null(pDataManager, "memory alloc failed");
	zassert_not_null(pDataManager->pInMbox, "pInMbox = NULL");
    zassert_equal(pDataManager->pInMbox->count(), 0, "pInMbox->count() != 0");
    zassert_equal(pDataManager->pInMbox, Mbox::getMboxById(Mbox::eDataManager),
                  "Error getting mailbox by id");

    Mbox::sendMsg(msgType::eAdcData, Mbox::eDataManager, Mbox::eDataManager);
    zassert_equal(pDataManager->pInMbox->count(), 1, "pInMbox->count() != 1");
    gbRetVal = false; 
    pDataManager->task();  
    zassert_equal(pDataManager->pInMbox->count(), 0, "pInMbox->count() != 0");
	delete pDataManager; 
}

//Test receive msg eShtcxTemperature with isComplete() = false
ZTEST(tests_pdata, test_pdata_rcv_msg_eShtcxTemperature)
{
    DataManager* pDataManager = new DataManager("data_mgr", true);
	zassert_not_null(pDataManager, "memory alloc failed");
	zassert_not_null(pDataManager->pInMbox, "pInMbox = NULL");
    zassert_equal(pDataManager->pInMbox->count(), 0, "pInMbox->count() != 0");
    zassert_equal(pDataManager->pInMbox, Mbox::getMboxById(Mbox::eDataManager),
                  "Error getting mailbox by id");

    Mbox::sendMsg(msgType::eShtcxTemperature, Mbox::eDataManager, Mbox::eDataManager);
    zassert_equal(pDataManager->pInMbox->count(), 1, "pInMbox->count() != 1");
    gbRetVal = false; 
    pDataManager->task();  
    zassert_equal(pDataManager->pInMbox->count(), 0, "pInMbox->count() != 0");
	delete pDataManager; 
}

//Test receive msg eShtcxHumidity with isComplete() = false
ZTEST(tests_pdata, test_pdata_rcv_msg_eShtcxHumidity)
{
    DataManager* pDataManager = new DataManager("data_mgr", true);
	zassert_not_null(pDataManager, "memory alloc failed");
	zassert_not_null(pDataManager->pInMbox, "pInMbox = NULL");
    zassert_equal(pDataManager->pInMbox->count(), 0, "pInMbox->count() != 0");
    zassert_equal(pDataManager->pInMbox, Mbox::getMboxById(Mbox::eDataManager),
                  "Error getting mailbox by id");

    Mbox::sendMsg(msgType::eShtcxHumidity, Mbox::eDataManager, Mbox::eDataManager);
    zassert_equal(pDataManager->pInMbox->count(), 1, "pInMbox->count() != 1");
    gbRetVal = false; 
    pDataManager->task();  
    zassert_equal(pDataManager->pInMbox->count(), 0, "pInMbox->count() != 0");
	delete pDataManager; 
}

//Test receive msg eLteIMSI with isComplete() = false
ZTEST(tests_pdata, test_pdata_rcv_msg_eLteIMSI)
{
    DataManager* pDataManager = new DataManager("data_mgr", true);
	zassert_not_null(pDataManager, "memory alloc failed");
	zassert_not_null(pDataManager->pInMbox, "pInMbox = NULL");
    zassert_equal(pDataManager->pInMbox->count(), 0, "pInMbox->count() != 0");
    zassert_equal(pDataManager->pInMbox, Mbox::getMboxById(Mbox::eDataManager),
                  "Error getting mailbox by id");

    Mbox::sendMsg(msgType::eLteIMSI, Mbox::eDataManager, Mbox::eDataManager);
    zassert_equal(pDataManager->pInMbox->count(), 1, "pInMbox->count() != 1");
    gbRetVal = false; 
    pDataManager->task();  
    zassert_equal(pDataManager->pInMbox->count(), 0, "pInMbox->count() != 0");
	delete pDataManager; 
}

//Test receive msg eLteFwVersion with isComplete() = false
ZTEST(tests_pdata, test_pdata_rcv_msg_eLteFwVersion)
{
    DataManager* pDataManager = new DataManager("data_mgr", true);
	zassert_not_null(pDataManager, "memory alloc failed");
	zassert_not_null(pDataManager->pInMbox, "pInMbox = NULL");
    zassert_equal(pDataManager->pInMbox->count(), 0, "pInMbox->count() != 0");
    zassert_equal(pDataManager->pInMbox, Mbox::getMboxById(Mbox::eDataManager),
                  "Error getting mailbox by id");

    Mbox::sendMsg(msgType::eLteFwVersion, Mbox::eDataManager, Mbox::eDataManager);
    zassert_equal(pDataManager->pInMbox->count(), 1, "pInMbox->count() != 1");
    gbRetVal = false; 
    pDataManager->task();  
    zassert_equal(pDataManager->pInMbox->count(), 0, "pInMbox->count() != 0");
	delete pDataManager; 
}

//Test receive msg eHwMetrics with isComplete() = false
ZTEST(tests_pdata, test_pdata_rcv_msg_eHwMetrics)
{
    DataManager* pDataManager = new DataManager("data_mgr", true);
	zassert_not_null(pDataManager, "memory alloc failed");
	zassert_not_null(pDataManager->pInMbox, "pInMbox = NULL");
    zassert_equal(pDataManager->pInMbox->count(), 0, "pInMbox->count() != 0");
    zassert_equal(pDataManager->pInMbox, Mbox::getMboxById(Mbox::eDataManager),
                  "Error getting mailbox by id");

    Mbox::sendMsg(msgType::eHwMetrics, Mbox::eDataManager, Mbox::eDataManager);
    zassert_equal(pDataManager->pInMbox->count(), 1, "pInMbox->count() != 1");
    gbRetVal = false; 
    pDataManager->task();  
    zassert_equal(pDataManager->pInMbox->count(), 0, "pInMbox->count() != 0");
	delete pDataManager; 
}

//Test receive msg eAdxl37xRmsData
ZTEST(tests_pdata, test_pdata_rcv_msg_eAdxl37xRmsData)
{
    DataManager* pDataManager = new DataManager("data_mgr", true);
	zassert_not_null(pDataManager, "memory alloc failed");
	zassert_not_null(pDataManager->pInMbox, "pInMbox = NULL");
    zassert_equal(pDataManager->pInMbox->count(), 0, "pInMbox->count() != 0");
    zassert_equal(pDataManager->pInMbox, Mbox::getMboxById(Mbox::eDataManager),
                  "Error getting mailbox by id");

    Mbox::sendMsg(msgType::eAdxl37xRmsData, Mbox::eDataManager, Mbox::eDataManager);
    zassert_equal(pDataManager->pInMbox->count(), 1, "pInMbox->count() != 1");
    pDataManager->task();  
    zassert_equal(pDataManager->pInMbox->count(), 0, "pInMbox->count() != 0");
	delete pDataManager; 
}

//Test receive msg eDispStatistics with isComplete() = false
ZTEST(tests_pdata, test_pdata_rcv_msg_eDispStatistics)
{
    DataManager* pDataManager = new DataManager("data_mgr", true);
	zassert_not_null(pDataManager, "memory alloc failed");
	zassert_not_null(pDataManager->pInMbox, "pInMbox = NULL");
    zassert_equal(pDataManager->pInMbox->count(), 0, "pInMbox->count() != 0");
    zassert_equal(pDataManager->pInMbox, Mbox::getMboxById(Mbox::eDataManager),
                  "Error getting mailbox by id");

    Mbox::sendMsg(msgType::eDispStatistics, Mbox::eDataManager, Mbox::eDataManager);
    zassert_equal(pDataManager->pInMbox->count(), 1, "pInMbox->count() != 1");
    gbRetVal = false; 
    pDataManager->task();  
    zassert_equal(pDataManager->pInMbox->count(), 0, "pInMbox->count() != 0");
	delete pDataManager; 
}

//Test receive msg eDispStatistics with isComplete() = true
ZTEST(tests_pdata, test_pdata_rcv_msg_eDispStatistics2)
{
    PoolElem* pElem;
    char str[64];

	Mbox* pMqttConnMbox = new Mbox("MqttConnectorMbox");
    Mbox::registerMbox(pMqttConnMbox, Mbox::eMqttConnector);
	zassert_not_null(pMqttConnMbox, "pMqttConnMbox = NULL");

    DataManager* pDataManager = new DataManager("data_mgr", true);
	zassert_not_null(pDataManager, "memory alloc failed");
	zassert_not_null(pDataManager->pInMbox, "pInMbox = NULL");
    zassert_equal(pDataManager->pInMbox->count(), 0, "pInMbox->count() != 0");
    zassert_equal(pDataManager->pInMbox, Mbox::getMboxById(Mbox::eDataManager),
                  "Error getting mailbox by id");

    Mbox::sendMsg(msgType::eDispStatistics, Mbox::eDataManager, Mbox::eDataManager);

    zassert_equal(pDataManager->pInMbox->count(), 1, "pInMbox->count() != 1");

    gbRetVal = true;
    
    pDataManager->task();  

    zassert_equal(pDataManager->pInMbox->count(), 0, "pInMbox->count() != 0");
    zassert_equal(pMqttConnMbox->count(), 1, "pMqttConnMbox->count() != 1");

    pElem = pMqttConnMbox->read();
    zassert_equal(pElem->mtype, msgType::eMeasurements, "Rcvd mtype != msgType::eMeasurements");
    zassert_equal(pElem->from, Mbox::eDataManager, "Rcvd sender != Mbox::eDataManager");
    snprintf(str, sizeof(str), "%s", (char*)pElem->getBuffer());
    //printk("payload = %s\n\n", str);
    zassert(strcmp(str, "{12345}")==0, "payload incorrect");

    pElem->free();
    delete pMqttConnMbox;
	delete pDataManager; 
}

//Test receive msg eMaxXYZ
ZTEST(tests_pdata, test_pdata_rcv_msg_eMaxXYZ)
{
    DataManager* pDataManager = new DataManager("data_mgr", true);
	zassert_not_null(pDataManager, "memory alloc failed");
	zassert_not_null(pDataManager->pInMbox, "pInMbox = NULL");
    zassert_equal(pDataManager->pInMbox->count(), 0, "pInMbox->count() != 0");
    zassert_equal(pDataManager->pInMbox, Mbox::getMboxById(Mbox::eDataManager),
                  "Error getting mailbox by id");

    Mbox::sendMsg(msgType::eMaxXYZ, Mbox::eDataManager, Mbox::eDataManager);
    zassert_equal(pDataManager->pInMbox->count(), 1, "pInMbox->count() != 1");
    pDataManager->task();  
    zassert_equal(pDataManager->pInMbox->count(), 0, "pInMbox->count() != 0");
	delete pDataManager; 
}

//Test receive msg eRmsReport with isComplete() = false
ZTEST(tests_pdata, test_pdata_rcv_msg_eRmsReport)
{
    DataManager* pDataManager = new DataManager("data_mgr", true);
	zassert_not_null(pDataManager, "memory alloc failed");
	zassert_not_null(pDataManager->pInMbox, "pInMbox = NULL");
    zassert_equal(pDataManager->pInMbox->count(), 0, "pInMbox->count() != 0");
    zassert_equal(pDataManager->pInMbox, Mbox::getMboxById(Mbox::eDataManager),
                  "Error getting mailbox by id");

    Mbox::sendMsg(msgType::eRmsReport, Mbox::eDataManager, Mbox::eDataManager);
    zassert_equal(pDataManager->pInMbox->count(), 1, "pInMbox->count() != 1");
    gbRetVal = false; 
    pDataManager->task();  
    zassert_equal(pDataManager->pInMbox->count(), 0, "pInMbox->count() != 0");
	delete pDataManager; 
}

//Test receive msg eRmsReport with isComplete() = true
ZTEST(tests_pdata, test_pdata_rcv_msg_eRmsReport2)
{
    PoolElem* pElem;
    char str[64];

	Mbox* pMqttConnMbox = new Mbox("MqttConnectorMbox");
    Mbox::registerMbox(pMqttConnMbox, Mbox::eMqttConnector);
	zassert_not_null(pMqttConnMbox, "pMqttConnMbox = NULL");

    DataManager* pDataManager = new DataManager("data_mgr", true);
	zassert_not_null(pDataManager, "memory alloc failed");
	zassert_not_null(pDataManager->pInMbox, "pInMbox = NULL");
    zassert_equal(pDataManager->pInMbox->count(), 0, "pInMbox->count() != 0");
    zassert_equal(pDataManager->pInMbox, Mbox::getMboxById(Mbox::eDataManager),
                  "Error getting mailbox by id");

    Mbox::sendMsg(msgType::eRmsReport, Mbox::eDataManager, Mbox::eDataManager);

    zassert_equal(pDataManager->pInMbox->count(), 1, "pInMbox->count() != 1");

    gbRetVal = true;
    
    pDataManager->task();  

    zassert_equal(pDataManager->pInMbox->count(), 0, "pInMbox->count() != 0");
    zassert_equal(pMqttConnMbox->count(), 1, "pMqttConnMbox->count() != 1");

    pElem = pMqttConnMbox->read();
    zassert_equal(pElem->mtype, msgType::eMeasurements, "Rcvd mtype != msgType::eMeasurements");
    zassert_equal(pElem->from, Mbox::eDataManager, "Rcvd sender != Mbox::eDataManager");
    snprintf(str, sizeof(str), "%s", (char*)pElem->getBuffer());
    //printk("payload = %s\n\n", str);
    zassert(strcmp(str, "{12345}")==0, "payload incorrect");

    pElem->free();
    delete pMqttConnMbox;
	delete pDataManager; 
}

//Test receive unexpected msg 
ZTEST(tests_pdata, test_pdata_rcv_msg_unexpected)
{
    DataManager* pDataManager = new DataManager("data_mgr", true);
	zassert_not_null(pDataManager, "memory alloc failed");
	zassert_not_null(pDataManager->pInMbox, "pInMbox = NULL");
    zassert_equal(pDataManager->pInMbox->count(), 0, "pInMbox->count() != 0");
    zassert_equal(pDataManager->pInMbox, Mbox::getMboxById(Mbox::eDataManager),
                  "Error getting mailbox by id");

    Mbox::sendMsg(msgType::eCalcDisplacementReq, Mbox::eDataManager, Mbox::eDataManager);

    zassert_equal(pDataManager->pInMbox->count(), 1, "pInMbox->count() != 1");

    pDataManager->task();  

    zassert_equal(pDataManager->pInMbox->count(), 0, "pInMbox->count() != 0");

	delete pDataManager; 
}

//Test receive no msg 
ZTEST(tests_pdata, test_pdata_rcv_msg_none)
{
    DataManager* pDataManager = new DataManager("data_mgr", true);
	zassert_not_null(pDataManager, "memory alloc failed");
	zassert_not_null(pDataManager->pInMbox, "pInMbox = NULL");
    zassert_equal(pDataManager->pInMbox->count(), 0, "pInMbox->count() != 0");
    zassert_equal(pDataManager->pInMbox, Mbox::getMboxById(Mbox::eDataManager),
                  "Error getting mailbox by id");

    pDataManager->pInMbox->write(NULL);
    
    pDataManager->task();  

    zassert_equal(pDataManager->pInMbox->count(), 0, "pInMbox->count() != 0");
	delete pDataManager; 
}


