#include <zephyr/ztest.h>

#define private public
#include "stubs.h"
#include "drv/adc/adconverter.h"


ZTEST_SUITE(tests_drv_ADConverter, NULL, NULL, NULL, NULL, NULL);

// test constructor and destructor
ZTEST(tests_drv_ADConverter, test_drv_ADConverter_constructor)
{
	ADConverter* pAdc = new ADConverter("adc");
	zassert_not_null(pAdc, "memory alloc failed");
	zassert_not_null(pAdc->pInMbox, "pInMbox = NULL");
    zassert_equal(pAdc->pInMbox->count(), 0, "pInMbox->count() != 0");
    zassert_equal(pAdc->pInMbox, Mbox::getMboxById(Mbox::eADConverter),
                  "Error getting mailbox by id");

	delete pAdc;
}

// MISSING for 100% branch coverage static instantiation does not deliver NULL
// here code lines
// pInMbox = new Mbox("Adc");
// static PoolElem e1 = {eTimerExpired, Mbox::MboxId::eADConverter, 'a', 'd'};
// are not covered completly.

// test function initDevice
ZTEST(tests_drv_ADConverter, test_drv_ADConverter_initDevice)
{
	ADConverter* pAdc = new ADConverter("adc");
	zassert_not_null(pAdc, "memory alloc failed");
	zassert_not_null(pAdc->pInMbox, "pInMbox = NULL");
    zassert_equal(pAdc->pInMbox->count(), 0, "pInMbox->count() != 0");
    zassert_equal(pAdc->pInMbox, Mbox::getMboxById(Mbox::eADConverter),
                  "Error getting mailbox by id");

	pAdc->initDevice();

	delete pAdc;
}

//!< Structure for an user-specific timer data
struct AdcTimerInfo {
    ADConverter* self;  //!< Pointer to AD converter instance
    uint8_t ch_mask;    //!< The AD channel bitmask
};

// test function timer_cb and timer1_expired
ZTEST(tests_drv_ADConverter, test_drv_ADConverter_timer_cb)
{
    PoolElem* pElem;

	ADConverter* pAdc = new ADConverter("adc");
	zassert_not_null(pAdc, "memory alloc failed");
	zassert_not_null(pAdc->pInMbox, "pInMbox = NULL");
    zassert_equal(pAdc->pInMbox->count(), 0, "pInMbox->count() != 0");
    zassert_equal(pAdc->pInMbox, Mbox::getMboxById(Mbox::eADConverter),
                  "Error getting mailbox by id");

    //ADConverter* self;  //!< Pointer to AD converter instance
    //uint8_t ch_mask;    //!< The AD channel bitmask
	struct AdcTimerInfo adcinfo = {pAdc, 27};

    pAdc->timer_cb(reinterpret_cast<k_timer*>(&adcinfo));

    zassert_equal(pAdc->pInMbox->count(), 1, "pInMbox->count() != 1");
    pElem = pAdc->pInMbox->read();
    zassert_equal(pElem->mtype, msgType::eTimerExpired, "Rcvd mtype != msgType::eTimerExpired");

    delete pAdc;
}    

// test function notifyListener
ZTEST(tests_drv_ADConverter, test_drv_ADConverter_notifyListener)
{
    PoolElem* pElem;

	Mbox* pHttpConnectorMbox = new Mbox("HttpConnectorMbox");
    Mbox::registerMbox(pHttpConnectorMbox, Mbox::eHttpConnector);
	zassert_not_null(pHttpConnectorMbox, "pHttpConnectorMbox = NULL");

	ADConverter* pAdc = new ADConverter("adc");
	zassert_not_null(pAdc, "memory alloc failed");
	zassert_not_null(pAdc->pInMbox, "pInMbox = NULL");
    zassert_equal(pAdc->pInMbox->count(), 0, "pInMbox->count() != 0");
    zassert_equal(pAdc->pInMbox, Mbox::getMboxById(Mbox::eADConverter),
                  "Error getting mailbox by id");

    pAdc->adc_listeners[0] = pHttpConnectorMbox;
	pAdc->notifyListener(0, 17, 27);

    pElem = pHttpConnectorMbox->read();
    zassert_equal(pElem->mtype, msgType::eAdcData, "Rcvd mtype != msgType::eAdcData");
    zassert_equal(pElem->from, Mbox::eADConverter, "Rcvd sender != Mbox::eADConverter");

	struct ADInput ad_in = *reinterpret_cast<struct ADInput*>(pElem->getBuffer());
    zassert_equal(ad_in.channel, 0, "wrong channel");
    zassert_equal(ad_in.raw, 17, "wrong raw");
    zassert_equal(ad_in.mv, 27, "wrong mv");
	
    pElem->free();
    delete pHttpConnectorMbox;
    delete pAdc;
}    

// test function sample
ZTEST(tests_drv_ADConverter, test_drv_ADConverter_sample)
{
	ADConverter* pAdc = new ADConverter("adc");
	zassert_not_null(pAdc, "memory alloc failed");
	zassert_not_null(pAdc->pInMbox, "pInMbox = NULL");
    zassert_equal(pAdc->pInMbox->count(), 0, "pInMbox->count() != 0");
    zassert_equal(pAdc->pInMbox, Mbox::getMboxById(Mbox::eADConverter),
                  "Error getting mailbox by id");

	pAdc->sample(0xFF);
	pAdc->sample(0x00);

    delete pAdc;
}    

//Test receive msg eTimerExpired
ZTEST(tests_drv_ADConverter, test_drv_ADConverter_rcv_msg_eTimerExpired)
{
	uint8_t p1 = 0xFF;

	ADConverter* pAdc = new ADConverter("adc");
	zassert_not_null(pAdc, "memory alloc failed");
	zassert_not_null(pAdc->pInMbox, "pInMbox = NULL");
    zassert_equal(pAdc->pInMbox->count(), 0, "pInMbox->count() != 0");
    zassert_equal(pAdc->pInMbox, Mbox::getMboxById(Mbox::eADConverter),
                  "Error getting mailbox by id");

    Mbox::sendMsg(msgType::eTimerExpired, Mbox::eADConverter, Mbox::eADConverter, p1);

    zassert_equal(pAdc->pInMbox->count(), 1, "pInMbox->count() != 1");

    pAdc->task();  

    zassert_equal(pAdc->pInMbox->count(), 0, "pInMbox->count() != 0");

	delete pAdc; 
}

//Test receive msg eAdcReport
ZTEST(tests_drv_ADConverter, test_drv_ADConverter_rcv_msg_eAdcReport)
{
	uint8_t p1 = 0xFF;

	ADConverter* pAdc = new ADConverter("adc");
	zassert_not_null(pAdc, "memory alloc failed");
	zassert_not_null(pAdc->pInMbox, "pInMbox = NULL");
    zassert_equal(pAdc->pInMbox->count(), 0, "pInMbox->count() != 0");
    zassert_equal(pAdc->pInMbox, Mbox::getMboxById(Mbox::eADConverter),
                  "Error getting mailbox by id");

    Mbox::sendMsg(msgType::eAdcReport, Mbox::eADConverter, Mbox::eADConverter, p1);

    zassert_equal(pAdc->pInMbox->count(), 1, "pInMbox->count() != 1");

    pAdc->task();  

    zassert_equal(pAdc->pInMbox->count(), 0, "pInMbox->count() != 0");

	delete pAdc; 
}

//Test receive unexpected msg 
ZTEST(tests_drv_ADConverter, test_drv_ADConverter_rcv_msg_unexpected)
{
	ADConverter* pAdc = new ADConverter("adc");
	zassert_not_null(pAdc, "memory alloc failed");
	zassert_not_null(pAdc->pInMbox, "pInMbox = NULL");
    zassert_equal(pAdc->pInMbox->count(), 0, "pInMbox->count() != 0");
    zassert_equal(pAdc->pInMbox, Mbox::getMboxById(Mbox::eADConverter),
                  "Error getting mailbox by id");

    Mbox::sendMsg(msgType::eResetCause, Mbox::eADConverter, Mbox::eADConverter);

    zassert_equal(pAdc->pInMbox->count(), 1, "pInMbox->count() != 1");

    pAdc->task();  

    zassert_equal(pAdc->pInMbox->count(), 0, "pInMbox->count() != 0");
	delete pAdc; 
}

//Test receive no msg 
ZTEST(tests_drv_ADConverter, test_drv_ADConverter_rcv_msg_none)
{
	ADConverter* pAdc = new ADConverter("adc");
	zassert_not_null(pAdc, "memory alloc failed");
	zassert_not_null(pAdc->pInMbox, "pInMbox = NULL");
    zassert_equal(pAdc->pInMbox->count(), 0, "pInMbox->count() != 0");
    zassert_equal(pAdc->pInMbox, Mbox::getMboxById(Mbox::eADConverter),
                  "Error getting mailbox by id");

    pAdc->pInMbox->write(NULL);
    
    pAdc->task();  

    zassert_equal(pAdc->pInMbox->count(), 0, "pInMbox->count() != 0");
	delete pAdc; 
}

