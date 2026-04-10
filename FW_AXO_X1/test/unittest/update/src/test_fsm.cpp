#include <zephyr/ztest.h>

#define private public
#define protected public
#include "update/fsm.h"
//#include "update/update_ctrl.h"

#include "rtos/mbox.h"
#include "stubs.h"



ZTEST_SUITE(tests_update_fsm, NULL, NULL, NULL, NULL, NULL);

// test constructor and destructor and function getStateId
ZTEST(tests_update_fsm, test_update_fsm_constructor)
{
	int iRet;

	FwUpdCtrl* pUpdCtrl = new FwUpdCtrl();
	FwFsm* pFwFsm = new FwFsm((void*)pUpdCtrl, FwFsm::StateId::eIdle);
	zassert_not_null(pFwFsm, "memory alloc failed");
	zassert_not_null(pFwFsm->stateTbl[0], "memory alloc failed");
	zassert_not_null(pFwFsm->stateTbl[1], "memory alloc failed");
	zassert_not_null(pFwFsm->stateTbl[2], "memory alloc failed");
	zassert_not_null(pFwFsm->stateTbl[3], "memory alloc failed");
	zassert_equal((FwUpdCtrl*)pFwFsm->pd, pUpdCtrl, "wrong owner class");
	zassert_equal(pFwFsm->myStateId, FwFsm::StateId::eIdle, "wrong initial state ID");

	iRet = pFwFsm->getStateId();
	zassert_equal(iRet, FwFsm::StateId::eIdle, "wrong initial state ID");

	delete pFwFsm;
	delete pUpdCtrl;
}

// MISSING for 100% branch coverage operator new does not deliver NULL
// here code lines 
// return new FwIdle(pd);
// return new FwTriggered(pd);
// return new FwDownload(pd);
// return new FwReady(pd);
// are not covered completly.


// test constructor and destructor and function getStateId
ZTEST(tests_update_fsm, test_update_fsm_constructor2)
{
	int iRet;

	FwUpdCtrl* pUpdCtrl = new FwUpdCtrl();
	FwFsm* pFwFsm = new FwFsm((void*)pUpdCtrl, FwFsm::StateId::eIdle);
	zassert_not_null(pFwFsm, "memory alloc failed");
	zassert_not_null(pFwFsm->stateTbl[0], "memory alloc failed");
	zassert_not_null(pFwFsm->stateTbl[1], "memory alloc failed");
	zassert_not_null(pFwFsm->stateTbl[2], "memory alloc failed");
	zassert_not_null(pFwFsm->stateTbl[3], "memory alloc failed");
	zassert_equal((FwUpdCtrl*)pFwFsm->pd, pUpdCtrl, "wrong owner class");
	zassert_equal(pFwFsm->myStateId, FwFsm::StateId::eIdle, "wrong initial state ID");

	pFwFsm->myState = NULL;

	delete pFwFsm;
	delete pUpdCtrl;
}

// test function setState and ReportOtaUpdateActive
ZTEST(tests_update_fsm, test_update_fsm_setState)
{
    PoolElem* pElem;

	Mbox* pAdxlMbox = new Mbox("AdxlMbox");
    Mbox::registerMbox(pAdxlMbox, Mbox::eAdxl37xTask);
	zassert_not_null(pAdxlMbox, "pAdxlMbox = NULL");

	FwUpdCtrl* pUpdCtrl = new FwUpdCtrl();
	FwFsm* pFwFsm = new FwFsm((void*)pUpdCtrl, FwFsm::StateId::eIdle);
	zassert_not_null(pFwFsm, "memory alloc failed");
	zassert_not_null(pFwFsm->stateTbl[0], "memory alloc failed");
	zassert_not_null(pFwFsm->stateTbl[1], "memory alloc failed");
	zassert_not_null(pFwFsm->stateTbl[2], "memory alloc failed");
	zassert_not_null(pFwFsm->stateTbl[3], "memory alloc failed");
	zassert_equal((FwUpdCtrl*)pFwFsm->pd, pUpdCtrl, "wrong owner class");
	zassert_equal(pFwFsm->myStateId, FwFsm::StateId::eIdle, "wrong initial state ID");

	pFwFsm->setState(FwFsm::StateId::eNoChange);
	zassert_equal(pFwFsm->myStateId, FwFsm::StateId::eIdle, "wrong state ID");

	pFwFsm->setState(FwFsm::StateId::eTriggered);
	zassert_equal(pFwFsm->myStateId, FwFsm::StateId::eTriggered, "wrong state ID");
    
	pElem = pAdxlMbox->read();
    zassert_equal(pElem->mtype, msgType::eFwUpdateActive, "Rcvd mtype != msgType::eFwUpdateActive");
    zassert_equal(pElem->from, Mbox::eFwUpdCtrl, "Rcvd sender != Mbox::eFwUpdCtrl");
	pElem->free();

	pFwFsm->setState(FwFsm::StateId::eDownload);
	zassert_equal(pFwFsm->myStateId, FwFsm::StateId::eDownload, "wrong state ID");
    
	pElem = pAdxlMbox->read();
    zassert_equal(pElem->mtype, msgType::eFwUpdateActive, "Rcvd mtype != msgType::eFwUpdateActive");
    zassert_equal(pElem->from, Mbox::eFwUpdCtrl, "Rcvd sender != Mbox::eFwUpdCtrl");
	pElem->free();

	pFwFsm->setState(FwFsm::StateId::eReady);
	zassert_equal(pFwFsm->myStateId, FwFsm::StateId::eReady, "wrong state ID");
    
	pElem = pAdxlMbox->read();
    zassert_equal(pElem->mtype, msgType::eFwUpdateActive, "Rcvd mtype != msgType::eFwUpdateActive");
    zassert_equal(pElem->from, Mbox::eFwUpdCtrl, "Rcvd sender != Mbox::eFwUpdCtrl");
	pElem->free();

	pFwFsm->myState = NULL;
	pFwFsm->setState(FwFsm::StateId::eIdle);
	zassert_equal(pFwFsm->myStateId, FwFsm::StateId::eIdle, "wrong state ID");
    
	delete pAdxlMbox;
	delete pFwFsm;
	delete pUpdCtrl;
}

// test function handle_OtaStart
ZTEST(tests_update_fsm, test_update_fsm_handle_OtaStart)
{
    PoolElem* pElem;

	Mbox* pAdxlMbox = new Mbox("AdxlMbox");
    Mbox::registerMbox(pAdxlMbox, Mbox::eAdxl37xTask);
	zassert_not_null(pAdxlMbox, "pAdxlMbox = NULL");

	FwUpdCtrl* pUpdCtrl = new FwUpdCtrl();
	FwFsm* pFwFsm = new FwFsm((void*)pUpdCtrl, FwFsm::StateId::eIdle);
	zassert_not_null(pFwFsm, "memory alloc failed");
	zassert_not_null(pFwFsm->stateTbl[0], "memory alloc failed");
	zassert_not_null(pFwFsm->stateTbl[1], "memory alloc failed");
	zassert_not_null(pFwFsm->stateTbl[2], "memory alloc failed");
	zassert_not_null(pFwFsm->stateTbl[3], "memory alloc failed");
	zassert_equal((FwUpdCtrl*)pFwFsm->pd, pUpdCtrl, "wrong owner class");
	zassert_equal(pFwFsm->myStateId, FwFsm::StateId::eIdle, "wrong initial state ID");

	pFwFsm->handle_OtaStart();
	zassert_equal(pFwFsm->myStateId, FwFsm::StateId::eTriggered, "wrong state ID");

	pElem = pAdxlMbox->read();
    zassert_equal(pElem->mtype, msgType::eFwUpdateActive, "Rcvd mtype != msgType::eFwUpdateActive");
    zassert_equal(pElem->from, Mbox::eFwUpdCtrl, "Rcvd sender != Mbox::eFwUpdCtrl");
	pElem->free();

	delete pAdxlMbox;
	delete pFwFsm;
	delete pUpdCtrl;
}

// test function handle_OtaStart
ZTEST(tests_update_fsm, test_update_fsm_handle_OtaStart2)
{
	FwUpdCtrl* pUpdCtrl = new FwUpdCtrl();
	FwFsm* pFwFsm = new FwFsm((void*)pUpdCtrl, FwFsm::StateId::eTriggered);
	zassert_not_null(pFwFsm, "memory alloc failed");
	zassert_not_null(pFwFsm->stateTbl[0], "memory alloc failed");
	zassert_not_null(pFwFsm->stateTbl[1], "memory alloc failed");
	zassert_not_null(pFwFsm->stateTbl[2], "memory alloc failed");
	zassert_not_null(pFwFsm->stateTbl[3], "memory alloc failed");
	zassert_equal((FwUpdCtrl*)pFwFsm->pd, pUpdCtrl, "wrong owner class");
	zassert_equal(pFwFsm->myStateId, FwFsm::StateId::eTriggered, "wrong initial state ID");

	pFwFsm->handle_OtaStart();
	zassert_equal(pFwFsm->myStateId, FwFsm::StateId::eTriggered, "wrong state ID");

	delete pFwFsm;
	delete pUpdCtrl;
}

// test function handle_FwInfo
ZTEST(tests_update_fsm, test_update_fsm_handle_FwInfo)
{
    PoolElem* pElem;

	std::string str = "{\"url\":\"https://gomera3.ddns.net/stg/" 
			"app_update.bin\",\"fw_version\":\"0.1\",\"size\":\"247996\",\"sha1\":"
			"\"3db38b191b\"}";
	PoolElem* e1 = new PoolElem();
    e1->mtype = eFwInfo;
    e1->from = Mbox::eFwUpdCtrl;
	e1->putString(str.c_str());

	FwUpdCtrl* pUpdCtrl = new FwUpdCtrl();
	FwFsm* pFwFsm = new FwFsm((void*)pUpdCtrl, FwFsm::StateId::eIdle);
	zassert_not_null(pFwFsm, "memory alloc failed");
	zassert_not_null(pFwFsm->stateTbl[0], "memory alloc failed");
	zassert_not_null(pFwFsm->stateTbl[1], "memory alloc failed");
	zassert_not_null(pFwFsm->stateTbl[2], "memory alloc failed");
	zassert_not_null(pFwFsm->stateTbl[3], "memory alloc failed");
	zassert_equal((FwUpdCtrl*)pFwFsm->pd, pUpdCtrl, "wrong owner class");
	zassert_equal(pFwFsm->myStateId, FwFsm::StateId::eIdle, "wrong initial state ID");

	pFwFsm->handle_FwInfo(&e1);
	zassert_equal(pFwFsm->myStateId, FwFsm::StateId::eIdle, "wrong state ID");

	e1->free();

	delete pFwFsm;
	delete pUpdCtrl;
}

// test function handle_FwInfo
ZTEST(tests_update_fsm, test_update_fsm_handle_FwInfo2)
{
    PoolElem* pElem;

	std::string str = "{\"url\":\"https://gomera3.ddns.net/stg/" 
			"app_update.bin\",\"fw_version\":\"0.1\",\"size\":\"247996\",\"sha1\":"
			"\"3db38b191b\"}";
	PoolElem* e1 = new PoolElem();
    e1->mtype = eFwInfo;
    e1->from = Mbox::eFwUpdCtrl;
	e1->putString(str.c_str());

	Mbox* pFwUpdMbox = new Mbox("FwUpdMbox");
    Mbox::registerMbox(pFwUpdMbox, Mbox::eFwUpd);
	zassert_not_null(pFwUpdMbox, "pFwUpdMbox = NULL");

	Mbox* pAdxlMbox = new Mbox("AdxlMbox");
    Mbox::registerMbox(pAdxlMbox, Mbox::eAdxl37xTask);
	zassert_not_null(pAdxlMbox, "pAdxlMbox = NULL");

	FwUpdCtrl* pUpdCtrl = new FwUpdCtrl();
	FwFsm* pFwFsm = new FwFsm((void*)pUpdCtrl, FwFsm::StateId::eTriggered);
	zassert_not_null(pFwFsm, "memory alloc failed");
	zassert_not_null(pFwFsm->stateTbl[0], "memory alloc failed");
	zassert_not_null(pFwFsm->stateTbl[1], "memory alloc failed");
	zassert_not_null(pFwFsm->stateTbl[2], "memory alloc failed");
	zassert_not_null(pFwFsm->stateTbl[3], "memory alloc failed");
	zassert_equal((FwUpdCtrl*)pFwFsm->pd, pUpdCtrl, "wrong owner class");
	zassert_equal(pFwFsm->myStateId, FwFsm::StateId::eTriggered, "wrong initial state ID");

	pFwFsm->handle_FwInfo(&e1);
	zassert_equal(pFwFsm->myStateId, FwFsm::StateId::eDownload, "wrong state ID");

	pElem = pFwUpdMbox->read();
    zassert_equal(pElem->mtype, msgType::eFwInfo, "Rcvd mtype != msgType::eFwInfo");
    zassert_equal(pElem->from, Mbox::eFwUpdCtrl, "Rcvd sender != Mbox::eFwUpdCtrl");
	pElem->free();

	pElem = pAdxlMbox->read();
    zassert_equal(pElem->mtype, msgType::eFwUpdateActive, "Rcvd mtype != msgType::eFwUpdateActive");
    zassert_equal(pElem->from, Mbox::eFwUpdCtrl, "Rcvd sender != Mbox::eFwUpdCtrl");
	pElem->free();

	delete pAdxlMbox;
	delete pFwFsm;
	delete pUpdCtrl;
}

// test function handle_DlComplete
ZTEST(tests_update_fsm, test_update_fsm_handle_DlComplete)
{
	PoolElem* e1 = new PoolElem();
    e1->mtype = eFwDlSuccess;
    e1->from = Mbox::eFwUpdCtrl;

	FwUpdCtrl* pUpdCtrl = new FwUpdCtrl();
	FwFsm* pFwFsm = new FwFsm((void*)pUpdCtrl, FwFsm::StateId::eIdle);
	zassert_not_null(pFwFsm, "memory alloc failed");
	zassert_not_null(pFwFsm->stateTbl[0], "memory alloc failed");
	zassert_not_null(pFwFsm->stateTbl[1], "memory alloc failed");
	zassert_not_null(pFwFsm->stateTbl[2], "memory alloc failed");
	zassert_not_null(pFwFsm->stateTbl[3], "memory alloc failed");
	zassert_equal((FwUpdCtrl*)pFwFsm->pd, pUpdCtrl, "wrong owner class");
	zassert_equal(pFwFsm->myStateId, FwFsm::StateId::eIdle, "wrong initial state ID");

	pFwFsm->handle_DlComplete(&e1);
	zassert_equal(pFwFsm->myStateId, FwFsm::StateId::eIdle, "wrong state ID");
	e1->free();

	delete pUpdCtrl;
	delete pFwFsm;
}

// test function handle_DlComplete
ZTEST(tests_update_fsm, test_update_fsm_handle_DlComplete2)
{
    PoolElem* pElem;

	PoolElem* e1 = new PoolElem();
    e1->mtype = eFwDlSuccess;
    e1->from = Mbox::eFwUpdCtrl;

	Mbox* pMqttMbox = new Mbox("MqttMbox");
    Mbox::registerMbox(pMqttMbox, Mbox::eMqttConnector);
	zassert_not_null(pMqttMbox, "pMqttMbox = NULL");

	Mbox* pAdxlMbox = new Mbox("AdxlMbox");
    Mbox::registerMbox(pAdxlMbox, Mbox::eAdxl37xTask);
	zassert_not_null(pAdxlMbox, "pAdxlMbox = NULL");

	FwUpdCtrl* pUpdCtrl = new FwUpdCtrl();
	FwFsm* pFwFsm = new FwFsm((void*)pUpdCtrl, FwFsm::StateId::eDownload);
	zassert_not_null(pFwFsm, "memory alloc failed");
	zassert_not_null(pFwFsm->stateTbl[0], "memory alloc failed");
	zassert_not_null(pFwFsm->stateTbl[1], "memory alloc failed");
	zassert_not_null(pFwFsm->stateTbl[2], "memory alloc failed");
	zassert_not_null(pFwFsm->stateTbl[3], "memory alloc failed");
	zassert_equal((FwUpdCtrl*)pFwFsm->pd, pUpdCtrl, "wrong owner class");
	zassert_equal(pFwFsm->myStateId, FwFsm::StateId::eDownload, "wrong initial state ID");

	pFwFsm->handle_DlComplete(&e1);
	zassert_equal(pFwFsm->myStateId, FwFsm::StateId::eReady, "wrong state ID");

	pElem = pMqttMbox->read();
    zassert_equal(pElem->mtype, msgType::eFwDlSuccess, "Rcvd mtype != msgType::eFwDlSuccess");
    zassert_equal(pElem->from, Mbox::eFwUpdCtrl, "Rcvd sender != Mbox::eFwUpdCtrl");
	pElem->free();

	pElem = pAdxlMbox->read();
    zassert_equal(pElem->mtype, msgType::eFwUpdateActive, "Rcvd mtype != msgType::eFwUpdateActive");
    zassert_equal(pElem->from, Mbox::eFwUpdCtrl, "Rcvd sender != Mbox::eFwUpdCtrl");
	pElem->free();

	delete pMqttMbox;
	delete pAdxlMbox;
	delete pFwFsm;
	delete pUpdCtrl;
}

// test function handle_Restart
ZTEST(tests_update_fsm, test_update_fsm_handle_Restart)
{
    PoolElem* pElem;

	Mbox* pAdxlMbox = new Mbox("AdxlMbox");
    Mbox::registerMbox(pAdxlMbox, Mbox::eAdxl37xTask);
	zassert_not_null(pAdxlMbox, "pAdxlMbox = NULL");

	FwUpdCtrl* pUpdCtrl = new FwUpdCtrl();
	FwFsm* pFwFsm = new FwFsm((void*)pUpdCtrl, FwFsm::StateId::eIdle);
	zassert_not_null(pFwFsm, "memory alloc failed");
	zassert_not_null(pFwFsm->stateTbl[0], "memory alloc failed");
	zassert_not_null(pFwFsm->stateTbl[1], "memory alloc failed");
	zassert_not_null(pFwFsm->stateTbl[2], "memory alloc failed");
	zassert_not_null(pFwFsm->stateTbl[3], "memory alloc failed");
	zassert_equal((FwUpdCtrl*)pFwFsm->pd, pUpdCtrl, "wrong owner class");
	zassert_equal(pFwFsm->myStateId, FwFsm::StateId::eIdle, "wrong initial state ID");

	pFwFsm->handle_Restart();
	zassert_equal(pFwFsm->myStateId, FwFsm::StateId::eTriggered, "wrong state ID");

	pElem = pAdxlMbox->read();
    zassert_equal(pElem->mtype, msgType::eFwUpdateActive, "Rcvd mtype != msgType::eFwUpdateActive");
    zassert_equal(pElem->from, Mbox::eFwUpdCtrl, "Rcvd sender != Mbox::eFwUpdCtrl");
	pElem->free();

	delete pAdxlMbox;
	delete pFwFsm;
	delete pUpdCtrl;
}

// test function handle_DlFailed
ZTEST(tests_update_fsm, test_update_fsm_handle_DlFailed)
{
	PoolElem* e1 = new PoolElem();
    e1->mtype = eFwDlFailed;
    e1->from = Mbox::eFwUpdCtrl;

	FwUpdCtrl* pUpdCtrl = new FwUpdCtrl();
	FwFsm* pFwFsm = new FwFsm((void*)pUpdCtrl, FwFsm::StateId::eIdle);
	zassert_not_null(pFwFsm, "memory alloc failed");
	zassert_not_null(pFwFsm->stateTbl[0], "memory alloc failed");
	zassert_not_null(pFwFsm->stateTbl[1], "memory alloc failed");
	zassert_not_null(pFwFsm->stateTbl[2], "memory alloc failed");
	zassert_not_null(pFwFsm->stateTbl[3], "memory alloc failed");
	zassert_equal((FwUpdCtrl*)pFwFsm->pd, pUpdCtrl, "wrong owner class");
	zassert_equal(pFwFsm->myStateId, FwFsm::StateId::eIdle, "wrong initial state ID");

	pFwFsm->handle_DlFailed(&e1);
	zassert_equal(pFwFsm->myStateId, FwFsm::StateId::eIdle, "wrong state ID");
	e1->free();

	delete pFwFsm;
	delete pUpdCtrl;
}

// test function handle_DlFailed
ZTEST(tests_update_fsm, test_update_fsm_handle_DlFailed2)
{
    PoolElem* pElem;

	PoolElem* e1 = new PoolElem();
    e1->mtype = eFwDlFailed;
    e1->from = Mbox::eFwUpdCtrl;

	Mbox* pMqttMbox = new Mbox("MqttMbox");
    Mbox::registerMbox(pMqttMbox, Mbox::eMqttConnector);
	zassert_not_null(pMqttMbox, "pMqttMbox = NULL");

	FwUpdCtrl* pUpdCtrl = new FwUpdCtrl();
	pUpdCtrl->retryCnt = 5;
	pUpdCtrl->maxRetries = 5;
	FwFsm* pFwFsm = new FwFsm((void*)pUpdCtrl, FwFsm::StateId::eDownload);
	zassert_not_null(pFwFsm, "memory alloc failed");
	zassert_not_null(pFwFsm->stateTbl[0], "memory alloc failed");
	zassert_not_null(pFwFsm->stateTbl[1], "memory alloc failed");
	zassert_not_null(pFwFsm->stateTbl[2], "memory alloc failed");
	zassert_not_null(pFwFsm->stateTbl[3], "memory alloc failed");
	zassert_equal((FwUpdCtrl*)pFwFsm->pd, pUpdCtrl, "wrong owner class");
	zassert_equal(pFwFsm->myStateId, FwFsm::StateId::eDownload, "wrong initial state ID");

	pFwFsm->handle_DlFailed(&e1);
	zassert_equal(pFwFsm->myStateId, FwFsm::StateId::eIdle, "wrong state ID");

	pElem = pMqttMbox->read();
    zassert_equal(pElem->mtype, msgType::eFwDlFailed, "Rcvd mtype != msgType::eFwDlFailed");
    zassert_equal(pElem->from, Mbox::eFwUpdCtrl, "Rcvd sender != Mbox::eFwUpdCtrl");
	pElem->free();

	delete pMqttMbox;
	delete pFwFsm;
	delete pUpdCtrl;
}

// test function handle_DlFailed
ZTEST(tests_update_fsm, test_update_fsm_handle_DlFailed3)
{
    PoolElem* pElem;

	PoolElem* e1 = new PoolElem();
    e1->mtype = eFwDlFailed;
    e1->from = Mbox::eFwUpdCtrl;

	FwUpdCtrl* pUpdCtrl = new FwUpdCtrl();
	pUpdCtrl->retryCnt = 0;
	pUpdCtrl->maxRetries = 5;
	FwFsm* pFwFsm = new FwFsm((void*)pUpdCtrl, FwFsm::StateId::eDownload);
	zassert_not_null(pFwFsm, "memory alloc failed");
	zassert_not_null(pFwFsm->stateTbl[0], "memory alloc failed");
	zassert_not_null(pFwFsm->stateTbl[1], "memory alloc failed");
	zassert_not_null(pFwFsm->stateTbl[2], "memory alloc failed");
	zassert_not_null(pFwFsm->stateTbl[3], "memory alloc failed");
	zassert_equal((FwUpdCtrl*)pFwFsm->pd, pUpdCtrl, "wrong owner class");
	zassert_equal(pFwFsm->myStateId, FwFsm::StateId::eDownload, "wrong initial state ID");

	pFwFsm->handle_DlFailed(&e1);
	zassert_equal(pFwFsm->myStateId, FwFsm::StateId::eDownload, "wrong state ID");

	delete pFwFsm;
	delete pUpdCtrl;
}

// test function handle_timer_exp
ZTEST(tests_update_fsm, test_update_fsm_handle_timer_exp)
{
    PoolElem* pElem;

	Mbox* pMqttMbox = new Mbox("MqttMbox");
    Mbox::registerMbox(pMqttMbox, Mbox::eMqttConnector);
	zassert_not_null(pMqttMbox, "pMqttMbox = NULL");

	FwUpdCtrl* pUpdCtrl = new FwUpdCtrl();
	pUpdCtrl->retryCnt = 5;
	pUpdCtrl->maxRetries = 5;
	FwFsm* pFwFsm = new FwFsm((void*)pUpdCtrl, FwFsm::StateId::eDownload);
	zassert_not_null(pFwFsm, "memory alloc failed");
	zassert_not_null(pFwFsm->stateTbl[0], "memory alloc failed");
	zassert_not_null(pFwFsm->stateTbl[1], "memory alloc failed");
	zassert_not_null(pFwFsm->stateTbl[2], "memory alloc failed");
	zassert_not_null(pFwFsm->stateTbl[3], "memory alloc failed");
	zassert_equal((FwUpdCtrl*)pFwFsm->pd, pUpdCtrl, "wrong owner class");
	zassert_equal(pFwFsm->myStateId, FwFsm::StateId::eDownload, "wrong initial state ID");

	gbRetVal = false;
	pFwFsm->handle_timer_exp();
	zassert_equal(pFwFsm->myStateId, FwFsm::StateId::eDownload, "wrong state ID");

	pElem = pMqttMbox->read();
    zassert_equal(pElem->mtype, msgType::eFwDlFailed, "Rcvd mtype != msgType::eFwDlFailed");
    zassert_equal(pElem->from, Mbox::eFwUpdCtrl, "Rcvd sender != Mbox::eFwUpdCtrl");
	pElem->free();

	delete pMqttMbox;
	delete pFwFsm;
	delete pUpdCtrl;
}

// test function handle_timer_exp
ZTEST(tests_update_fsm, test_update_fsm_handle_timer_exp1)
{
    PoolElem* pElem;

	Mbox* pMqttMbox = new Mbox("MqttMbox");
    Mbox::registerMbox(pMqttMbox, Mbox::eMqttConnector);
	zassert_not_null(pMqttMbox, "pMqttMbox = NULL");

	FwUpdCtrl* pUpdCtrl = new FwUpdCtrl();
	pUpdCtrl->retryCnt = 1;
	pUpdCtrl->maxRetries = 5;
	FwFsm* pFwFsm = new FwFsm((void*)pUpdCtrl, FwFsm::StateId::eDownload);
	zassert_not_null(pFwFsm, "memory alloc failed");
	zassert_not_null(pFwFsm->stateTbl[0], "memory alloc failed");
	zassert_not_null(pFwFsm->stateTbl[1], "memory alloc failed");
	zassert_not_null(pFwFsm->stateTbl[2], "memory alloc failed");
	zassert_not_null(pFwFsm->stateTbl[3], "memory alloc failed");
	zassert_equal((FwUpdCtrl*)pFwFsm->pd, pUpdCtrl, "wrong owner class");
	zassert_equal(pFwFsm->myStateId, FwFsm::StateId::eDownload, "wrong initial state ID");

	gbRetVal = true;
	pFwFsm->myState->startDownloadTimestamp = 0;
	pFwFsm->handle_timer_exp();
	zassert_equal(pFwFsm->myStateId, FwFsm::StateId::eDownload, "wrong state ID");

	pElem = pMqttMbox->read();
    zassert_equal(pElem->mtype, msgType::eFwDlFailed, "Rcvd mtype != msgType::eFwDlFailed");
    zassert_equal(pElem->from, Mbox::eFwUpdCtrl, "Rcvd sender != Mbox::eFwUpdCtrl");
	pElem->free();

	delete pMqttMbox;
	delete pFwFsm;
	delete pUpdCtrl;
}

// test function handle_timer_exp
ZTEST(tests_update_fsm, test_update_fsm_handle_timer_exp2)
{
	FwUpdCtrl* pUpdCtrl = new FwUpdCtrl();
	pUpdCtrl->retryCnt = 1;
	pUpdCtrl->maxRetries = 5;
	FwFsm* pFwFsm = new FwFsm((void*)pUpdCtrl, FwFsm::StateId::eDownload);
	zassert_not_null(pFwFsm, "memory alloc failed");
	zassert_not_null(pFwFsm->stateTbl[0], "memory alloc failed");
	zassert_not_null(pFwFsm->stateTbl[1], "memory alloc failed");
	zassert_not_null(pFwFsm->stateTbl[2], "memory alloc failed");
	zassert_not_null(pFwFsm->stateTbl[3], "memory alloc failed");
	zassert_equal((FwUpdCtrl*)pFwFsm->pd, pUpdCtrl, "wrong owner class");
	zassert_equal(pFwFsm->myStateId, FwFsm::StateId::eDownload, "wrong initial state ID");

	gbRetVal = false;
	pFwFsm->handle_timer_exp();
	zassert_equal(pFwFsm->myStateId, FwFsm::StateId::eDownload, "wrong state ID");

	delete pFwFsm;
	delete pUpdCtrl;
}

// test function handle_timer_exp
ZTEST(tests_update_fsm, test_update_fsm_handle_timer_exp3)
{
	FwUpdCtrl* pUpdCtrl = new FwUpdCtrl();
	pUpdCtrl->retryCnt = 1;
	pUpdCtrl->maxRetries = 5;
	FwFsm* pFwFsm = new FwFsm((void*)pUpdCtrl, FwFsm::StateId::eIdle);
	zassert_not_null(pFwFsm, "memory alloc failed");
	zassert_not_null(pFwFsm->stateTbl[0], "memory alloc failed");
	zassert_not_null(pFwFsm->stateTbl[1], "memory alloc failed");
	zassert_not_null(pFwFsm->stateTbl[2], "memory alloc failed");
	zassert_not_null(pFwFsm->stateTbl[3], "memory alloc failed");
	zassert_equal((FwUpdCtrl*)pFwFsm->pd, pUpdCtrl, "wrong owner class");
	zassert_equal(pFwFsm->myStateId, FwFsm::StateId::eIdle, "wrong initial state ID");

	gbRetVal = false;
	pFwFsm->handle_timer_exp();
	zassert_equal(pFwFsm->myStateId, FwFsm::StateId::eIdle, "wrong state ID");

	delete pFwFsm;
	delete pUpdCtrl;
}

// test function handle_timer_exp
ZTEST(tests_update_fsm, test_update_fsm_handle_timer_exp4)
{
    PoolElem* pElem;

	Mbox* pAdxlMbox = new Mbox("AdxlMbox");
    Mbox::registerMbox(pAdxlMbox, Mbox::eAdxl37xTask);
	zassert_not_null(pAdxlMbox, "pAdxlMbox = NULL");

	FwUpdCtrl* pUpdCtrl = new FwUpdCtrl();
	pUpdCtrl->retryCnt = 1;
	pUpdCtrl->maxRetries = 5;
	FwFsm* pFwFsm = new FwFsm((void*)pUpdCtrl, FwFsm::StateId::eTriggered);
	zassert_not_null(pFwFsm, "memory alloc failed");
	zassert_not_null(pFwFsm->stateTbl[0], "memory alloc failed");
	zassert_not_null(pFwFsm->stateTbl[1], "memory alloc failed");
	zassert_not_null(pFwFsm->stateTbl[2], "memory alloc failed");
	zassert_not_null(pFwFsm->stateTbl[3], "memory alloc failed");
	zassert_equal((FwUpdCtrl*)pFwFsm->pd, pUpdCtrl, "wrong owner class");
	zassert_equal(pFwFsm->myStateId, FwFsm::StateId::eTriggered, "wrong initial state ID");

	gbRetVal = false;
	pFwFsm->handle_timer_exp();
	zassert_equal(pFwFsm->myStateId, FwFsm::StateId::eIdle, "wrong state ID");

	pElem = pAdxlMbox->read();
    zassert_equal(pElem->mtype, msgType::eAdxlResumeMeas, "Rcvd mtype != msgType::eAdxlResumeMeas");
    zassert_equal(pElem->from, Mbox::eFwUpdCtrl, "Rcvd sender != Mbox::eFwUpdCtrl");
	pElem->free();

	delete pAdxlMbox;
	delete pFwFsm;
	delete pUpdCtrl;
}

// test function handle_timer_exp
ZTEST(tests_update_fsm, test_update_fsm_handle_timer_exp5)
{
	FwUpdCtrl* pUpdCtrl = new FwUpdCtrl();
	FwFsm* pFwFsm = new FwFsm((void*)pUpdCtrl, FwFsm::StateId::eReady);
	zassert_not_null(pFwFsm, "memory alloc failed");
	zassert_not_null(pFwFsm->stateTbl[0], "memory alloc failed");
	zassert_not_null(pFwFsm->stateTbl[1], "memory alloc failed");
	zassert_not_null(pFwFsm->stateTbl[2], "memory alloc failed");
	zassert_not_null(pFwFsm->stateTbl[3], "memory alloc failed");
	zassert_equal((FwUpdCtrl*)pFwFsm->pd, pUpdCtrl, "wrong owner class");
	zassert_equal(pFwFsm->myStateId, FwFsm::StateId::eReady, "wrong initial state ID");

	gbRetVal = false;
	pFwFsm->handle_timer_exp();
	zassert_equal(pFwFsm->myStateId, FwFsm::StateId::eIdle, "wrong state ID");

	delete pFwFsm;
	delete pUpdCtrl;
}




// ---------------------------------------------------------------------------------

// test constructor and destructor
ZTEST(tests_update_fsm, test_update_fwstate_constructor)
{
	FwUpdCtrl* pUpdCtrl = new FwUpdCtrl();
	FwState* pFwState = new FwState(pUpdCtrl);

	zassert_not_null(pFwState, "memory alloc failed");

	delete pFwState;
	delete pUpdCtrl;
}

// ---------------------------------------------------------------------------------

// test constructor and destructor
ZTEST(tests_update_fsm, test_update_fwidle_constructor)
{
	FwUpdCtrl* pUpdCtrl = new FwUpdCtrl();
	FwIdle* pFwIdle = new FwIdle(pUpdCtrl);

	zassert_not_null(pFwIdle, "memory alloc failed");

	delete pFwIdle;
	delete pUpdCtrl;
}

// ---------------------------------------------------------------------------------

// test constructor and destructor
ZTEST(tests_update_fsm, test_update_fwtriggered_constructor)
{
	FwUpdCtrl* pUpdCtrl = new FwUpdCtrl();
	FwTriggered* pFwTriggered = new FwTriggered(pUpdCtrl);

	zassert_not_null(pFwTriggered, "memory alloc failed");

	delete pFwTriggered;
	delete pUpdCtrl;
}

// ---------------------------------------------------------------------------------

// test constructor and destructor
ZTEST(tests_update_fsm, test_update_fwdownload_constructor)
{
	FwUpdCtrl* pUpdCtrl = new FwUpdCtrl();
	FwDownload* pFwDownload = new FwDownload(pUpdCtrl);

	zassert_not_null(pFwDownload, "memory alloc failed");

	delete pFwDownload;
	delete pUpdCtrl;
}

// ---------------------------------------------------------------------------------

// test constructor and destructor
ZTEST(tests_update_fsm, test_update_fwready_constructor)
{
	FwUpdCtrl* pUpdCtrl = new FwUpdCtrl();
	FwReady* pFwReady = new FwReady(pUpdCtrl);

	zassert_not_null(pFwReady, "memory alloc failed");

	delete pFwReady;
	delete pUpdCtrl;
}



