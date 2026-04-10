#include <zephyr/ztest.h>
#include <vector>
#include <algorithm>

#define private public
#include "net/http_connector.h"
#include "data/adxl_store.h"
#include "data/rms_store.h"

#include "stubs.h"

ZTEST_SUITE(tests_net_http_connector, NULL, NULL, NULL, NULL, NULL);

// test constructor and destructor
ZTEST(tests_net_http_connector, test_http_connector_constructor)
{
	HttpConnector* pHttpConnector = new HttpConnector("http");
	zassert_not_null(pHttpConnector, "memory alloc failed");
	zassert_not_null(pHttpConnector->pInMbox, "pInMbox alloc failed");
	zassert_not_null(pHttpConnector->delayTimer, "delayTimer alloc failed");
	zassert_not_null(pHttpConnector->oneDayTimer, "oneDayTimer alloc failed");
	zassert_false(pHttpConnector->lte_connected, "lte_connected != false");
	zassert_false(pHttpConnector->http_connected, "http_connected != false");
	zassert_false(pHttpConnector->bRawTransferEnabled, "bRawTransferEnabled != false");
	zassert_false(pHttpConnector->bDailyRawSendCntExceeded, "bDailyRawSendCntExceeded != false");
	zassert_equal(pHttpConnector->dailyRawSendCnt, 0, "dailyRawSendCnt != 0");
	zassert_equal(pHttpConnector->zMaxRms, 0.0, "zMaxRms != 0");
	
	delete pHttpConnector;
}

// MISSING for 100% branch coverage operator new does not deliver NULL
// test constructor and destructor
// here code lines 
// pInMbox = new Mbox(name); 
// delayTimer  = new Timer(HttpConnector::delayTimer_cb, this, 5);
// oneDayTimer = new Timer(HttpConnector::oneDayTimer_cb, this, ONE_DAY_IN_SEC, ONE_DAY_IN_SEC);
// static PoolElem e1 = {eTimerExpired, Mbox::MboxId::eHttpConnector, 'm', 'q'};
// static PoolElem e2 = {eOneDayTimerExpired, Mbox::MboxId::eHttpConnector, 'm', 'q'};
// else if (dailyRawSendCnt >= maxRawPerDay) -> unreachable code in else case
// are not covered completly.



// test function configure
ZTEST(tests_net_http_connector, test_http_connector_configure)
{
	HttpConnector* pHttpConnector = new HttpConnector("http");
	zassert_not_null(pHttpConnector, "memory alloc failed");
	zassert_not_null(pHttpConnector->pInMbox, "pInMbox alloc failed");
	zassert_not_null(pHttpConnector->delayTimer, "delayTimer alloc failed");
	zassert_not_null(pHttpConnector->oneDayTimer, "oneDayTimer alloc failed");

	gbRetVal = false;
	pHttpConnector->configure();

	zassert_false(pHttpConnector->bDispModeOff, "bDispModeOff != false");
	zassert_false(pHttpConnector->bRawTransferEnabled, "bRawTransferEnabled != false");
	zassert_false(pHttpConnector->bDispTransferEnabled, "bDispTransferEnabled != false");
	zassert_equal(pHttpConnector->rms_z_Thres, 27, "rms_z_Thres != 27");
	zassert_equal(pHttpConnector->maxRawPerDay, 7, "maxRawPerDay != 7");
	
	delete pHttpConnector;
}

// test function configure
ZTEST(tests_net_http_connector, test_http_connector_configure2)
{
	HttpConnector* pHttpConnector = new HttpConnector("http");
	zassert_not_null(pHttpConnector, "memory alloc failed");
	zassert_not_null(pHttpConnector->pInMbox, "pInMbox alloc failed");
	zassert_not_null(pHttpConnector->delayTimer, "delayTimer alloc failed");
	zassert_not_null(pHttpConnector->oneDayTimer, "oneDayTimer alloc failed");

	gbRetVal = true;
	pHttpConnector->configure();

	zassert_true(pHttpConnector->bDispModeOff, "bDispModeOff != true");
	zassert_true(pHttpConnector->bRawTransferEnabled, "bRawTransferEnabled != true");
	zassert_true(pHttpConnector->bDispTransferEnabled, "bDispTransferEnabled != true");
	zassert_equal(pHttpConnector->rms_z_Thres, 25, "rms_z_Thres != 25");
	zassert_equal(pHttpConnector->maxRawPerDay, 5, "maxRawPerDay != 5");
	
	delete pHttpConnector;
}

// test function blocking_connect
ZTEST(tests_net_http_connector, test_http_connector_blocking_connect)
{
	HttpConnector* pHttpConnector = new HttpConnector("http");
	zassert_not_null(pHttpConnector, "memory alloc failed");
	zassert_not_null(pHttpConnector->pInMbox, "pInMbox alloc failed");
	zassert_not_null(pHttpConnector->delayTimer, "delayTimer alloc failed");
	zassert_not_null(pHttpConnector->oneDayTimer, "oneDayTimer alloc failed");

	gIntVal = 1;
	pHttpConnector->blocking_connect(0, NULL, 1);

	delete pHttpConnector;
}

// test function blocking_connect
ZTEST(tests_net_http_connector, test_http_connector_blocking_connect2)
{
	HttpConnector* pHttpConnector = new HttpConnector("http");
	zassert_not_null(pHttpConnector, "memory alloc failed");
	zassert_not_null(pHttpConnector->pInMbox, "pInMbox alloc failed");
	zassert_not_null(pHttpConnector->delayTimer, "delayTimer alloc failed");
	zassert_not_null(pHttpConnector->oneDayTimer, "oneDayTimer alloc failed");

	gIntVal = 2;
	pHttpConnector->blocking_connect(0, NULL, 1);

	delete pHttpConnector;
}

// test function blocking_connect
ZTEST(tests_net_http_connector, test_http_connector_blocking_connect3)
{
	HttpConnector* pHttpConnector = new HttpConnector("http");
	zassert_not_null(pHttpConnector, "memory alloc failed");
	zassert_not_null(pHttpConnector->pInMbox, "pInMbox alloc failed");
	zassert_not_null(pHttpConnector->delayTimer, "delayTimer alloc failed");
	zassert_not_null(pHttpConnector->oneDayTimer, "oneDayTimer alloc failed");

	gIntVal = 3;
	pHttpConnector->blocking_connect(0, NULL, 1);

	delete pHttpConnector;
}

// test function blocking_send
ZTEST(tests_net_http_connector, test_http_connector_blocking_send)
{
	HttpConnector* pHttpConnector = new HttpConnector("http");
	zassert_not_null(pHttpConnector, "memory alloc failed");
	zassert_not_null(pHttpConnector->pInMbox, "pInMbox alloc failed");
	zassert_not_null(pHttpConnector->delayTimer, "delayTimer alloc failed");
	zassert_not_null(pHttpConnector->oneDayTimer, "oneDayTimer alloc failed");

	gIntVal = 1;
	pHttpConnector->blocking_send(0, NULL, 1, 1);

	delete pHttpConnector;
}

// test function blocking_send
ZTEST(tests_net_http_connector, test_http_connector_blocking_send2)
{
	HttpConnector* pHttpConnector = new HttpConnector("http");
	zassert_not_null(pHttpConnector, "memory alloc failed");
	zassert_not_null(pHttpConnector->pInMbox, "pInMbox alloc failed");
	zassert_not_null(pHttpConnector->delayTimer, "delayTimer alloc failed");
	zassert_not_null(pHttpConnector->oneDayTimer, "oneDayTimer alloc failed");

	gIntVal = 2;
	pHttpConnector->blocking_send(0, NULL, 1, 1);

	delete pHttpConnector;
}

// test function blocking_send
ZTEST(tests_net_http_connector, test_http_connector_blocking_send3)
{
	HttpConnector* pHttpConnector = new HttpConnector("http");
	zassert_not_null(pHttpConnector, "memory alloc failed");
	zassert_not_null(pHttpConnector->pInMbox, "pInMbox alloc failed");
	zassert_not_null(pHttpConnector->delayTimer, "delayTimer alloc failed");
	zassert_not_null(pHttpConnector->oneDayTimer, "oneDayTimer alloc failed");

	gIntVal = 3;
	pHttpConnector->blocking_send(0, NULL, 1, 1);

	delete pHttpConnector;
}

// test function blocking_recv
ZTEST(tests_net_http_connector, test_http_connector_blocking_recv)
{
	HttpConnector* pHttpConnector = new HttpConnector("http");
	zassert_not_null(pHttpConnector, "memory alloc failed");
	zassert_not_null(pHttpConnector->pInMbox, "pInMbox alloc failed");
	zassert_not_null(pHttpConnector->delayTimer, "delayTimer alloc failed");
	zassert_not_null(pHttpConnector->oneDayTimer, "oneDayTimer alloc failed");

	gIntVal = 1;
	pHttpConnector->blocking_recv(0, NULL, 1, 1);

	delete pHttpConnector;
}

// test function blocking_recv
ZTEST(tests_net_http_connector, test_http_connector_blocking_recv2)
{
	HttpConnector* pHttpConnector = new HttpConnector("http");
	zassert_not_null(pHttpConnector, "memory alloc failed");
	zassert_not_null(pHttpConnector->pInMbox, "pInMbox alloc failed");
	zassert_not_null(pHttpConnector->delayTimer, "delayTimer alloc failed");
	zassert_not_null(pHttpConnector->oneDayTimer, "oneDayTimer alloc failed");

	gIntVal = 2;
	pHttpConnector->blocking_recv(0, NULL, 1, 1);

	delete pHttpConnector;
}

// test function blocking_recv
ZTEST(tests_net_http_connector, test_http_connector_blocking_recv3)
{
	HttpConnector* pHttpConnector = new HttpConnector("http");
	zassert_not_null(pHttpConnector, "memory alloc failed");
	zassert_not_null(pHttpConnector->pInMbox, "pInMbox alloc failed");
	zassert_not_null(pHttpConnector->delayTimer, "delayTimer alloc failed");
	zassert_not_null(pHttpConnector->oneDayTimer, "oneDayTimer alloc failed");

	gIntVal = 3;
	pHttpConnector->blocking_recv(0, NULL, 1, 1);

	delete pHttpConnector;
}

// test function tls_setup
ZTEST(tests_net_http_connector, test_http_connector_tls_setup)
{
	HttpConnector* pHttpConnector = new HttpConnector("http");
	zassert_not_null(pHttpConnector, "memory alloc failed");
	zassert_not_null(pHttpConnector->pInMbox, "pInMbox alloc failed");
	zassert_not_null(pHttpConnector->delayTimer, "delayTimer alloc failed");
	zassert_not_null(pHttpConnector->oneDayTimer, "oneDayTimer alloc failed");

	gIntVal = 1;
	pHttpConnector->tls_setup(1);

	delete pHttpConnector;
}

// test function tls_setup
ZTEST(tests_net_http_connector, test_http_connector_tls_setup2)
{
	HttpConnector* pHttpConnector = new HttpConnector("http");
	zassert_not_null(pHttpConnector, "memory alloc failed");
	zassert_not_null(pHttpConnector->pInMbox, "pInMbox alloc failed");
	zassert_not_null(pHttpConnector->delayTimer, "delayTimer alloc failed");
	zassert_not_null(pHttpConnector->oneDayTimer, "oneDayTimer alloc failed");

	gIntVal = 2;
	pHttpConnector->tls_setup(1);

	delete pHttpConnector;
}

// test function tls_setup
ZTEST(tests_net_http_connector, test_http_connector_tls_setup3)
{
	HttpConnector* pHttpConnector = new HttpConnector("http");
	zassert_not_null(pHttpConnector, "memory alloc failed");
	zassert_not_null(pHttpConnector->pInMbox, "pInMbox alloc failed");
	zassert_not_null(pHttpConnector->delayTimer, "delayTimer alloc failed");
	zassert_not_null(pHttpConnector->oneDayTimer, "oneDayTimer alloc failed");

	gIntVal = 3;
	pHttpConnector->tls_setup(1);

	delete pHttpConnector;
}

// test function tls_setup
ZTEST(tests_net_http_connector, test_http_connector_tls_setup4)
{
	HttpConnector* pHttpConnector = new HttpConnector("http");
	zassert_not_null(pHttpConnector, "memory alloc failed");
	zassert_not_null(pHttpConnector->pInMbox, "pInMbox alloc failed");
	zassert_not_null(pHttpConnector->delayTimer, "delayTimer alloc failed");
	zassert_not_null(pHttpConnector->oneDayTimer, "oneDayTimer alloc failed");

	gIntVal = 4;
	pHttpConnector->tls_setup(1);

	delete pHttpConnector;
}

// test function http_connect
ZTEST(tests_net_http_connector, test_http_connector_http_connect)
{
	HttpConnector* pHttpConnector = new HttpConnector("http");
	zassert_not_null(pHttpConnector, "memory alloc failed");
	zassert_not_null(pHttpConnector->pInMbox, "pInMbox alloc failed");
	zassert_not_null(pHttpConnector->delayTimer, "delayTimer alloc failed");
	zassert_not_null(pHttpConnector->oneDayTimer, "oneDayTimer alloc failed");

	gIntVal = 0;
	pHttpConnector->http_connect();

	delete pHttpConnector;	
}

// test function http_connect
ZTEST(tests_net_http_connector, test_http_connector_http_connect2)
{
	HttpConnector* pHttpConnector = new HttpConnector("http");
	zassert_not_null(pHttpConnector, "memory alloc failed");
	zassert_not_null(pHttpConnector->pInMbox, "pInMbox alloc failed");
	zassert_not_null(pHttpConnector->delayTimer, "delayTimer alloc failed");
	zassert_not_null(pHttpConnector->oneDayTimer, "oneDayTimer alloc failed");

	gIntVal = 1;
	pHttpConnector->http_connect();

	delete pHttpConnector;	
}

// test function http_connect
ZTEST(tests_net_http_connector, test_http_connector_http_connect3)
{
	HttpConnector* pHttpConnector = new HttpConnector("http");
	zassert_not_null(pHttpConnector, "memory alloc failed");
	zassert_not_null(pHttpConnector->pInMbox, "pInMbox alloc failed");
	zassert_not_null(pHttpConnector->delayTimer, "delayTimer alloc failed");
	zassert_not_null(pHttpConnector->oneDayTimer, "oneDayTimer alloc failed");

	gIntVal = 3;
	pHttpConnector->http_connect();

	delete pHttpConnector;	
}

// test function http_connect
ZTEST(tests_net_http_connector, test_http_connector_http_connect4)
{
	HttpConnector* pHttpConnector = new HttpConnector("http");
	zassert_not_null(pHttpConnector, "memory alloc failed");
	zassert_not_null(pHttpConnector->pInMbox, "pInMbox alloc failed");
	zassert_not_null(pHttpConnector->delayTimer, "delayTimer alloc failed");
	zassert_not_null(pHttpConnector->oneDayTimer, "oneDayTimer alloc failed");

	gIntVal = 4;
	pHttpConnector->http_connect();

	delete pHttpConnector;	
}

// test function SendHttpPostHeader, BuildDataHeader
ZTEST(tests_net_http_connector, test_http_connector_tls_SendHttpPostHeader)
{
	HttpConnector* pHttpConnector = new HttpConnector("http");
	zassert_not_null(pHttpConnector, "memory alloc failed");
	zassert_not_null(pHttpConnector->pInMbox, "pInMbox alloc failed");
	zassert_not_null(pHttpConnector->delayTimer, "delayTimer alloc failed");
	zassert_not_null(pHttpConnector->oneDayTimer, "oneDayTimer alloc failed");

	pHttpConnector->SendHttpPostHeader(eDATA_TYPE_DISP);
	pHttpConnector->BuildDataHeader(eDATA_TYPE_DISP);

	delete pHttpConnector;
}

// test function SendHttpPostHeader, BuildDataHeader
ZTEST(tests_net_http_connector, test_http_connector_tls_SendHttpPostHeader2)
{
	HttpConnector* pHttpConnector = new HttpConnector("http");
	zassert_not_null(pHttpConnector, "memory alloc failed");
	zassert_not_null(pHttpConnector->pInMbox, "pInMbox alloc failed");
	zassert_not_null(pHttpConnector->delayTimer, "delayTimer alloc failed");
	zassert_not_null(pHttpConnector->oneDayTimer, "oneDayTimer alloc failed");

	pHttpConnector->SendHttpPostHeader(eDATA_TYPE_NONE);
	pHttpConnector->BuildDataHeader(eDATA_TYPE_NONE);

	delete pHttpConnector;
}

// test function reqData
ZTEST(tests_net_http_connector, test_http_connector_reqData)
{
    PoolElem* pElem;

	Mbox* pAdxlStoreMbox = new Mbox("AdxlStoreMbox");
    Mbox::registerMbox(pAdxlStoreMbox, Mbox::eAdxlStore);
	zassert_not_null(pAdxlStoreMbox, "pAdxlStoreMbox = NULL");

	HttpConnector* pHttpConnector = new HttpConnector("http");
	zassert_not_null(pHttpConnector, "memory alloc failed");
	zassert_not_null(pHttpConnector->pInMbox, "pInMbox alloc failed");
	zassert_not_null(pHttpConnector->delayTimer, "delayTimer alloc failed");
	zassert_not_null(pHttpConnector->oneDayTimer, "oneDayTimer alloc failed");

	pHttpConnector->totalNumEntries = 110;
	pHttpConnector->rcvdEntries = 100;
	pHttpConnector->reqData(msgType::eRawData);

    zassert_equal(pHttpConnector->requestedEntries, 10, "requestedEntries != 10");

	pElem = pAdxlStoreMbox->read(); 
    zassert_equal(pElem->mtype, msgType::eRawData, "Rcvd mtype != msgType::eRawData");
    zassert_equal(pElem->from, Mbox::eHttpConnector, "Rcvd sender != Mbox::eHttpConnector");

	pElem->free();
	delete pAdxlStoreMbox;
	delete pHttpConnector;
}


// ...

// test function delayTimer_cb and timer1_expired
ZTEST(tests_net_http_connector, test_http_connector_delayTimer_cb)
{
    PoolElem* pElem;

	HttpConnector* pHttpConnector = new HttpConnector("http");
	zassert_not_null(pHttpConnector, "memory alloc failed");
	zassert_not_null(pHttpConnector->pInMbox, "pInMbox alloc failed");
	zassert_not_null(pHttpConnector->delayTimer, "delayTimer alloc failed");
	zassert_not_null(pHttpConnector->oneDayTimer, "oneDayTimer alloc failed");
    zassert_equal(pHttpConnector->pInMbox->count(), 0, "pInMbox->count() != 0");
    zassert_equal(pHttpConnector->pInMbox, Mbox::getMboxById(Mbox::eHttpConnector),
                  "Error getting mailbox by id");

    pHttpConnector->delayTimer_cb(reinterpret_cast<k_timer*>(pHttpConnector));

    zassert_equal(pHttpConnector->pInMbox->count(), 1, "pInMbox->count() != 1");
    pElem = pHttpConnector->pInMbox->read();
    zassert_equal(pElem->mtype, msgType::eTimerExpired, "Rcvd mtype != msgType::eTimerExpired");

    delete pHttpConnector;
}    

// test function oneDayTimer_cb and oneDayTimer_expired
ZTEST(tests_net_http_connector, test_http_connector_oneDayTimer_cb)
{
    PoolElem* pElem;

	HttpConnector* pHttpConnector = new HttpConnector("http");
	zassert_not_null(pHttpConnector, "memory alloc failed");
	zassert_not_null(pHttpConnector->pInMbox, "pInMbox alloc failed");
	zassert_not_null(pHttpConnector->delayTimer, "delayTimer alloc failed");
	zassert_not_null(pHttpConnector->oneDayTimer, "oneDayTimer alloc failed");
    zassert_equal(pHttpConnector->pInMbox->count(), 0, "pInMbox->count() != 0");
    zassert_equal(pHttpConnector->pInMbox, Mbox::getMboxById(Mbox::eHttpConnector),
                  "Error getting mailbox by id");

    pHttpConnector->oneDayTimer_cb(reinterpret_cast<k_timer*>(pHttpConnector));

    zassert_equal(pHttpConnector->pInMbox->count(), 1, "pInMbox->count() != 1");
    pElem = pHttpConnector->pInMbox->read();
    zassert_equal(pElem->mtype, msgType::eOneDayTimerExpired, "Rcvd mtype != msgType::eOneDayTimerExpired");

    delete pHttpConnector;
}    

//Test receive msg eTimerExpired 
ZTEST(tests_net_http_connector, test_http_connector_rcv_msg_eTimerExpired)
{
    PoolElem* pElem;

	Mbox* pAdxlStoreMbox = new Mbox("AdxlStoreMbox");
    Mbox::registerMbox(pAdxlStoreMbox, Mbox::eAdxlStore);
	zassert_not_null(pAdxlStoreMbox, "pAdxlStoreMbox = NULL");

	HttpConnector* pHttpConnector = new HttpConnector("http");
	zassert_not_null(pHttpConnector, "memory alloc failed");
	zassert_not_null(pHttpConnector->pInMbox, "pInMbox alloc failed");
	zassert_not_null(pHttpConnector->delayTimer, "delayTimer alloc failed");
	zassert_not_null(pHttpConnector->oneDayTimer, "oneDayTimer alloc failed");
    zassert_equal(pHttpConnector->pInMbox->count(), 0, "pInMbox->count() != 0");
    zassert_equal(pHttpConnector->pInMbox, Mbox::getMboxById(Mbox::eHttpConnector),
                  "Error getting mailbox by id");

    Mbox::sendMsg(msgType::eTimerExpired, Mbox::eHttpConnector, Mbox::eHttpConnector);

    zassert_equal(pHttpConnector->pInMbox->count(), 1, "pInMbox->count() != 1");

    pHttpConnector->task(); 

	pElem = pAdxlStoreMbox->read(); 
    zassert_equal(pElem->mtype, msgType::eAdxlIdleReq, "Rcvd mtype != msgType::eAdxlIdleReq");
    zassert_equal(pElem->from, Mbox::eHttpConnector, "Rcvd sender != Mbox::eHttpConnector");

	pElem->free();
	delete pAdxlStoreMbox;
	delete pHttpConnector; 
}

//Test receive msg eOneDayTimerExpired 
ZTEST(tests_net_http_connector, test_http_connector_rcv_msg_eOneDayTimerExpired)
{
	HttpConnector* pHttpConnector = new HttpConnector("http");
	zassert_not_null(pHttpConnector, "memory alloc failed");
	zassert_not_null(pHttpConnector->pInMbox, "pInMbox alloc failed");
	zassert_not_null(pHttpConnector->delayTimer, "delayTimer alloc failed");
	zassert_not_null(pHttpConnector->oneDayTimer, "oneDayTimer alloc failed");
    zassert_equal(pHttpConnector->pInMbox->count(), 0, "pInMbox->count() != 0");
    zassert_equal(pHttpConnector->pInMbox, Mbox::getMboxById(Mbox::eHttpConnector),
                  "Error getting mailbox by id");

    Mbox::sendMsg(msgType::eOneDayTimerExpired, Mbox::eHttpConnector, Mbox::eHttpConnector);

    zassert_equal(pHttpConnector->pInMbox->count(), 1, "pInMbox->count() != 1");

    pHttpConnector->dailyRawSendCnt = 17;
    pHttpConnector->bDailyRawSendCntExceeded = true;

    pHttpConnector->task(); 
	zassert_equal(pHttpConnector->dailyRawSendCnt, 0, "dailyRawSendCnt != 0");
	zassert_false(pHttpConnector->bDailyRawSendCntExceeded, "bDailyRawSendCntExceeded != false");

	delete pHttpConnector; 
}

//Test receive msg eLteSleepEnter 
ZTEST(tests_net_http_connector, test_http_connector_rcv_msg_eLteSleepEnter)
{
	HttpConnector* pHttpConnector = new HttpConnector("http");
	zassert_not_null(pHttpConnector, "memory alloc failed");
	zassert_not_null(pHttpConnector->pInMbox, "pInMbox alloc failed");
	zassert_not_null(pHttpConnector->delayTimer, "delayTimer alloc failed");
	zassert_not_null(pHttpConnector->oneDayTimer, "oneDayTimer alloc failed");
    zassert_equal(pHttpConnector->pInMbox->count(), 0, "pInMbox->count() != 0");
    zassert_equal(pHttpConnector->pInMbox, Mbox::getMboxById(Mbox::eHttpConnector),
                  "Error getting mailbox by id");

    Mbox::sendMsg(msgType::eLteSleepEnter, Mbox::eHttpConnector, Mbox::eHttpConnector);

    zassert_equal(pHttpConnector->pInMbox->count(), 1, "pInMbox->count() != 1");

    pHttpConnector->task(); 
    zassert_equal(pHttpConnector->pInMbox->count(), 0, "pInMbox->count() != 0");

	delete pHttpConnector; 
}

//Test receive msg eLteSleepExit 
ZTEST(tests_net_http_connector, test_http_connector_rcv_msg_eLteSleepExit)
{
	HttpConnector* pHttpConnector = new HttpConnector("http");
	zassert_not_null(pHttpConnector, "memory alloc failed");
	zassert_not_null(pHttpConnector->pInMbox, "pInMbox alloc failed");
	zassert_not_null(pHttpConnector->delayTimer, "delayTimer alloc failed");
	zassert_not_null(pHttpConnector->oneDayTimer, "oneDayTimer alloc failed");
    zassert_equal(pHttpConnector->pInMbox->count(), 0, "pInMbox->count() != 0");
    zassert_equal(pHttpConnector->pInMbox, Mbox::getMboxById(Mbox::eHttpConnector),
                  "Error getting mailbox by id");

    Mbox::sendMsg(msgType::eLteSleepExit, Mbox::eHttpConnector, Mbox::eHttpConnector);

    zassert_equal(pHttpConnector->pInMbox->count(), 1, "pInMbox->count() != 1");

    pHttpConnector->task(); 
    zassert_equal(pHttpConnector->pInMbox->count(), 0, "pInMbox->count() != 0");

	delete pHttpConnector; 
}

//Test receive msg eLteConnected 
ZTEST(tests_net_http_connector, test_http_connector_rcv_msg_eLteConnected)
{
    PoolElem* pElem;

	Mbox* pAdxlMbox = new Mbox("AdxlMbox");
    Mbox::registerMbox(pAdxlMbox, Mbox::eAdxl37xTask);
	zassert_not_null(pAdxlMbox, "pAdxlMbox = NULL");

	HttpConnector* pHttpConnector = new HttpConnector("http");
	zassert_not_null(pHttpConnector, "memory alloc failed");
	zassert_not_null(pHttpConnector->pInMbox, "pInMbox alloc failed");
	zassert_not_null(pHttpConnector->delayTimer, "delayTimer alloc failed");
	zassert_not_null(pHttpConnector->oneDayTimer, "oneDayTimer alloc failed");
    zassert_equal(pHttpConnector->pInMbox->count(), 0, "pInMbox->count() != 0");
    zassert_equal(pHttpConnector->pInMbox, Mbox::getMboxById(Mbox::eHttpConnector),
                  "Error getting mailbox by id");

    Mbox::sendMsg(msgType::eLteConnected, Mbox::eHttpConnector, Mbox::eHttpConnector);

    zassert_equal(pHttpConnector->pInMbox->count(), 1, "pInMbox->count() != 1");

    pHttpConnector->lte_connected = false;

    pHttpConnector->task(); 

	pElem = pAdxlMbox->read(); 
    zassert_equal(pElem->mtype, msgType::eAdxlResumeMeas, "Rcvd mtype != msgType::eAdxlResumeMeas");
    zassert_equal(pElem->from, Mbox::eHttpConnector, "Rcvd sender != Mbox::eHttpConnector");
    zassert_true(pHttpConnector->lte_connected, "lte_connected != true");

	pElem->free();
	delete pAdxlMbox;
	delete pHttpConnector; 
}

//Test receive msg eLteConnected 
ZTEST(tests_net_http_connector, test_http_connector_rcv_msg_eLteConnected2)
{
    PoolElem* pElem;

	Mbox* pAdxlMbox = new Mbox("AdxlMbox");
    Mbox::registerMbox(pAdxlMbox, Mbox::eAdxl37xTask);
	zassert_not_null(pAdxlMbox, "pAdxlMbox = NULL");

	HttpConnector* pHttpConnector = new HttpConnector("http");
	zassert_not_null(pHttpConnector, "memory alloc failed");
	zassert_not_null(pHttpConnector->pInMbox, "pInMbox alloc failed");
	zassert_not_null(pHttpConnector->delayTimer, "delayTimer alloc failed");
	zassert_not_null(pHttpConnector->oneDayTimer, "oneDayTimer alloc failed");
    zassert_equal(pHttpConnector->pInMbox->count(), 0, "pInMbox->count() != 0");
    zassert_equal(pHttpConnector->pInMbox, Mbox::getMboxById(Mbox::eHttpConnector),
                  "Error getting mailbox by id");

    Mbox::sendMsg(msgType::eLteConnected, Mbox::eHttpConnector, Mbox::eHttpConnector);

    zassert_equal(pHttpConnector->pInMbox->count(), 1, "pInMbox->count() != 1");

    pHttpConnector->lte_connected = true;

    pHttpConnector->task(); 

	pElem = pAdxlMbox->read(); 
    zassert_equal(pElem->mtype, msgType::eAdxlResumeMeas, "Rcvd mtype != msgType::eAdxlResumeMeas");
    zassert_equal(pElem->from, Mbox::eHttpConnector, "Rcvd sender != Mbox::eHttpConnector");
    zassert_true(pHttpConnector->lte_connected, "lte_connected != true");

	pElem->free();
	delete pAdxlMbox;
	delete pHttpConnector; 
}

//Test receive msg eLteDisconnected 
ZTEST(tests_net_http_connector, test_http_connector_rcv_msg_eLteDisconnected)
{
 	HttpConnector* pHttpConnector = new HttpConnector("http");
	zassert_not_null(pHttpConnector, "memory alloc failed");
	zassert_not_null(pHttpConnector->pInMbox, "pInMbox alloc failed");
	zassert_not_null(pHttpConnector->delayTimer, "delayTimer alloc failed");
	zassert_not_null(pHttpConnector->oneDayTimer, "oneDayTimer alloc failed");
    zassert_equal(pHttpConnector->pInMbox->count(), 0, "pInMbox->count() != 0");
    zassert_equal(pHttpConnector->pInMbox, Mbox::getMboxById(Mbox::eHttpConnector),
                  "Error getting mailbox by id");

    Mbox::sendMsg(msgType::eLteDisconnected, Mbox::eHttpConnector, Mbox::eHttpConnector);

    zassert_equal(pHttpConnector->pInMbox->count(), 1, "pInMbox->count() != 1");

    pHttpConnector->lte_connected = true;

    pHttpConnector->task(); 

    zassert_false(pHttpConnector->lte_connected, "lte_connected != false");

	delete pHttpConnector; 
}

//Test receive msg eLteIMEI 
ZTEST(tests_net_http_connector, test_http_connector_rcv_msg_eLteIMEI)
{
	std::string s = "1234IMEI";

 	HttpConnector* pHttpConnector = new HttpConnector("http");
	zassert_not_null(pHttpConnector, "memory alloc failed");
	zassert_not_null(pHttpConnector->pInMbox, "pInMbox alloc failed");
	zassert_not_null(pHttpConnector->delayTimer, "delayTimer alloc failed");
	zassert_not_null(pHttpConnector->oneDayTimer, "oneDayTimer alloc failed");
    zassert_equal(pHttpConnector->pInMbox->count(), 0, "pInMbox->count() != 0");
    zassert_equal(pHttpConnector->pInMbox, Mbox::getMboxById(Mbox::eHttpConnector),
                  "Error getting mailbox by id");

    Mbox::sendMsg(msgType::eLteIMEI, Mbox::eHttpConnector, Mbox::eHttpConnector, s);

    zassert_equal(pHttpConnector->pInMbox->count(), 1, "pInMbox->count() != 1");

    pHttpConnector->task(); 

	zassert(strcmp(pHttpConnector->imei.c_str(), "1234IMEI")==0, "wrong IMEI");

	delete pHttpConnector; 
}

//Test receive msg eTimestamp 
ZTEST(tests_net_http_connector, test_http_connector_rcv_msg_eTimestamp)
{
	std::string s = "1234Timestamp";

 	HttpConnector* pHttpConnector = new HttpConnector("http");
	zassert_not_null(pHttpConnector, "memory alloc failed");
	zassert_not_null(pHttpConnector->pInMbox, "pInMbox alloc failed");
	zassert_not_null(pHttpConnector->delayTimer, "delayTimer alloc failed");
	zassert_not_null(pHttpConnector->oneDayTimer, "oneDayTimer alloc failed");
    zassert_equal(pHttpConnector->pInMbox->count(), 0, "pInMbox->count() != 0");
    zassert_equal(pHttpConnector->pInMbox, Mbox::getMboxById(Mbox::eHttpConnector),
                  "Error getting mailbox by id");

    Mbox::sendMsg(msgType::eTimestamp, Mbox::eHttpConnector, Mbox::eHttpConnector, s);

    zassert_equal(pHttpConnector->pInMbox->count(), 1, "pInMbox->count() != 1");

    pHttpConnector->task(); 

	zassert(strcmp(pHttpConnector->timestamp.c_str(), "1234Timestamp")==0, "wrong timestamp");

	delete pHttpConnector; 
}

//Test receive msg eRawDataStatus 
ZTEST(tests_net_http_connector, test_http_connector_rcv_msg_eRawDataStatus)
{
    PoolElem* pElem;

	Mbox* pAdxlStoreMbox = new Mbox("AdxlStoreMbox");
    Mbox::registerMbox(pAdxlStoreMbox, Mbox::eAdxlStore);
	zassert_not_null(pAdxlStoreMbox, "pAdxlStoreMbox = NULL");

	HttpConnector* pHttpConnector = new HttpConnector("http");
	zassert_not_null(pHttpConnector, "memory alloc failed");
	zassert_not_null(pHttpConnector->pInMbox, "pInMbox alloc failed");
	zassert_not_null(pHttpConnector->delayTimer, "delayTimer alloc failed");
	zassert_not_null(pHttpConnector->oneDayTimer, "oneDayTimer alloc failed");
    zassert_equal(pHttpConnector->pInMbox->count(), 0, "pInMbox->count() != 0");
    zassert_equal(pHttpConnector->pInMbox, Mbox::getMboxById(Mbox::eHttpConnector),
                  "Error getting mailbox by id");

    struct AdxlRawDataStatus st = {0, 700};
	Mbox::sendMsg(msgType::eRawDataStatus, Mbox::eHttpConnector, Mbox::eHttpConnector, (uint8_t*)&st, sizeof(st));

    zassert_equal(pHttpConnector->pInMbox->count(), 1, "pInMbox->count() != 1");

    pHttpConnector->task(); 

	pElem = pAdxlStoreMbox->read(); 
    zassert_equal(pElem->mtype, msgType::eRawData, "Rcvd mtype != msgType::eRawData");
    zassert_equal(pElem->from, Mbox::eHttpConnector, "Rcvd sender != Mbox::eHttpConnector");
	//printk("%d\n\n", pHttpConnector->startReadPos);
    zassert_equal(pHttpConnector->startReadPos, 0, "startReadPos != 0");
    zassert_equal(pHttpConnector->rcvdEntries, 0, "rcvdEntries != 0");
    zassert_equal(pHttpConnector->totalNumEntries, 700, "totalNumEntries != 700");

	pElem->free();
	delete pAdxlStoreMbox;
	delete pHttpConnector; 
}

//Test receive msg eRawData
ZTEST(tests_net_http_connector, test_http_connector_rcv_msg_eRawData)
{
    PoolElem* pElem;

	Mbox* pAdxlStoreMbox = new Mbox("AdxlStoreMbox");
    Mbox::registerMbox(pAdxlStoreMbox, Mbox::eAdxlStore);
	zassert_not_null(pAdxlStoreMbox, "pAdxlStoreMbox = NULL");

	HttpConnector* pHttpConnector = new HttpConnector("http");
	zassert_not_null(pHttpConnector, "memory alloc failed");
	zassert_not_null(pHttpConnector->pInMbox, "pInMbox alloc failed");
	zassert_not_null(pHttpConnector->delayTimer, "delayTimer alloc failed");
	zassert_not_null(pHttpConnector->oneDayTimer, "oneDayTimer alloc failed");
    zassert_equal(pHttpConnector->pInMbox->count(), 0, "pInMbox->count() != 0");
    zassert_equal(pHttpConnector->pInMbox, Mbox::getMboxById(Mbox::eHttpConnector),
                  "Error getting mailbox by id");

	pHttpConnector->rcvdEntries = 0;
	pHttpConnector->totalNumEntries = 700;
	pHttpConnector->requestedEntries = 10;
	
	Mbox::sendMsg(msgType::eRawData, Mbox::eHttpConnector, Mbox::eHttpConnector);

    zassert_equal(pHttpConnector->pInMbox->count(), 1, "pInMbox->count() != 1");

    pHttpConnector->task(); 

	pElem = pAdxlStoreMbox->read(); 
    zassert_equal(pElem->mtype, msgType::eRawData, "Rcvd mtype != msgType::eRawData");
    zassert_equal(pElem->from, Mbox::eHttpConnector, "Rcvd sender != Mbox::eHttpConnector");

	struct AdxlRawDataProgress progress =
        *reinterpret_cast<struct AdxlRawDataProgress*>(pElem->getBuffer());

    zassert_equal(progress.startRd, 0, "startRd != 0");
    zassert_equal(progress.rcvdEntries, 10, "rcvdEntries != 10");
	//printk("%d\n\n", progress.requestedEntries);
    zassert_equal(progress.requestedEntries, 100, "requestedEntries != 100");

	pElem->free();
	delete pAdxlStoreMbox;
	delete pHttpConnector; 
}

//Test receive msg eRawData
ZTEST(tests_net_http_connector, test_http_connector_rcv_msg_eRawData2)
{
    PoolElem* pElem;

	Mbox* pAdxlMbox = new Mbox("AdxlMbox");
    Mbox::registerMbox(pAdxlMbox, Mbox::eAdxl37xTask);
	zassert_not_null(pAdxlMbox, "pAdxlMbox = NULL");

	HttpConnector* pHttpConnector = new HttpConnector("http");
	zassert_not_null(pHttpConnector, "memory alloc failed");
	zassert_not_null(pHttpConnector->pInMbox, "pInMbox alloc failed");
	zassert_not_null(pHttpConnector->delayTimer, "delayTimer alloc failed");
	zassert_not_null(pHttpConnector->oneDayTimer, "oneDayTimer alloc failed");
    zassert_equal(pHttpConnector->pInMbox->count(), 0, "pInMbox->count() != 0");
    zassert_equal(pHttpConnector->pInMbox, Mbox::getMboxById(Mbox::eHttpConnector),
                  "Error getting mailbox by id");

	pHttpConnector->rcvdEntries = 600;
	pHttpConnector->totalNumEntries = 700;
	pHttpConnector->requestedEntries = 100;
	pHttpConnector->bDispModeOff = true;
	
	Mbox::sendMsg(msgType::eRawData, Mbox::eHttpConnector, Mbox::eHttpConnector);

    zassert_equal(pHttpConnector->pInMbox->count(), 1, "pInMbox->count() != 1");

    pHttpConnector->task(); 

	pElem = pAdxlMbox->read(); 
    zassert_equal(pElem->mtype, msgType::eRawDataTransferComplete, "Rcvd mtype != eRawDataTransferComplete");
    zassert_equal(pElem->from, Mbox::eHttpConnector, "Rcvd sender != Mbox::eHttpConnector");
	pElem->free();

	pElem = pAdxlMbox->read(); 
    zassert_equal(pElem->mtype, msgType::eAdxlResumeMeas, "Rcvd mtype != eAdxlResumeMeas");
    zassert_equal(pElem->from, Mbox::eHttpConnector, "Rcvd sender != Mbox::eHttpConnector");

	pElem->free();
	delete pAdxlMbox;
	delete pHttpConnector; 
}

//Test receive msg eRawData
ZTEST(tests_net_http_connector, test_http_connector_rcv_msg_eRawData3)
{
    PoolElem* pElem;

	Mbox* pAdxlMbox = new Mbox("AdxlMbox");
    Mbox::registerMbox(pAdxlMbox, Mbox::eAdxl37xTask);
	zassert_not_null(pAdxlMbox, "pAdxlMbox = NULL");

	Mbox* pDispTaskMbox = new Mbox("DispTaskMbox");
    Mbox::registerMbox(pDispTaskMbox, Mbox::eDispTask);
	zassert_not_null(pDispTaskMbox, "pDispTaskMbox = NULL");

	HttpConnector* pHttpConnector = new HttpConnector("http");
	zassert_not_null(pHttpConnector, "memory alloc failed");
	zassert_not_null(pHttpConnector->pInMbox, "pInMbox alloc failed");
	zassert_not_null(pHttpConnector->delayTimer, "delayTimer alloc failed");
	zassert_not_null(pHttpConnector->oneDayTimer, "oneDayTimer alloc failed");
    zassert_equal(pHttpConnector->pInMbox->count(), 0, "pInMbox->count() != 0");
    zassert_equal(pHttpConnector->pInMbox, Mbox::getMboxById(Mbox::eHttpConnector),
                  "Error getting mailbox by id");

	pHttpConnector->rcvdEntries = 600;
	pHttpConnector->totalNumEntries = 700;
	pHttpConnector->requestedEntries = 100;
	pHttpConnector->bDispModeOff = false;
	
	Mbox::sendMsg(msgType::eRawData, Mbox::eHttpConnector, Mbox::eHttpConnector);

    zassert_equal(pHttpConnector->pInMbox->count(), 1, "pInMbox->count() != 1");

    pHttpConnector->task(); 

	pElem = pAdxlMbox->read(); 
    zassert_equal(pElem->mtype, msgType::eRawDataTransferComplete, "Rcvd mtype != eRawDataTransferComplete");
    zassert_equal(pElem->from, Mbox::eHttpConnector, "Rcvd sender != Mbox::eHttpConnector");
	pElem->free();

	pElem = pDispTaskMbox->read(); 
    zassert_equal(pElem->mtype, msgType::eCalcDisplacementReq, "Rcvd mtype != eCalcDisplacementReq");
    zassert_equal(pElem->from, Mbox::eHttpConnector, "Rcvd sender != Mbox::eHttpConnector");

	pElem->free();
	delete pAdxlMbox;
	delete pDispTaskMbox;
	delete pHttpConnector; 
}

//Test receive msg eRawData
ZTEST(tests_net_http_connector, test_http_connector_rcv_msg_eRawData4)
{
    PoolElem* pElem;

	Mbox* pAdxlMbox = new Mbox("AdxlMbox");
    Mbox::registerMbox(pAdxlMbox, Mbox::eAdxl37xTask);
	zassert_not_null(pAdxlMbox, "pAdxlMbox = NULL");

	Mbox* pDispTaskMbox = new Mbox("DispTaskMbox");
    Mbox::registerMbox(pDispTaskMbox, Mbox::eDispTask);
	zassert_not_null(pDispTaskMbox, "pDispTaskMbox = NULL");

	HttpConnector* pHttpConnector = new HttpConnector("http");
	zassert_not_null(pHttpConnector, "memory alloc failed");
	zassert_not_null(pHttpConnector->pInMbox, "pInMbox alloc failed");
	zassert_not_null(pHttpConnector->delayTimer, "delayTimer alloc failed");
	zassert_not_null(pHttpConnector->oneDayTimer, "oneDayTimer alloc failed");
    zassert_equal(pHttpConnector->pInMbox->count(), 0, "pInMbox->count() != 0");
    zassert_equal(pHttpConnector->pInMbox, Mbox::getMboxById(Mbox::eHttpConnector),
                  "Error getting mailbox by id");

	pHttpConnector->rcvdEntries = 0;
	pHttpConnector->totalNumEntries = 0;
	pHttpConnector->requestedEntries = 0;
	pHttpConnector->bDispModeOff = false;
	
	Mbox::sendMsg(msgType::eRawData, Mbox::eHttpConnector, Mbox::eHttpConnector);

    zassert_equal(pHttpConnector->pInMbox->count(), 1, "pInMbox->count() != 1");

    pHttpConnector->task(); 

	pElem = pAdxlMbox->read(); 
    zassert_equal(pElem->mtype, msgType::eRawDataTransferComplete, "Rcvd mtype != eRawDataTransferComplete");
    zassert_equal(pElem->from, Mbox::eHttpConnector, "Rcvd sender != Mbox::eHttpConnector");
	pElem->free();

	pElem = pDispTaskMbox->read(); 
    zassert_equal(pElem->mtype, msgType::eCalcDisplacementReq, "Rcvd mtype != eCalcDisplacementReq");
    zassert_equal(pElem->from, Mbox::eHttpConnector, "Rcvd sender != Mbox::eHttpConnector");

	pElem->free();
	delete pAdxlMbox;
	delete pDispTaskMbox;
	delete pHttpConnector; 
}

//Test receive msg eAdxlIdleReq 
ZTEST(tests_net_http_connector, test_http_connector_rcv_msg_eAdxlIdleReq)
{
    PoolElem* pElem;

	Mbox* pAdxlStoreMbox = new Mbox("AdxlStoreMbox");
    Mbox::registerMbox(pAdxlStoreMbox, Mbox::eAdxlStore);
	zassert_not_null(pAdxlStoreMbox, "pAdxlStoreMbox = NULL");

	HttpConnector* pHttpConnector = new HttpConnector("http");
	zassert_not_null(pHttpConnector, "memory alloc failed");
	zassert_not_null(pHttpConnector->pInMbox, "pInMbox alloc failed");
	zassert_not_null(pHttpConnector->delayTimer, "delayTimer alloc failed");
	zassert_not_null(pHttpConnector->oneDayTimer, "oneDayTimer alloc failed");
    zassert_equal(pHttpConnector->pInMbox->count(), 0, "pInMbox->count() != 0");
    zassert_equal(pHttpConnector->pInMbox, Mbox::getMboxById(Mbox::eHttpConnector),
                  "Error getting mailbox by id");

	Mbox::sendMsg(msgType::eAdxlIdleReq, Mbox::eHttpConnector, Mbox::eHttpConnector);

    zassert_equal(pHttpConnector->pInMbox->count(), 1, "pInMbox->count() != 1");

    pHttpConnector->task(); 

	pElem = pAdxlStoreMbox->read(); 
    zassert_equal(pElem->mtype, msgType::eAdxlIdleReq, "Rcvd mtype != msgType::eAdxlIdleReq");
    zassert_equal(pElem->from, Mbox::eHttpConnector, "Rcvd sender != Mbox::eHttpConnector");

	pElem->free();
	delete pAdxlStoreMbox;
	delete pHttpConnector; 
}

//Test receive msg eAdxl37xRmsData 
ZTEST(tests_net_http_connector, test_http_connector_rcv_msg_eAdxl37xRmsData)
{
    PoolElem* pElem;

	Mbox* pDataMgrMbox = new Mbox("DataMgrMbox");
    Mbox::registerMbox(pDataMgrMbox, Mbox::eDataManager);
	zassert_not_null(pDataMgrMbox, "pDataMgrMbox = NULL");

	HttpConnector* pHttpConnector = new HttpConnector("http");
	zassert_not_null(pHttpConnector, "memory alloc failed");
	zassert_not_null(pHttpConnector->pInMbox, "pInMbox alloc failed");
	zassert_not_null(pHttpConnector->delayTimer, "delayTimer alloc failed");
	zassert_not_null(pHttpConnector->oneDayTimer, "oneDayTimer alloc failed");
    zassert_equal(pHttpConnector->pInMbox->count(), 0, "pInMbox->count() != 0");
    zassert_equal(pHttpConnector->pInMbox, Mbox::getMboxById(Mbox::eHttpConnector),
                  "Error getting mailbox by id");

	struct adxl_rms_data st = {0, 0.0,0.0,17.7,1,1};
	pHttpConnector->zMaxRms = 0.0;
	Mbox::sendMsg(msgType::eAdxl37xRmsData, Mbox::eHttpConnector, Mbox::eHttpConnector, (uint8_t*)&st, sizeof(st));

    zassert_equal(pHttpConnector->pInMbox->count(), 1, "pInMbox->count() != 1");

    pHttpConnector->task(); 

	pElem = pDataMgrMbox->read(); 
    zassert_equal(pElem->mtype, msgType::eAdxl37xRmsData, "Rcvd mtype != msgType::eAdxl37xRmsData");
    zassert_equal(pElem->from, Mbox::eHttpConnector, "Rcvd sender != Mbox::eHttpConnector");
	printk("%f\n\n", pHttpConnector->zMaxRms);
    zassert_true(pHttpConnector->zMaxRms > 17.6, "zMaxRms != 17.7");
    zassert_true(pHttpConnector->zMaxRms < 17.8, "zMaxRms != 17.7");

	pElem->free();
	delete pDataMgrMbox;
	delete pHttpConnector; 
}

//Test receive msg eAdxl37xRmsData 
ZTEST(tests_net_http_connector, test_http_connector_rcv_msg_eAdxl37xRmsData2)
{
    PoolElem* pElem;

	Mbox* pDataMgrMbox = new Mbox("DataMgrMbox");
    Mbox::registerMbox(pDataMgrMbox, Mbox::eDataManager);
	zassert_not_null(pDataMgrMbox, "pDataMgrMbox = NULL");

	HttpConnector* pHttpConnector = new HttpConnector("http");
	zassert_not_null(pHttpConnector, "memory alloc failed");
	zassert_not_null(pHttpConnector->pInMbox, "pInMbox alloc failed");
	zassert_not_null(pHttpConnector->delayTimer, "delayTimer alloc failed");
	zassert_not_null(pHttpConnector->oneDayTimer, "oneDayTimer alloc failed");
    zassert_equal(pHttpConnector->pInMbox->count(), 0, "pInMbox->count() != 0");
    zassert_equal(pHttpConnector->pInMbox, Mbox::getMboxById(Mbox::eHttpConnector),
                  "Error getting mailbox by id");

	struct adxl_rms_data st = {0, 0.0,0.0,17.7,1,1};
	pHttpConnector->zMaxRms = 27.7;
	Mbox::sendMsg(msgType::eAdxl37xRmsData, Mbox::eHttpConnector, Mbox::eHttpConnector, (uint8_t*)&st, sizeof(st));

    zassert_equal(pHttpConnector->pInMbox->count(), 1, "pInMbox->count() != 1");

    pHttpConnector->task(); 

	pElem = pDataMgrMbox->read(); 
    zassert_equal(pElem->mtype, msgType::eAdxl37xRmsData, "Rcvd mtype != msgType::eAdxl37xRmsData");
    zassert_equal(pElem->from, Mbox::eHttpConnector, "Rcvd sender != Mbox::eHttpConnector");
	printk("%f\n\n", pHttpConnector->zMaxRms);
    zassert_true(pHttpConnector->zMaxRms > 27.6, "zMaxRms != 27.7");
    zassert_true(pHttpConnector->zMaxRms < 27.8, "zMaxRms != 27.7");

	pElem->free();
	delete pDataMgrMbox;
	delete pHttpConnector; 
}

//Test receive msg eAdxl37xIdle 
ZTEST(tests_net_http_connector, test_http_connector_rcv_msg_eAdxl37xIdle)
{
 	HttpConnector* pHttpConnector = new HttpConnector("http");
	zassert_not_null(pHttpConnector, "memory alloc failed");
	zassert_not_null(pHttpConnector->pInMbox, "pInMbox alloc failed");
	zassert_not_null(pHttpConnector->delayTimer, "delayTimer alloc failed");
	zassert_not_null(pHttpConnector->oneDayTimer, "oneDayTimer alloc failed");
    zassert_equal(pHttpConnector->pInMbox->count(), 0, "pInMbox->count() != 0");
    zassert_equal(pHttpConnector->pInMbox, Mbox::getMboxById(Mbox::eHttpConnector),
                  "Error getting mailbox by id");

    pHttpConnector->zMaxRms = 27.7;
	
	Mbox::sendMsg(msgType::eAdxl37xIdle, Mbox::eHttpConnector, Mbox::eHttpConnector);

    zassert_equal(pHttpConnector->pInMbox->count(), 1, "pInMbox->count() != 1");

    pHttpConnector->task(); 

	printk("%f\n\n", pHttpConnector->zMaxRms);
    zassert_true(pHttpConnector->zMaxRms > -0.01, "zMaxRms != 0.0");
    zassert_true(pHttpConnector->zMaxRms < 0.01, "zMaxRms != 0.0");

	delete pHttpConnector; 
}

//Test receive msg eAdxlIdleInd 
ZTEST(tests_net_http_connector, test_http_connector_rcv_msg_eAdxlIdleInd)
{
    PoolElem* pElem;

	Mbox* pAdxlMbox = new Mbox("AdxlMbox");
    Mbox::registerMbox(pAdxlMbox, Mbox::eAdxl37xTask);
	zassert_not_null(pAdxlMbox, "pAdxlMbox = NULL");

	HttpConnector* pHttpConnector = new HttpConnector("http");
	zassert_not_null(pHttpConnector, "memory alloc failed");
	zassert_not_null(pHttpConnector->pInMbox, "pInMbox alloc failed");
	zassert_not_null(pHttpConnector->delayTimer, "delayTimer alloc failed");
	zassert_not_null(pHttpConnector->oneDayTimer, "oneDayTimer alloc failed");
    zassert_equal(pHttpConnector->pInMbox->count(), 0, "pInMbox->count() != 0");
    zassert_equal(pHttpConnector->pInMbox, Mbox::getMboxById(Mbox::eHttpConnector),
                  "Error getting mailbox by id");

	pHttpConnector->bRawTransferEnabled = false;
	pHttpConnector->bDispModeOff = true;
	bool bIdle = false;
	Mbox::sendMsg(msgType::eAdxlIdleInd, Mbox::eHttpConnector, Mbox::eHttpConnector, (uint8_t)bIdle);

    zassert_equal(pHttpConnector->pInMbox->count(), 1, "pInMbox->count() != 1");

    pHttpConnector->task(); 

	pElem = pAdxlMbox->read(); 
    zassert_equal(pElem->mtype, msgType::eAdxlResumeMeas, "Rcvd mtype != msgType::eAdxlResumeMeas");
    zassert_equal(pElem->from, Mbox::eHttpConnector, "Rcvd sender != Mbox::eHttpConnector");

	pElem->free();
	delete pAdxlMbox;
	delete pHttpConnector; 
}

//Test receive msg eAdxlIdleInd 
ZTEST(tests_net_http_connector, test_http_connector_rcv_msg_eAdxlIdleInd2)
{
    PoolElem* pElem;

	Mbox* pAdxlMbox = new Mbox("AdxlMbox");
    Mbox::registerMbox(pAdxlMbox, Mbox::eAdxl37xTask);
	zassert_not_null(pAdxlMbox, "pAdxlMbox = NULL");

	HttpConnector* pHttpConnector = new HttpConnector("http");
	zassert_not_null(pHttpConnector, "memory alloc failed");
	zassert_not_null(pHttpConnector->pInMbox, "pInMbox alloc failed");
	zassert_not_null(pHttpConnector->delayTimer, "delayTimer alloc failed");
	zassert_not_null(pHttpConnector->oneDayTimer, "oneDayTimer alloc failed");
    zassert_equal(pHttpConnector->pInMbox->count(), 0, "pInMbox->count() != 0");
    zassert_equal(pHttpConnector->pInMbox, Mbox::getMboxById(Mbox::eHttpConnector),
                  "Error getting mailbox by id");

	pHttpConnector->bRawTransferEnabled = false;
	pHttpConnector->bDispModeOff = false;
	pHttpConnector->zMaxRms = 10.0;
	pHttpConnector->rms_z_Thres = 15.0;
	bool bIdle = false;
	Mbox::sendMsg(msgType::eAdxlIdleInd, Mbox::eHttpConnector, Mbox::eHttpConnector, (uint8_t)bIdle);

    zassert_equal(pHttpConnector->pInMbox->count(), 1, "pInMbox->count() != 1");

    pHttpConnector->task(); 

	pElem = pAdxlMbox->read(); 
    zassert_equal(pElem->mtype, msgType::eAdxlResumeMeas, "Rcvd mtype != msgType::eAdxlResumeMeas");
    zassert_equal(pElem->from, Mbox::eHttpConnector, "Rcvd sender != Mbox::eHttpConnector");

	pElem->free();
	delete pAdxlMbox;
	delete pHttpConnector; 
}

//Test receive msg eAdxlIdleInd 
ZTEST(tests_net_http_connector, test_http_connector_rcv_msg_eAdxlIdleInd3)
{
    PoolElem* pElem;

	Mbox* pDispTaskMbox = new Mbox("DispTaskMbox");
    Mbox::registerMbox(pDispTaskMbox, Mbox::eDispTask);
	zassert_not_null(pDispTaskMbox, "pDispTaskMbox = NULL");

	HttpConnector* pHttpConnector = new HttpConnector("http");
	zassert_not_null(pHttpConnector, "memory alloc failed");
	zassert_not_null(pHttpConnector->pInMbox, "pInMbox alloc failed");
	zassert_not_null(pHttpConnector->delayTimer, "delayTimer alloc failed");
	zassert_not_null(pHttpConnector->oneDayTimer, "oneDayTimer alloc failed");
    zassert_equal(pHttpConnector->pInMbox->count(), 0, "pInMbox->count() != 0");
    zassert_equal(pHttpConnector->pInMbox, Mbox::getMboxById(Mbox::eHttpConnector),
                  "Error getting mailbox by id");

	pHttpConnector->bRawTransferEnabled = false;
	pHttpConnector->bDispModeOff = false;
	pHttpConnector->zMaxRms = 10.0;
	pHttpConnector->rms_z_Thres = 5.0;
	bool bIdle = false;
	Mbox::sendMsg(msgType::eAdxlIdleInd, Mbox::eHttpConnector, Mbox::eHttpConnector, (uint8_t)bIdle);

    zassert_equal(pHttpConnector->pInMbox->count(), 1, "pInMbox->count() != 1");

    pHttpConnector->task(); 

	pElem = pDispTaskMbox->read(); 
    zassert_equal(pElem->mtype, msgType::eCalcDisplacementReq, "Rcvd mtype != msgType::eCalcDisplacementReq");
    zassert_equal(pElem->from, Mbox::eHttpConnector, "Rcvd sender != Mbox::eHttpConnector");

	pElem->free();
	delete pDispTaskMbox;
	delete pHttpConnector; 
}

//Test receive msg eAdxlIdleInd 
ZTEST(tests_net_http_connector, test_http_connector_rcv_msg_eAdxlIdleInd4)
{
	HttpConnector* pHttpConnector = new HttpConnector("http");
	zassert_not_null(pHttpConnector, "memory alloc failed");
	zassert_not_null(pHttpConnector->pInMbox, "pInMbox alloc failed");
	zassert_not_null(pHttpConnector->delayTimer, "delayTimer alloc failed");
	zassert_not_null(pHttpConnector->oneDayTimer, "oneDayTimer alloc failed");
    zassert_equal(pHttpConnector->pInMbox->count(), 0, "pInMbox->count() != 0");
    zassert_equal(pHttpConnector->pInMbox, Mbox::getMboxById(Mbox::eHttpConnector),
                  "Error getting mailbox by id");

	pHttpConnector->bRawTransferEnabled = true;
	pHttpConnector->bDispModeOff = false;
	pHttpConnector->zMaxRms = 10.0;
	pHttpConnector->rms_z_Thres = 5.0;
	bool bIdle = false;
	Mbox::sendMsg(msgType::eAdxlIdleInd, Mbox::eHttpConnector, Mbox::eHttpConnector, (uint8_t)bIdle);

    zassert_equal(pHttpConnector->pInMbox->count(), 1, "pInMbox->count() != 1");

    pHttpConnector->task(); 

	delete pHttpConnector; 
}

//Test receive msg eAdxlIdleInd 
ZTEST(tests_net_http_connector, test_http_connector_rcv_msg_eAdxlIdleInd5)
{
    PoolElem* pElem;

	HttpConnector* pHttpConnector = new HttpConnector("http");
	zassert_not_null(pHttpConnector, "memory alloc failed");
	zassert_not_null(pHttpConnector->pInMbox, "pInMbox alloc failed");
	zassert_not_null(pHttpConnector->delayTimer, "delayTimer alloc failed");
	zassert_not_null(pHttpConnector->oneDayTimer, "oneDayTimer alloc failed");
    zassert_equal(pHttpConnector->pInMbox->count(), 0, "pInMbox->count() != 0");
    zassert_equal(pHttpConnector->pInMbox, Mbox::getMboxById(Mbox::eHttpConnector),
                  "Error getting mailbox by id");

	pHttpConnector->bRawTransferEnabled = true;
	pHttpConnector->bDispModeOff = false;
	pHttpConnector->zMaxRms = 10.0;
	pHttpConnector->rms_z_Thres = 5.0;
	pHttpConnector->dailyRawSendCnt = 5;
	pHttpConnector->maxRawPerDay = 10;
	bool bIdle = true;
	Mbox::sendMsg(msgType::eAdxlIdleInd, Mbox::eHttpConnector, Mbox::eHttpConnector, (uint8_t)bIdle);

    zassert_equal(pHttpConnector->pInMbox->count(), 1, "pInMbox->count() != 1");

    pHttpConnector->task(); 

    zassert_equal(pHttpConnector->pInMbox->count(), 1, "pInMbox->count() != 1");
    zassert_equal(pHttpConnector->dailyRawSendCnt, 6, "dailyRawSendCnt != 6");

	pElem = pHttpConnector->pInMbox->read(); 
    zassert_equal(pElem->mtype, msgType::eHttpRawDataSend, "Rcvd mtype != msgType::eHttpRawDataSend");
    zassert_equal(pElem->from, Mbox::eHttpConnector, "Rcvd sender != Mbox::eHttpConnector");

	pElem->free();
	delete pHttpConnector; 
}

//Test receive msg eAdxlIdleInd 
ZTEST(tests_net_http_connector, test_http_connector_rcv_msg_eAdxlIdleInd6)
{
    PoolElem* pElem;

	Mbox* pAdxlMbox = new Mbox("AdxlMbox");
    Mbox::registerMbox(pAdxlMbox, Mbox::eAdxl37xTask);
	zassert_not_null(pAdxlMbox, "pAdxlMbox = NULL");

	HttpConnector* pHttpConnector = new HttpConnector("http");
	zassert_not_null(pHttpConnector, "memory alloc failed");
	zassert_not_null(pHttpConnector->pInMbox, "pInMbox alloc failed");
	zassert_not_null(pHttpConnector->delayTimer, "delayTimer alloc failed");
	zassert_not_null(pHttpConnector->oneDayTimer, "oneDayTimer alloc failed");
    zassert_equal(pHttpConnector->pInMbox->count(), 0, "pInMbox->count() != 0");
    zassert_equal(pHttpConnector->pInMbox, Mbox::getMboxById(Mbox::eHttpConnector),
                  "Error getting mailbox by id");

	pHttpConnector->bRawTransferEnabled = true;
	pHttpConnector->bDispModeOff = false;
	pHttpConnector->zMaxRms = 10.0;
	pHttpConnector->rms_z_Thres = 15.0;
	pHttpConnector->dailyRawSendCnt = 5;
	pHttpConnector->maxRawPerDay = 10;
	bool bIdle = true;
	// if (zMaxRms <= rms_z_Thres)
	Mbox::sendMsg(msgType::eAdxlIdleInd, Mbox::eHttpConnector, Mbox::eHttpConnector, (uint8_t)bIdle);

    zassert_equal(pHttpConnector->pInMbox->count(), 1, "pInMbox->count() != 1");

    pHttpConnector->task(); 

    zassert_equal(pAdxlMbox->count(), 1, "pInMbox->count() != 1");

	pElem = pAdxlMbox->read(); 
    zassert_equal(pElem->mtype, msgType::eAdxlResumeMeas, "Rcvd mtype != msgType::eAdxlResumeMeas");
    zassert_equal(pElem->from, Mbox::eHttpConnector, "Rcvd sender != Mbox::eHttpConnector");

	pElem->free();
	delete pAdxlMbox;
	delete pHttpConnector; 
}

//Test receive msg eAdxlIdleInd 
ZTEST(tests_net_http_connector, test_http_connector_rcv_msg_eAdxlIdleInd7)
{
    PoolElem* pElem;

	Mbox* pAdxlMbox = new Mbox("AdxlMbox");
    Mbox::registerMbox(pAdxlMbox, Mbox::eAdxl37xTask);
	zassert_not_null(pAdxlMbox, "pAdxlMbox = NULL");

	HttpConnector* pHttpConnector = new HttpConnector("http");
	zassert_not_null(pHttpConnector, "memory alloc failed");
	zassert_not_null(pHttpConnector->pInMbox, "pInMbox alloc failed");
	zassert_not_null(pHttpConnector->delayTimer, "delayTimer alloc failed");
	zassert_not_null(pHttpConnector->oneDayTimer, "oneDayTimer alloc failed");
    zassert_equal(pHttpConnector->pInMbox->count(), 0, "pInMbox->count() != 0");
    zassert_equal(pHttpConnector->pInMbox, Mbox::getMboxById(Mbox::eHttpConnector),
                  "Error getting mailbox by id");

	pHttpConnector->bRawTransferEnabled = true;
	pHttpConnector->bDispModeOff = false;
	pHttpConnector->zMaxRms = 10.0;
	pHttpConnector->rms_z_Thres = 15.0;
	pHttpConnector->dailyRawSendCnt = 9;
	pHttpConnector->maxRawPerDay = 10;
	bool bIdle = true;
	// if (zMaxRms <= rms_z_Thres)
	Mbox::sendMsg(msgType::eAdxlIdleInd, Mbox::eHttpConnector, Mbox::eHttpConnector, (uint8_t)bIdle);

    zassert_equal(pHttpConnector->pInMbox->count(), 1, "pInMbox->count() != 1");

    pHttpConnector->task(); 

    zassert_equal(pAdxlMbox->count(), 1, "pInMbox->count() != 1");

	pElem = pAdxlMbox->read(); 
    zassert_equal(pElem->mtype, msgType::eAdxlResumeMeas, "Rcvd mtype != msgType::eAdxlResumeMeas");
    zassert_equal(pElem->from, Mbox::eHttpConnector, "Rcvd sender != Mbox::eHttpConnector");

	pElem->free();
	delete pAdxlMbox;
	delete pHttpConnector; 
}

//Test receive msg eAdxlIdleInd 
ZTEST(tests_net_http_connector, test_http_connector_rcv_msg_eAdxlIdleInd8)
{
    PoolElem* pElem;

	Mbox* pDispTaskMbox = new Mbox("DispTaskMbox");
    Mbox::registerMbox(pDispTaskMbox, Mbox::eDispTask);
	zassert_not_null(pDispTaskMbox, "pDispTaskMbox = NULL");

	HttpConnector* pHttpConnector = new HttpConnector("http");
	zassert_not_null(pHttpConnector, "memory alloc failed");
	zassert_not_null(pHttpConnector->pInMbox, "pInMbox alloc failed");
	zassert_not_null(pHttpConnector->delayTimer, "delayTimer alloc failed");
	zassert_not_null(pHttpConnector->oneDayTimer, "oneDayTimer alloc failed");
    zassert_equal(pHttpConnector->pInMbox->count(), 0, "pInMbox->count() != 0");
    zassert_equal(pHttpConnector->pInMbox, Mbox::getMboxById(Mbox::eHttpConnector),
                  "Error getting mailbox by id");

	pHttpConnector->bRawTransferEnabled = true;
	pHttpConnector->bDispModeOff = false;
	pHttpConnector->zMaxRms = 10.0;
	pHttpConnector->rms_z_Thres = 5.0;
	pHttpConnector->dailyRawSendCnt = 15;
	pHttpConnector->maxRawPerDay = 10;
	bool bIdle = true;
	// else if (dailyRawSendCnt >= maxRawPerDay)
	Mbox::sendMsg(msgType::eAdxlIdleInd, Mbox::eHttpConnector, Mbox::eHttpConnector, (uint8_t)bIdle);

    zassert_equal(pHttpConnector->pInMbox->count(), 1, "pInMbox->count() != 1");

    pHttpConnector->task(); 

    zassert_equal(pDispTaskMbox->count(), 1, "pInMbox->count() != 1");

	pElem = pDispTaskMbox->read(); 
    zassert_equal(pElem->mtype, msgType::eCalcDisplacementReq, "Rcvd mtype != msgType::eCalcDisplacementReq");
    zassert_equal(pElem->from, Mbox::eHttpConnector, "Rcvd sender != Mbox::eHttpConnector");

	pElem->free();
	delete pDispTaskMbox;
	delete pHttpConnector; 
}

//Test receive msg eAdxlIdleInd 
ZTEST(tests_net_http_connector, test_http_connector_rcv_msg_eAdxlIdleInd9)
{
	// see comment in DUT code
	// NOTE Unit test: Unreachable code, so that branch coverage is not fullfilled
}

//Test receive msg eHttpRawDataSend
ZTEST(tests_net_http_connector, test_http_connector_rcv_msg_eHttpRawDataSend)
{
 	HttpConnector* pHttpConnector = new HttpConnector("http");
	zassert_not_null(pHttpConnector, "memory alloc failed");
	zassert_not_null(pHttpConnector->pInMbox, "pInMbox alloc failed");
	zassert_not_null(pHttpConnector->delayTimer, "delayTimer alloc failed");
	zassert_not_null(pHttpConnector->oneDayTimer, "oneDayTimer alloc failed");
    zassert_equal(pHttpConnector->pInMbox->count(), 0, "pInMbox->count() != 0");
    zassert_equal(pHttpConnector->pInMbox, Mbox::getMboxById(Mbox::eHttpConnector),
                  "Error getting mailbox by id");

    pHttpConnector->lte_connected = false;
    pHttpConnector->bRawTransferEnabled = false;
	
	Mbox::sendMsg(msgType::eHttpRawDataSend, Mbox::eHttpConnector, Mbox::eHttpConnector);

    zassert_equal(pHttpConnector->pInMbox->count(), 1, "pInMbox->count() != 1");

    pHttpConnector->task(); 

	delete pHttpConnector; 
}

//Test receive msg eAdxeHttpRawDataSendl37xIdle 
ZTEST(tests_net_http_connector, test_http_connector_rcv_msg_eHttpRawDataSend2)
{
 	HttpConnector* pHttpConnector = new HttpConnector("http");
	zassert_not_null(pHttpConnector, "memory alloc failed");
	zassert_not_null(pHttpConnector->pInMbox, "pInMbox alloc failed");
	zassert_not_null(pHttpConnector->delayTimer, "delayTimer alloc failed");
	zassert_not_null(pHttpConnector->oneDayTimer, "oneDayTimer alloc failed");
    zassert_equal(pHttpConnector->pInMbox->count(), 0, "pInMbox->count() != 0");
    zassert_equal(pHttpConnector->pInMbox, Mbox::getMboxById(Mbox::eHttpConnector),
                  "Error getting mailbox by id");

    pHttpConnector->lte_connected = false;
    pHttpConnector->bRawTransferEnabled = true;
	
	Mbox::sendMsg(msgType::eHttpRawDataSend, Mbox::eHttpConnector, Mbox::eHttpConnector);

    zassert_equal(pHttpConnector->pInMbox->count(), 1, "pInMbox->count() != 1");

    pHttpConnector->task(); 

	delete pHttpConnector; 
}

//Test receive msg eAdxeHttpRawDataSendl37xIdle 
ZTEST(tests_net_http_connector, test_http_connector_rcv_msg_eHttpRawDataSend3)
{
 	HttpConnector* pHttpConnector = new HttpConnector("http");
	zassert_not_null(pHttpConnector, "memory alloc failed");
	zassert_not_null(pHttpConnector->pInMbox, "pInMbox alloc failed");
	zassert_not_null(pHttpConnector->delayTimer, "delayTimer alloc failed");
	zassert_not_null(pHttpConnector->oneDayTimer, "oneDayTimer alloc failed");
    zassert_equal(pHttpConnector->pInMbox->count(), 0, "pInMbox->count() != 0");
    zassert_equal(pHttpConnector->pInMbox, Mbox::getMboxById(Mbox::eHttpConnector),
                  "Error getting mailbox by id");

    pHttpConnector->lte_connected = true;
    pHttpConnector->bRawTransferEnabled = false;
	
	Mbox::sendMsg(msgType::eHttpRawDataSend, Mbox::eHttpConnector, Mbox::eHttpConnector);

    zassert_equal(pHttpConnector->pInMbox->count(), 1, "pInMbox->count() != 1");

    pHttpConnector->task(); 

	delete pHttpConnector; 
}

//Test receive msg eAdxeHttpRawDataSendl37xIdle 
ZTEST(tests_net_http_connector, test_http_connector_rcv_msg_eHttpRawDataSend4)
{
    PoolElem* pElem;

	Mbox* pAdxlMbox = new Mbox("AdxlMbox");
    Mbox::registerMbox(pAdxlMbox, Mbox::eAdxl37xTask);
	zassert_not_null(pAdxlMbox, "pAdxlMbox = NULL");

 	HttpConnector* pHttpConnector = new HttpConnector("http");
	zassert_not_null(pHttpConnector, "memory alloc failed");
	zassert_not_null(pHttpConnector->pInMbox, "pInMbox alloc failed");
	zassert_not_null(pHttpConnector->delayTimer, "delayTimer alloc failed");
	zassert_not_null(pHttpConnector->oneDayTimer, "oneDayTimer alloc failed");
    zassert_equal(pHttpConnector->pInMbox->count(), 0, "pInMbox->count() != 0");
    zassert_equal(pHttpConnector->pInMbox, Mbox::getMboxById(Mbox::eHttpConnector),
                  "Error getting mailbox by id");

    pHttpConnector->lte_connected = true;
    pHttpConnector->bRawTransferEnabled = true;
	gIntVal = 3;
	
	Mbox::sendMsg(msgType::eHttpRawDataSend, Mbox::eHttpConnector, Mbox::eHttpConnector);

    zassert_equal(pHttpConnector->pInMbox->count(), 1, "pInMbox->count() != 1");

    pHttpConnector->task(); 

	pElem = pAdxlMbox->read(); 
    zassert_equal(pElem->mtype, msgType::eRawDataTransferActive, "Rcvd mtype != msgType::eRawDataTransferActive");
    zassert_equal(pElem->from, Mbox::eHttpConnector, "Rcvd sender != Mbox::eHttpConnector");
	pElem->free();

	pElem = pAdxlMbox->read(); 
    zassert_equal(pElem->mtype, msgType::eRawDataTransferComplete, "Rcvd mtype != msgType::eRawDataTransferComplete");
    zassert_equal(pElem->from, Mbox::eHttpConnector, "Rcvd sender != Mbox::eHttpConnector");
	pElem->free();

	pElem = pAdxlMbox->read(); 
    zassert_equal(pElem->mtype, msgType::eAdxlResumeMeas, "Rcvd mtype != msgType::eAdxlResumeMeas");
    zassert_equal(pElem->from, Mbox::eHttpConnector, "Rcvd sender != Mbox::eHttpConnector");
	pElem->free();

	delete pAdxlMbox;
	delete pHttpConnector; 
}

//Test receive msg eAdxeHttpRawDataSendl37xIdle 
ZTEST(tests_net_http_connector, test_http_connector_rcv_msg_eHttpRawDataSend5)
{
    PoolElem* pElem;

	Mbox* pAdxlMbox = new Mbox("AdxlMbox");
    Mbox::registerMbox(pAdxlMbox, Mbox::eAdxl37xTask);
	zassert_not_null(pAdxlMbox, "pAdxlMbox = NULL");

	Mbox* pAdxlStoreMbox = new Mbox("AdxlStoreMbox");
    Mbox::registerMbox(pAdxlStoreMbox, Mbox::eAdxlStore);
	zassert_not_null(pAdxlStoreMbox, "pAdxlStoreMbox = NULL");

 	HttpConnector* pHttpConnector = new HttpConnector("http");
	zassert_not_null(pHttpConnector, "memory alloc failed");
	zassert_not_null(pHttpConnector->pInMbox, "pInMbox alloc failed");
	zassert_not_null(pHttpConnector->delayTimer, "delayTimer alloc failed");
	zassert_not_null(pHttpConnector->oneDayTimer, "oneDayTimer alloc failed");
    zassert_equal(pHttpConnector->pInMbox->count(), 0, "pInMbox->count() != 0");
    zassert_equal(pHttpConnector->pInMbox, Mbox::getMboxById(Mbox::eHttpConnector),
                  "Error getting mailbox by id");

    pHttpConnector->lte_connected = true;
    pHttpConnector->bRawTransferEnabled = true;
	gIntVal = 0;
	
	Mbox::sendMsg(msgType::eHttpRawDataSend, Mbox::eHttpConnector, Mbox::eHttpConnector);

    zassert_equal(pHttpConnector->pInMbox->count(), 1, "pInMbox->count() !=1");

	//pHttpConnector->http_connected = true;

    pHttpConnector->task(); 

	pElem = pAdxlMbox->read(); 
    zassert_equal(pElem->mtype, msgType::eRawDataTransferActive, "Rcvd mtype != msgType::eRawDataTransferActive");
    zassert_equal(pElem->from, Mbox::eHttpConnector, "Rcvd sender != Mbox::eHttpConnector");
	pElem->free();

	pElem = pAdxlStoreMbox->read(); 
    zassert_equal(pElem->mtype, msgType::eRawDataStatus, "Rcvd mtype != msgType::eRawDataStatus");
    zassert_equal(pElem->from, Mbox::eHttpConnector, "Rcvd sender != Mbox::eHttpConnector");
	pElem->free();

	delete pAdxlMbox;
	delete pHttpConnector; 
}

//Test receive msg eSendDispDataReq 
ZTEST(tests_net_http_connector, test_http_connector_rcv_msg_eSendDispDataReq)
{
    PoolElem* pElem;

	Mbox* pAdxlMbox = new Mbox("AdxlMbox");
    Mbox::registerMbox(pAdxlMbox, Mbox::eAdxl37xTask);
	zassert_not_null(pAdxlMbox, "pAdxlMbox = NULL");

	HttpConnector* pHttpConnector = new HttpConnector("http");
	zassert_not_null(pHttpConnector, "memory alloc failed");
	zassert_not_null(pHttpConnector->pInMbox, "pInMbox alloc failed");
	zassert_not_null(pHttpConnector->delayTimer, "delayTimer alloc failed");
	zassert_not_null(pHttpConnector->oneDayTimer, "oneDayTimer alloc failed");
    zassert_equal(pHttpConnector->pInMbox->count(), 0, "pInMbox->count() != 0");
    zassert_equal(pHttpConnector->pInMbox, Mbox::getMboxById(Mbox::eHttpConnector),
                  "Error getting mailbox by id");

	pHttpConnector->lte_connected = false;
	pHttpConnector->bDispTransferEnabled = false;
	pHttpConnector->bDailyRawSendCntExceeded = true;
	Mbox::sendMsg(msgType::eSendDispDataReq, Mbox::eHttpConnector, Mbox::eHttpConnector);

    zassert_equal(pHttpConnector->pInMbox->count(), 1, "pInMbox->count() != 1");

    pHttpConnector->task(); 

	pElem = pAdxlMbox->read(); 
    zassert_equal(pElem->mtype, msgType::eAdxlResumeMeas, "Rcvd mtype != msgType::eAdxlResumeMeas");
    zassert_equal(pElem->from, Mbox::eHttpConnector, "Rcvd sender != Mbox::eHttpConnector");

	pElem->free();
	delete pAdxlMbox;
	delete pHttpConnector; 
}

//Test receive msg eSendDispDataReq 
ZTEST(tests_net_http_connector, test_http_connector_rcv_msg_eSendDispDataReq2)
{
    PoolElem* pElem;

	Mbox* pAdxlMbox = new Mbox("AdxlMbox");
    Mbox::registerMbox(pAdxlMbox, Mbox::eAdxl37xTask);
	zassert_not_null(pAdxlMbox, "pAdxlMbox = NULL");

	HttpConnector* pHttpConnector = new HttpConnector("http");
	zassert_not_null(pHttpConnector, "memory alloc failed");
	zassert_not_null(pHttpConnector->pInMbox, "pInMbox alloc failed");
	zassert_not_null(pHttpConnector->delayTimer, "delayTimer alloc failed");
	zassert_not_null(pHttpConnector->oneDayTimer, "oneDayTimer alloc failed");
    zassert_equal(pHttpConnector->pInMbox->count(), 0, "pInMbox->count() != 0");
    zassert_equal(pHttpConnector->pInMbox, Mbox::getMboxById(Mbox::eHttpConnector),
                  "Error getting mailbox by id");

	pHttpConnector->lte_connected = false;
	pHttpConnector->bDispTransferEnabled = false;
	pHttpConnector->bDailyRawSendCntExceeded = false;
	Mbox::sendMsg(msgType::eSendDispDataReq, Mbox::eHttpConnector, Mbox::eHttpConnector);

    zassert_equal(pHttpConnector->pInMbox->count(), 1, "pInMbox->count() != 1");

    pHttpConnector->task(); 

	pElem = pAdxlMbox->read(); 
    zassert_equal(pElem->mtype, msgType::eAdxlResumeMeas, "Rcvd mtype != msgType::eAdxlResumeMeas");
    zassert_equal(pElem->from, Mbox::eHttpConnector, "Rcvd sender != Mbox::eHttpConnector");

	pElem->free();
	delete pAdxlMbox;
	delete pHttpConnector; 
}

//Test receive msg eSendDispDataReq 
ZTEST(tests_net_http_connector, test_http_connector_rcv_msg_eSendDispDataReq3)
{
    PoolElem* pElem;

	Mbox* pAdxlMbox = new Mbox("AdxlMbox");
    Mbox::registerMbox(pAdxlMbox, Mbox::eAdxl37xTask);
	zassert_not_null(pAdxlMbox, "pAdxlMbox = NULL");

	HttpConnector* pHttpConnector = new HttpConnector("http");
	zassert_not_null(pHttpConnector, "memory alloc failed");
	zassert_not_null(pHttpConnector->pInMbox, "pInMbox alloc failed");
	zassert_not_null(pHttpConnector->delayTimer, "delayTimer alloc failed");
	zassert_not_null(pHttpConnector->oneDayTimer, "oneDayTimer alloc failed");
    zassert_equal(pHttpConnector->pInMbox->count(), 0, "pInMbox->count() != 0");
    zassert_equal(pHttpConnector->pInMbox, Mbox::getMboxById(Mbox::eHttpConnector),
                  "Error getting mailbox by id");

	pHttpConnector->lte_connected = false;
	pHttpConnector->bDispTransferEnabled = true;
	pHttpConnector->bDailyRawSendCntExceeded = false;
	Mbox::sendMsg(msgType::eSendDispDataReq, Mbox::eHttpConnector, Mbox::eHttpConnector);

    zassert_equal(pHttpConnector->pInMbox->count(), 1, "pInMbox->count() != 1");

    pHttpConnector->task(); 

	pElem = pAdxlMbox->read(); 
    zassert_equal(pElem->mtype, msgType::eAdxlResumeMeas, "Rcvd mtype != msgType::eAdxlResumeMeas");
    zassert_equal(pElem->from, Mbox::eHttpConnector, "Rcvd sender != Mbox::eHttpConnector");

	pElem->free();
	delete pAdxlMbox;
	delete pHttpConnector; 
}

//Test receive msg eSendDispDataReq 
ZTEST(tests_net_http_connector, test_http_connector_rcv_msg_eSendDispDataReq4)
{
    PoolElem* pElem;

	Mbox* pAdxlMbox = new Mbox("AdxlMbox");
    Mbox::registerMbox(pAdxlMbox, Mbox::eAdxl37xTask);
	zassert_not_null(pAdxlMbox, "pAdxlMbox = NULL");

	HttpConnector* pHttpConnector = new HttpConnector("http");
	zassert_not_null(pHttpConnector, "memory alloc failed");
	zassert_not_null(pHttpConnector->pInMbox, "pInMbox alloc failed");
	zassert_not_null(pHttpConnector->delayTimer, "delayTimer alloc failed");
	zassert_not_null(pHttpConnector->oneDayTimer, "oneDayTimer alloc failed");
    zassert_equal(pHttpConnector->pInMbox->count(), 0, "pInMbox->count() != 0");
    zassert_equal(pHttpConnector->pInMbox, Mbox::getMboxById(Mbox::eHttpConnector),
                  "Error getting mailbox by id");

	pHttpConnector->lte_connected = true;
	pHttpConnector->bDispTransferEnabled = false;
	pHttpConnector->bDailyRawSendCntExceeded = false;
	Mbox::sendMsg(msgType::eSendDispDataReq, Mbox::eHttpConnector, Mbox::eHttpConnector);

    zassert_equal(pHttpConnector->pInMbox->count(), 1, "pInMbox->count() != 1");

    pHttpConnector->task(); 

	pElem = pAdxlMbox->read(); 
    zassert_equal(pElem->mtype, msgType::eAdxlResumeMeas, "Rcvd mtype != msgType::eAdxlResumeMeas");
    zassert_equal(pElem->from, Mbox::eHttpConnector, "Rcvd sender != Mbox::eHttpConnector");

	pElem->free();
	delete pAdxlMbox;
	delete pHttpConnector; 
}

//Test receive msg eSendDispDataReq 
ZTEST(tests_net_http_connector, test_http_connector_rcv_msg_eSendDispDataReq5)
{
    PoolElem* pElem;

	Mbox* pAdxlMbox = new Mbox("AdxlMbox");
    Mbox::registerMbox(pAdxlMbox, Mbox::eAdxl37xTask);
	zassert_not_null(pAdxlMbox, "pAdxlMbox = NULL");

	HttpConnector* pHttpConnector = new HttpConnector("http");
	zassert_not_null(pHttpConnector, "memory alloc failed");
	zassert_not_null(pHttpConnector->pInMbox, "pInMbox alloc failed");
	zassert_not_null(pHttpConnector->delayTimer, "delayTimer alloc failed");
	zassert_not_null(pHttpConnector->oneDayTimer, "oneDayTimer alloc failed");
    zassert_equal(pHttpConnector->pInMbox->count(), 0, "pInMbox->count() != 0");
    zassert_equal(pHttpConnector->pInMbox, Mbox::getMboxById(Mbox::eHttpConnector),
                  "Error getting mailbox by id");

	pHttpConnector->lte_connected = true;
	pHttpConnector->bDispTransferEnabled = true;
	pHttpConnector->bDailyRawSendCntExceeded = true;
	Mbox::sendMsg(msgType::eSendDispDataReq, Mbox::eHttpConnector, Mbox::eHttpConnector);

    zassert_equal(pHttpConnector->pInMbox->count(), 1, "pInMbox->count() != 1");

    pHttpConnector->task(); 

	pElem = pAdxlMbox->read(); 
    zassert_equal(pElem->mtype, msgType::eAdxlResumeMeas, "Rcvd mtype != msgType::eAdxlResumeMeas");
    zassert_equal(pElem->from, Mbox::eHttpConnector, "Rcvd sender != Mbox::eHttpConnector");

	pElem->free();
	delete pAdxlMbox;
	delete pHttpConnector; 
}

//Test receive msg eSendDispDataReq 
ZTEST(tests_net_http_connector, test_http_connector_rcv_msg_eSendDispDataReq6)
{
    PoolElem* pElem;

	Mbox* pAdxlMbox = new Mbox("AdxlMbox");
    Mbox::registerMbox(pAdxlMbox, Mbox::eAdxl37xTask);
	zassert_not_null(pAdxlMbox, "pAdxlMbox = NULL");

	HttpConnector* pHttpConnector = new HttpConnector("http");
	zassert_not_null(pHttpConnector, "memory alloc failed");
	zassert_not_null(pHttpConnector->pInMbox, "pInMbox alloc failed");
	zassert_not_null(pHttpConnector->delayTimer, "delayTimer alloc failed");
	zassert_not_null(pHttpConnector->oneDayTimer, "oneDayTimer alloc failed");
    zassert_equal(pHttpConnector->pInMbox->count(), 0, "pInMbox->count() != 0");
    zassert_equal(pHttpConnector->pInMbox, Mbox::getMboxById(Mbox::eHttpConnector),
                  "Error getting mailbox by id");

	pHttpConnector->lte_connected = false;
	pHttpConnector->bDispTransferEnabled = true;
	pHttpConnector->bDailyRawSendCntExceeded = true;
	Mbox::sendMsg(msgType::eSendDispDataReq, Mbox::eHttpConnector, Mbox::eHttpConnector);

    zassert_equal(pHttpConnector->pInMbox->count(), 1, "pInMbox->count() != 1");

    pHttpConnector->task(); 

	pElem = pAdxlMbox->read(); 
    zassert_equal(pElem->mtype, msgType::eAdxlResumeMeas, "Rcvd mtype != msgType::eAdxlResumeMeas");
    zassert_equal(pElem->from, Mbox::eHttpConnector, "Rcvd sender != Mbox::eHttpConnector");

	pElem->free();
	delete pAdxlMbox;
	delete pHttpConnector; 
}

//Test receive msg eSendDispDataReq 
ZTEST(tests_net_http_connector, test_http_connector_rcv_msg_eSendDispDataReq7)
{
    PoolElem* pElem;

	Mbox* pAdxlMbox = new Mbox("AdxlMbox");
    Mbox::registerMbox(pAdxlMbox, Mbox::eAdxl37xTask);
	zassert_not_null(pAdxlMbox, "pAdxlMbox = NULL");

	HttpConnector* pHttpConnector = new HttpConnector("http");
	zassert_not_null(pHttpConnector, "memory alloc failed");
	zassert_not_null(pHttpConnector->pInMbox, "pInMbox alloc failed");
	zassert_not_null(pHttpConnector->delayTimer, "delayTimer alloc failed");
	zassert_not_null(pHttpConnector->oneDayTimer, "oneDayTimer alloc failed");
    zassert_equal(pHttpConnector->pInMbox->count(), 0, "pInMbox->count() != 0");
    zassert_equal(pHttpConnector->pInMbox, Mbox::getMboxById(Mbox::eHttpConnector),
                  "Error getting mailbox by id");

	pHttpConnector->lte_connected = true;
	pHttpConnector->bDispTransferEnabled = false;
	pHttpConnector->bDailyRawSendCntExceeded = true;
	Mbox::sendMsg(msgType::eSendDispDataReq, Mbox::eHttpConnector, Mbox::eHttpConnector);

    zassert_equal(pHttpConnector->pInMbox->count(), 1, "pInMbox->count() != 1");

    pHttpConnector->task(); 

	pElem = pAdxlMbox->read(); 
    zassert_equal(pElem->mtype, msgType::eAdxlResumeMeas, "Rcvd mtype != msgType::eAdxlResumeMeas");
    zassert_equal(pElem->from, Mbox::eHttpConnector, "Rcvd sender != Mbox::eHttpConnector");

	pElem->free();
	delete pAdxlMbox;
	delete pHttpConnector; 
}

//Test receive msg eSendDispDataReq 
ZTEST(tests_net_http_connector, test_http_connector_rcv_msg_eSendDispDataReq8)
{
    PoolElem* pElem;

	Mbox* pAdxlMbox = new Mbox("AdxlMbox");
    Mbox::registerMbox(pAdxlMbox, Mbox::eAdxl37xTask);
	zassert_not_null(pAdxlMbox, "pAdxlMbox = NULL");

	HttpConnector* pHttpConnector = new HttpConnector("http");
	zassert_not_null(pHttpConnector, "memory alloc failed");
	zassert_not_null(pHttpConnector->pInMbox, "pInMbox alloc failed");
	zassert_not_null(pHttpConnector->delayTimer, "delayTimer alloc failed");
	zassert_not_null(pHttpConnector->oneDayTimer, "oneDayTimer alloc failed");
    zassert_equal(pHttpConnector->pInMbox->count(), 0, "pInMbox->count() != 0");
    zassert_equal(pHttpConnector->pInMbox, Mbox::getMboxById(Mbox::eHttpConnector),
                  "Error getting mailbox by id");

	pHttpConnector->lte_connected = true;
	pHttpConnector->bDispTransferEnabled = true;
	pHttpConnector->bDailyRawSendCntExceeded = false;
	Mbox::sendMsg(msgType::eSendDispDataReq, Mbox::eHttpConnector, Mbox::eHttpConnector);

    zassert_equal(pHttpConnector->pInMbox->count(), 1, "pInMbox->count() != 1");

    gIntVal = 3;
    pHttpConnector->task(); 

	pElem = pAdxlMbox->read(); 
    zassert_equal(pElem->mtype, msgType::eDispDataTransferActive, "Rcvd mtype != msgType::eDispDataTransferActive");
    zassert_equal(pElem->from, Mbox::eHttpConnector, "Rcvd sender != Mbox::eHttpConnector");
	pElem->free();

	pElem = pAdxlMbox->read(); 
    zassert_equal(pElem->mtype, msgType::eDispDataTransferComplete, "Rcvd mtype != msgType::eDispDataTransferComplete");
    zassert_equal(pElem->from, Mbox::eHttpConnector, "Rcvd sender != Mbox::eHttpConnector");

	pElem->free();
	delete pAdxlMbox;
	delete pHttpConnector; 
}

//Test function StartHttpDispDataTransmission
ZTEST(tests_net_http_connector, test_http_connector_StartHttpDispDataTransmission)
{
    PoolElem* pElem;

	Mbox* pAdxlMbox = new Mbox("AdxlMbox");
    Mbox::registerMbox(pAdxlMbox, Mbox::eAdxl37xTask);
	zassert_not_null(pAdxlMbox, "pAdxlMbox = NULL");

	HttpConnector* pHttpConnector = new HttpConnector("http");
	zassert_not_null(pHttpConnector, "memory alloc failed");
	zassert_not_null(pHttpConnector->pInMbox, "pInMbox alloc failed");
	zassert_not_null(pHttpConnector->delayTimer, "delayTimer alloc failed");
	zassert_not_null(pHttpConnector->oneDayTimer, "oneDayTimer alloc failed");
    zassert_equal(pHttpConnector->pInMbox->count(), 0, "pInMbox->count() != 0");
    zassert_equal(pHttpConnector->pInMbox, Mbox::getMboxById(Mbox::eHttpConnector),
                  "Error getting mailbox by id");

    gIntVal = 3;
    pHttpConnector->StartHttpDispDataTransmission(); 

	pElem = pAdxlMbox->read(); 
    zassert_equal(pElem->mtype, msgType::eDispDataTransferActive, "Rcvd mtype != msgType::eDispDataTransferActive");
    zassert_equal(pElem->from, Mbox::eHttpConnector, "Rcvd sender != Mbox::eHttpConnector");
	pElem->free();

	pElem = pAdxlMbox->read(); 
    zassert_equal(pElem->mtype, msgType::eDispDataTransferComplete, "Rcvd mtype != msgType::eDispDataTransferComplete");
    zassert_equal(pElem->from, Mbox::eHttpConnector, "Rcvd sender != Mbox::eHttpConnector");

	pElem->free();
	delete pAdxlMbox;
	delete pHttpConnector; 
}

//Test function StartHttpDispDataTransmission
ZTEST(tests_net_http_connector, test_http_connector_StartHttpDispDataTransmission2)
{
    PoolElem* pElem;

	Mbox* pAdxlMbox = new Mbox("AdxlMbox");
    Mbox::registerMbox(pAdxlMbox, Mbox::eAdxl37xTask);
	zassert_not_null(pAdxlMbox, "pAdxlMbox = NULL");

	Mbox* pAdxlStoreMbox = new Mbox("AdxlStoreMbox");
    Mbox::registerMbox(pAdxlStoreMbox, Mbox::eAdxlStore);
	zassert_not_null(pAdxlStoreMbox, "pAdxlMbox = NULL");

	HttpConnector* pHttpConnector = new HttpConnector("http");
	zassert_not_null(pHttpConnector, "memory alloc failed");
	zassert_not_null(pHttpConnector->pInMbox, "pInMbox alloc failed");
	zassert_not_null(pHttpConnector->delayTimer, "delayTimer alloc failed");
	zassert_not_null(pHttpConnector->oneDayTimer, "oneDayTimer alloc failed");
    zassert_equal(pHttpConnector->pInMbox->count(), 0, "pInMbox->count() != 0");
    zassert_equal(pHttpConnector->pInMbox, Mbox::getMboxById(Mbox::eHttpConnector),
                  "Error getting mailbox by id");

    gIntVal = 0;
    pHttpConnector->StartHttpDispDataTransmission(); 

	pElem = pAdxlMbox->read(); 
    zassert_equal(pElem->mtype, msgType::eDispDataTransferActive, "Rcvd mtype != msgType::eDispDataTransferActive");
    zassert_equal(pElem->from, Mbox::eHttpConnector, "Rcvd sender != Mbox::eHttpConnector");
	pElem->free();

	pElem = pAdxlStoreMbox->read(); 
    zassert_equal(pElem->mtype, msgType::eDispData, "Rcvd mtype != msgType::eDispData");
    zassert_equal(pElem->from, Mbox::eHttpConnector, "Rcvd sender != Mbox::eHttpConnector");
	pElem->free();

	delete pAdxlMbox;
	delete pAdxlStoreMbox;
	delete pHttpConnector; 
}

//Test receive msg eDispData 
ZTEST(tests_net_http_connector, test_http_connector_rcv_msg_eDispData)
{
    PoolElem* pElem;

	Mbox* pAdxlMbox = new Mbox("AdxlMbox");
    Mbox::registerMbox(pAdxlMbox, Mbox::eAdxl37xTask);
	zassert_not_null(pAdxlMbox, "pAdxlMbox = NULL");

	HttpConnector* pHttpConnector = new HttpConnector("http");
	zassert_not_null(pHttpConnector, "memory alloc failed");
	zassert_not_null(pHttpConnector->pInMbox, "pInMbox alloc failed");
	zassert_not_null(pHttpConnector->delayTimer, "delayTimer alloc failed");
	zassert_not_null(pHttpConnector->oneDayTimer, "oneDayTimer alloc failed");
    zassert_equal(pHttpConnector->pInMbox->count(), 0, "pInMbox->count() != 0");
    zassert_equal(pHttpConnector->pInMbox, Mbox::getMboxById(Mbox::eHttpConnector),
                  "Error getting mailbox by id");

	pHttpConnector->requestedEntries = 100;
	pHttpConnector->rcvdEntries = 900;
	pHttpConnector->totalNumEntries = 1000;

    //struct AdxlRawDataProgress {
    //    uint32_t startRd;      
    //    uint32_t rcvdEntries;  
    //    uint32_t requestedEntries;  };
    struct AdxlRawDataProgress p = {0, 900, 100};
	Mbox::sendMsg(msgType::eDispData, Mbox::eHttpConnector, Mbox::eHttpConnector, (uint8_t*)&p, sizeof(p));

    zassert_equal(pHttpConnector->pInMbox->count(), 1, "pInMbox->count() != 1");

    pHttpConnector->task(); 
    zassert_equal(pAdxlMbox->count(), 2, "pAdxlMbox->count() != 2");

	pElem = pAdxlMbox->read(); 
    zassert_equal(pElem->mtype, msgType::eDispDataTransferComplete, "Rcvd mtype != msgType::eDispDataTransferComplete");
    zassert_equal(pElem->from, Mbox::eHttpConnector, "Rcvd sender != Mbox::eHttpConnector");

	pElem = pAdxlMbox->read(); 
    zassert_equal(pElem->mtype, msgType::eAdxlResumeMeas, "Rcvd mtype != msgType::eAdxlResumeMeas");
    zassert_equal(pElem->from, Mbox::eHttpConnector, "Rcvd sender != Mbox::eHttpConnector");

	pElem->free();
	delete pAdxlMbox;
	delete pHttpConnector; 
}

// ------------------   !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!111
//Test receive msg eDispData 
ZTEST(tests_net_http_connector, test_http_connector_rcv_msg_eDispData2)
{
    PoolElem* pElem;

	Mbox* pAdxlMbox = new Mbox("AdxlMbox");
    Mbox::registerMbox(pAdxlMbox, Mbox::eAdxl37xTask);
	zassert_not_null(pAdxlMbox, "pAdxlMbox = NULL");

	HttpConnector* pHttpConnector = new HttpConnector("http");
	zassert_not_null(pHttpConnector, "memory alloc failed");
	zassert_not_null(pHttpConnector->pInMbox, "pInMbox alloc failed");
	zassert_not_null(pHttpConnector->delayTimer, "delayTimer alloc failed");
	zassert_not_null(pHttpConnector->oneDayTimer, "oneDayTimer alloc failed");
    zassert_equal(pHttpConnector->pInMbox->count(), 0, "pInMbox->count() != 0");
    zassert_equal(pHttpConnector->pInMbox, Mbox::getMboxById(Mbox::eHttpConnector),
                  "Error getting mailbox by id");

	pHttpConnector->requestedEntries = 10;
	//pHttpConnector->rcvdEntries = 200;
	pHttpConnector->rcvdEntries = 900;
	pHttpConnector->totalNumEntries = 1000;

    //struct AdxlRawDataProgress {
    //    uint32_t startRd;      
    //    uint32_t rcvdEntries;  
    //    uint32_t requestedEntries;  };
    //struct AdxlRawDataProgress p = {0, 200, 100};
    struct AdxlRawDataProgress p = {0, 900, 10};
	Mbox::sendMsg(msgType::eDispData, Mbox::eHttpConnector, Mbox::eHttpConnector, (uint8_t*)&p, sizeof(p));

    zassert_equal(pHttpConnector->pInMbox->count(), 1, "pInMbox->count() != 1");

    pHttpConnector->task(); 
    zassert_equal(pAdxlMbox->count(), 2, "pAdxlMbox->count() != 2");

	pElem = pAdxlMbox->read(); 
    zassert_equal(pElem->mtype, msgType::eDispDataTransferComplete, "Rcvd mtype != msgType::eDispDataTransferComplete");
    zassert_equal(pElem->from, Mbox::eHttpConnector, "Rcvd sender != Mbox::eHttpConnector");

	pElem = pAdxlMbox->read(); 
    zassert_equal(pElem->mtype, msgType::eAdxlResumeMeas, "Rcvd mtype != msgType::eAdxlResumeMeas");
    zassert_equal(pElem->from, Mbox::eHttpConnector, "Rcvd sender != Mbox::eHttpConnector");

	pElem->free();
	delete pAdxlMbox;
	delete pHttpConnector; 
}

//Test receive unexpected msg 
ZTEST(tests_net_http_connector, test_http_connector_rcv_msg_unexpected)
{
	HttpConnector* pHttpConnector = new HttpConnector("http");
	zassert_not_null(pHttpConnector, "memory alloc failed");
	zassert_not_null(pHttpConnector->pInMbox, "pInMbox alloc failed");
	zassert_not_null(pHttpConnector->delayTimer, "delayTimer alloc failed");
	zassert_not_null(pHttpConnector->oneDayTimer, "oneDayTimer alloc failed");
    zassert_equal(pHttpConnector->pInMbox->count(), 0, "pInMbox->count() != 0");
    zassert_equal(pHttpConnector->pInMbox, Mbox::getMboxById(Mbox::eHttpConnector),
                  "Error getting mailbox by id");

    Mbox::sendMsg(msgType::eResetCause, Mbox::eHttpConnector, Mbox::eHttpConnector);

    zassert_equal(pHttpConnector->pInMbox->count(), 1, "pInMbox->count() != 1");

    pHttpConnector->task();  

    zassert_equal(pHttpConnector->pInMbox->count(), 0, "pInMbox->count() != 0");
	delete pHttpConnector; 
}

//Test receive no msg 
ZTEST(tests_net_http_connector, test_http_connector_rcv_msg_none)
{
	HttpConnector* pHttpConnector = new HttpConnector("http");
	zassert_not_null(pHttpConnector, "memory alloc failed");
	zassert_not_null(pHttpConnector->pInMbox, "pInMbox alloc failed");
	zassert_not_null(pHttpConnector->delayTimer, "delayTimer alloc failed");
	zassert_not_null(pHttpConnector->oneDayTimer, "oneDayTimer alloc failed");
    zassert_equal(pHttpConnector->pInMbox->count(), 0, "pInMbox->count() != 0");
    zassert_equal(pHttpConnector->pInMbox, Mbox::getMboxById(Mbox::eHttpConnector),
                  "Error getting mailbox by id");

    pHttpConnector->pInMbox->write(NULL);
    
    pHttpConnector->task();  

    zassert_equal(pHttpConnector->pInMbox->count(), 0, "pInMbox->count() != 0");
	delete pHttpConnector; 
}
// +++++++++++++++++++++++++++++++++++++
//Test function HttpRawDataSend
ZTEST(tests_net_http_connector, test_http_connector_HttpRawDataSend)
{
    PoolElem* pElem;

	Mbox* pHttpMbox = new Mbox("HttpMbox");
    Mbox::registerMbox(pHttpMbox, Mbox::eHttpConnector);
	zassert_not_null(pHttpMbox, "pHttpMbox = NULL");

	HttpConnector::HttpRawDataSend(NULL, 0, NULL);

	pElem = pHttpMbox->read(); 
    zassert_equal(pElem->mtype, msgType::eAdxlIdleReq, "Rcvd mtype != msgType::eAdxlIdleReq");
    zassert_equal(pElem->from, Mbox::eHttpConnector, "Rcvd sender != Mbox::eHttpConnector");

	pElem->free();
	delete pHttpMbox;
}

//Test function HttpDispDataSend
ZTEST(tests_net_http_connector, test_http_connector_HttpDispDataSend)
{
    PoolElem* pElem;

	Mbox* pHttpMbox = new Mbox("HttpMbox");
    Mbox::registerMbox(pHttpMbox, Mbox::eHttpConnector);
	zassert_not_null(pHttpMbox, "pHttpMbox = NULL");

	HttpConnector::HttpDispDataSend(NULL, 0, NULL);

	pElem = pHttpMbox->read(); 
    zassert_equal(pElem->mtype, msgType::eSendDispDataReq, "Rcvd mtype != msgType::eSendDispDataReq");
    zassert_equal(pElem->from, Mbox::eHttpConnector, "Rcvd sender != Mbox::eHttpConnector");

	pElem->free();
	delete pHttpMbox;
}
