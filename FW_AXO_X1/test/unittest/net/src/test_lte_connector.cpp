#include <zephyr/ztest.h>
#include <vector>
#include <algorithm>

#define private public
#include "net/lte_connector.h"

#include "stubs.h"

ZTEST_SUITE(tests_net_lte_connector, NULL, NULL, NULL, NULL, NULL);

// test constructor and destructor
ZTEST(tests_net_lte_connector, test_net_lte_connector_constructor)
{
	LteConnector* pLteConnector = new LteConnector("LTE");
	zassert_not_null(pLteConnector, "memory alloc failed");
	zassert_not_null(pLteConnector->pInMbox, "pInMbox alloc failed");
	zassert_not_null(pLteConnector->myTimer, "myTimer alloc failed");
	zassert_false(pLteConnector->bLteConnected, "bLteConnected not false");
	zassert_false(pLteConnector->bConnectRetry, "bConnectRetry not false");
	delete pLteConnector;
}

// MISSING for 100% branch coverage operator new does not deliver NULL
// here code lines 
// pInMbox = new Mbox(name); 
// myTimer = new Timer(LteConnector::timer_cb, this, connectTimeout);
// static PoolElem e1 = {eTimerExpired, Mbox::MboxId::eLteConnector, 'l', 'c'};
// are not covered completly.


// test function lte_event_handler
ZTEST(tests_net_lte_connector, test_net_lte_connector_lte_event_handler)
{
	struct lte_lc_evt ev;

	ev.type = LTE_LC_EVT_MODEM_EVENT;
	ev.modem_evt = LTE_LC_MODEM_EVT_RESET_LOOP;
	LteConnector::lte_event_handler(&ev);

	ev.type = LTE_LC_EVT_MODEM_EVENT;
	ev.modem_evt = LTE_LC_MODEM_EVT_BATTERY_LOW;
	LteConnector::lte_event_handler(&ev);

	ev.type = LTE_LC_EVT_NW_REG_STATUS;
	LteConnector::lte_event_handler(&ev);

	ev.type = LTE_LC_EVT_NW_REG_STATUS;
	ev.nw_reg_status = LTE_LC_NW_REG_REGISTERED_HOME;
	LteConnector::lte_event_handler(&ev);

	ev.type = LTE_LC_EVT_NW_REG_STATUS;
	ev.nw_reg_status = LTE_LC_NW_REG_REGISTERED_ROAMING;
	LteConnector::lte_event_handler(&ev);

	ev.type = LTE_LC_EVT_PSM_UPDATE;
	LteConnector::lte_event_handler(&ev);

	ev.type = LTE_LC_EVT_EDRX_UPDATE;
	LteConnector::lte_event_handler(&ev);

	ev.type = LTE_LC_EVT_RRC_UPDATE;
	LteConnector::lte_event_handler(&ev);

	ev.type = LTE_LC_EVT_CELL_UPDATE;
	LteConnector::lte_event_handler(&ev);

	ev.type = LTE_LC_EVT_LTE_MODE_UPDATE;
	LteConnector::lte_event_handler(&ev);

	ev.type = LTE_LC_EVT_TAU_PRE_WARNING;
	LteConnector::lte_event_handler(&ev);

	ev.type = LTE_LC_EVT_NEIGHBOR_CELL_MEAS;
	LteConnector::lte_event_handler(&ev);

	ev.type = LTE_LC_EVT_MODEM_SLEEP_EXIT_PRE_WARNING;
	LteConnector::lte_event_handler(&ev);

	ev.type = LTE_LC_EVT_MODEM_SLEEP_EXIT;
	LteConnector::lte_event_handler(&ev);

	ev.type = LTE_LC_EVT_MODEM_SLEEP_ENTER;
	LteConnector::lte_event_handler(&ev);

	ev.type = LTE_LC_EVT_MODEM_SLEEP_ENTER;
    ev.modem_sleep.type = LTE_LC_MODEM_SLEEP_PSM;
	LteConnector::lte_event_handler(&ev);

	ev.type = LTE_LC_EVT_MODEM_SLEEP_ENTER;
    ev.modem_sleep.type = LTE_LC_MODEM_SLEEP_RF_INACTIVITY;
	LteConnector::lte_event_handler(&ev);

	ev.type = LTE_LC_EVT_MODEM_SLEEP_ENTER;
    ev.modem_sleep.type = LTE_LC_MODEM_SLEEP_FLIGHT_MODE;
	LteConnector::lte_event_handler(&ev);

	ev.type = LTE_LC_EVT_MODEM_EVENT;
	LteConnector::lte_event_handler(&ev);

	ev.type = (enum lte_lc_evt_type)0xFF;
	LteConnector::lte_event_handler(&ev);
}

// test function pdn_event_handler
ZTEST(tests_net_lte_connector, test_net_lte_connector_pdn_event_handler)
{
	LteConnector::pdn_event_handler(0, PDN_EVENT_CNEC_ESM, 0);

	LteConnector::pdn_event_handler(0, (enum pdn_event)0xFF, 0);
}

// test function pdn_event_handler
ZTEST(tests_net_lte_connector, test_net_lte_connector_pdn_event_handler2)
{
    PoolElem* pElem;

	Mbox* pLteConnMbox = new Mbox("LteConnectorMbox");
    Mbox::registerMbox(pLteConnMbox, Mbox::eLteConnector);
	zassert_not_null(pLteConnMbox, "pLteConnMbox = NULL");

	LteConnector::pdn_event_handler(0, PDN_EVENT_ACTIVATED, 0);
    zassert_equal(pLteConnMbox->count(), 1, "pLteConnMbox->count() != 1");

    pElem = pLteConnMbox->read();
    zassert_equal(pElem->mtype, msgType::eLteConnected, "Rcvd mtype != msgType::eLteConnected");
    zassert_equal(pElem->from, Mbox::eLteConnector, "Rcvd sender != Mbox::eLteConnector");

    pElem->free();
    delete pLteConnMbox;
}

// test function pdn_event_handler
ZTEST(tests_net_lte_connector, test_net_lte_connector_pdn_event_handler3)
{
    PoolElem* pElem;

	Mbox* pLteConnMbox = new Mbox("LteConnectorMbox");
    Mbox::registerMbox(pLteConnMbox, Mbox::eLteConnector);
	zassert_not_null(pLteConnMbox, "pLteConnMbox = NULL");

	LteConnector::pdn_event_handler(0, PDN_EVENT_DEACTIVATED, 0);
    zassert_equal(pLteConnMbox->count(), 1, "pLteConnMbox->count() != 1");

    pElem = pLteConnMbox->read();
    zassert_equal(pElem->mtype, msgType::eLteDisconnected, "Rcvd mtype != msgType::eLteDisconnected");
    zassert_equal(pElem->from, Mbox::eLteConnector, "Rcvd sender != Mbox::eLteConnector");

    pElem->free();
    delete pLteConnMbox;
}

// test function pdn_event_handler
ZTEST(tests_net_lte_connector, test_net_lte_connector_pdn_event_handler4)
{
    PoolElem* pElem;

	Mbox* pLteConnMbox = new Mbox("LteConnectorMbox");
    Mbox::registerMbox(pLteConnMbox, Mbox::eLteConnector);
	zassert_not_null(pLteConnMbox, "pLteConnMbox = NULL");

	LteConnector::pdn_event_handler(0, PDN_EVENT_IPV6_UP, 0);
    zassert_equal(pLteConnMbox->count(), 1, "pLteConnMbox->count() != 1");

    pElem = pLteConnMbox->read();
    zassert_equal(pElem->mtype, msgType::eIp6Up, "Rcvd mtype != msgType::eIp6Up");
    zassert_equal(pElem->from, Mbox::eLteConnector, "Rcvd sender != Mbox::eLteConnector");

    pElem->free();
    delete pLteConnMbox;
}

// test function pdn_event_handler
ZTEST(tests_net_lte_connector, test_net_lte_connector_pdn_event_handler5)
{
    PoolElem* pElem;

	Mbox* pLteConnMbox = new Mbox("LteConnectorMbox");
    Mbox::registerMbox(pLteConnMbox, Mbox::eLteConnector);
	zassert_not_null(pLteConnMbox, "pLteConnMbox = NULL");

	LteConnector::pdn_event_handler(0, PDN_EVENT_IPV6_DOWN, 0);
    zassert_equal(pLteConnMbox->count(), 1, "pLteConnMbox->count() != 1");

    pElem = pLteConnMbox->read();
    zassert_equal(pElem->mtype, msgType::eIp6Down, "Rcvd mtype != msgType::eIp6Down");
    zassert_equal(pElem->from, Mbox::eLteConnector, "Rcvd sender != Mbox::eLteConnector");

    pElem->free();
    delete pLteConnMbox;
}

// test function timer_cb and timer1_expired
ZTEST(tests_net_lte_connector, test_net_lte_connector_timer_cb)
{
    PoolElem* pElem;

    LteConnector* pLteConnector = new LteConnector("LTE");
	zassert_not_null(pLteConnector, "memory alloc failed");
	zassert_not_null(pLteConnector->pInMbox, "pInMbox alloc failed");
	zassert_not_null(pLteConnector->myTimer, "myTimer alloc failed");
    zassert_equal(pLteConnector->pInMbox->count(), 0, "pInMbox->count() != 0");
    zassert_equal(pLteConnector->pInMbox, Mbox::getMboxById(Mbox::eLteConnector),
                  "Error getting mailbox by id");

    pLteConnector->timer_cb(reinterpret_cast<k_timer*>(pLteConnector));

    zassert_equal(pLteConnector->pInMbox->count(), 1, "pInMbox->count() != 1");
    pElem = pLteConnector->pInMbox->read();
    zassert_equal(pElem->mtype, msgType::eTimerExpired, "Rcvd mtype != msgType::eTimerExpired");

    delete pLteConnector;
}    

// test function notify
ZTEST(tests_net_lte_connector, test_net_lte_connector_notify)
{
    PoolElem* pElem;

	Mbox* pMqttConnMbox = new Mbox("MqttConnectorMbox");
    Mbox::registerMbox(pMqttConnMbox, Mbox::eMqttConnector);
	zassert_not_null(pMqttConnMbox, "pMqttConnMbox = NULL");

	Mbox* pHttpConnMbox = new Mbox("HttpConnectorMbox");
    Mbox::registerMbox(pHttpConnMbox, Mbox::eHttpConnector);
	zassert_not_null(pHttpConnMbox, "pHttpConnMbox = NULL");

	LteConnector* pLteConnector = new LteConnector("LTE");
	zassert_not_null(pLteConnector, "memory alloc failed");
	zassert_not_null(pLteConnector->pInMbox, "pInMbox alloc failed");
	zassert_not_null(pLteConnector->myTimer, "myTimer alloc failed");

	pLteConnector->listeners[0] = pMqttConnMbox;
	pLteConnector->listeners[1] = pHttpConnMbox;

	pLteConnector->notify(eLteConnected);

    zassert_equal(pMqttConnMbox->count(), 1, "pMqttConnMbox->count() != 1");
    zassert_equal(pHttpConnMbox->count(), 1, "pHttpConnMbox->count() != 1");

    pElem = pMqttConnMbox->read();
    zassert_equal(pElem->mtype, msgType::eLteConnected, "Rcvd mtype != msgType::eLteConnected");
    zassert_equal(pElem->from, Mbox::eLteConnector, "Rcvd sender != Mbox::eLteConnector");
	pElem->free();

    pElem = pHttpConnMbox->read();
    zassert_equal(pElem->mtype, msgType::eLteConnected, "Rcvd mtype != msgType::eLteConnected");
    zassert_equal(pElem->from, Mbox::eLteConnector, "Rcvd sender != Mbox::eLteConnector");
    pElem->free();

    delete pMqttConnMbox;
    delete pHttpConnMbox;
	delete pLteConnector;
}

// test function registerListener
ZTEST(tests_net_lte_connector, test_net_lte_connector_registerListener)
{
	Mbox* pMqttConnMbox = new Mbox("MqttConnectorMbox");
    Mbox::registerMbox(pMqttConnMbox, Mbox::eMqttConnector);
	zassert_not_null(pMqttConnMbox, "pMqttConnMbox = NULL");

	Mbox* pHttpConnMbox = new Mbox("HttpConnectorMbox");
    Mbox::registerMbox(pHttpConnMbox, Mbox::eHttpConnector);
	zassert_not_null(pHttpConnMbox, "pHttpConnMbox = NULL");

	LteConnector* pLteConnector = new LteConnector("LTE");
	zassert_not_null(pLteConnector, "memory alloc failed");
	zassert_not_null(pLteConnector->pInMbox, "pInMbox alloc failed");
	zassert_not_null(pLteConnector->myTimer, "myTimer alloc failed");

	pLteConnector->registerListener(pMqttConnMbox);
	pLteConnector->registerListener(pHttpConnMbox);
	zassert_equal(pLteConnector->listeners[0], pMqttConnMbox, "listener != pMqttConnMbox");
	zassert_equal(pLteConnector->listeners[1], pHttpConnMbox, "listener != pHttpConnMbox");

    delete pMqttConnMbox;
    delete pHttpConnMbox;
	delete pLteConnector;
}

// test function registerListener
ZTEST(tests_net_lte_connector, test_net_lte_connector_registerListener2)
{
	Mbox* pMqttConnMbox = new Mbox("MqttConnectorMbox");
    Mbox::registerMbox(pMqttConnMbox, Mbox::eMqttConnector);
	zassert_not_null(pMqttConnMbox, "pMqttConnMbox = NULL");

	Mbox* pHttpConnMbox = new Mbox("HttpConnectorMbox");
    Mbox::registerMbox(pHttpConnMbox, Mbox::eHttpConnector);
	zassert_not_null(pHttpConnMbox, "pHttpConnMbox = NULL");

	LteConnector* pLteConnector = new LteConnector("LTE");
	zassert_not_null(pLteConnector, "memory alloc failed");
	zassert_not_null(pLteConnector->pInMbox, "pInMbox alloc failed");
	zassert_not_null(pLteConnector->myTimer, "myTimer alloc failed");

	pLteConnector->listeners[0] = pMqttConnMbox;
	pLteConnector->listeners[1] = pMqttConnMbox;
	pLteConnector->listeners[2] = pMqttConnMbox;

	pLteConnector->registerListener(pHttpConnMbox);

    delete pMqttConnMbox;
    delete pHttpConnMbox;
	delete pLteConnector;
}

//Test receive msg eTimerExpired 
ZTEST(tests_net_lte_connector, test_net_lte_connector_rcv_msg_eTimerExpired)
{
	LteConnector* pLteConnector = new LteConnector("LTE");
	zassert_not_null(pLteConnector, "memory alloc failed");
	zassert_not_null(pLteConnector->pInMbox, "pInMbox alloc failed");
	zassert_not_null(pLteConnector->myTimer, "myTimer alloc failed");
    zassert_equal(pLteConnector->pInMbox->count(), 0, "pInMbox->count() != 0");
    zassert_equal(pLteConnector->pInMbox, Mbox::getMboxById(Mbox::eLteConnector),
                  "Error getting mailbox by id");

    pLteConnector->bConnectRetry = false;
    pLteConnector->bLteConnected = false;

    Mbox::sendMsg(msgType::eTimerExpired, Mbox::eLteConnector, Mbox::eLteConnector);

    zassert_equal(pLteConnector->pInMbox->count(), 1, "pInMbox->count() != 1");

    pLteConnector->task();  

    zassert_true(pLteConnector->bConnectRetry, "bConnectRetry != true");
    zassert_false(pLteConnector->bLteConnected, "bLteConnected != false");

	delete pLteConnector; 
}

//Test receive msg eTimerExpired 
ZTEST(tests_net_lte_connector, test_net_lte_connector_rcv_msg_eTimerExpired2)
{
	LteConnector* pLteConnector = new LteConnector("LTE");
	zassert_not_null(pLteConnector, "memory alloc failed");
	zassert_not_null(pLteConnector->pInMbox, "pInMbox alloc failed");
	zassert_not_null(pLteConnector->myTimer, "myTimer alloc failed");
    zassert_equal(pLteConnector->pInMbox->count(), 0, "pInMbox->count() != 0");
    zassert_equal(pLteConnector->pInMbox, Mbox::getMboxById(Mbox::eLteConnector),
                  "Error getting mailbox by id");

    pLteConnector->bConnectRetry = true;
    pLteConnector->bLteConnected = false;

    Mbox::sendMsg(msgType::eTimerExpired, Mbox::eLteConnector, Mbox::eLteConnector);

    zassert_equal(pLteConnector->pInMbox->count(), 1, "pInMbox->count() != 1");

    pLteConnector->task();  

    zassert_true(pLteConnector->bConnectRetry, "bConnectRetry != true");
    zassert_false(pLteConnector->bLteConnected, "bLteConnected != false");

	delete pLteConnector; 
}

//Test receive msg eTimerExpired 
ZTEST(tests_net_lte_connector, test_net_lte_connector_rcv_msg_eTimerExpired3)
{
	LteConnector* pLteConnector = new LteConnector("LTE");
	zassert_not_null(pLteConnector, "memory alloc failed");
	zassert_not_null(pLteConnector->pInMbox, "pInMbox alloc failed");
	zassert_not_null(pLteConnector->myTimer, "myTimer alloc failed");
    zassert_equal(pLteConnector->pInMbox->count(), 0, "pInMbox->count() != 0");
    zassert_equal(pLteConnector->pInMbox, Mbox::getMboxById(Mbox::eLteConnector),
                  "Error getting mailbox by id");

    pLteConnector->bConnectRetry = false;
    pLteConnector->bLteConnected = true;

    Mbox::sendMsg(msgType::eTimerExpired, Mbox::eLteConnector, Mbox::eLteConnector);

    zassert_equal(pLteConnector->pInMbox->count(), 1, "pInMbox->count() != 1");

    pLteConnector->task();  

    zassert_false(pLteConnector->bConnectRetry, "bConnectRetry != false");
    zassert_true(pLteConnector->bLteConnected, "bLteConnected != true");

	delete pLteConnector; 
}

//Test receive msg eLteConnected 
ZTEST(tests_net_lte_connector, test_net_lte_connector_rcv_msg_eLteConnected)
{
    PoolElem* pElem;

	Mbox* pMqttConnMbox = new Mbox("MqttConnectorMbox");
    Mbox::registerMbox(pMqttConnMbox, Mbox::eMqttConnector);
	zassert_not_null(pMqttConnMbox, "pMqttConnMbox = NULL");

	Mbox* pHttpConnMbox = new Mbox("HttpConnectorMbox");
    Mbox::registerMbox(pHttpConnMbox, Mbox::eHttpConnector);
	zassert_not_null(pHttpConnMbox, "pHttpConnMbox = NULL");

	LteConnector* pLteConnector = new LteConnector("LTE");
	zassert_not_null(pLteConnector, "memory alloc failed");
	zassert_not_null(pLteConnector->pInMbox, "pInMbox alloc failed");
	zassert_not_null(pLteConnector->myTimer, "myTimer alloc failed");
    zassert_equal(pLteConnector->pInMbox->count(), 0, "pInMbox->count() != 0");
    zassert_equal(pLteConnector->pInMbox, Mbox::getMboxById(Mbox::eLteConnector),
                  "Error getting mailbox by id");

    pLteConnector->bLteConnected = false;
	pLteConnector->listeners[0] = pMqttConnMbox;
	pLteConnector->listeners[1] = pHttpConnMbox;

    Mbox::sendMsg(msgType::eLteConnected, Mbox::eLteConnector, Mbox::eLteConnector);

    zassert_equal(pLteConnector->pInMbox->count(), 1, "pInMbox->count() != 1");

    pLteConnector->task();  

    zassert_equal(pLteConnector->pInMbox->count(), 0, "pInMbox->count() != 0");
    zassert_equal(pMqttConnMbox->count(), 1, "pMqttConnMbox->count() != 1");
    zassert_equal(pHttpConnMbox->count(), 1, "pHttpConnMbox->count() != 1");

    pElem = pMqttConnMbox->read();
    zassert_equal(pElem->mtype, msgType::eLteConnected, "Rcvd mtype != msgType::eLteConnected");
    zassert_equal(pElem->from, Mbox::eLteConnector, "Rcvd sender != Mbox::eLteConnector");
	pElem->free();

    pElem = pHttpConnMbox->read();
    zassert_equal(pElem->mtype, msgType::eLteConnected, "Rcvd mtype != msgType::eLteConnected");
    zassert_equal(pElem->from, Mbox::eLteConnector, "Rcvd sender != Mbox::eLteConnector");
    pElem->free();

    zassert_true(pLteConnector->bLteConnected, "bLteConnected != true");
	zassert_equal(pLteConnector->listeners[0], pMqttConnMbox, "listener != pMqttConnMbox");
	zassert_equal(pLteConnector->listeners[1], pHttpConnMbox, "listener != pHttpConnMbox");

    delete pMqttConnMbox;
    delete pHttpConnMbox;
	delete pLteConnector; 
}

//Test receive msg eLteDisconnected 
ZTEST(tests_net_lte_connector, test_net_lte_connector_rcv_msg_eLteDisconnected)
{
    PoolElem* pElem;

	Mbox* pMqttConnMbox = new Mbox("MqttConnectorMbox");
    Mbox::registerMbox(pMqttConnMbox, Mbox::eMqttConnector);
	zassert_not_null(pMqttConnMbox, "pMqttConnMbox = NULL");

	Mbox* pHttpConnMbox = new Mbox("HttpConnectorMbox");
    Mbox::registerMbox(pHttpConnMbox, Mbox::eHttpConnector);
	zassert_not_null(pHttpConnMbox, "pHttpConnMbox = NULL");

	LteConnector* pLteConnector = new LteConnector("LTE");
	zassert_not_null(pLteConnector, "memory alloc failed");
	zassert_not_null(pLteConnector->pInMbox, "pInMbox alloc failed");
	zassert_not_null(pLteConnector->myTimer, "myTimer alloc failed");
    zassert_equal(pLteConnector->pInMbox->count(), 0, "pInMbox->count() != 0");
    zassert_equal(pLteConnector->pInMbox, Mbox::getMboxById(Mbox::eLteConnector),
                  "Error getting mailbox by id");

    pLteConnector->bLteConnected = false;
	pLteConnector->listeners[0] = pMqttConnMbox;
	pLteConnector->listeners[1] = pHttpConnMbox;

    Mbox::sendMsg(msgType::eLteDisconnected, Mbox::eLteConnector, Mbox::eLteConnector);

    zassert_equal(pLteConnector->pInMbox->count(), 1, "pInMbox->count() != 1");

    pLteConnector->task();  

    zassert_equal(pLteConnector->pInMbox->count(), 0, "pInMbox->count() != 0");
    zassert_equal(pMqttConnMbox->count(), 1, "pMqttConnMbox->count() != 1");
    zassert_equal(pHttpConnMbox->count(), 1, "pHttpConnMbox->count() != 1");

    pElem = pMqttConnMbox->read();
    zassert_equal(pElem->mtype, msgType::eLteDisconnected, "Rcvd mtype != msgType::eLteDisconnected");
    zassert_equal(pElem->from, Mbox::eLteConnector, "Rcvd sender != Mbox::eLteConnector");
	pElem->free();

    pElem = pHttpConnMbox->read();
    zassert_equal(pElem->mtype, msgType::eLteDisconnected, "Rcvd mtype != msgType::eLteDisconnected");
    zassert_equal(pElem->from, Mbox::eLteConnector, "Rcvd sender != Mbox::eLteConnector");
    pElem->free();

    zassert_false(pLteConnector->bLteConnected, "bLteConnected != false");
	zassert_equal(pLteConnector->listeners[0], pMqttConnMbox, "listener != pMqttConnMbox");
	zassert_equal(pLteConnector->listeners[1], pHttpConnMbox, "listener != pHttpConnMbox");

    delete pMqttConnMbox;
    delete pHttpConnMbox;
	delete pLteConnector; 
}

//Test receive msg eLteSleepEnter 
ZTEST(tests_net_lte_connector, test_net_lte_connector_rcv_msg_eLteSleepEnter)
{
    PoolElem* pElem;

	Mbox* pMqttConnMbox = new Mbox("MqttConnectorMbox");
    Mbox::registerMbox(pMqttConnMbox, Mbox::eMqttConnector);
	zassert_not_null(pMqttConnMbox, "pMqttConnMbox = NULL");

	Mbox* pHttpConnMbox = new Mbox("HttpConnectorMbox");
    Mbox::registerMbox(pHttpConnMbox, Mbox::eHttpConnector);
	zassert_not_null(pHttpConnMbox, "pHttpConnMbox = NULL");

	LteConnector* pLteConnector = new LteConnector("LTE");
	zassert_not_null(pLteConnector, "memory alloc failed");
	zassert_not_null(pLteConnector->pInMbox, "pInMbox alloc failed");
	zassert_not_null(pLteConnector->myTimer, "myTimer alloc failed");
    zassert_equal(pLteConnector->pInMbox->count(), 0, "pInMbox->count() != 0");
    zassert_equal(pLteConnector->pInMbox, Mbox::getMboxById(Mbox::eLteConnector),
                  "Error getting mailbox by id");

    pLteConnector->bLteConnected = false;
	pLteConnector->listeners[0] = pMqttConnMbox;
	pLteConnector->listeners[1] = pHttpConnMbox;

    Mbox::sendMsg(msgType::eLteSleepEnter, Mbox::eLteConnector, Mbox::eLteConnector);

    zassert_equal(pLteConnector->pInMbox->count(), 1, "pInMbox->count() != 1");

    pLteConnector->task();  

    zassert_equal(pLteConnector->pInMbox->count(), 0, "pInMbox->count() != 0");
    zassert_equal(pMqttConnMbox->count(), 1, "pMqttConnMbox->count() != 1");
    zassert_equal(pHttpConnMbox->count(), 1, "pHttpConnMbox->count() != 1");

    pElem = pMqttConnMbox->read();
    zassert_equal(pElem->mtype, msgType::eLteSleepEnter, "Rcvd mtype != msgType::eLteSleepEnter");
    zassert_equal(pElem->from, Mbox::eLteConnector, "Rcvd sender != Mbox::eLteConnector");
	pElem->free();

    pElem = pHttpConnMbox->read();
    zassert_equal(pElem->mtype, msgType::eLteSleepEnter, "Rcvd mtype != msgType::eLteSleepEnter");
    zassert_equal(pElem->from, Mbox::eLteConnector, "Rcvd sender != Mbox::eLteConnector");
    pElem->free();

	zassert_equal(pLteConnector->listeners[0], pMqttConnMbox, "listener != pMqttConnMbox");
	zassert_equal(pLteConnector->listeners[1], pHttpConnMbox, "listener != pHttpConnMbox");

    delete pMqttConnMbox;
    delete pHttpConnMbox;
	delete pLteConnector; 
}

//Test receive msg eLteSleepExit 
ZTEST(tests_net_lte_connector, test_net_lte_connector_rcv_msg_eLteSleepExit)
{
    PoolElem* pElem;

	Mbox* pMqttConnMbox = new Mbox("MqttConnectorMbox");
    Mbox::registerMbox(pMqttConnMbox, Mbox::eMqttConnector);
	zassert_not_null(pMqttConnMbox, "pMqttConnMbox = NULL");

	Mbox* pHttpConnMbox = new Mbox("HttpConnectorMbox");
    Mbox::registerMbox(pHttpConnMbox, Mbox::eHttpConnector);
	zassert_not_null(pHttpConnMbox, "pHttpConnMbox = NULL");

	LteConnector* pLteConnector = new LteConnector("LTE");
	zassert_not_null(pLteConnector, "memory alloc failed");
	zassert_not_null(pLteConnector->pInMbox, "pInMbox alloc failed");
	zassert_not_null(pLteConnector->myTimer, "myTimer alloc failed");
    zassert_equal(pLteConnector->pInMbox->count(), 0, "pInMbox->count() != 0");
    zassert_equal(pLteConnector->pInMbox, Mbox::getMboxById(Mbox::eLteConnector),
                  "Error getting mailbox by id");

    pLteConnector->bLteConnected = false;
	pLteConnector->listeners[0] = pMqttConnMbox;
	pLteConnector->listeners[1] = pHttpConnMbox;

    Mbox::sendMsg(msgType::eLteSleepExit, Mbox::eLteConnector, Mbox::eLteConnector);

    zassert_equal(pLteConnector->pInMbox->count(), 1, "pInMbox->count() != 1");

    pLteConnector->task();  

    zassert_equal(pLteConnector->pInMbox->count(), 0, "pInMbox->count() != 0");
    zassert_equal(pMqttConnMbox->count(), 1, "pMqttConnMbox->count() != 1");
    zassert_equal(pHttpConnMbox->count(), 1, "pHttpConnMbox->count() != 1");

    pElem = pMqttConnMbox->read();
    zassert_equal(pElem->mtype, msgType::eLteSleepExit, "Rcvd mtype != msgType::eLteSleepExit");
    zassert_equal(pElem->from, Mbox::eLteConnector, "Rcvd sender != Mbox::eLteConnector");
	pElem->free();

    pElem = pHttpConnMbox->read();
    zassert_equal(pElem->mtype, msgType::eLteSleepExit, "Rcvd mtype != msgType::eLteSleepExit");
    zassert_equal(pElem->from, Mbox::eLteConnector, "Rcvd sender != Mbox::eLteConnector");
    pElem->free();

	zassert_equal(pLteConnector->listeners[0], pMqttConnMbox, "listener != pMqttConnMbox");
	zassert_equal(pLteConnector->listeners[1], pHttpConnMbox, "listener != pHttpConnMbox");

    delete pMqttConnMbox;
    delete pHttpConnMbox;
	delete pLteConnector; 
}

//Test receive msg eIp6Up 
ZTEST(tests_net_lte_connector, test_net_lte_connector_rcv_msg_eIp6Up)
{
	LteConnector* pLteConnector = new LteConnector("LTE");
	zassert_not_null(pLteConnector, "memory alloc failed");
	zassert_not_null(pLteConnector->pInMbox, "pInMbox alloc failed");
	zassert_not_null(pLteConnector->myTimer, "myTimer alloc failed");
    zassert_equal(pLteConnector->pInMbox->count(), 0, "pInMbox->count() != 0");
    zassert_equal(pLteConnector->pInMbox, Mbox::getMboxById(Mbox::eLteConnector),
                  "Error getting mailbox by id");

    Mbox::sendMsg(msgType::eIp6Up, Mbox::eLteConnector, Mbox::eLteConnector);

    zassert_equal(pLteConnector->pInMbox->count(), 1, "pInMbox->count() != 1");

    pLteConnector->task();  

    zassert_equal(pLteConnector->pInMbox->count(), 0, "pInMbox->count() != 0");

	delete pLteConnector; 
}

//Test receive msg eIp6Down 
ZTEST(tests_net_lte_connector, test_net_lte_connector_rcv_msg_eIp6Down)
{
	LteConnector* pLteConnector = new LteConnector("LTE");
	zassert_not_null(pLteConnector, "memory alloc failed");
	zassert_not_null(pLteConnector->pInMbox, "pInMbox alloc failed");
	zassert_not_null(pLteConnector->myTimer, "myTimer alloc failed");
    zassert_equal(pLteConnector->pInMbox->count(), 0, "pInMbox->count() != 0");
    zassert_equal(pLteConnector->pInMbox, Mbox::getMboxById(Mbox::eLteConnector),
                  "Error getting mailbox by id");

    Mbox::sendMsg(msgType::eIp6Down, Mbox::eLteConnector, Mbox::eLteConnector);

    zassert_equal(pLteConnector->pInMbox->count(), 1, "pInMbox->count() != 1");

    pLteConnector->task();  

    zassert_equal(pLteConnector->pInMbox->count(), 0, "pInMbox->count() != 0");

	delete pLteConnector; 
}

//Test receive msg eResume 
ZTEST(tests_net_lte_connector, test_net_lte_connector_rcv_msg_eResume)
{
	LteConnector* pLteConnector = new LteConnector("LTE");
	zassert_not_null(pLteConnector, "memory alloc failed");
	zassert_not_null(pLteConnector->pInMbox, "pInMbox alloc failed");
	zassert_not_null(pLteConnector->myTimer, "myTimer alloc failed");
    zassert_equal(pLteConnector->pInMbox->count(), 0, "pInMbox->count() != 0");
    zassert_equal(pLteConnector->pInMbox, Mbox::getMboxById(Mbox::eLteConnector),
                  "Error getting mailbox by id");

    Mbox::sendMsg(msgType::eResume, Mbox::eLteConnector, Mbox::eLteConnector);

    zassert_equal(pLteConnector->pInMbox->count(), 1, "pInMbox->count() != 1");

    pLteConnector->task();  

    zassert_equal(pLteConnector->pInMbox->count(), 0, "pInMbox->count() != 0");

	delete pLteConnector; 
}

//Test receive msg eSuspend 
ZTEST(tests_net_lte_connector, test_net_lte_connector_rcv_msg_eSuspend)
{
	LteConnector* pLteConnector = new LteConnector("LTE");
	zassert_not_null(pLteConnector, "memory alloc failed");
	zassert_not_null(pLteConnector->pInMbox, "pInMbox alloc failed");
	zassert_not_null(pLteConnector->myTimer, "myTimer alloc failed");
    zassert_equal(pLteConnector->pInMbox->count(), 0, "pInMbox->count() != 0");
    zassert_equal(pLteConnector->pInMbox, Mbox::getMboxById(Mbox::eLteConnector),
                  "Error getting mailbox by id");

    Mbox::sendMsg(msgType::eSuspend, Mbox::eLteConnector, Mbox::eLteConnector);

    zassert_equal(pLteConnector->pInMbox->count(), 1, "pInMbox->count() != 1");

    pLteConnector->task();  

    zassert_equal(pLteConnector->pInMbox->count(), 0, "pInMbox->count() != 0");

	delete pLteConnector; 
}

//Test receive unexpected msg 
ZTEST(tests_net_lte_connector, test_net_lte_connector_rcv_msg_unexpected)
{
	LteConnector* pLteConnector = new LteConnector("LTE");
	zassert_not_null(pLteConnector, "memory alloc failed");
	zassert_not_null(pLteConnector->pInMbox, "pInMbox alloc failed");
	zassert_not_null(pLteConnector->myTimer, "myTimer alloc failed");
    zassert_equal(pLteConnector->pInMbox->count(), 0, "pInMbox->count() != 0");
    zassert_equal(pLteConnector->pInMbox, Mbox::getMboxById(Mbox::eLteConnector),
                  "Error getting mailbox by id");

    Mbox::sendMsg(msgType::eResetCause, Mbox::eLteConnector, Mbox::eLteConnector);

    zassert_equal(pLteConnector->pInMbox->count(), 1, "pInMbox->count() != 1");

    pLteConnector->task();  

    zassert_equal(pLteConnector->pInMbox->count(), 0, "pInMbox->count() != 0");
	delete pLteConnector; 
}

//Test receive no msg 
ZTEST(tests_net_lte_connector, test_net_lte_connector_rcv_msg_none)
{
	LteConnector* pLteConnector = new LteConnector("LTE");
	zassert_not_null(pLteConnector, "memory alloc failed");
	zassert_not_null(pLteConnector->pInMbox, "pInMbox alloc failed");
	zassert_not_null(pLteConnector->myTimer, "myTimer alloc failed");
    zassert_equal(pLteConnector->pInMbox->count(), 0, "pInMbox->count() != 0");
    zassert_equal(pLteConnector->pInMbox, Mbox::getMboxById(Mbox::eLteConnector),
                  "Error getting mailbox by id");

    pLteConnector->pInMbox->write(NULL);
    
    pLteConnector->task();  

    zassert_equal(pLteConnector->pInMbox->count(), 0, "pInMbox->count() != 0");
	delete pLteConnector; 
}
