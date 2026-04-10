#include <zephyr/ztest.h>

#define private public
#include "drv/pm/boost.h"

#include "stubs.h"

ZTEST_SUITE(tests_drv_BoostConverter, NULL, NULL, NULL, NULL, NULL);

// test constructor and destructor
ZTEST(tests_drv_BoostConverter, test_drv_BoostConverter_constructor)
{
	gbRetVal = true;
	gbReady  = true;

	BoostConverter* pBoost = new BoostConverter("boost");
	zassert_not_null(pBoost, "memory alloc failed");
	zassert_not_null(pBoost->pInMbox, "pInMbox = NULL");
	zassert_not_null(pBoost->myTimer, "myTimer = NULL");
    zassert_equal(pBoost->pInMbox->count(), 0, "pInMbox->count() != 0");
    zassert_equal(pBoost->pInMbox, Mbox::getMboxById(Mbox::eBoostConverter),
                  "Error getting mailbox by id");

    zassert_true(pBoost->device_good, "device_good != true");
    zassert_true(pBoost->isEnabled, "isEnabled != true");

	delete pBoost;
}

// MISSING for 100% branch coverage static instantiation does not deliver NULL
// here code line 
// static PoolElem e1 = {eTimerExpired, Mbox::MboxId::eBoostConverter, 'b', 'b'};
// pInMbox = new Mbox(name);
// myTimer = new Timer(BoostConverter::timer_cb, this, 0);
// are not covered completly.

// test constructor and destructor
ZTEST(tests_drv_BoostConverter, test_drv_BoostConverter_constructor2)
{
	gbRetVal = false;
	gbReady  = false;

	BoostConverter* pBoost = new BoostConverter("boost");
	zassert_not_null(pBoost, "memory alloc failed");
	zassert_not_null(pBoost->pInMbox, "pInMbox = NULL");
	zassert_not_null(pBoost->myTimer, "myTimer = NULL");
    zassert_equal(pBoost->pInMbox->count(), 0, "pInMbox->count() != 0");
    zassert_equal(pBoost->pInMbox, Mbox::getMboxById(Mbox::eBoostConverter),
                  "Error getting mailbox by id");

    zassert_false(pBoost->device_good, "device_good != false");
    zassert_false(pBoost->isEnabled, "isEnabled != false");

	delete pBoost;
}

// test function timer_cb and timer1_expired
ZTEST(tests_drv_BoostConverter, test_drv_BoostConverter_timer_cb)
{
    PoolElem* pElem;

	BoostConverter* pBoost = new BoostConverter("boost");
	zassert_not_null(pBoost, "memory alloc failed");
	zassert_not_null(pBoost->pInMbox, "pInMbox = NULL");
	zassert_not_null(pBoost->myTimer, "myTimer = NULL");
    zassert_equal(pBoost->pInMbox->count(), 0, "pInMbox->count() != 0");
    zassert_equal(pBoost->pInMbox, Mbox::getMboxById(Mbox::eBoostConverter),
                  "Error getting mailbox by id");

    pBoost->timer_cb(reinterpret_cast<k_timer*>(pBoost));

    zassert_equal(pBoost->pInMbox->count(), 1, "pInMbox->count() != 1");
    pElem = pBoost->pInMbox->read();
    zassert_equal(pElem->mtype, msgType::eTimerExpired, "Rcvd mtype != msgType::eTimerExpired");

    delete pBoost;
}    

// test function on and off
ZTEST(tests_drv_BoostConverter, test_drv_BoostConverter_on)
{
	BoostConverter* pBoost = new BoostConverter("boost");
	zassert_not_null(pBoost, "memory alloc failed");
	zassert_not_null(pBoost->pInMbox, "pInMbox = NULL");
	zassert_not_null(pBoost->myTimer, "myTimer = NULL");
    zassert_equal(pBoost->pInMbox->count(), 0, "pInMbox->count() != 0");
    zassert_equal(pBoost->pInMbox, Mbox::getMboxById(Mbox::eBoostConverter),
                  "Error getting mailbox by id");
    
    gbRetVal = true;
    pBoost->on();
    zassert_true(pBoost->isOn, "isOn != true");

    gbRetVal = false;
    pBoost->on();
    zassert_true(pBoost->isOn, "isOn != true");

    gbRetVal = true;
    pBoost->off();
    zassert_false(pBoost->isOn, "isOn != false");

    gbRetVal = false;
    pBoost->off();
    zassert_false(pBoost->isOn, "isOn != false");

    delete pBoost;
}    

//Test receive msg eTimerExpired
ZTEST(tests_drv_BoostConverter, test_drv_BoostConverter_rcv_msg_eTimerExpired)
{
	BoostConverter* pBoost = new BoostConverter("boost");
	zassert_not_null(pBoost, "memory alloc failed");
	zassert_not_null(pBoost->pInMbox, "pInMbox = NULL");
	zassert_not_null(pBoost->myTimer, "myTimer = NULL");
    zassert_equal(pBoost->pInMbox->count(), 0, "pInMbox->count() != 0");
    zassert_equal(pBoost->pInMbox, Mbox::getMboxById(Mbox::eBoostConverter),
                  "Error getting mailbox by id");
    
    pBoost->isOn = true;
    Mbox::sendMsg(msgType::eTimerExpired, Mbox::eBoostConverter, Mbox::eBoostConverter);

    zassert_equal(pBoost->pInMbox->count(), 1, "pInMbox->count() != 1");

    pBoost->task();  
    zassert_false(pBoost->isOn, "isOn != false");

    zassert_equal(pBoost->pInMbox->count(), 0, "pInMbox->count() != 0");

	delete pBoost; 
}

//Test receive msg eResume
ZTEST(tests_drv_BoostConverter, test_drv_BoostConverter_rcv_msg_eResume)
{
    uint8_t p1 = 0;

	BoostConverter* pBoost = new BoostConverter("boost");
	zassert_not_null(pBoost, "memory alloc failed");
	zassert_not_null(pBoost->pInMbox, "pInMbox = NULL");
	zassert_not_null(pBoost->myTimer, "myTimer = NULL");
    zassert_equal(pBoost->pInMbox->count(), 0, "pInMbox->count() != 0");
    zassert_equal(pBoost->pInMbox, Mbox::getMboxById(Mbox::eBoostConverter),
                  "Error getting mailbox by id");
    
    pBoost->isEnabled = true;
    pBoost->isOn = false;
    Mbox::sendMsg(msgType::eResume, Mbox::eBoostConverter, Mbox::eBoostConverter, p1);

    zassert_equal(pBoost->pInMbox->count(), 1, "pInMbox->count() != 1");

    pBoost->task();  
    zassert_true(pBoost->isOn, "isOn != true");

    zassert_equal(pBoost->pInMbox->count(), 0, "pInMbox->count() != 0");

	delete pBoost; 
}

//Test receive msg eResume
ZTEST(tests_drv_BoostConverter, test_drv_BoostConverter_rcv_msg_eResume2)
{
    uint8_t p1 = 10;

	BoostConverter* pBoost = new BoostConverter("boost");
	zassert_not_null(pBoost, "memory alloc failed");
	zassert_not_null(pBoost->pInMbox, "pInMbox = NULL");
	zassert_not_null(pBoost->myTimer, "myTimer = NULL");
    zassert_equal(pBoost->pInMbox->count(), 0, "pInMbox->count() != 0");
    zassert_equal(pBoost->pInMbox, Mbox::getMboxById(Mbox::eBoostConverter),
                  "Error getting mailbox by id");
    
    pBoost->isEnabled = true;
    pBoost->isOn = false;
    Mbox::sendMsg(msgType::eResume, Mbox::eBoostConverter, Mbox::eBoostConverter, p1);

    zassert_equal(pBoost->pInMbox->count(), 1, "pInMbox->count() != 1");

    pBoost->task();  
    zassert_true(pBoost->isOn, "isOn != true");

    zassert_equal(pBoost->pInMbox->count(), 0, "pInMbox->count() != 0");

	delete pBoost; 
}

// Test receive msg eResume
ZTEST(tests_drv_BoostConverter, test_drv_BoostConverter_rcv_msg_eResume3)
{
    uint8_t p1 = 10;

	BoostConverter* pBoost = new BoostConverter("boost");
	zassert_not_null(pBoost, "memory alloc failed");
	zassert_not_null(pBoost->pInMbox, "pInMbox = NULL");
	zassert_not_null(pBoost->myTimer, "myTimer = NULL");
    zassert_equal(pBoost->pInMbox->count(), 0, "pInMbox->count() != 0");
    zassert_equal(pBoost->pInMbox, Mbox::getMboxById(Mbox::eBoostConverter),
                  "Error getting mailbox by id");
    
    pBoost->isEnabled = false;
    pBoost->isOn = false;
    Mbox::sendMsg(msgType::eResume, Mbox::eBoostConverter, Mbox::eBoostConverter, p1);

    zassert_equal(pBoost->pInMbox->count(), 1, "pInMbox->count() != 1");

    pBoost->task();  
    zassert_false(pBoost->isOn, "isOn != false");

    zassert_equal(pBoost->pInMbox->count(), 0, "pInMbox->count() != 0");

	delete pBoost; 
}

// Test receive msg eSuspend
ZTEST(tests_drv_BoostConverter, test_drv_BoostConverter_rcv_msg_eSuspend)
{
	BoostConverter* pBoost = new BoostConverter("boost");
	zassert_not_null(pBoost, "memory alloc failed");
	zassert_not_null(pBoost->pInMbox, "pInMbox = NULL");
	zassert_not_null(pBoost->myTimer, "myTimer = NULL");
    zassert_equal(pBoost->pInMbox->count(), 0, "pInMbox->count() != 0");
    zassert_equal(pBoost->pInMbox, Mbox::getMboxById(Mbox::eBoostConverter),
                  "Error getting mailbox by id");
    
    pBoost->isOn = true;
    Mbox::sendMsg(msgType::eSuspend, Mbox::eBoostConverter, Mbox::eBoostConverter);

    zassert_equal(pBoost->pInMbox->count(), 1, "pInMbox->count() != 1");

    pBoost->task();  
    zassert_false(pBoost->isOn, "isOn != false");

    zassert_equal(pBoost->pInMbox->count(), 0, "pInMbox->count() != 0");

	delete pBoost; 
}

// Test receive msg eBoostOnReq
ZTEST(tests_drv_BoostConverter, test_drv_BoostConverter_rcv_msg_eBoostOnReq)
{
	BoostConverter* pBoost = new BoostConverter("boost");
	zassert_not_null(pBoost, "memory alloc failed");
	zassert_not_null(pBoost->pInMbox, "pInMbox = NULL");
	zassert_not_null(pBoost->myTimer, "myTimer = NULL");
    zassert_equal(pBoost->pInMbox->count(), 0, "pInMbox->count() != 0");
    zassert_equal(pBoost->pInMbox, Mbox::getMboxById(Mbox::eBoostConverter),
                  "Error getting mailbox by id");
    
    gbRetVal = true;
    pBoost->isEnabled = false;
    Mbox::sendMsg(msgType::eBoostOnReq, Mbox::eBoostConverter, Mbox::eBoostConverter);

    zassert_equal(pBoost->pInMbox->count(), 1, "pInMbox->count() != 1");

    pBoost->task();  
    zassert_true(pBoost->isEnabled, "isOn != true");

    zassert_equal(pBoost->pInMbox->count(), 0, "pInMbox->count() != 0");

	delete pBoost; 
}

// Test receive msg eBoostOnReq
ZTEST(tests_drv_BoostConverter, test_drv_BoostConverter_rcv_msg_eBoostOnReq2)
{
	BoostConverter* pBoost = new BoostConverter("boost");
	zassert_not_null(pBoost, "memory alloc failed");
	zassert_not_null(pBoost->pInMbox, "pInMbox = NULL");
	zassert_not_null(pBoost->myTimer, "myTimer = NULL");
    zassert_equal(pBoost->pInMbox->count(), 0, "pInMbox->count() != 0");
    zassert_equal(pBoost->pInMbox, Mbox::getMboxById(Mbox::eBoostConverter),
                  "Error getting mailbox by id");
    
    gbRetVal = false;
    pBoost->isEnabled = false;
    Mbox::sendMsg(msgType::eBoostOnReq, Mbox::eBoostConverter, Mbox::eBoostConverter);

    zassert_equal(pBoost->pInMbox->count(), 1, "pInMbox->count() != 1");

    pBoost->task();  
    zassert_true(pBoost->isEnabled, "isOn != true");

    zassert_equal(pBoost->pInMbox->count(), 0, "pInMbox->count() != 0");

	delete pBoost; 
}

// Test receive msg eBoostOffReq
ZTEST(tests_drv_BoostConverter, test_drv_BoostConverter_rcv_msg_eBoostOffReq)
{
	BoostConverter* pBoost = new BoostConverter("boost");
	zassert_not_null(pBoost, "memory alloc failed");
	zassert_not_null(pBoost->pInMbox, "pInMbox = NULL");
	zassert_not_null(pBoost->myTimer, "myTimer = NULL");
    zassert_equal(pBoost->pInMbox->count(), 0, "pInMbox->count() != 0");
    zassert_equal(pBoost->pInMbox, Mbox::getMboxById(Mbox::eBoostConverter),
                  "Error getting mailbox by id");
    
    gbRetVal = true;
    pBoost->isEnabled = true;
    pBoost->isOn = true;
    Mbox::sendMsg(msgType::eBoostOffReq, Mbox::eBoostConverter, Mbox::eBoostConverter);

    zassert_equal(pBoost->pInMbox->count(), 1, "pInMbox->count() != 1");

    pBoost->task();  
    zassert_false(pBoost->isEnabled, "isEnabled != false");
    zassert_false(pBoost->isOn, "isOn != false");

    zassert_equal(pBoost->pInMbox->count(), 0, "pInMbox->count() != 0");

	delete pBoost; 
}

// Test receive msg eBoostOffReq
ZTEST(tests_drv_BoostConverter, test_drv_BoostConverter_rcv_msg_eBoostOffReq2)
{
	BoostConverter* pBoost = new BoostConverter("boost");
	zassert_not_null(pBoost, "memory alloc failed");
	zassert_not_null(pBoost->pInMbox, "pInMbox = NULL");
	zassert_not_null(pBoost->myTimer, "myTimer = NULL");
    zassert_equal(pBoost->pInMbox->count(), 0, "pInMbox->count() != 0");
    zassert_equal(pBoost->pInMbox, Mbox::getMboxById(Mbox::eBoostConverter),
                  "Error getting mailbox by id");
    
    gbRetVal = false;
    pBoost->isEnabled = true;
    pBoost->isOn = true;
    Mbox::sendMsg(msgType::eBoostOffReq, Mbox::eBoostConverter, Mbox::eBoostConverter);

    zassert_equal(pBoost->pInMbox->count(), 1, "pInMbox->count() != 1");

    pBoost->task();  
    zassert_false(pBoost->isEnabled, "isEnabled != false");
    zassert_false(pBoost->isOn, "isOn != false");

    zassert_equal(pBoost->pInMbox->count(), 0, "pInMbox->count() != 0");

	delete pBoost; 
}

//Test receive unexpected msg 
ZTEST(tests_drv_BoostConverter, test_drv_BoostConverter_rcv_msg_unexpected)
{
	BoostConverter* pBoost = new BoostConverter("boost");
	zassert_not_null(pBoost, "memory alloc failed");
	zassert_not_null(pBoost->pInMbox, "pInMbox = NULL");
	zassert_not_null(pBoost->myTimer, "myTimer = NULL");
    zassert_equal(pBoost->pInMbox->count(), 0, "pInMbox->count() != 0");
    zassert_equal(pBoost->pInMbox, Mbox::getMboxById(Mbox::eBoostConverter),
                  "Error getting mailbox by id");
    

    Mbox::sendMsg(msgType::eResetCause, Mbox::eBoostConverter, Mbox::eBoostConverter);

    zassert_equal(pBoost->pInMbox->count(), 1, "pInMbox->count() != 1");

    pBoost->task();  

    zassert_equal(pBoost->pInMbox->count(), 0, "pInMbox->count() != 0");
	delete pBoost; 
}

//Test receive no msg 
ZTEST(tests_drv_BoostConverter, test_drv_BoostConverter_rcv_msg_none)
{
	BoostConverter* pBoost = new BoostConverter("boost");
	zassert_not_null(pBoost, "memory alloc failed");
	zassert_not_null(pBoost->pInMbox, "pInMbox = NULL");
	zassert_not_null(pBoost->myTimer, "myTimer = NULL");
    zassert_equal(pBoost->pInMbox->count(), 0, "pInMbox->count() != 0");
    zassert_equal(pBoost->pInMbox, Mbox::getMboxById(Mbox::eBoostConverter),
                  "Error getting mailbox by id");
    
    pBoost->pInMbox->write(NULL);
    
    pBoost->task();  

    zassert_equal(pBoost->pInMbox->count(), 0, "pInMbox->count() != 0");
	delete pBoost; 
}





