#include <zephyr/ztest.h>
//#include <vector>
//#include <algorithm>

#define private public
#include "pm/reset_ctrl.h"

#include "stubs.h"

ZTEST_SUITE(tests_pm_reset_ctrl, NULL, NULL, NULL, NULL, NULL);

// test constructor and destructor
ZTEST(tests_pm_reset_ctrl, test_pm_reset_ctrl_constructor)
{
	ResetCtrl* pResetCtrl = new ResetCtrl("ResetCtrl");
	zassert_not_null(pResetCtrl, "memory alloc failed");
	zassert_not_null(pResetCtrl->pInMbox, "pInMbox = NULL");
	zassert_not_null(pResetCtrl->myTimer, "myTimer = NULL");
    zassert_equal(pResetCtrl->pInMbox->count(), 0, "pInMbox->count() != 0");
    zassert_equal(pResetCtrl->pInMbox, Mbox::getMboxById(Mbox::eResetCtrl),
                  "Error getting mailbox by id");

	delete pResetCtrl;
}

// MISSING for 100% branch coverage operator new does not deliver NULL
// here code lines 
// pInMbox = new Mbox(name); 
// myTimer = new Timer(ResetCtrl::timer_cb, this, 7 * 24 * 60 * 60, 7 * 24 * 60 * 60);
// static PoolElem e1 = {eTimerExpired, Mbox::MboxId::eResetCtrl, 'n', 't'};
// are not covered completly.



// test function timer_cb and timer1_expired
ZTEST(tests_pm_reset_ctrl, test_pm_reset_ctrl_timer_cb)
{
    PoolElem* pElem;

	ResetCtrl* pResetCtrl = new ResetCtrl("ResetCtrl");
	zassert_not_null(pResetCtrl, "memory alloc failed");
	zassert_not_null(pResetCtrl->pInMbox, "pInMbox = NULL");
	zassert_not_null(pResetCtrl->myTimer, "myTimer = NULL");
    zassert_equal(pResetCtrl->pInMbox->count(), 0, "pInMbox->count() != 0");
    zassert_equal(pResetCtrl->pInMbox, Mbox::getMboxById(Mbox::eResetCtrl),
                  "Error getting mailbox by id");

    pResetCtrl->timer_cb(reinterpret_cast<k_timer*>(pResetCtrl));

    zassert_equal(pResetCtrl->pInMbox->count(), 1, "pInMbox->count() != 1");
    pElem = pResetCtrl->pInMbox->read();
    zassert_equal(pElem->mtype, msgType::eTimerExpired, "Rcvd mtype != msgType::eTimerExpired");

    delete pResetCtrl;
}    

// test function trigger
ZTEST(tests_pm_reset_ctrl, test_pm_reset_ctrl_trigger)
{
	ResetCtrl* pResetCtrl = new ResetCtrl("ResetCtrl");
	zassert_not_null(pResetCtrl, "memory alloc failed");
	zassert_not_null(pResetCtrl->pInMbox, "pInMbox = NULL");
	zassert_not_null(pResetCtrl->myTimer, "myTimer = NULL");
    zassert_equal(pResetCtrl->pInMbox->count(), 0, "pInMbox->count() != 0");
    zassert_equal(pResetCtrl->pInMbox, Mbox::getMboxById(Mbox::eResetCtrl),
                  "Error getting mailbox by id");

    pResetCtrl->trigger();

	delete pResetCtrl;
}

// test function performSwReset
ZTEST(tests_pm_reset_ctrl, test_pm_reset_ctrl_performSwReset)
{
	ResetCtrl* pResetCtrl = new ResetCtrl("ResetCtrl");
	zassert_not_null(pResetCtrl, "memory alloc failed");
	zassert_not_null(pResetCtrl->pInMbox, "pInMbox = NULL");
	zassert_not_null(pResetCtrl->myTimer, "myTimer = NULL");
    zassert_equal(pResetCtrl->pInMbox->count(), 0, "pInMbox->count() != 0");
    zassert_equal(pResetCtrl->pInMbox, Mbox::getMboxById(Mbox::eResetCtrl),
                  "Error getting mailbox by id");

    gbRetVal = true;
    pResetCtrl->performSwReset();

	delete pResetCtrl;
}

// test function performSwReset
ZTEST(tests_pm_reset_ctrl, test_pm_reset_ctrl_performSwReset2)
{
	ResetCtrl* pResetCtrl = new ResetCtrl("ResetCtrl");
	zassert_not_null(pResetCtrl, "memory alloc failed");
	zassert_not_null(pResetCtrl->pInMbox, "pInMbox = NULL");
	zassert_not_null(pResetCtrl->myTimer, "myTimer = NULL");
    zassert_equal(pResetCtrl->pInMbox->count(), 0, "pInMbox->count() != 0");
    zassert_equal(pResetCtrl->pInMbox, Mbox::getMboxById(Mbox::eResetCtrl),
                  "Error getting mailbox by id");

    gbRetVal = false;
    pResetCtrl->performSwReset();

	delete pResetCtrl;
}

//Test receive msg eTimerExpired 
ZTEST(tests_pm_reset_ctrl, test_pm_reset_ctrl_rcv_msg_eTimerExpired)
{
	ResetCtrl* pResetCtrl = new ResetCtrl("ResetCtrl");
	zassert_not_null(pResetCtrl, "memory alloc failed");
	zassert_not_null(pResetCtrl->pInMbox, "pInMbox = NULL");
	zassert_not_null(pResetCtrl->myTimer, "myTimer = NULL");
    zassert_equal(pResetCtrl->pInMbox->count(), 0, "pInMbox->count() != 0");
    zassert_equal(pResetCtrl->pInMbox, Mbox::getMboxById(Mbox::eResetCtrl),
                  "Error getting mailbox by id");

    Mbox::sendMsg(msgType::eTimerExpired, Mbox::eResetCtrl, Mbox::eResetCtrl);

    zassert_equal(pResetCtrl->pInMbox->count(), 1, "pInMbox->count() != 1");

    pResetCtrl->task();  

    zassert_equal(pResetCtrl->pInMbox->count(), 0, "pInMbox->count() != 0");

	delete pResetCtrl; 
}

//Test receive msg eColdStartReq 
ZTEST(tests_pm_reset_ctrl, test_pm_reset_ctrl_rcv_msg_eColdStartReq)
{
	ResetCtrl* pResetCtrl = new ResetCtrl("ResetCtrl");
	zassert_not_null(pResetCtrl, "memory alloc failed");
	zassert_not_null(pResetCtrl->pInMbox, "pInMbox = NULL");
	zassert_not_null(pResetCtrl->myTimer, "myTimer = NULL");
    zassert_equal(pResetCtrl->pInMbox->count(), 0, "pInMbox->count() != 0");
    zassert_equal(pResetCtrl->pInMbox, Mbox::getMboxById(Mbox::eResetCtrl),
                  "Error getting mailbox by id");

    Mbox::sendMsg(msgType::eColdStartReq, Mbox::eResetCtrl, Mbox::eResetCtrl);

    zassert_equal(pResetCtrl->pInMbox->count(), 1, "pInMbox->count() != 1");

    pResetCtrl->task();  

    zassert_equal(pResetCtrl->pInMbox->count(), 0, "pInMbox->count() != 0");

	delete pResetCtrl; 
}

//Test receive unexpected msg 
ZTEST(tests_pm_reset_ctrl, test_pm_reset_ctrl_rcv_msg_unexpected)
{
	ResetCtrl* pResetCtrl = new ResetCtrl("ResetCtrl");
	zassert_not_null(pResetCtrl, "memory alloc failed");
	zassert_not_null(pResetCtrl->pInMbox, "pInMbox = NULL");
	zassert_not_null(pResetCtrl->myTimer, "myTimer = NULL");
    zassert_equal(pResetCtrl->pInMbox->count(), 0, "pInMbox->count() != 0");
    zassert_equal(pResetCtrl->pInMbox, Mbox::getMboxById(Mbox::eResetCtrl),
                  "Error getting mailbox by id");

    Mbox::sendMsg(msgType::eResetCause, Mbox::eResetCtrl, Mbox::eResetCtrl);

    zassert_equal(pResetCtrl->pInMbox->count(), 1, "pInMbox->count() != 1");

    pResetCtrl->task();  

    zassert_equal(pResetCtrl->pInMbox->count(), 0, "pInMbox->count() != 0");
	delete pResetCtrl; 
}

//Test receive no msg 
ZTEST(tests_pm_reset_ctrl, test_pm_reset_ctrl_rcv_msg_none)
{
	ResetCtrl* pResetCtrl = new ResetCtrl("ResetCtrl");
	zassert_not_null(pResetCtrl, "memory alloc failed");
	zassert_not_null(pResetCtrl->pInMbox, "pInMbox = NULL");
	zassert_not_null(pResetCtrl->myTimer, "myTimer = NULL");
    zassert_equal(pResetCtrl->pInMbox->count(), 0, "pInMbox->count() != 0");
    zassert_equal(pResetCtrl->pInMbox, Mbox::getMboxById(Mbox::eResetCtrl),
                  "Error getting mailbox by id");

    pResetCtrl->pInMbox->write(NULL);
    
    pResetCtrl->task();  

    zassert_equal(pResetCtrl->pInMbox->count(), 0, "pInMbox->count() != 0");
	delete pResetCtrl; 
}

