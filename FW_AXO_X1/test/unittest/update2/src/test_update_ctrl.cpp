#include <zephyr/ztest.h>

#define private public
#define protected public
#include "update/update_ctrl.h"
#include "update/fsm.h"

#include "rtos/mbox.h"
#include "rtos/timer.h"

#include "stubs.h"

ZTEST_SUITE(tests_update_FwUpdCtrl, NULL, NULL, NULL, NULL, NULL);

// test constructor and destructor
ZTEST(tests_update_FwUpdCtrl, test_update_FwUpdCtrl_constructor)
{
	FwUpdCtrl* pFwUpdCtrl = new FwUpdCtrl("FwUpdCtrl");
	zassert_not_null(pFwUpdCtrl, "memory alloc failed");
	zassert_not_null(pFwUpdCtrl->pInMbox, "pInMbox = NULL");   
    zassert_not_null(pFwUpdCtrl->myTimer, "myTimer = NULL");
    zassert_not_null(pFwUpdCtrl->s1, "fsm = NULL");
    zassert_equal(pFwUpdCtrl->pInMbox->count(), 0, "pInMbox->count() != 0");
    zassert_equal(pFwUpdCtrl->pInMbox, Mbox::getMboxById(Mbox::eFwUpdCtrl),
                  "Error getting mailbox by id");
    zassert_equal(pFwUpdCtrl->s1->myStateId, FwFsm::StateId::eIdle, "wrong initial state");

	delete pFwUpdCtrl;
}

// MISSING for 100% branch coverage operator new does not deliver NULL
// here code lines 
// pInMbox = new Mbox(name);
// myTimer = new Timer(FwUpdCtrl::timer_cb, this, 3, 0);
// s1 = new FwFsm(reinterpret_cast<void*>(this), FwFsm::StateId::eIdle);
// static PoolElem e1 = {eTimerExpired, Mbox::MboxId::eFwUpdCtrl, 'u', 'p'};
// are not covered completly.


// test function timer_cb and timer1_expired
ZTEST(tests_update_FwUpdCtrl, test_update_FwUpdCtrl_timer_cb)
{
    PoolElem* pElem;

	FwUpdCtrl* pFwUpdCtrl = new FwUpdCtrl("FwUpdCtrl");
	zassert_not_null(pFwUpdCtrl, "memory alloc failed");
	zassert_not_null(pFwUpdCtrl->pInMbox, "pInMbox = NULL");   
    zassert_not_null(pFwUpdCtrl->myTimer, "myTimer = NULL");
    zassert_not_null(pFwUpdCtrl->s1, "fsm = NULL");
    zassert_equal(pFwUpdCtrl->pInMbox->count(), 0, "pInMbox->count() != 0");

    pFwUpdCtrl->timer_cb(reinterpret_cast<k_timer*>(pFwUpdCtrl));

    zassert_equal(pFwUpdCtrl->pInMbox->count(), 1, "pInMbox->count() != 1");
    pElem = pFwUpdCtrl->pInMbox->read();
    zassert_equal(pElem->mtype, msgType::eTimerExpired, "Rcvd mtype != msgType::eTimerExpired");

	delete pFwUpdCtrl; 
}  

// test function startRebootTimer, startTimeout and startRetryTimer
ZTEST(tests_update_FwUpdCtrl, test_update_FwUpdCtrl_startRetryTimer)
{
	FwUpdCtrl* pFwUpdCtrl = new FwUpdCtrl("FwUpdCtrl");
	zassert_not_null(pFwUpdCtrl, "memory alloc failed");
	zassert_not_null(pFwUpdCtrl->pInMbox, "pInMbox = NULL");   
    zassert_not_null(pFwUpdCtrl->myTimer, "myTimer = NULL");
    zassert_not_null(pFwUpdCtrl->s1, "fsm = NULL");
    zassert_equal(pFwUpdCtrl->pInMbox->count(), 0, "pInMbox->count() != 0");

    pFwUpdCtrl->startRebootTimer();
	pFwUpdCtrl->myTimer->stop();

	pFwUpdCtrl->startTimeout(10);
	pFwUpdCtrl->myTimer->stop();
	
	pFwUpdCtrl->retryCnt = 0;
	pFwUpdCtrl->retryTmoCur = 1;
	pFwUpdCtrl->retryTmoInit = 10;
	pFwUpdCtrl->startRetryTimer();
    zassert_equal(pFwUpdCtrl->retryTmoCur, 10, "wrong retryTmoCur");
    zassert_equal(pFwUpdCtrl->retryCnt, 1, "wrong retryCnt");
	pFwUpdCtrl->myTimer->stop();

    pFwUpdCtrl->retryCnt = 10;
	pFwUpdCtrl->retryTmoCur = 10;
	pFwUpdCtrl->retryTmoMax = 30;
	pFwUpdCtrl->startRetryTimer();
    zassert_equal(pFwUpdCtrl->retryTmoCur, 20, "wrong retryTmoCur");
    zassert_equal(pFwUpdCtrl->retryCnt, 11, "wrong retryCnt");
	pFwUpdCtrl->myTimer->stop();

    pFwUpdCtrl->retryCnt = 10;
	pFwUpdCtrl->retryTmoCur = 20;
	pFwUpdCtrl->retryTmoMax = 30;
	pFwUpdCtrl->startRetryTimer();
    zassert_equal(pFwUpdCtrl->retryTmoCur, 30, "wrong retryTmoCur");
    zassert_equal(pFwUpdCtrl->retryCnt, 11, "wrong retryCnt");
	pFwUpdCtrl->myTimer->stop();

	delete pFwUpdCtrl; 
}  

// test function requestDownload
ZTEST(tests_update_FwUpdCtrl, test_update_FwUpdCtrl_requestDownload)
{
    PoolElem* pElem;

	Mbox* pFwUpdMbox = new Mbox("FwUpdMbox");
    Mbox::registerMbox(pFwUpdMbox, Mbox::eFwUpd);
	zassert_not_null(pFwUpdMbox, "pFwUpdMbox = NULL");

	FwUpdCtrl* pFwUpdCtrl = new FwUpdCtrl("FwUpdCtrl");
	zassert_not_null(pFwUpdCtrl, "memory alloc failed");
	zassert_not_null(pFwUpdCtrl->pInMbox, "pInMbox = NULL");   
    zassert_not_null(pFwUpdCtrl->myTimer, "myTimer = NULL");
    zassert_not_null(pFwUpdCtrl->s1, "fsm = NULL");
    zassert_equal(pFwUpdCtrl->pInMbox->count(), 0, "pInMbox->count() != 0");

    pFwUpdCtrl->requestDownload();

	pElem = pFwUpdMbox->read();
    zassert_equal(pElem->mtype, msgType::eFwDownload, "Rcvd mtype != msgType::eFwDownload");
    zassert_equal(pElem->from, Mbox::eFwUpdCtrl, "Rcvd sender != Mbox::eFwUpdCtrl");
	pElem->free();

	delete pFwUpdMbox;
	delete pFwUpdCtrl; 
}  

// test function otaStartConfirm
ZTEST(tests_update_FwUpdCtrl, test_update_FwUpdCtrl_otaStartConfirm)
{
    PoolElem* pElem;

	Mbox* pMqttMbox = new Mbox("MqttMbox");
    Mbox::registerMbox(pMqttMbox, Mbox::eMqttConnector);
	zassert_not_null(pMqttMbox, "pMqttMbox = NULL");

	FwUpdCtrl* pFwUpdCtrl = new FwUpdCtrl("FwUpdCtrl");
	zassert_not_null(pFwUpdCtrl, "memory alloc failed");
	zassert_not_null(pFwUpdCtrl->pInMbox, "pInMbox = NULL");   
    zassert_not_null(pFwUpdCtrl->myTimer, "myTimer = NULL");
    zassert_not_null(pFwUpdCtrl->s1, "fsm = NULL");
    zassert_equal(pFwUpdCtrl->pInMbox->count(), 0, "pInMbox->count() != 0");

    pFwUpdCtrl->otaStartConfirm();

	pElem = pMqttMbox->read();
    zassert_equal(pElem->mtype, msgType::eOtaStart, "Rcvd mtype != msgType::eOtaStart");
    zassert_equal(pElem->from, Mbox::eFwUpdCtrl, "Rcvd sender != Mbox::eFwUpdCtrl");
	std::string payload = std::string(reinterpret_cast<char*>(pElem->getBuffer()));
	zassert(strcmp(payload.c_str(), "{\"cmd\":\"OTA_START\"}")==0, "wrong payload string");
	pElem->free();

	delete pMqttMbox;
	delete pFwUpdCtrl; 
}  

// test function otaStartReject
ZTEST(tests_update_FwUpdCtrl, test_update_FwUpdCtrl_otaStartReject)
{
    PoolElem* pElem;

	Mbox* pMqttMbox = new Mbox("MqttMbox");
    Mbox::registerMbox(pMqttMbox, Mbox::eMqttConnector);
	zassert_not_null(pMqttMbox, "pMqttMbox = NULL");

	FwUpdCtrl* pFwUpdCtrl = new FwUpdCtrl("FwUpdCtrl");
	zassert_not_null(pFwUpdCtrl, "memory alloc failed");
	zassert_not_null(pFwUpdCtrl->pInMbox, "pInMbox = NULL");   
    zassert_not_null(pFwUpdCtrl->myTimer, "myTimer = NULL");
    zassert_not_null(pFwUpdCtrl->s1, "fsm = NULL");
    zassert_equal(pFwUpdCtrl->pInMbox->count(), 0, "pInMbox->count() != 0");

    pFwUpdCtrl->otaStartReject();

	pElem = pMqttMbox->read();
    zassert_equal(pElem->mtype, msgType::eCmdReject, "Rcvd mtype != msgType::eCmdReject");
    zassert_equal(pElem->from, Mbox::eFwUpdCtrl, "Rcvd sender != Mbox::eFwUpdCtrl");
	std::string payload = std::string(reinterpret_cast<char*>(pElem->getBuffer()));
	zassert(strstr(payload.c_str(), "{\"cmd\":\"OTA_START\"")!=NULL, "wrong payload string 1");
	zassert(strstr(payload.c_str(), ",\"code\":\"error101\"")!=NULL, "wrong payload string 2");
	zassert(strstr(payload.c_str(), ",\"message\":\"wrong state\"")!=NULL, "wrong payload string 3");
	pElem->free();

	delete pMqttMbox;
	delete pFwUpdCtrl; 
}  

// test function otaFwConfirm
ZTEST(tests_update_FwUpdCtrl, test_update_FwUpdCtrl_otaFwConfirm)
{
    PoolElem* pElem;

	Mbox* pMqttMbox = new Mbox("MqttMbox");
    Mbox::registerMbox(pMqttMbox, Mbox::eMqttConnector);
	zassert_not_null(pMqttMbox, "pMqttMbox = NULL");

	FwUpdCtrl* pFwUpdCtrl = new FwUpdCtrl("FwUpdCtrl");
	zassert_not_null(pFwUpdCtrl, "memory alloc failed");
	zassert_not_null(pFwUpdCtrl->pInMbox, "pInMbox = NULL");   
    zassert_not_null(pFwUpdCtrl->myTimer, "myTimer = NULL");
    zassert_not_null(pFwUpdCtrl->s1, "fsm = NULL");
    zassert_equal(pFwUpdCtrl->pInMbox->count(), 0, "pInMbox->count() != 0");

    pFwUpdCtrl->otaFwConfirm();

	pElem = pMqttMbox->read();
    zassert_equal(pElem->mtype, msgType::eFwAccept, "Rcvd mtype != msgType::eFwAccept");
    zassert_equal(pElem->from, Mbox::eFwUpdCtrl, "Rcvd sender != Mbox::eFwUpdCtrl");
	std::string payload = std::string(reinterpret_cast<char*>(pElem->getBuffer()));
	zassert(strcmp(payload.c_str(), "{}")==0, "wrong payload string");
	pElem->free();

	delete pMqttMbox;
	delete pFwUpdCtrl; 
}  

// test function otaFwReject
ZTEST(tests_update_FwUpdCtrl, test_update_FwUpdCtrl_otaFwReject)
{
    PoolElem* pElem;

	Mbox* pMqttMbox = new Mbox("MqttMbox");
    Mbox::registerMbox(pMqttMbox, Mbox::eMqttConnector);
	zassert_not_null(pMqttMbox, "pMqttMbox = NULL");

	FwUpdCtrl* pFwUpdCtrl = new FwUpdCtrl("FwUpdCtrl");
	zassert_not_null(pFwUpdCtrl, "memory alloc failed");
	zassert_not_null(pFwUpdCtrl->pInMbox, "pInMbox = NULL");   
    zassert_not_null(pFwUpdCtrl->myTimer, "myTimer = NULL");
    zassert_not_null(pFwUpdCtrl->s1, "fsm = NULL");
    zassert_equal(pFwUpdCtrl->pInMbox->count(), 0, "pInMbox->count() != 0");

    pFwUpdCtrl->otaFwReject();

	pElem = pMqttMbox->read();
    zassert_equal(pElem->mtype, msgType::eFwReject, "Rcvd mtype != msgType::eFwReject");
    zassert_equal(pElem->from, Mbox::eFwUpdCtrl, "Rcvd sender != Mbox::eFwUpdCtrl");
	std::string payload = std::string(reinterpret_cast<char*>(pElem->getBuffer()));
	zassert(strstr(payload.c_str(), "\"code\":\"error101\"")!=NULL, "wrong payload string 2");
	zassert(strstr(payload.c_str(), ",\"message\":\"wrong state\"")!=NULL, "wrong payload string 3");
	pElem->free();

	delete pMqttMbox;
	delete pFwUpdCtrl; 
}  

//Test receive msg eTimerExpired
ZTEST(tests_update_FwUpdCtrl, test_update_FwUpdCtrl_rcv_msg_eTimerExpired)
{
	FwUpdCtrl* pFwUpdCtrl = new FwUpdCtrl("FwUpdCtrl");
	zassert_not_null(pFwUpdCtrl, "memory alloc failed");
	zassert_not_null(pFwUpdCtrl->pInMbox, "pInMbox = NULL");   
    zassert_not_null(pFwUpdCtrl->myTimer, "myTimer = NULL");
    zassert_not_null(pFwUpdCtrl->s1, "fsm = NULL");
    zassert_equal(pFwUpdCtrl->pInMbox->count(), 0, "pInMbox->count() != 0");
    zassert_equal(pFwUpdCtrl->pInMbox, Mbox::getMboxById(Mbox::eFwUpdCtrl),
                  "Error getting mailbox by id");
    zassert_equal(pFwUpdCtrl->s1->myStateId, FwFsm::StateId::eIdle, "wrong initial state");

    Mbox::sendMsg(msgType::eTimerExpired, Mbox::eFwUpdCtrl, Mbox::eFwUpdCtrl);

    zassert_equal(pFwUpdCtrl->pInMbox->count(), 1, "pInMbox->count() != 1");

    pFwUpdCtrl->task();  
    zassert_equal(pFwUpdCtrl->pInMbox->count(), 0, "pInMbox->count() != 0");
	zassert_equal(pFwUpdCtrl->s1->myStateId, FwFsm::StateId::eIdle, "wrong state ID");

	delete pFwUpdCtrl; 
}

//Test receive msg eOtaStart
ZTEST(tests_update_FwUpdCtrl, test_update_FwUpdCtrl_rcv_msg_eOtaStart)
{
    PoolElem* pElem;

	Mbox* pMqttMbox = new Mbox("MqttMbox");
    Mbox::registerMbox(pMqttMbox, Mbox::eMqttConnector);
	zassert_not_null(pMqttMbox, "pMqttMbox = NULL");

	Mbox* pAdxlMbox = new Mbox("AdxlMbox");
    Mbox::registerMbox(pAdxlMbox, Mbox::eAdxl37xTask);
	zassert_not_null(pAdxlMbox, "pAdxlMbox = NULL");

	FwUpdCtrl* pFwUpdCtrl = new FwUpdCtrl("FwUpdCtrl");
	zassert_not_null(pFwUpdCtrl, "memory alloc failed");
	zassert_not_null(pFwUpdCtrl->pInMbox, "pInMbox = NULL");   
    zassert_not_null(pFwUpdCtrl->myTimer, "myTimer = NULL");
    zassert_not_null(pFwUpdCtrl->s1, "fsm = NULL");
    zassert_equal(pFwUpdCtrl->pInMbox->count(), 0, "pInMbox->count() != 0");
    zassert_equal(pFwUpdCtrl->pInMbox, Mbox::getMboxById(Mbox::eFwUpdCtrl),
                  "Error getting mailbox by id");
    zassert_equal(pFwUpdCtrl->s1->myStateId, FwFsm::StateId::eIdle, "wrong initial state");

    Mbox::sendMsg(msgType::eOtaStart, Mbox::eFwUpdCtrl, Mbox::eFwUpdCtrl);

    zassert_equal(pFwUpdCtrl->pInMbox->count(), 1, "pInMbox->count() != 1");

    pFwUpdCtrl->task();  
    zassert_equal(pFwUpdCtrl->pInMbox->count(), 0, "pInMbox->count() != 0");
	zassert_equal(pFwUpdCtrl->s1->myStateId, FwFsm::StateId::eTriggered, "wrong state ID");

	pElem = pMqttMbox->read();
    zassert_equal(pElem->mtype, msgType::eOtaStart, "Rcvd mtype != msgType::eOtaStart");
    zassert_equal(pElem->from, Mbox::eFwUpdCtrl, "Rcvd sender != Mbox::eFwUpdCtrl");
	std::string payload = std::string(reinterpret_cast<char*>(pElem->getBuffer()));
	zassert(strcmp(payload.c_str(), "{\"cmd\":\"OTA_START\"}")==0, "wrong payload string");
	pElem->free();

	pElem = pAdxlMbox->read();
    zassert_equal(pElem->mtype, msgType::eFwUpdateActive, "Rcvd mtype != msgType::eFwUpdateActive");
    zassert_equal(pElem->from, Mbox::eFwUpdCtrl, "Rcvd sender != Mbox::eFwUpdCtrl");
	pElem->free();

	delete pMqttMbox;
	delete pAdxlMbox;
	delete pFwUpdCtrl; 
}

//Test receive msg eFwInfo
ZTEST(tests_update_FwUpdCtrl, test_update_FwUpdCtrl_rcv_msg_eFwInfo)
{
    PoolElem* pElem;

	std::string str = "{\"url\":\"https://gomera3.ddns.net/stg/" 
			"app_update.bin\",\"fw_version\":\"0.1\",\"size\":\"247996\",\"sha1\":"
			"\"3db38b191b\"}";
	PoolElem* e1 = new PoolElem();
    e1->mtype = eFwInfo;
    e1->from = Mbox::eFwUpdCtrl;
	e1->putString(str.c_str());

	Mbox* pMqttMbox = new Mbox("MqttMbox");
    Mbox::registerMbox(pMqttMbox, Mbox::eMqttConnector);
	zassert_not_null(pMqttMbox, "pMqttMbox = NULL");

	FwUpdCtrl* pFwUpdCtrl = new FwUpdCtrl("FwUpdCtrl");
	zassert_not_null(pFwUpdCtrl, "memory alloc failed");
	zassert_not_null(pFwUpdCtrl->pInMbox, "pInMbox = NULL");   
    zassert_not_null(pFwUpdCtrl->myTimer, "myTimer = NULL");
    zassert_not_null(pFwUpdCtrl->s1, "fsm = NULL");
    zassert_equal(pFwUpdCtrl->pInMbox->count(), 0, "pInMbox->count() != 0");
    zassert_equal(pFwUpdCtrl->pInMbox, Mbox::getMboxById(Mbox::eFwUpdCtrl),
                  "Error getting mailbox by id");
    zassert_equal(pFwUpdCtrl->s1->myStateId, FwFsm::StateId::eIdle, "wrong initial state");

    Mbox::sendMsg(msgType::eFwInfo, Mbox::eFwUpdCtrl, Mbox::eFwUpdCtrl);

    zassert_equal(pFwUpdCtrl->pInMbox->count(), 1, "pInMbox->count() != 1");

    pFwUpdCtrl->task();  
    zassert_equal(pFwUpdCtrl->pInMbox->count(), 0, "pInMbox->count() != 0");
	zassert_equal(pFwUpdCtrl->s1->myStateId, FwFsm::StateId::eIdle, "wrong state ID");

	pElem = pMqttMbox->read();
    zassert_equal(pElem->mtype, msgType::eFwReject, "Rcvd mtype != msgType::eFwReject");
    zassert_equal(pElem->from, Mbox::eFwUpdCtrl, "Rcvd sender != Mbox::eFwUpdCtrl");
	pElem->free();

	delete pMqttMbox;
	delete pFwUpdCtrl; 
}

//Test receive msg eFwDlSuccess
ZTEST(tests_update_FwUpdCtrl, test_update_FwUpdCtrl_rcv_msg_eFwDlSuccess)
{
	FwUpdCtrl* pFwUpdCtrl = new FwUpdCtrl("FwUpdCtrl");
	zassert_not_null(pFwUpdCtrl, "memory alloc failed");
	zassert_not_null(pFwUpdCtrl->pInMbox, "pInMbox = NULL");   
    zassert_not_null(pFwUpdCtrl->myTimer, "myTimer = NULL");
    zassert_not_null(pFwUpdCtrl->s1, "fsm = NULL");
    zassert_equal(pFwUpdCtrl->pInMbox->count(), 0, "pInMbox->count() != 0");
    zassert_equal(pFwUpdCtrl->pInMbox, Mbox::getMboxById(Mbox::eFwUpdCtrl),
                  "Error getting mailbox by id");
    zassert_equal(pFwUpdCtrl->s1->myStateId, FwFsm::StateId::eIdle, "wrong initial state");

    Mbox::sendMsg(msgType::eFwDlSuccess, Mbox::eFwUpdCtrl, Mbox::eFwUpdCtrl);

    zassert_equal(pFwUpdCtrl->pInMbox->count(), 1, "pInMbox->count() != 1");

    pFwUpdCtrl->task();  
    zassert_equal(pFwUpdCtrl->pInMbox->count(), 0, "pInMbox->count() != 0");
	zassert_equal(pFwUpdCtrl->s1->myStateId, FwFsm::StateId::eIdle, "wrong state ID");

	delete pFwUpdCtrl; 
}

//Test receive msg eFwDlFailed
ZTEST(tests_update_FwUpdCtrl, test_update_FwUpdCtrl_rcv_msg_eFwDlFailed)
{
	FwUpdCtrl* pFwUpdCtrl = new FwUpdCtrl("FwUpdCtrl");
	zassert_not_null(pFwUpdCtrl, "memory alloc failed");
	zassert_not_null(pFwUpdCtrl->pInMbox, "pInMbox = NULL");   
    zassert_not_null(pFwUpdCtrl->myTimer, "myTimer = NULL");
    zassert_not_null(pFwUpdCtrl->s1, "fsm = NULL");
    zassert_equal(pFwUpdCtrl->pInMbox->count(), 0, "pInMbox->count() != 0");
    zassert_equal(pFwUpdCtrl->pInMbox, Mbox::getMboxById(Mbox::eFwUpdCtrl),
                  "Error getting mailbox by id");
    zassert_equal(pFwUpdCtrl->s1->myStateId, FwFsm::StateId::eIdle, "wrong initial state");

    Mbox::sendMsg(msgType::eFwDlFailed, Mbox::eFwUpdCtrl, Mbox::eFwUpdCtrl);

    zassert_equal(pFwUpdCtrl->pInMbox->count(), 1, "pInMbox->count() != 1");

    pFwUpdCtrl->task();  
    zassert_equal(pFwUpdCtrl->pInMbox->count(), 0, "pInMbox->count() != 0");
	zassert_equal(pFwUpdCtrl->s1->myStateId, FwFsm::StateId::eIdle, "wrong state ID");

	delete pFwUpdCtrl; 
}



//Test receive unexpected msg 
ZTEST(tests_update_FwUpdCtrl, test_update_FwUpdCtrl_rcv_msg_unexpected)
{
	FwUpdCtrl* pFwUpdCtrl = new FwUpdCtrl("FwUpdCtrl");
	zassert_not_null(pFwUpdCtrl, "memory alloc failed");
	zassert_not_null(pFwUpdCtrl->pInMbox, "pInMbox = NULL");   
    zassert_not_null(pFwUpdCtrl->myTimer, "myTimer = NULL");
    zassert_not_null(pFwUpdCtrl->s1, "fsm = NULL");
    zassert_equal(pFwUpdCtrl->pInMbox->count(), 0, "pInMbox->count() != 0");

    Mbox::sendMsg(msgType::eCalcDisplacementReq, Mbox::eFwUpdCtrl, Mbox::eFwUpdCtrl);

    zassert_equal(pFwUpdCtrl->pInMbox->count(), 1, "pInMbox->count() != 1");

    pFwUpdCtrl->task();  

    zassert_equal(pFwUpdCtrl->pInMbox->count(), 0, "pInMbox->count() != 0");

	delete pFwUpdCtrl; 
}

//Test receive no msg 
ZTEST(tests_update_FwUpdCtrl, test_update_FwUpdCtrl_rcv_msg_none)
{
	FwUpdCtrl* pFwUpdCtrl = new FwUpdCtrl("FwUpdCtrl");
	zassert_not_null(pFwUpdCtrl, "memory alloc failed");
	zassert_not_null(pFwUpdCtrl->pInMbox, "pInMbox = NULL");   
    zassert_not_null(pFwUpdCtrl->myTimer, "myTimer = NULL");
    zassert_not_null(pFwUpdCtrl->s1, "fsm = NULL");
    zassert_equal(pFwUpdCtrl->pInMbox->count(), 0, "pInMbox->count() != 0");

    pFwUpdCtrl->pInMbox->write(NULL);
    
    pFwUpdCtrl->task();  

    zassert_equal(pFwUpdCtrl->pInMbox->count(), 0, "pInMbox->count() != 0");
	delete pFwUpdCtrl; 
}



