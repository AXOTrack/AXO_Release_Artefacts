#include <zephyr/ztest.h>
#include <vector>
#include <algorithm>

#define private public
#include "net/mqtt_connector.h"

#include "stubs.h"

ZTEST_SUITE(tests_net_mqtt_connector, NULL, NULL, NULL, NULL, NULL);

// test constructor and destructor
ZTEST(tests_net_mqtt_connector, test_mqtt_connector_constructor)
{
	MqttConnector* pMqttConnector = new MqttConnector("Mqtt");
	zassert_not_null(pMqttConnector, "memory alloc failed");
	zassert_not_null(pMqttConnector->pInMbox, "pInMbox alloc failed");
	zassert_not_null(pMqttConnector->myTimer, "myTimer alloc failed");
	//printk("%s\n\n", pMqttConnector->measurement_topic);
	zassert(strcmp(pMqttConnector->measurement_topic, "/device/123456789012345/measurement/update")==0, "measurement_topic wrong");
	zassert(strcmp(pMqttConnector->metrics_topic, "/device/123456789012345/metric/update")==0, "metrics_topic wrong");
	zassert(strcmp(pMqttConnector->pubkey_topic, "/device/123456789012345/pubkey")==0, "pubkey_topic wrong");
	zassert(strcmp(pMqttConnector->command_topic, "/device/123456789012345/command")==0, "command_topic wrong");
	zassert(strcmp(pMqttConnector->ota_topic, "/device/123456789012345/ota/update")==0, "ota_topic wrong");
	zassert(strcmp(pMqttConnector->event_topic, "/device/123456789012345/event")==0, "event_topic wrong");
	zassert(strcmp(pMqttConnector->config_topic, "/device/123456789012345/config/update")==0, "config_topic wrong");

	delete pMqttConnector;
}

// MISSING for 100% branch coverage operator new does not deliver NULL
// here code lines 
// pInMbox = new Mbox(name); 
// myTimer = new Timer(MqttConnector::timer_cb, this, CONNECT_RETRY_DURATION, CONNECT_RETRY_DURATION);
// static PoolElem e1 = {eTimerExpired, Mbox::MboxId::eMqttConnector, 'm', 'q'};
// are not covered completly.
// NOTE: Feature Broker Switching was not tested, because it will be removed in the future!

// test callback function on_connack
ZTEST(tests_net_mqtt_connector, test_mqtt_connector_on_connack)
{
    PoolElem* pElem;

	Mbox* pMqttConnMbox = new Mbox("MqttConnectorMbox");
    Mbox::registerMbox(pMqttConnMbox, Mbox::eMqttConnector);
	zassert_not_null(pMqttConnMbox, "pLteConnMbox = NULL");

	MqttConnector::on_connack(MQTT_CONNECTION_ACCEPTED);
    zassert_equal(pMqttConnMbox->count(), 1, "pMqttConnMbox->count() != 1");

    pElem = pMqttConnMbox->read();
    zassert_equal(pElem->mtype, msgType::eMqttConnected, "Rcvd mtype != msgType::eMqttConnected");
    zassert_equal(pElem->from, Mbox::eMqttConnector, "Rcvd sender != Mbox::eMqttConnector");

    pElem->free();
    delete pMqttConnMbox;
}

// test callback function on_connack
ZTEST(tests_net_mqtt_connector, test_mqtt_connector_on_connack2)
{
	MqttConnector::on_connack(MQTT_UNACCEPTABLE_PROTOCOL_VERSION);
}

// test callback function on_disconnect
ZTEST(tests_net_mqtt_connector, test_mqtt_connector_on_disconnect)
{
    PoolElem* pElem;

	Mbox* pMqttConnMbox = new Mbox("MqttConnectorMbox");
    Mbox::registerMbox(pMqttConnMbox, Mbox::eMqttConnector);
	zassert_not_null(pMqttConnMbox, "pLteConnMbox = NULL");

	MqttConnector::on_disconnect(0);
    zassert_equal(pMqttConnMbox->count(), 1, "pMqttConnMbox->count() != 1");

    pElem = pMqttConnMbox->read();
    zassert_equal(pElem->mtype, msgType::eMqttDisconnected, "Rcvd mtype != msgType::eMqttDisconnected");
    zassert_equal(pElem->from, Mbox::eMqttConnector, "Rcvd sender != Mbox::eMqttConnector");

    pElem->free();
    delete pMqttConnMbox;
}

// test callback function on_publish
ZTEST(tests_net_mqtt_connector, test_mqtt_connector_on_publish)
{
    PoolElem* pElem;
	struct mqtt_helper_buf topic;
	struct mqtt_helper_buf payload;
	char tbuf[128];
	char pbuf[1024];

	Mbox* pMqttConnMbox = new Mbox("MqttConnectorMbox");
    Mbox::registerMbox(pMqttConnMbox, Mbox::eMqttConnector);
	zassert_not_null(pMqttConnMbox, "pMqttConnMbox = NULL");

	strcpy(tbuf, "/device/123456789012345/config/update");
	strcpy(pbuf, "egal");
    topic.ptr = tbuf;
	topic.size = strlen(tbuf);
    payload.ptr = pbuf;
	payload.size = strlen(pbuf);

	MqttConnector::on_publish(topic, payload);
	printk("count = %d\n", pMqttConnMbox->count());
    zassert_equal(pMqttConnMbox->count(), 1, "pMqttConnMbox->count() != 1");
    pElem = pMqttConnMbox->read();
    zassert_equal(pElem->mtype, msgType::eJsonConfig, "Rcvd mtype != msgType::eJsonConfig");
    zassert_equal(pElem->from, Mbox::eMqttConnector, "Rcvd sender != Mbox::eMqttConnector");
    pElem->free();

    delete pMqttConnMbox;
}

// test callback function on_publish
ZTEST(tests_net_mqtt_connector, test_mqtt_connector_on_publish2)
{
    PoolElem* pElem;
	struct mqtt_helper_buf topic;
	struct mqtt_helper_buf payload;
	char tbuf[128];
	char pbuf[1024];

	Mbox* pFwUpdCtrl = new Mbox("FwUpdCtrlMbox");
    Mbox::registerMbox(pFwUpdCtrl, Mbox::eFwUpdCtrl);
	zassert_not_null(pFwUpdCtrl, "pFwUpdCtrl = NULL");

	strcpy(tbuf, "egal");
	strcpy(pbuf, "OTA_START");
    topic.ptr = tbuf;
	topic.size = strlen(tbuf);
    payload.ptr = pbuf;
	payload.size = strlen(pbuf);

	MqttConnector::on_publish(topic, payload);
    zassert_equal(pFwUpdCtrl->count(), 1, "pFwUpdCtrl->count() != 1");
    pElem = pFwUpdCtrl->read();
    zassert_equal(pElem->mtype, msgType::eOtaStart, "Rcvd mtype != msgType::eOtaStart / %d", pElem->mtype);
    zassert_equal(pElem->from, Mbox::eMqttConnector, "Rcvd sender != Mbox::eMqttConnector");
    pElem->free();

	delete pFwUpdCtrl;
}

// test callback function on_publish
ZTEST(tests_net_mqtt_connector, test_mqtt_connector_on_publish3)
{
    PoolElem* pElem;
	struct mqtt_helper_buf topic;
	struct mqtt_helper_buf payload;
	char tbuf[128];
	char pbuf[1024];

	Mbox* pFwUpdCtrl = new Mbox("FwUpdCtrlMbox");
    Mbox::registerMbox(pFwUpdCtrl, Mbox::eFwUpdCtrl);
	zassert_not_null(pFwUpdCtrl, "pFwUpdCtrl = NULL");

	strcpy(tbuf, "egal");
	strcpy(pbuf, "fw_version");
    topic.ptr = tbuf;
	topic.size = strlen(tbuf);
    payload.ptr = pbuf;
	payload.size = strlen(pbuf);

	MqttConnector::on_publish(topic, payload);
    zassert_equal(pFwUpdCtrl->count(), 1, "pFwUpdCtrl->count() != 1");
    pElem = pFwUpdCtrl->read();
    zassert_equal(pElem->mtype, msgType::eFwInfo, "Rcvd mtype != msgType::eFwInfo");
    zassert_equal(pElem->from, Mbox::eMqttConnector, "Rcvd sender != Mbox::eMqttConnector");
    pElem->free();

	delete pFwUpdCtrl;
}

// test callback function on_publish
ZTEST(tests_net_mqtt_connector, test_mqtt_connector_on_publish4)
{
	struct mqtt_helper_buf topic;
	struct mqtt_helper_buf payload;
	char tbuf[128];
	char pbuf[1024];

	strcpy(tbuf, "egal");
	strcpy(pbuf, "RESET");
    topic.ptr = tbuf;
	topic.size = strlen(tbuf);
    payload.ptr = pbuf;
	payload.size = strlen(pbuf);

	MqttConnector::on_publish(topic, payload);
}

// test callback function on_publish
ZTEST(tests_net_mqtt_connector, test_mqtt_connector_on_publish5)
{
    PoolElem* pElem;
	struct mqtt_helper_buf topic;
	struct mqtt_helper_buf payload;
	char tbuf[128];
	char pbuf[1024];

	Mbox* pMqttConnMbox = new Mbox("MqttConnectorMbox");
    Mbox::registerMbox(pMqttConnMbox, Mbox::eMqttConnector);
	zassert_not_null(pMqttConnMbox, "pMqttConnMbox = NULL");

	strcpy(tbuf, "egal");
	strcpy(pbuf, "CFG_FACTORY_DEFAULT");
    topic.ptr = tbuf;
	topic.size = strlen(tbuf);
    payload.ptr = pbuf;
	payload.size = strlen(pbuf);

	MqttConnector::on_publish(topic, payload);
	printk("count = %d\n", pMqttConnMbox->count());
    zassert_equal(pMqttConnMbox->count(), 1, "pMqttConnMbox->count() != 1 / %d", pMqttConnMbox->count());
    pElem = pMqttConnMbox->read();
    zassert_equal(pElem->mtype, msgType::eFactoryDefault, "Rcvd mtype != msgType::eFactoryDefault");
    zassert_equal(pElem->from, Mbox::eMqttConnector, "Rcvd sender != Mbox::eMqttConnector");
    pElem->free();

    delete pMqttConnMbox;

}

// test callback function on_publish
ZTEST(tests_net_mqtt_connector, test_mqtt_connector_on_publish6)
{
    PoolElem* pElem;
	struct mqtt_helper_buf topic;
	struct mqtt_helper_buf payload;
	char tbuf[128];
	char pbuf[1024];

	Mbox* pMqttConnMbox = new Mbox("MqttConnectorMbox");
    Mbox::registerMbox(pMqttConnMbox, Mbox::eMqttConnector);
	zassert_not_null(pMqttConnMbox, "pMqttConnMbox = NULL");

	strcpy(tbuf, "egal");
	strcpy(pbuf, "COLDSTART");
    topic.ptr = tbuf;
	topic.size = strlen(tbuf);
    payload.ptr = pbuf;
	payload.size = strlen(pbuf);

	MqttConnector::on_publish(topic, payload);
	printk("count = %d\n", pMqttConnMbox->count());
    zassert_equal(pMqttConnMbox->count(), 1, "pMqttConnMbox->count() != 1");
    pElem = pMqttConnMbox->read();
    zassert_equal(pElem->mtype, msgType::eColdStartReq, "Rcvd mtype != msgType::eColdStartReq");
    zassert_equal(pElem->from, Mbox::eMqttConnector, "Rcvd sender != Mbox::eMqttConnector");
    pElem->free();

    delete pMqttConnMbox;
}

// test callback function on_publish
ZTEST(tests_net_mqtt_connector, test_mqtt_connector_on_publish7)
{
    PoolElem* pElem;
	struct mqtt_helper_buf topic;
	struct mqtt_helper_buf payload;
	char tbuf[128];
	char pbuf[1024];

	Mbox* pMqttConnMbox = new Mbox("MqttConnectorMbox");
    Mbox::registerMbox(pMqttConnMbox, Mbox::eMqttConnector);
	zassert_not_null(pMqttConnMbox, "pMqttConnMbox = NULL");

	strcpy(tbuf, "egal");
	strcpy(pbuf, "BOOST_ON");
    topic.ptr = tbuf;
	topic.size = strlen(tbuf);
    payload.ptr = pbuf;
	payload.size = strlen(pbuf);

	MqttConnector::on_publish(topic, payload);
    zassert_equal(pMqttConnMbox->count(), 1, "pMqttConnMbox->count() != 1");
    pElem = pMqttConnMbox->read();
    zassert_equal(pElem->mtype, msgType::eBoostOnReq, "Rcvd mtype != msgType::eBoostOnReq");
    zassert_equal(pElem->from, Mbox::eMqttConnector, "Rcvd sender != Mbox::eMqttConnector");
    pElem->free();

    delete pMqttConnMbox;
}

// test callback function on_publish
ZTEST(tests_net_mqtt_connector, test_mqtt_connector_on_publish8)
{
    PoolElem* pElem;
	struct mqtt_helper_buf topic;
	struct mqtt_helper_buf payload;
	char tbuf[128];
	char pbuf[1024];

	Mbox* pMqttConnMbox = new Mbox("MqttConnectorMbox");
    Mbox::registerMbox(pMqttConnMbox, Mbox::eMqttConnector);
	zassert_not_null(pMqttConnMbox, "pMqttConnMbox = NULL");

	strcpy(tbuf, "egal");
	strcpy(pbuf, "BOOST_OFF");
    topic.ptr = tbuf;
	topic.size = strlen(tbuf);
    payload.ptr = pbuf;
	payload.size = strlen(pbuf);

	MqttConnector::on_publish(topic, payload);
    zassert_equal(pMqttConnMbox->count(), 1, "pMqttConnMbox->count() != 1");
    pElem = pMqttConnMbox->read();
    zassert_equal(pElem->mtype, msgType::eBoostOffReq, "Rcvd mtype != msgType::eBoostOffReq");
    zassert_equal(pElem->from, Mbox::eMqttConnector, "Rcvd sender != Mbox::eMqttConnector");
    pElem->free();

    delete pMqttConnMbox;
}

// test callback function on_publish
ZTEST(tests_net_mqtt_connector, test_mqtt_connector_on_publish9)
{
    PoolElem* pElem;
	struct mqtt_helper_buf topic;
	struct mqtt_helper_buf payload;
	char tbuf[128];
	char pbuf[1024];

	Mbox* pMqttConnMbox = new Mbox("MqttConnectorMbox");
    Mbox::registerMbox(pMqttConnMbox, Mbox::eMqttConnector);
	zassert_not_null(pMqttConnMbox, "pMqttConnMbox = NULL");

	strcpy(tbuf, "egal");
	strcpy(pbuf, "CONNECT_JOVIA");
    topic.ptr = tbuf;
	topic.size = strlen(tbuf);
    payload.ptr = pbuf;
	payload.size = strlen(pbuf);

	MqttConnector::on_publish(topic, payload);
    zassert_equal(pMqttConnMbox->count(), 1, "pMqttConnMbox->count() != 1");
    pElem = pMqttConnMbox->read();
    zassert_equal(pElem->mtype, msgType::eConnectJovia, "Rcvd mtype != msgType::eConnectJovia");
    zassert_equal(pElem->from, Mbox::eMqttConnector, "Rcvd sender != Mbox::eMqttConnector");
    pElem->free();

    delete pMqttConnMbox;
}

// test callback function on_publish
ZTEST(tests_net_mqtt_connector, test_mqtt_connector_on_publish10)
{
    PoolElem* pElem;
	struct mqtt_helper_buf topic;
	struct mqtt_helper_buf payload;
	char tbuf[128];
	char pbuf[1024];

	Mbox* pMqttConnMbox = new Mbox("MqttConnectorMbox");
    Mbox::registerMbox(pMqttConnMbox, Mbox::eMqttConnector);
	zassert_not_null(pMqttConnMbox, "pMqttConnMbox = NULL");

	strcpy(tbuf, "egal");
	strcpy(pbuf, "CONNECT_TB");
    topic.ptr = tbuf;
	topic.size = strlen(tbuf);
    payload.ptr = pbuf;
	payload.size = strlen(pbuf);

	MqttConnector::on_publish(topic, payload);
    zassert_equal(pMqttConnMbox->count(), 1, "pMqttConnMbox->count() != 1");
    pElem = pMqttConnMbox->read();
    zassert_equal(pElem->mtype, msgType::eConnectThingsboard, "Rcvd mtype != msgType::eConnectThingsboard");
    zassert_equal(pElem->from, Mbox::eMqttConnector, "Rcvd sender != Mbox::eMqttConnector");
    pElem->free();

    delete pMqttConnMbox;
}

// test callback function on_publish
ZTEST(tests_net_mqtt_connector, test_mqtt_connector_on_publish11)
{
    PoolElem* pElem;
	struct mqtt_helper_buf topic;
	struct mqtt_helper_buf payload;
	char tbuf[128];
	char pbuf[1024];

	Mbox* pMqttConnMbox = new Mbox("MqttConnectorMbox");
    Mbox::registerMbox(pMqttConnMbox, Mbox::eMqttConnector);
	zassert_not_null(pMqttConnMbox, "pMqttConnMbox = NULL");

	strcpy(tbuf, "egal");
	strcpy(pbuf, "CERT_JOVIA1");
    topic.ptr = tbuf;
	topic.size = strlen(tbuf);
    payload.ptr = pbuf;
	payload.size = strlen(pbuf);

	MqttConnector::on_publish(topic, payload);
    zassert_equal(pMqttConnMbox->count(), 1, "pMqttConnMbox->count() != 1");
    pElem = pMqttConnMbox->read();
    zassert_equal(pElem->mtype, msgType::eCertJovia1, "Rcvd mtype != msgType::eCertJovia1");
    zassert_equal(pElem->from, Mbox::eMqttConnector, "Rcvd sender != Mbox::eMqttConnector");
    pElem->free();

    delete pMqttConnMbox;
}

// test callback function on_publish
ZTEST(tests_net_mqtt_connector, test_mqtt_connector_on_publish12)
{
    PoolElem* pElem;
	struct mqtt_helper_buf topic;
	struct mqtt_helper_buf payload;
	char tbuf[128];
	char pbuf[1024];

	Mbox* pMqttConnMbox = new Mbox("MqttConnectorMbox");
    Mbox::registerMbox(pMqttConnMbox, Mbox::eMqttConnector);
	zassert_not_null(pMqttConnMbox, "pMqttConnMbox = NULL");

	strcpy(tbuf, "egal");
	strcpy(pbuf, "CERT_JOVIA2");
    topic.ptr = tbuf;
	topic.size = strlen(tbuf);
    payload.ptr = pbuf;
	payload.size = strlen(pbuf);

	MqttConnector::on_publish(topic, payload);
    zassert_equal(pMqttConnMbox->count(), 1, "pMqttConnMbox->count() != 1");
    pElem = pMqttConnMbox->read();
    zassert_equal(pElem->mtype, msgType::eCertJovia2, "Rcvd mtype != msgType::eCertJovia2");
    zassert_equal(pElem->from, Mbox::eMqttConnector, "Rcvd sender != Mbox::eMqttConnector");
    pElem->free();

    delete pMqttConnMbox;
}

// test callback function on_publish
ZTEST(tests_net_mqtt_connector, test_mqtt_connector_on_publish13)
{
    PoolElem* pElem;
	struct mqtt_helper_buf topic;
	struct mqtt_helper_buf payload;
	char tbuf[128];
	char pbuf[1024];

	Mbox* pMqttConnMbox = new Mbox("MqttConnectorMbox");
    Mbox::registerMbox(pMqttConnMbox, Mbox::eMqttConnector);
	zassert_not_null(pMqttConnMbox, "pMqttConnMbox = NULL");

	strcpy(tbuf, "egal");
	strcpy(pbuf, "KEY_JOVIA");
    topic.ptr = tbuf;
	topic.size = strlen(tbuf);
    payload.ptr = pbuf;
	payload.size = strlen(pbuf);

	MqttConnector::on_publish(topic, payload);
    zassert_equal(pMqttConnMbox->count(), 1, "pMqttConnMbox->count() != 1");
    pElem = pMqttConnMbox->read();
    zassert_equal(pElem->mtype, msgType::eCertJoviaKey, "Rcvd mtype != msgType::eCertJoviaKey");
    zassert_equal(pElem->from, Mbox::eMqttConnector, "Rcvd sender != Mbox::eMqttConnector");
    pElem->free();

    delete pMqttConnMbox;
}

// test callback function on_publish
ZTEST(tests_net_mqtt_connector, test_mqtt_connector_on_publish14)
{
    PoolElem* pElem;
	struct mqtt_helper_buf topic;
	struct mqtt_helper_buf payload;
	char tbuf[128];
	char pbuf[1024];

	Mbox* pMqttConnMbox = new Mbox("MqttConnectorMbox");
    Mbox::registerMbox(pMqttConnMbox, Mbox::eMqttConnector);
	zassert_not_null(pMqttConnMbox, "pMqttConnMbox = NULL");

	strcpy(tbuf, "egal");
	strcpy(pbuf, "CERT_TB1");
    topic.ptr = tbuf;
	topic.size = strlen(tbuf);
    payload.ptr = pbuf;
	payload.size = strlen(pbuf);

	MqttConnector::on_publish(topic, payload);
    zassert_equal(pMqttConnMbox->count(), 1, "pMqttConnMbox->count() != 1");
    pElem = pMqttConnMbox->read();
    zassert_equal(pElem->mtype, msgType::eCertThingsboard1, "Rcvd mtype != msgType::eCertThingsboard1");
    zassert_equal(pElem->from, Mbox::eMqttConnector, "Rcvd sender != Mbox::eMqttConnector");
    pElem->free();

    delete pMqttConnMbox;
}

// test callback function on_publish
ZTEST(tests_net_mqtt_connector, test_mqtt_connector_on_publish15)
{
    PoolElem* pElem;
	struct mqtt_helper_buf topic;
	struct mqtt_helper_buf payload;
	char tbuf[128];
	char pbuf[1024];

	Mbox* pMqttConnMbox = new Mbox("MqttConnectorMbox");
    Mbox::registerMbox(pMqttConnMbox, Mbox::eMqttConnector);
	zassert_not_null(pMqttConnMbox, "pMqttConnMbox = NULL");

	strcpy(tbuf, "egal");
	strcpy(pbuf, "CERT_TB2");
    topic.ptr = tbuf;
	topic.size = strlen(tbuf);
    payload.ptr = pbuf;
	payload.size = strlen(pbuf);

	MqttConnector::on_publish(topic, payload);
    zassert_equal(pMqttConnMbox->count(), 1, "pMqttConnMbox->count() != 1");
    pElem = pMqttConnMbox->read();
    zassert_equal(pElem->mtype, msgType::eCertThingsboard2, "Rcvd mtype != msgType::eCertThingsboard2");
    zassert_equal(pElem->from, Mbox::eMqttConnector, "Rcvd sender != Mbox::eMqttConnector");
    pElem->free();

    delete pMqttConnMbox;
}

// test callback function on_publish
ZTEST(tests_net_mqtt_connector, test_mqtt_connector_on_publish16)
{
    PoolElem* pElem;
	struct mqtt_helper_buf topic;
	struct mqtt_helper_buf payload;
	char tbuf[128];
	char pbuf[1024];

	Mbox* pMqttConnMbox = new Mbox("MqttConnectorMbox");
    Mbox::registerMbox(pMqttConnMbox, Mbox::eMqttConnector);
	zassert_not_null(pMqttConnMbox, "pMqttConnMbox = NULL");

	strcpy(tbuf, "egal");
	strcpy(pbuf, "KEY_TB");
    topic.ptr = tbuf;
	topic.size = strlen(tbuf);
    payload.ptr = pbuf;
	payload.size = strlen(pbuf);

	MqttConnector::on_publish(topic, payload);
    zassert_equal(pMqttConnMbox->count(), 1, "pMqttConnMbox->count() != 1");
    pElem = pMqttConnMbox->read();
    zassert_equal(pElem->mtype, msgType::eCertThingsboardKey, "Rcvd mtype != msgType::eCertThingsboardKey");
    zassert_equal(pElem->from, Mbox::eMqttConnector, "Rcvd sender != Mbox::eMqttConnector");
    pElem->free();

    delete pMqttConnMbox;
}

// test callback function on_publish
ZTEST(tests_net_mqtt_connector, test_mqtt_connector_on_publish17)
{
    PoolElem* pElem;
	struct mqtt_helper_buf topic;
	struct mqtt_helper_buf payload;
	char tbuf[128];
	char pbuf[1024];

	Mbox* pMqttConnMbox = new Mbox("MqttConnectorMbox");
    Mbox::registerMbox(pMqttConnMbox, Mbox::eMqttConnector);
	zassert_not_null(pMqttConnMbox, "pMqttConnMbox = NULL");

	strcpy(tbuf, "egal");
	strcpy(pbuf, "FS_ERASE");
    topic.ptr = tbuf;
	topic.size = strlen(tbuf);
    payload.ptr = pbuf;
	payload.size = strlen(pbuf);

	MqttConnector::on_publish(topic, payload);
    zassert_equal(pMqttConnMbox->count(), 1, "pMqttConnMbox->count() != 1");
    pElem = pMqttConnMbox->read();
    zassert_equal(pElem->mtype, msgType::eFsErase, "Rcvd mtype != msgType::eFsErase");
    zassert_equal(pElem->from, Mbox::eMqttConnector, "Rcvd sender != Mbox::eMqttConnector");
    pElem->free();

    delete pMqttConnMbox;
}

// test callback function on_publish
ZTEST(tests_net_mqtt_connector, test_mqtt_connector_on_publish18)
{
    PoolElem* pElem;
	struct mqtt_helper_buf topic;
	struct mqtt_helper_buf payload;
	char tbuf[128];
	char pbuf[1024];

	Mbox* pMqttConnMbox = new Mbox("MqttConnectorMbox");
    Mbox::registerMbox(pMqttConnMbox, Mbox::eMqttConnector);
	zassert_not_null(pMqttConnMbox, "pMqttConnMbox = NULL");

	strcpy(tbuf, "egal");
	strcpy(pbuf, "FS_STAT");
    topic.ptr = tbuf;
	topic.size = strlen(tbuf);
    payload.ptr = pbuf;
	payload.size = strlen(pbuf);

	MqttConnector::on_publish(topic, payload);
    zassert_equal(pMqttConnMbox->count(), 1, "pMqttConnMbox->count() != 1");
    pElem = pMqttConnMbox->read();
    zassert_equal(pElem->mtype, msgType::eFsStatus, "Rcvd mtype != msgType::eFsStatus");
    zassert_equal(pElem->from, Mbox::eMqttConnector, "Rcvd sender != Mbox::eMqttConnector");
    pElem->free();

    delete pMqttConnMbox;
}

// test callback function on_publish
ZTEST(tests_net_mqtt_connector, test_mqtt_connector_on_publish19)
{
    PoolElem* pElem;
	struct mqtt_helper_buf topic;
	struct mqtt_helper_buf payload;
	char tbuf[128];
	char pbuf[1024];

	Mbox* pMqttConnMbox = new Mbox("MqttConnectorMbox");
    Mbox::registerMbox(pMqttConnMbox, Mbox::eMqttConnector);
	zassert_not_null(pMqttConnMbox, "pMqttConnMbox = NULL");

	strcpy(tbuf, "egal");
	strcpy(pbuf, "DISABLE_ADXL");
    topic.ptr = tbuf;
	topic.size = strlen(tbuf);
    payload.ptr = pbuf;
	payload.size = strlen(pbuf);

	MqttConnector::on_publish(topic, payload);
    zassert_equal(pMqttConnMbox->count(), 1, "pMqttConnMbox->count() != 1");
    pElem = pMqttConnMbox->read();
    zassert_equal(pElem->mtype, msgType::eDisableADXL, "Rcvd mtype != msgType::eDisableADXL");
    zassert_equal(pElem->from, Mbox::eMqttConnector, "Rcvd sender != Mbox::eMqttConnector");
    pElem->free();

    delete pMqttConnMbox;
}

// test callback function on_publish
ZTEST(tests_net_mqtt_connector, test_mqtt_connector_on_publish20)
{
    PoolElem* pElem;
	struct mqtt_helper_buf topic;
	struct mqtt_helper_buf payload;
	char tbuf[128];
	char pbuf[1024];

	Mbox* pMqttConnMbox = new Mbox("MqttConnectorMbox");
    Mbox::registerMbox(pMqttConnMbox, Mbox::eMqttConnector);
	zassert_not_null(pMqttConnMbox, "pMqttConnMbox = NULL");

	strcpy(tbuf, "egal");
	strcpy(pbuf, "ENABLE_ADXL");
    topic.ptr = tbuf;
	topic.size = strlen(tbuf);
    payload.ptr = pbuf;
	payload.size = strlen(pbuf);

	MqttConnector::on_publish(topic, payload);
    zassert_equal(pMqttConnMbox->count(), 1, "pMqttConnMbox->count() != 1");
    pElem = pMqttConnMbox->read();
    zassert_equal(pElem->mtype, msgType::eEnableADXL, "Rcvd mtype != msgType::eEnableADXL");
    zassert_equal(pElem->from, Mbox::eMqttConnector, "Rcvd sender != Mbox::eMqttConnector");
    pElem->free();

    delete pMqttConnMbox;
}

// test callback function on_publish
ZTEST(tests_net_mqtt_connector, test_mqtt_connector_on_publish21)
{
    PoolElem* pElem;
	struct mqtt_helper_buf topic;
	struct mqtt_helper_buf payload;
	char tbuf[128];
	char pbuf[1024];

	Mbox* pMqttConnMbox = new Mbox("MqttConnectorMbox");
    Mbox::registerMbox(pMqttConnMbox, Mbox::eMqttConnector);
	zassert_not_null(pMqttConnMbox, "pMqttConnMbox = NULL");

	strcpy(tbuf, "egal");
	strcpy(pbuf, "GET_CONFIG");
    topic.ptr = tbuf;
	topic.size = strlen(tbuf);
    payload.ptr = pbuf;
	payload.size = strlen(pbuf);

	MqttConnector::on_publish(topic, payload);
    zassert_equal(pMqttConnMbox->count(), 1, "pMqttConnMbox->count() != 1");
    pElem = pMqttConnMbox->read();
    zassert_equal(pElem->mtype, msgType::eGetConfig, "Rcvd mtype != msgType::eGetConfig");
    zassert_equal(pElem->from, Mbox::eMqttConnector, "Rcvd sender != Mbox::eMqttConnector");
    pElem->free();

    delete pMqttConnMbox;
}

// test callback function on_publish
ZTEST(tests_net_mqtt_connector, test_mqtt_connector_on_publish22)
{
    PoolElem* pElem;
	struct mqtt_helper_buf topic;
	struct mqtt_helper_buf payload;
	char tbuf[128];
	char pbuf[1024];

	Mbox* pMqttConnMbox = new Mbox("MqttConnectorMbox");
    Mbox::registerMbox(pMqttConnMbox, Mbox::eMqttConnector);
	zassert_not_null(pMqttConnMbox, "pMqttConnMbox = NULL");

	strcpy(tbuf, "egal");
	strcpy(pbuf, "SEND_PUBLIC_KEY");
    topic.ptr = tbuf;
	topic.size = strlen(tbuf);
    payload.ptr = pbuf;
	payload.size = strlen(pbuf);

	MqttConnector::on_publish(topic, payload);
    zassert_equal(pMqttConnMbox->count(), 1, "pMqttConnMbox->count() != 1");
    pElem = pMqttConnMbox->read();
    zassert_equal(pElem->mtype, msgType::eGetConfig, "Rcvd mtype != msgType::eGetConfig");
    zassert_equal(pElem->from, Mbox::eMqttConnector, "Rcvd sender != Mbox::eMqttConnector");
    pElem->free();

    delete pMqttConnMbox;
}

// test callback function on_publish
ZTEST(tests_net_mqtt_connector, test_mqtt_connector_on_publish23)
{
	struct mqtt_helper_buf topic;
	struct mqtt_helper_buf payload;
	char tbuf[128];
	char pbuf[1024];

	strcpy(tbuf, "egal");
	strcpy(pbuf, "egal");
    topic.ptr = tbuf;
	topic.size = strlen(tbuf);
    payload.ptr = pbuf;
	payload.size = strlen(pbuf);
	MqttConnector::on_publish(topic, payload);
}

// test callback function on_puback
ZTEST(tests_net_mqtt_connector, test_mqtt_connector_on_puback)
{
	MqttConnector::on_puback(0, 0);
}

// test callback function on_suback
ZTEST(tests_net_mqtt_connector, test_mqtt_connector_on_suback)
{
	MqttConnector::on_suback(SUBSCRIBE_TOPIC_ID, 0);
	MqttConnector::on_suback(SUBSCRIBE_TOPIC_ID, 1);
	MqttConnector::on_suback(0, 1);
	MqttConnector::on_suback(0, 0);
}

// test callback function publish
ZTEST(tests_net_mqtt_connector, test_mqtt_connector_publish)
{
	struct topic t;
	struct payload p;
	char tbuf[128];
	char pbuf[1024];

	MqttConnector* pMqttConnector = new MqttConnector("Mqtt");
	zassert_not_null(pMqttConnector, "memory alloc failed");
	zassert_not_null(pMqttConnector->pInMbox, "pInMbox alloc failed");
	zassert_not_null(pMqttConnector->myTimer, "myTimer alloc failed");

	strcpy(t.string, "egal");
	strcpy(p.string, "egal");

	pMqttConnector->last_msg_id = 6;

	gbRetVal = true;

	pMqttConnector->publish(&p, &t);
	zassert_equal(pMqttConnector->last_msg_id, 7, "pMqttConnector->last_msg_id != 7");

	delete pMqttConnector;
}

// test callback function publish
ZTEST(tests_net_mqtt_connector, test_mqtt_connector_publish2)
{
	struct topic t;
	struct payload p;
	char tbuf[128];
	char pbuf[1024];

	MqttConnector* pMqttConnector = new MqttConnector("Mqtt");
	zassert_not_null(pMqttConnector, "memory alloc failed");
	zassert_not_null(pMqttConnector->pInMbox, "pInMbox alloc failed");
	zassert_not_null(pMqttConnector->myTimer, "myTimer alloc failed");

	strcpy(t.string, "egal");
	strcpy(p.string, "egal");

	pMqttConnector->last_msg_id = 6;

	gbRetVal = false;

	pMqttConnector->publish(&p, &t);
	zassert_equal(pMqttConnector->last_msg_id, 7, "pMqttConnector->last_msg_id != 7");

	delete pMqttConnector;
}

// test callback function subscribe
ZTEST(tests_net_mqtt_connector, test_mqtt_connector_subscribe)
{
	MqttConnector* pMqttConnector = new MqttConnector("Mqtt");
	zassert_not_null(pMqttConnector, "memory alloc failed");
	zassert_not_null(pMqttConnector->pInMbox, "pInMbox alloc failed");
	zassert_not_null(pMqttConnector->myTimer, "myTimer alloc failed");

	pMqttConnector->subscribe((const uint8_t*)"topic");

	delete pMqttConnector;
}

// test callback function connect
ZTEST(tests_net_mqtt_connector, test_mqtt_connector_connect)
{
	MqttConnector* pMqttConnector = new MqttConnector("Mqtt");
	zassert_not_null(pMqttConnector, "memory alloc failed");
	zassert_not_null(pMqttConnector->pInMbox, "pInMbox alloc failed");
	zassert_not_null(pMqttConnector->myTimer, "myTimer alloc failed");

	gbRetVal = true;

    pMqttConnector->numConnectRetries = 1;
	pMqttConnector->connect();
	zassert_equal(pMqttConnector->numConnectRetries, 2, "numConnectRetries != 2");

    pMqttConnector->numConnectRetries = 3;
	pMqttConnector->connect();
	zassert_equal(pMqttConnector->numConnectRetries, 0, "numConnectRetries != 0");

	gbRetVal = false;

    pMqttConnector->numConnectRetries = 1;
	pMqttConnector->connect();
	zassert_equal(pMqttConnector->numConnectRetries, 2, "numConnectRetries != 2");

	delete pMqttConnector;
}

// test callback function disconnect
ZTEST(tests_net_mqtt_connector, test_mqtt_connector_disconnect)
{
	MqttConnector* pMqttConnector = new MqttConnector("Mqtt");
	zassert_not_null(pMqttConnector, "memory alloc failed");
	zassert_not_null(pMqttConnector->pInMbox, "pInMbox alloc failed");
	zassert_not_null(pMqttConnector->myTimer, "myTimer alloc failed");

	pMqttConnector->disconnect();

	delete pMqttConnector;
}

// test callback function abort
ZTEST(tests_net_mqtt_connector, test_mqtt_connector_abort)
{
	MqttConnector* pMqttConnector = new MqttConnector("Mqtt");
	zassert_not_null(pMqttConnector, "memory alloc failed");
	zassert_not_null(pMqttConnector->pInMbox, "pInMbox alloc failed");
	zassert_not_null(pMqttConnector->myTimer, "myTimer alloc failed");

	pMqttConnector->abort();

	delete pMqttConnector;
}

// test callback function configure
ZTEST(tests_net_mqtt_connector, test_mqtt_connector_configure)
{
	MqttConnector* pMqttConnector = new MqttConnector("Mqtt");
	zassert_not_null(pMqttConnector, "memory alloc failed");
	zassert_not_null(pMqttConnector->pInMbox, "pInMbox alloc failed");
	zassert_not_null(pMqttConnector->myTimer, "myTimer alloc failed");

	pMqttConnector->configure();
	zassert_equal(pMqttConnector->metricsInterval, 120, "numConnectRetries != 120");

	delete pMqttConnector;
}

// test function timer_cb and timer1_expired
ZTEST(tests_net_mqtt_connector, test_net_mqtt_connector_timer_cb)
{
	PoolElem* pElem;

	MqttConnector* pMqttConnector = new MqttConnector("Mqtt");
	zassert_not_null(pMqttConnector, "memory alloc failed");
	zassert_not_null(pMqttConnector->pInMbox, "pInMbox alloc failed");
	zassert_not_null(pMqttConnector->myTimer, "myTimer alloc failed");
    zassert_equal(pMqttConnector->pInMbox->count(), 0, "pInMbox->count() != 0");
    zassert_equal(pMqttConnector->pInMbox, Mbox::getMboxById(Mbox::eMqttConnector),
                  "Error getting mailbox by id");

    pMqttConnector->timer_cb(reinterpret_cast<k_timer*>(pMqttConnector));

    zassert_equal(pMqttConnector->pInMbox->count(), 1, "pInMbox->count() != 1");
    pElem = pMqttConnector->pInMbox->read();
    zassert_equal(pElem->mtype, msgType::eTimerExpired, "Rcvd mtype != msgType::eTimerExpired");

    delete pMqttConnector;
}    

//Test receive msg eTimerExpired 
ZTEST(tests_net_mqtt_connector, test_net_mqtt_connector_rcv_msg_eTimerExpired)
{
	MqttConnector* pMqttConnector = new MqttConnector("Mqtt");
	zassert_not_null(pMqttConnector, "memory alloc failed");
	zassert_not_null(pMqttConnector->pInMbox, "pInMbox alloc failed");
	zassert_not_null(pMqttConnector->myTimer, "myTimer alloc failed");

	pMqttConnector->lte_connected = true;
    pMqttConnector->connected = false;

    Mbox::sendMsg(msgType::eTimerExpired, Mbox::eMqttConnector, Mbox::eMqttConnector);

    zassert_equal(pMqttConnector->pInMbox->count(), 1, "pInMbox->count() != 1");

    pMqttConnector->task();  

	delete pMqttConnector; 
}

//Test receive msg eTimerExpired 
ZTEST(tests_net_mqtt_connector, test_net_mqtt_connector_rcv_msg_eTimerExpired1)
{
	MqttConnector* pMqttConnector = new MqttConnector("Mqtt");
	zassert_not_null(pMqttConnector, "memory alloc failed");
	zassert_not_null(pMqttConnector->pInMbox, "pInMbox alloc failed");
	zassert_not_null(pMqttConnector->myTimer, "myTimer alloc failed");

	pMqttConnector->lte_connected = false;
    pMqttConnector->connected = true;

    Mbox::sendMsg(msgType::eTimerExpired, Mbox::eMqttConnector, Mbox::eMqttConnector);

    zassert_equal(pMqttConnector->pInMbox->count(), 1, "pInMbox->count() != 1");

    pMqttConnector->task();  

	delete pMqttConnector; 
}

//Test receive msg eTimerExpired 
ZTEST(tests_net_mqtt_connector, test_net_mqtt_connector_rcv_msg_eTimerExpired2)
{
	MqttConnector* pMqttConnector = new MqttConnector("Mqtt");
	zassert_not_null(pMqttConnector, "memory alloc failed");
	zassert_not_null(pMqttConnector->pInMbox, "pInMbox alloc failed");
	zassert_not_null(pMqttConnector->myTimer, "myTimer alloc failed");

	pMqttConnector->lte_connected = true;
    pMqttConnector->connected = true;

    Mbox::sendMsg(msgType::eTimerExpired, Mbox::eMqttConnector, Mbox::eMqttConnector);

    zassert_equal(pMqttConnector->pInMbox->count(), 1, "pInMbox->count() != 1");

    pMqttConnector->task();  

	delete pMqttConnector; 
}

//Test receive msg eTimerExpired 
ZTEST(tests_net_mqtt_connector, test_net_mqtt_connector_rcv_msg_eTimerExpired3)
{
	MqttConnector* pMqttConnector = new MqttConnector("Mqtt");
	zassert_not_null(pMqttConnector, "memory alloc failed");
	zassert_not_null(pMqttConnector->pInMbox, "pInMbox alloc failed");
	zassert_not_null(pMqttConnector->myTimer, "myTimer alloc failed");

	pMqttConnector->lte_connected = false;
    pMqttConnector->connected = false;

    Mbox::sendMsg(msgType::eTimerExpired, Mbox::eMqttConnector, Mbox::eMqttConnector);

    zassert_equal(pMqttConnector->pInMbox->count(), 1, "pInMbox->count() != 1");

    pMqttConnector->task();  

	delete pMqttConnector; 
}

//Test receive msg eCmdAccept 
ZTEST(tests_net_mqtt_connector, test_net_mqtt_connector_rcv_msg_eCmdAccept)
{
	MqttConnector* pMqttConnector = new MqttConnector("Mqtt");
	zassert_not_null(pMqttConnector, "memory alloc failed");
	zassert_not_null(pMqttConnector->pInMbox, "pInMbox alloc failed");
	zassert_not_null(pMqttConnector->myTimer, "myTimer alloc failed");

    pMqttConnector->connected = true;

    Mbox::sendMsg(msgType::eCmdAccept, Mbox::eMqttConnector, Mbox::eMqttConnector);

    zassert_equal(pMqttConnector->pInMbox->count(), 1, "pInMbox->count() != 1");

    pMqttConnector->task();  

	delete pMqttConnector; 
}

//Test receive msg eCmdAccept 
ZTEST(tests_net_mqtt_connector, test_net_mqtt_connector_rcv_msg_eCmdAccept2)
{
	MqttConnector* pMqttConnector = new MqttConnector("Mqtt");
	zassert_not_null(pMqttConnector, "memory alloc failed");
	zassert_not_null(pMqttConnector->pInMbox, "pInMbox alloc failed");
	zassert_not_null(pMqttConnector->myTimer, "myTimer alloc failed");

    pMqttConnector->connected = false;
 
    Mbox::sendMsg(msgType::eCmdAccept, Mbox::eMqttConnector, Mbox::eMqttConnector);

    zassert_equal(pMqttConnector->pInMbox->count(), 1, "pInMbox->count() != 1");

    pMqttConnector->task();  

	delete pMqttConnector; 
}

//Test receive msg eCmdReject 
ZTEST(tests_net_mqtt_connector, test_net_mqtt_connector_rcv_msg_eCmdReject)
{
	MqttConnector* pMqttConnector = new MqttConnector("Mqtt");
	zassert_not_null(pMqttConnector, "memory alloc failed");
	zassert_not_null(pMqttConnector->pInMbox, "pInMbox alloc failed");
	zassert_not_null(pMqttConnector->myTimer, "myTimer alloc failed");

    pMqttConnector->connected = true;

    Mbox::sendMsg(msgType::eCmdReject, Mbox::eMqttConnector, Mbox::eMqttConnector);

    zassert_equal(pMqttConnector->pInMbox->count(), 1, "pInMbox->count() != 1");

    pMqttConnector->task();  

	delete pMqttConnector; 
}

//Test receive msg eCmdReject 
ZTEST(tests_net_mqtt_connector, test_net_mqtt_connector_rcv_msg_eCmdReject2)
{
	MqttConnector* pMqttConnector = new MqttConnector("Mqtt");
	zassert_not_null(pMqttConnector, "memory alloc failed");
	zassert_not_null(pMqttConnector->pInMbox, "pInMbox alloc failed");
	zassert_not_null(pMqttConnector->myTimer, "myTimer alloc failed");

    pMqttConnector->connected = false;

    Mbox::sendMsg(msgType::eCmdReject, Mbox::eMqttConnector, Mbox::eMqttConnector);

    zassert_equal(pMqttConnector->pInMbox->count(), 1, "pInMbox->count() != 1");

    pMqttConnector->task();  

	delete pMqttConnector; 
}

//Test receive msg eOtaStart 
ZTEST(tests_net_mqtt_connector, test_net_mqtt_connector_rcv_msg_eOtaStart)
{
	MqttConnector* pMqttConnector = new MqttConnector("Mqtt");
	zassert_not_null(pMqttConnector, "memory alloc failed");
	zassert_not_null(pMqttConnector->pInMbox, "pInMbox alloc failed");
	zassert_not_null(pMqttConnector->myTimer, "myTimer alloc failed");

    pMqttConnector->connected = true;

    Mbox::sendMsg(msgType::eOtaStart, Mbox::eMqttConnector, Mbox::eMqttConnector);

    zassert_equal(pMqttConnector->pInMbox->count(), 1, "pInMbox->count() != 1");

    pMqttConnector->task();  

	delete pMqttConnector; 
}

//Test receive msg eOtaStart 
ZTEST(tests_net_mqtt_connector, test_net_mqtt_connector_rcv_msg_eOtaStart2)
{
	MqttConnector* pMqttConnector = new MqttConnector("Mqtt");
	zassert_not_null(pMqttConnector, "memory alloc failed");
	zassert_not_null(pMqttConnector->pInMbox, "pInMbox alloc failed");
	zassert_not_null(pMqttConnector->myTimer, "myTimer alloc failed");

    pMqttConnector->connected = false;

    Mbox::sendMsg(msgType::eOtaStart, Mbox::eMqttConnector, Mbox::eMqttConnector);

    zassert_equal(pMqttConnector->pInMbox->count(), 1, "pInMbox->count() != 1");

    pMqttConnector->task();  

	delete pMqttConnector; 
}

//Test receive msg eFwAccept 
ZTEST(tests_net_mqtt_connector, test_net_mqtt_connector_rcv_msg_eFwAccept)
{
	MqttConnector* pMqttConnector = new MqttConnector("Mqtt");
	zassert_not_null(pMqttConnector, "memory alloc failed");
	zassert_not_null(pMqttConnector->pInMbox, "pInMbox alloc failed");
	zassert_not_null(pMqttConnector->myTimer, "myTimer alloc failed");

    pMqttConnector->connected = true;

    Mbox::sendMsg(msgType::eFwAccept, Mbox::eMqttConnector, Mbox::eMqttConnector);

    zassert_equal(pMqttConnector->pInMbox->count(), 1, "pInMbox->count() != 1");

    pMqttConnector->task();  

	delete pMqttConnector; 
}

//Test receive msg eFwAccept 
ZTEST(tests_net_mqtt_connector, test_net_mqtt_connector_rcv_msg_eFwAccept2)
{
	MqttConnector* pMqttConnector = new MqttConnector("Mqtt");
	zassert_not_null(pMqttConnector, "memory alloc failed");
	zassert_not_null(pMqttConnector->pInMbox, "pInMbox alloc failed");
	zassert_not_null(pMqttConnector->myTimer, "myTimer alloc failed");

    pMqttConnector->connected = false;

    Mbox::sendMsg(msgType::eFwAccept, Mbox::eMqttConnector, Mbox::eMqttConnector);

    zassert_equal(pMqttConnector->pInMbox->count(), 1, "pInMbox->count() != 1");

    pMqttConnector->task();  

	delete pMqttConnector; 
}

//Test receive msg eFwReject 
ZTEST(tests_net_mqtt_connector, test_net_mqtt_connector_rcv_msg_eFwReject)
{
	MqttConnector* pMqttConnector = new MqttConnector("Mqtt");
	zassert_not_null(pMqttConnector, "memory alloc failed");
	zassert_not_null(pMqttConnector->pInMbox, "pInMbox alloc failed");
	zassert_not_null(pMqttConnector->myTimer, "myTimer alloc failed");

    pMqttConnector->connected = true;

    Mbox::sendMsg(msgType::eFwReject, Mbox::eMqttConnector, Mbox::eMqttConnector);

    zassert_equal(pMqttConnector->pInMbox->count(), 1, "pInMbox->count() != 1");

    pMqttConnector->task();  

	delete pMqttConnector; 
}

//Test receive msg eFwReject 
ZTEST(tests_net_mqtt_connector, test_net_mqtt_connector_rcv_msg_eFwReject2)
{
	MqttConnector* pMqttConnector = new MqttConnector("Mqtt");
	zassert_not_null(pMqttConnector, "memory alloc failed");
	zassert_not_null(pMqttConnector->pInMbox, "pInMbox alloc failed");
	zassert_not_null(pMqttConnector->myTimer, "myTimer alloc failed");

    pMqttConnector->connected = false;

    Mbox::sendMsg(msgType::eFwReject, Mbox::eMqttConnector, Mbox::eMqttConnector);

    zassert_equal(pMqttConnector->pInMbox->count(), 1, "pInMbox->count() != 1");

    pMqttConnector->task();  

	delete pMqttConnector; 
}

//Test receive msg eFwDlSuccess 
ZTEST(tests_net_mqtt_connector, test_net_mqtt_connector_rcv_msg_eFwDlSuccess)
{
	MqttConnector* pMqttConnector = new MqttConnector("Mqtt");
	zassert_not_null(pMqttConnector, "memory alloc failed");
	zassert_not_null(pMqttConnector->pInMbox, "pInMbox alloc failed");
	zassert_not_null(pMqttConnector->myTimer, "myTimer alloc failed");

    pMqttConnector->connected = true;

    Mbox::sendMsg(msgType::eFwDlSuccess, Mbox::eMqttConnector, Mbox::eMqttConnector);

    zassert_equal(pMqttConnector->pInMbox->count(), 1, "pInMbox->count() != 1");

    pMqttConnector->task();  

	delete pMqttConnector; 
}

//Test receive msg eFwDlSuccess 
ZTEST(tests_net_mqtt_connector, test_net_mqtt_connector_rcv_msg_eFwDlSuccess2)
{
	MqttConnector* pMqttConnector = new MqttConnector("Mqtt");
	zassert_not_null(pMqttConnector, "memory alloc failed");
	zassert_not_null(pMqttConnector->pInMbox, "pInMbox alloc failed");
	zassert_not_null(pMqttConnector->myTimer, "myTimer alloc failed");

    pMqttConnector->connected = false;

    Mbox::sendMsg(msgType::eFwDlSuccess, Mbox::eMqttConnector, Mbox::eMqttConnector);

    zassert_equal(pMqttConnector->pInMbox->count(), 1, "pInMbox->count() != 1");

    pMqttConnector->task();  

	delete pMqttConnector; 
}

//Test receive msg eFwDlFailed 
ZTEST(tests_net_mqtt_connector, test_net_mqtt_connector_rcv_msg_eFwDlFailed)
{
	MqttConnector* pMqttConnector = new MqttConnector("Mqtt");
	zassert_not_null(pMqttConnector, "memory alloc failed");
	zassert_not_null(pMqttConnector->pInMbox, "pInMbox alloc failed");
	zassert_not_null(pMqttConnector->myTimer, "myTimer alloc failed");

    pMqttConnector->connected = true;

    Mbox::sendMsg(msgType::eFwDlFailed, Mbox::eMqttConnector, Mbox::eMqttConnector);

    zassert_equal(pMqttConnector->pInMbox->count(), 1, "pInMbox->count() != 1");

    pMqttConnector->task();  

	delete pMqttConnector; 
}

//Test receive msg eFwDlFailed 
ZTEST(tests_net_mqtt_connector, test_net_mqtt_connector_rcv_msg_eFwDlFailed2)
{
	MqttConnector* pMqttConnector = new MqttConnector("Mqtt");
	zassert_not_null(pMqttConnector, "memory alloc failed");
	zassert_not_null(pMqttConnector->pInMbox, "pInMbox alloc failed");
	zassert_not_null(pMqttConnector->myTimer, "myTimer alloc failed");

    pMqttConnector->connected = false;

    Mbox::sendMsg(msgType::eFwDlFailed, Mbox::eMqttConnector, Mbox::eMqttConnector);

    zassert_equal(pMqttConnector->pInMbox->count(), 1, "pInMbox->count() != 1");

    pMqttConnector->task();  

	delete pMqttConnector; 
}

//Test receive msg eMqttConnected 
ZTEST(tests_net_mqtt_connector, test_net_mqtt_connector_rcv_msg_eMqttConnected)
{
    PoolElem* pElem;

	Mbox* pPowerMgrMbox = new Mbox("PowerMgrMbox");
    Mbox::registerMbox(pPowerMgrMbox, Mbox::ePowerManager);
	zassert_not_null(pPowerMgrMbox, "pPowerMgrMbox = NULL");

	MqttConnector* pMqttConnector = new MqttConnector("Mqtt");
	zassert_not_null(pMqttConnector, "memory alloc failed");
	zassert_not_null(pMqttConnector->pInMbox, "pInMbox alloc failed");
	zassert_not_null(pMqttConnector->myTimer, "myTimer alloc failed");

    gbRetVal = true;
	pMqttConnector->connected = false;

    Mbox::sendMsg(msgType::eMqttConnected, Mbox::eMqttConnector, Mbox::eMqttConnector);

    zassert_equal(pMqttConnector->pInMbox->count(), 1, "pInMbox->count() != 1");

    pMqttConnector->task();  

    zassert_equal(pMqttConnector->pInMbox->count(), 0, "pInMbox->count() != 0");
    zassert_equal(pPowerMgrMbox->count(), 1, "pMqttConnMbox->count() != 1");

    pElem = pPowerMgrMbox->read();
    zassert_equal(pElem->mtype, msgType::eMqttConnected, "Rcvd mtype != msgType::eMqttConnected");
    zassert_equal(pElem->from, Mbox::eMqttConnector, "Rcvd sender != Mbox::eMqttConnector");
    zassert_true(pMqttConnector->connected, "connected != true");

	pElem->free();
	delete pPowerMgrMbox;
	delete pMqttConnector; 
}

//Test receive msg eMqttConnected 
ZTEST(tests_net_mqtt_connector, test_net_mqtt_connector_rcv_msg_eMqttConnected2)
{
    PoolElem* pElem;

	Mbox* pPowerMgrMbox = new Mbox("PowerMgrMbox");
    Mbox::registerMbox(pPowerMgrMbox, Mbox::ePowerManager);
	zassert_not_null(pPowerMgrMbox, "pPowerMgrMbox = NULL");

	MqttConnector* pMqttConnector = new MqttConnector("Mqtt");
	zassert_not_null(pMqttConnector, "memory alloc failed");
	zassert_not_null(pMqttConnector->pInMbox, "pInMbox alloc failed");
	zassert_not_null(pMqttConnector->myTimer, "myTimer alloc failed");

    gbRetVal = false;
	pMqttConnector->connected = false;

    Mbox::sendMsg(msgType::eMqttConnected, Mbox::eMqttConnector, Mbox::eMqttConnector);

    zassert_equal(pMqttConnector->pInMbox->count(), 1, "pInMbox->count() != 1");

    pMqttConnector->task();  

    zassert_equal(pMqttConnector->pInMbox->count(), 1, "pInMbox->count() != 1");
    zassert_equal(pPowerMgrMbox->count(), 1, "pMqttConnMbox->count() != 1");

    pElem = pPowerMgrMbox->read();
    zassert_equal(pElem->mtype, msgType::eMqttConnected, "Rcvd mtype != msgType::eMqttConnected");
    zassert_equal(pElem->from, Mbox::eMqttConnector, "Rcvd sender != Mbox::eMqttConnector");
    zassert_true(pMqttConnector->connected, "connected != true");

    pMqttConnector->task();  

    zassert_equal(pMqttConnector->pInMbox->count(), 0, "pInMbox->count() != 0");

	pElem->free();
	delete pPowerMgrMbox;
	delete pMqttConnector; 
}

//Test receive msg eMqttDisconnected 
ZTEST(tests_net_mqtt_connector, test_net_mqtt_connector_rcv_msg_eMqttDisconnected)
{
	MqttConnector* pMqttConnector = new MqttConnector("Mqtt");
	zassert_not_null(pMqttConnector, "memory alloc failed");
	zassert_not_null(pMqttConnector->pInMbox, "pInMbox alloc failed");
	zassert_not_null(pMqttConnector->myTimer, "myTimer alloc failed");

    pMqttConnector->connected = true;
	pMqttConnector->lte_connected = true;

    Mbox::sendMsg(msgType::eMqttDisconnected, Mbox::eMqttConnector, Mbox::eMqttConnector);

    zassert_equal(pMqttConnector->pInMbox->count(), 1, "pInMbox->count() != 1");

    pMqttConnector->task();  
    zassert_false(pMqttConnector->connected, "connected != false");

	delete pMqttConnector; 
}

//Test receive msg eMqttDisconnected 
ZTEST(tests_net_mqtt_connector, test_net_mqtt_connector_rcv_msg_eMqttDisconnected2)
{
	MqttConnector* pMqttConnector = new MqttConnector("Mqtt");
	zassert_not_null(pMqttConnector, "memory alloc failed");
	zassert_not_null(pMqttConnector->pInMbox, "pInMbox alloc failed");
	zassert_not_null(pMqttConnector->myTimer, "myTimer alloc failed");

    pMqttConnector->connected = true;
	pMqttConnector->lte_connected = false;

    Mbox::sendMsg(msgType::eMqttDisconnected, Mbox::eMqttConnector, Mbox::eMqttConnector);

    zassert_equal(pMqttConnector->pInMbox->count(), 1, "pInMbox->count() != 1");

    pMqttConnector->task();  
    zassert_false(pMqttConnector->connected, "connected != false");

	delete pMqttConnector; 
}

//Test receive msg eLteSleepEnter 
ZTEST(tests_net_mqtt_connector, test_net_mqtt_connector_rcv_msg_eLteSleepEnter)
{
	MqttConnector* pMqttConnector = new MqttConnector("Mqtt");
	zassert_not_null(pMqttConnector, "memory alloc failed");
	zassert_not_null(pMqttConnector->pInMbox, "pInMbox alloc failed");
	zassert_not_null(pMqttConnector->myTimer, "myTimer alloc failed");

    Mbox::sendMsg(msgType::eLteSleepEnter, Mbox::eMqttConnector, Mbox::eMqttConnector);

    zassert_equal(pMqttConnector->pInMbox->count(), 1, "pInMbox->count() != 1");

    pMqttConnector->task();  

	delete pMqttConnector; 
}

//Test receive msg eLteSleepExit 
ZTEST(tests_net_mqtt_connector, test_net_mqtt_connector_rcv_msg_eLteSleepExit)
{
	MqttConnector* pMqttConnector = new MqttConnector("Mqtt");
	zassert_not_null(pMqttConnector, "memory alloc failed");
	zassert_not_null(pMqttConnector->pInMbox, "pInMbox alloc failed");
	zassert_not_null(pMqttConnector->myTimer, "myTimer alloc failed");

    Mbox::sendMsg(msgType::eLteSleepExit, Mbox::eMqttConnector, Mbox::eMqttConnector);

    zassert_equal(pMqttConnector->pInMbox->count(), 1, "pInMbox->count() != 1");

    pMqttConnector->task();  

	delete pMqttConnector; 
}

//Test receive msg eKeyTransferReq 
ZTEST(tests_net_mqtt_connector, test_net_mqtt_connector_rcv_msg_eKeyTransferReq)
{
	MqttConnector* pMqttConnector = new MqttConnector("Mqtt");
	zassert_not_null(pMqttConnector, "memory alloc failed");
	zassert_not_null(pMqttConnector->pInMbox, "pInMbox alloc failed");
	zassert_not_null(pMqttConnector->myTimer, "myTimer alloc failed");

    pMqttConnector->connected = true;

    Mbox::sendMsg(msgType::eKeyTransferReq, Mbox::eMqttConnector, Mbox::eMqttConnector);

    zassert_equal(pMqttConnector->pInMbox->count(), 1, "pInMbox->count() != 1");

    pMqttConnector->task();  

	delete pMqttConnector; 
}

//Test receive msg eKeyTransferReq 
ZTEST(tests_net_mqtt_connector, test_net_mqtt_connector_rcv_msg_eKeyTransferReq2)
{
	MqttConnector* pMqttConnector = new MqttConnector("Mqtt");
	zassert_not_null(pMqttConnector, "memory alloc failed");
	zassert_not_null(pMqttConnector->pInMbox, "pInMbox alloc failed");
	zassert_not_null(pMqttConnector->myTimer, "myTimer alloc failed");

    pMqttConnector->connected = false;

    Mbox::sendMsg(msgType::eKeyTransferReq, Mbox::eMqttConnector, Mbox::eMqttConnector);

    zassert_equal(pMqttConnector->pInMbox->count(), 1, "pInMbox->count() != 1");

    pMqttConnector->task();  

	delete pMqttConnector; 
}

//Test receive msg eMetrics 
ZTEST(tests_net_mqtt_connector, test_net_mqtt_connector_rcv_msg_eMetrics)
{
	MqttConnector* pMqttConnector = new MqttConnector("Mqtt");
	zassert_not_null(pMqttConnector, "memory alloc failed");
	zassert_not_null(pMqttConnector->pInMbox, "pInMbox alloc failed");
	zassert_not_null(pMqttConnector->myTimer, "myTimer alloc failed");

    pMqttConnector->connected = true;

    Mbox::sendMsg(msgType::eMetrics, Mbox::eMqttConnector, Mbox::eMqttConnector);

    zassert_equal(pMqttConnector->pInMbox->count(), 1, "pInMbox->count() != 1");

    pMqttConnector->task();  

	delete pMqttConnector; 
}

//Test receive msg eMetrics 
ZTEST(tests_net_mqtt_connector, test_net_mqtt_connector_rcv_msg_eMetrics2)
{
	MqttConnector* pMqttConnector = new MqttConnector("Mqtt");
	zassert_not_null(pMqttConnector, "memory alloc failed");
	zassert_not_null(pMqttConnector->pInMbox, "pInMbox alloc failed");
	zassert_not_null(pMqttConnector->myTimer, "myTimer alloc failed");

    pMqttConnector->connected = false;
    pMqttConnector->lte_connected = true;

    Mbox::sendMsg(msgType::eMetrics, Mbox::eMqttConnector, Mbox::eMqttConnector);

    zassert_equal(pMqttConnector->pInMbox->count(), 1, "pInMbox->count() != 1");

    pMqttConnector->task();  

	delete pMqttConnector; 
}

//Test receive msg eMetrics 
ZTEST(tests_net_mqtt_connector, test_net_mqtt_connector_rcv_msg_eMetrics3)
{
	MqttConnector* pMqttConnector = new MqttConnector("Mqtt");
	zassert_not_null(pMqttConnector, "memory alloc failed");
	zassert_not_null(pMqttConnector->pInMbox, "pInMbox alloc failed");
	zassert_not_null(pMqttConnector->myTimer, "myTimer alloc failed");

    pMqttConnector->connected = false;
    pMqttConnector->lte_connected = false;

    Mbox::sendMsg(msgType::eMetrics, Mbox::eMqttConnector, Mbox::eMqttConnector);

    zassert_equal(pMqttConnector->pInMbox->count(), 1, "pInMbox->count() != 1");

    pMqttConnector->task();  

	delete pMqttConnector; 
}

//Test receive msg eMeasurements 
ZTEST(tests_net_mqtt_connector, test_net_mqtt_connector_rcv_msg_eMeasurements)
{
	MqttConnector* pMqttConnector = new MqttConnector("Mqtt");
	zassert_not_null(pMqttConnector, "memory alloc failed");
	zassert_not_null(pMqttConnector->pInMbox, "pInMbox alloc failed");
	zassert_not_null(pMqttConnector->myTimer, "myTimer alloc failed");

    pMqttConnector->connected = true;

    Mbox::sendMsg(msgType::eMeasurements, Mbox::eMqttConnector, Mbox::eMqttConnector);

    zassert_equal(pMqttConnector->pInMbox->count(), 1, "pInMbox->count() != 1");

    pMqttConnector->task();  

	delete pMqttConnector; 
}

//Test receive msg eMeasurements 
ZTEST(tests_net_mqtt_connector, test_net_mqtt_connector_rcv_msg_eMeasurements2)
{
	MqttConnector* pMqttConnector = new MqttConnector("Mqtt");
	zassert_not_null(pMqttConnector, "memory alloc failed");
	zassert_not_null(pMqttConnector->pInMbox, "pInMbox alloc failed");
	zassert_not_null(pMqttConnector->myTimer, "myTimer alloc failed");

    pMqttConnector->connected = false;
    pMqttConnector->lte_connected = true;

    Mbox::sendMsg(msgType::eMeasurements, Mbox::eMqttConnector, Mbox::eMqttConnector);

    zassert_equal(pMqttConnector->pInMbox->count(), 1, "pInMbox->count() != 1");

    pMqttConnector->task();  

	delete pMqttConnector; 
}

//Test receive msg eMeasurements 
ZTEST(tests_net_mqtt_connector, test_net_mqtt_connector_rcv_msg_eMeasurements3)
{
	MqttConnector* pMqttConnector = new MqttConnector("Mqtt");
	zassert_not_null(pMqttConnector, "memory alloc failed");
	zassert_not_null(pMqttConnector->pInMbox, "pInMbox alloc failed");
	zassert_not_null(pMqttConnector->myTimer, "myTimer alloc failed");

    pMqttConnector->connected = false;
    pMqttConnector->lte_connected = false;

    Mbox::sendMsg(msgType::eMeasurements, Mbox::eMqttConnector, Mbox::eMqttConnector);

    zassert_equal(pMqttConnector->pInMbox->count(), 1, "pInMbox->count() != 1");

    pMqttConnector->task();  

	delete pMqttConnector; 
}

//Test receive msg eLteConnected 
ZTEST(tests_net_mqtt_connector, test_net_mqtt_connector_rcv_msg_eLteConnected)
{
	MqttConnector* pMqttConnector = new MqttConnector("Mqtt");
	zassert_not_null(pMqttConnector, "memory alloc failed");
	zassert_not_null(pMqttConnector->pInMbox, "pInMbox alloc failed");
	zassert_not_null(pMqttConnector->myTimer, "myTimer alloc failed");

    pMqttConnector->lte_connected = true;

    Mbox::sendMsg(msgType::eLteConnected, Mbox::eMqttConnector, Mbox::eMqttConnector);

    zassert_equal(pMqttConnector->pInMbox->count(), 1, "pInMbox->count() != 1");

    pMqttConnector->task();  

	delete pMqttConnector; 
}

//Test receive msg eLteConnected 
ZTEST(tests_net_mqtt_connector, test_net_mqtt_connector_rcv_msg_eLteConnected2)
{
	MqttConnector* pMqttConnector = new MqttConnector("Mqtt");
	zassert_not_null(pMqttConnector, "memory alloc failed");
	zassert_not_null(pMqttConnector->pInMbox, "pInMbox alloc failed");
	zassert_not_null(pMqttConnector->myTimer, "myTimer alloc failed");

    pMqttConnector->lte_connected = false;
    pMqttConnector->connected = false;

    Mbox::sendMsg(msgType::eLteConnected, Mbox::eMqttConnector, Mbox::eMqttConnector);

    zassert_equal(pMqttConnector->pInMbox->count(), 1, "pInMbox->count() != 1");

    pMqttConnector->task();  

	delete pMqttConnector; 
}

//Test receive msg eLteConnected 
ZTEST(tests_net_mqtt_connector, test_net_mqtt_connector_rcv_msg_eLteConnected3)
{
	MqttConnector* pMqttConnector = new MqttConnector("Mqtt");
	zassert_not_null(pMqttConnector, "memory alloc failed");
	zassert_not_null(pMqttConnector->pInMbox, "pInMbox alloc failed");
	zassert_not_null(pMqttConnector->myTimer, "myTimer alloc failed");

    pMqttConnector->lte_connected = false;
    pMqttConnector->connected = true;

    Mbox::sendMsg(msgType::eLteConnected, Mbox::eMqttConnector, Mbox::eMqttConnector);

    zassert_equal(pMqttConnector->pInMbox->count(), 1, "pInMbox->count() != 1");

    pMqttConnector->task();  

	delete pMqttConnector; 
}

//Test receive msg eLteDisconnected 
ZTEST(tests_net_mqtt_connector, test_net_mqtt_connector_rcv_msg_eLteDisconnected)
{
	MqttConnector* pMqttConnector = new MqttConnector("Mqtt");
	zassert_not_null(pMqttConnector, "memory alloc failed");
	zassert_not_null(pMqttConnector->pInMbox, "pInMbox alloc failed");
	zassert_not_null(pMqttConnector->myTimer, "myTimer alloc failed");

    pMqttConnector->lte_connected = true;

    Mbox::sendMsg(msgType::eLteDisconnected, Mbox::eMqttConnector, Mbox::eMqttConnector);

    zassert_equal(pMqttConnector->pInMbox->count(), 1, "pInMbox->count() != 1");

    pMqttConnector->task();  
    zassert_false(pMqttConnector->lte_connected, "lte_connected != false");

	delete pMqttConnector; 
}

//Test receive msg eJsonConfig 
ZTEST(tests_net_mqtt_connector, test_net_mqtt_connector_rcv_msg_eJsonConfig)
{
	uint8_t buf[16] = {1,2,3,4,5,6,7,8,9,0,1,2,3,4,5,6};

	MqttConnector* pMqttConnector = new MqttConnector("Mqtt");
	zassert_not_null(pMqttConnector, "memory alloc failed");
	zassert_not_null(pMqttConnector->pInMbox, "pInMbox alloc failed");
	zassert_not_null(pMqttConnector->myTimer, "myTimer alloc failed");

    pMqttConnector->lte_connected = true;
	gbRetVal = true;

    Mbox::sendMsg(msgType::eJsonConfig, Mbox::eMqttConnector, Mbox::eMqttConnector, buf, sizeof(buf));

    zassert_equal(pMqttConnector->pInMbox->count(), 1, "pInMbox->count() != 1");

    pMqttConnector->task();  

	delete pMqttConnector; 
}

//Test receive msg eJsonConfig 
ZTEST(tests_net_mqtt_connector, test_net_mqtt_connector_rcv_msg_eJsonConfig2)
{
	uint8_t buf[16] = {1,2,3,4,5,6,7,8,9,0,1,2,3,4,5,6};

	MqttConnector* pMqttConnector = new MqttConnector("Mqtt");
	zassert_not_null(pMqttConnector, "memory alloc failed");
	zassert_not_null(pMqttConnector->pInMbox, "pInMbox alloc failed");
	zassert_not_null(pMqttConnector->myTimer, "myTimer alloc failed");

    pMqttConnector->lte_connected = true;
	gbRetVal = false;

    Mbox::sendMsg(msgType::eJsonConfig, Mbox::eMqttConnector, Mbox::eMqttConnector, buf, sizeof(buf));

    zassert_equal(pMqttConnector->pInMbox->count(), 1, "pInMbox->count() != 1");

    pMqttConnector->task();  

	delete pMqttConnector; 
}

//Test receive msg eFactoryDefault 
ZTEST(tests_net_mqtt_connector, test_net_mqtt_connector_rcv_msg_eFactoryDefault)
{
	MqttConnector* pMqttConnector = new MqttConnector("Mqtt");
	zassert_not_null(pMqttConnector, "memory alloc failed");
	zassert_not_null(pMqttConnector->pInMbox, "pInMbox alloc failed");
	zassert_not_null(pMqttConnector->myTimer, "myTimer alloc failed");

    pMqttConnector->lte_connected = true;

    Mbox::sendMsg(msgType::eFactoryDefault, Mbox::eMqttConnector, Mbox::eMqttConnector);

    zassert_equal(pMqttConnector->pInMbox->count(), 1, "pInMbox->count() != 1");

    pMqttConnector->task();  

	delete pMqttConnector; 
}


//Test receive msg eColdStartReq 
ZTEST(tests_net_mqtt_connector, test_net_mqtt_connector_rcv_msg_eColdStartReq)
{
    PoolElem* pElem;

	Mbox* pResetCtrlMbox = new Mbox("ResetCtrlMbox");
    Mbox::registerMbox(pResetCtrlMbox, Mbox::eResetCtrl);
	zassert_not_null(pResetCtrlMbox, "pResetCtrlMbox = NULL");

	MqttConnector* pMqttConnector = new MqttConnector("Mqtt");
	zassert_not_null(pMqttConnector, "memory alloc failed");
	zassert_not_null(pMqttConnector->pInMbox, "pInMbox alloc failed");
	zassert_not_null(pMqttConnector->myTimer, "myTimer alloc failed");

    pMqttConnector->lte_connected = true;

    Mbox::sendMsg(msgType::eColdStartReq, Mbox::eMqttConnector, Mbox::eMqttConnector);

    zassert_equal(pMqttConnector->pInMbox->count(), 1, "pInMbox->count() != 1");

    pMqttConnector->task();  

    zassert_equal(pMqttConnector->pInMbox->count(), 0, "pInMbox->count() != 0");
    zassert_equal(pResetCtrlMbox->count(), 1, "pMqttConnMbox->count() != 1");

    pElem = pResetCtrlMbox->read();
    zassert_equal(pElem->mtype, msgType::eColdStartReq, "Rcvd mtype != msgType::eColdStartReq");
    zassert_equal(pElem->from, Mbox::eMqttConnector, "Rcvd sender != Mbox::eMqttConnector");

	pElem->free();
	delete pResetCtrlMbox;
	delete pMqttConnector; 
}

//Test receive msg eBoostOnReq 
ZTEST(tests_net_mqtt_connector, test_net_mqtt_connector_rcv_msg_eBoostOnReq)
{
    PoolElem* pElem;

	Mbox* pBoostMbox = new Mbox("BoostMbox");
    Mbox::registerMbox(pBoostMbox, Mbox::eBoostConverter);
	zassert_not_null(pBoostMbox, "pBoostMbox = NULL");

	MqttConnector* pMqttConnector = new MqttConnector("Mqtt");
	zassert_not_null(pMqttConnector, "memory alloc failed");
	zassert_not_null(pMqttConnector->pInMbox, "pInMbox alloc failed");
	zassert_not_null(pMqttConnector->myTimer, "myTimer alloc failed");

    pMqttConnector->lte_connected = true;

    Mbox::sendMsg(msgType::eBoostOnReq, Mbox::eMqttConnector, Mbox::eMqttConnector);

    zassert_equal(pMqttConnector->pInMbox->count(), 1, "pInMbox->count() != 1");

    pMqttConnector->task();  

    zassert_equal(pMqttConnector->pInMbox->count(), 0, "pInMbox->count() != 0");
    zassert_equal(pBoostMbox->count(), 1, "pMqttConnMbox->count() != 1");

    pElem = pBoostMbox->read();
    zassert_equal(pElem->mtype, msgType::eBoostOnReq, "Rcvd mtype != msgType::eBoostOnReq");
    zassert_equal(pElem->from, Mbox::eMqttConnector, "Rcvd sender != Mbox::eMqttConnector");

	pElem->free();
	delete pBoostMbox;
	delete pMqttConnector; 
}

//Test receive msg eBoostOffReq 
ZTEST(tests_net_mqtt_connector, test_net_mqtt_connector_rcv_msg_eBoostOffReq)
{
   PoolElem* pElem;

	Mbox* pBoostMbox = new Mbox("BoostMbox");
    Mbox::registerMbox(pBoostMbox, Mbox::eBoostConverter);
	zassert_not_null(pBoostMbox, "pBoostMbox = NULL");

	MqttConnector* pMqttConnector = new MqttConnector("Mqtt");
	zassert_not_null(pMqttConnector, "memory alloc failed");
	zassert_not_null(pMqttConnector->pInMbox, "pInMbox alloc failed");
	zassert_not_null(pMqttConnector->myTimer, "myTimer alloc failed");

    pMqttConnector->lte_connected = true;

    Mbox::sendMsg(msgType::eBoostOffReq, Mbox::eMqttConnector, Mbox::eMqttConnector);

    zassert_equal(pMqttConnector->pInMbox->count(), 1, "pInMbox->count() != 1");

    pMqttConnector->task();  

    zassert_equal(pMqttConnector->pInMbox->count(), 0, "pInMbox->count() != 0");
    zassert_equal(pBoostMbox->count(), 1, "pMqttConnMbox->count() != 1");

    pElem = pBoostMbox->read();
    zassert_equal(pElem->mtype, msgType::eBoostOffReq, "Rcvd mtype != msgType::eBoostOffReq");
    zassert_equal(pElem->from, Mbox::eMqttConnector, "Rcvd sender != Mbox::eMqttConnector");

	pElem->free();
	delete pBoostMbox;
	delete pMqttConnector; 
}

//Test receive msg eFsErase 
ZTEST(tests_net_mqtt_connector, test_net_mqtt_connector_rcv_msg_eFsErase)
{
	MqttConnector* pMqttConnector = new MqttConnector("Mqtt");
	zassert_not_null(pMqttConnector, "memory alloc failed");
	zassert_not_null(pMqttConnector->pInMbox, "pInMbox alloc failed");
	zassert_not_null(pMqttConnector->myTimer, "myTimer alloc failed");

    pMqttConnector->lte_connected = true;

    Mbox::sendMsg(msgType::eFsErase, Mbox::eMqttConnector, Mbox::eMqttConnector);

    zassert_equal(pMqttConnector->pInMbox->count(), 1, "pInMbox->count() != 1");

    pMqttConnector->task();  

	delete pMqttConnector; 
}

//Test receive msg eFsStatus 
ZTEST(tests_net_mqtt_connector, test_net_mqtt_connector_rcv_msg_eFsStatus)
{
	MqttConnector* pMqttConnector = new MqttConnector("Mqtt");
	zassert_not_null(pMqttConnector, "memory alloc failed");
	zassert_not_null(pMqttConnector->pInMbox, "pInMbox alloc failed");
	zassert_not_null(pMqttConnector->myTimer, "myTimer alloc failed");

    pMqttConnector->lte_connected = true;

    Mbox::sendMsg(msgType::eFsStatus, Mbox::eMqttConnector, Mbox::eMqttConnector);

    zassert_equal(pMqttConnector->pInMbox->count(), 1, "pInMbox->count() != 1");

    pMqttConnector->task();  

	delete pMqttConnector; 
}

//Test receive msg eDisableADXL 
ZTEST(tests_net_mqtt_connector, test_net_mqtt_connector_rcv_msg_eDisableADXL)
{
    PoolElem* pElem;

	Mbox* pAdxlMbox = new Mbox("AdxlMbox");
    Mbox::registerMbox(pAdxlMbox, Mbox::eAdxl37xTask);
	zassert_not_null(pAdxlMbox, "pAdxlMbox = NULL");

	MqttConnector* pMqttConnector = new MqttConnector("Mqtt");
	zassert_not_null(pMqttConnector, "memory alloc failed");
	zassert_not_null(pMqttConnector->pInMbox, "pInMbox alloc failed");
	zassert_not_null(pMqttConnector->myTimer, "myTimer alloc failed");

    pMqttConnector->lte_connected = true;

    Mbox::sendMsg(msgType::eDisableADXL, Mbox::eMqttConnector, Mbox::eMqttConnector);

    zassert_equal(pMqttConnector->pInMbox->count(), 1, "pInMbox->count() != 1");

    pMqttConnector->task();  

    zassert_equal(pMqttConnector->pInMbox->count(), 0, "pInMbox->count() != 0");
    zassert_equal(pAdxlMbox->count(), 1, "pMqttConnMbox->count() != 1");

    pElem = pAdxlMbox->read();
    zassert_equal(pElem->mtype, msgType::eDisableADXL, "Rcvd mtype != msgType::eDisableADXL");
    zassert_equal(pElem->from, Mbox::eMqttConnector, "Rcvd sender != Mbox::eMqttConnector");

	pElem->free();
	delete pAdxlMbox;
	delete pMqttConnector; 
}

//Test receive msg eEnableADXL 
ZTEST(tests_net_mqtt_connector, test_net_mqtt_connector_rcv_msg_eEnableADXL)
{
    PoolElem* pElem;

	Mbox* pAdxlMbox = new Mbox("AdxlMbox");
    Mbox::registerMbox(pAdxlMbox, Mbox::eAdxl37xTask);
	zassert_not_null(pAdxlMbox, "pAdxlMbox = NULL");

	MqttConnector* pMqttConnector = new MqttConnector("Mqtt");
	zassert_not_null(pMqttConnector, "memory alloc failed");
	zassert_not_null(pMqttConnector->pInMbox, "pInMbox alloc failed");
	zassert_not_null(pMqttConnector->myTimer, "myTimer alloc failed");

    pMqttConnector->lte_connected = true;

    Mbox::sendMsg(msgType::eEnableADXL, Mbox::eMqttConnector, Mbox::eMqttConnector);

    zassert_equal(pMqttConnector->pInMbox->count(), 1, "pInMbox->count() != 1");

    pMqttConnector->task();  

    zassert_equal(pMqttConnector->pInMbox->count(), 0, "pInMbox->count() != 0");
    zassert_equal(pAdxlMbox->count(), 1, "pMqttConnMbox->count() != 1");

    pElem = pAdxlMbox->read();
    zassert_equal(pElem->mtype, msgType::eEnableADXL, "Rcvd mtype != msgType::eEnableADXL");
    zassert_equal(pElem->from, Mbox::eMqttConnector, "Rcvd sender != Mbox::eMqttConnector");

	pElem->free();
	delete pAdxlMbox;
	delete pMqttConnector; 
}

//Test receive msg eGetConfig 
ZTEST(tests_net_mqtt_connector, test_net_mqtt_connector_rcv_msg_eGetConfig)
{
	MqttConnector* pMqttConnector = new MqttConnector("Mqtt");
	zassert_not_null(pMqttConnector, "memory alloc failed");
	zassert_not_null(pMqttConnector->pInMbox, "pInMbox alloc failed");
	zassert_not_null(pMqttConnector->myTimer, "myTimer alloc failed");

    pMqttConnector->lte_connected = true;

    Mbox::sendMsg(msgType::eGetConfig, Mbox::eMqttConnector, Mbox::eMqttConnector);

    zassert_equal(pMqttConnector->pInMbox->count(), 1, "pInMbox->count() != 1");

    pMqttConnector->task();  

	delete pMqttConnector; 
}

//Test receive unexpected msg 
ZTEST(tests_net_mqtt_connector, test_net_mqtt_connector_rcv_msg_unexpected)
{
	MqttConnector* pMqttConnector = new MqttConnector("Mqtt");
	zassert_not_null(pMqttConnector, "memory alloc failed");
	zassert_not_null(pMqttConnector->pInMbox, "pInMbox alloc failed");
	zassert_not_null(pMqttConnector->myTimer, "myTimer alloc failed");

    Mbox::sendMsg(msgType::eResetCause, Mbox::eMqttConnector, Mbox::eMqttConnector);

    zassert_equal(pMqttConnector->pInMbox->count(), 1, "pInMbox->count() != 1");

    pMqttConnector->task();  

    zassert_equal(pMqttConnector->pInMbox->count(), 0, "pInMbox->count() != 0");
	delete pMqttConnector; 
}

//Test receive no msg 
ZTEST(tests_net_mqtt_connector, test_net_mqtt_connector_rcv_msg_none)
{
	MqttConnector* pMqttConnector = new MqttConnector("Mqtt");
	zassert_not_null(pMqttConnector, "memory alloc failed");
	zassert_not_null(pMqttConnector->pInMbox, "pInMbox alloc failed");
	zassert_not_null(pMqttConnector->myTimer, "myTimer alloc failed");

    pMqttConnector->pInMbox->write(NULL);
    
    pMqttConnector->task();  

    zassert_equal(pMqttConnector->pInMbox->count(), 0, "pInMbox->count() != 0");
	delete pMqttConnector; 
}

