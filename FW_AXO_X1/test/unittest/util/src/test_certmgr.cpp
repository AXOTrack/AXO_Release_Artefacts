#include <zephyr/ztest.h>
#include <vector>
#include <algorithm>

#include <cJSON.h>
#include <cJSON_os.h>

#define private public
#include "util/certmgr.h"

#include "stubs.h"

ZTEST_SUITE(tests_util_certmgr, NULL, NULL, NULL, NULL, NULL);

// test constructor and destructor
ZTEST(tests_util_certmgr, test_util_certmgr_constructor)
{
	certmgr* pCertmgr = new certmgr();

	zassert_not_null(pCertmgr, "memory alloc failed");

	delete pCertmgr;
}

// test function init
ZTEST(tests_util_certmgr, test_util_certmgr_init)
{
	gbRetVal = true;
	gIntRetVal = 1;
	strncpy(gBuf, "3,3", 1024);
	certmgr::inst()->init();
	zassert_equal(certmgr::inst()->brokerCfg.currentBroker, 3, "currentBroker != 3");
	zassert_equal(certmgr::inst()->brokerCfg.desiredBroker, 3, "desiredBroker != 3");
}

// test function CheckCertExistInModem
ZTEST(tests_util_certmgr, test_util_certmgr_CheckCertExistInModem)
{
	bool bRet; 
	gIntRetVal = -1;
	bRet = certmgr::inst()->CheckCertExistInModem(MODEM_KEY_MGMT_CRED_TYPE_PUBLIC_CERT);
	zassert_false(bRet, "bRet != false");

	gIntRetVal = 0;
	bRet = certmgr::inst()->CheckCertExistInModem(MODEM_KEY_MGMT_CRED_TYPE_PUBLIC_CERT);
	zassert_false(bRet, "bRet != false");

	gIntRetVal = 1;
	bRet = certmgr::inst()->CheckCertExistInModem(MODEM_KEY_MGMT_CRED_TYPE_PUBLIC_CERT);
	zassert_true(bRet, "bRet != true");
}

// test function CompareRootCertInModem
#if 0
ZTEST(tests_util_certmgr, test_util_certmgr_CompareRootCertInModem)
{
	bool bRet; 
	gbRetVal = true;
	bRet = certmgr::inst()->CompareRootCertInModem(JoviaRootCert, sizeof(JoviaRootCert));
	zassert_true(bRet, "bRet != true");

	gbRetVal = false;
	bRet = certmgr::inst()->CheckCertExistInModem(JoviaRootCert, sizeof(JoviaRootCert));
	zassert_false(bRet, "bRet != false");
}
#endif

// test function GetConfiguredBrokerInModem
ZTEST(tests_util_certmgr, test_util_certmgr_GetConfiguredBrokerInModem)
{
	enBroker eRet; 

	gIntRetVal = 1;
	gbRetVal = true;
	eRet = certmgr::inst()->GetConfiguredBrokerInModem();
	//printk("broker == %d\n", eRet);
	zassert_equal(eRet, eBroker_JOVIA, "eRet != eBroker_JOVIA");

	gbRetVal = false;
	eRet = certmgr::inst()->GetConfiguredBrokerInModem();
	//printk("broker == %d\n", eRet);
	zassert_equal(eRet, eBroker_THINGSBOARD, "eRet != eBroker_THINGSBOARD");

	gbRetVal = true;
	eRet = certmgr::inst()->GetConfiguredBrokerInModem();
	//printk("broker == %d\n", eRet);
	zassert_equal(eRet, eBroker_JOVIA, "eRet != eBroker_JOVIA");

	gbRetVal = false;
	eRet = certmgr::inst()->GetConfiguredBrokerInModem();
	//printk("broker == %d\n", eRet);
	zassert_equal(eRet, eBroker_THINGSBOARD, "eRet != eBroker_THINGSBOARD");

	gIntRetVal = -1;
	gbRetVal = false;
	eRet = certmgr::inst()->GetConfiguredBrokerInModem();
	//printk("broker == %d\n", eRet);
	zassert_equal(eRet, eBroker_NONE, "eRet != eBroker_NONE");

}

// test function WriteJoviaRootCert2Modem
ZTEST(tests_util_certmgr, test_util_certmgr_WriteJoviaRootCert2Modem)
{
	int iRet; 
	gbRetVal = true;

	iRet = certmgr::inst()->WriteJoviaRootCert2Modem();
	zassert_equal(iRet, 0, "iRet != 0");
}

// test function WriteThingsboardRootCert2Modem
ZTEST(tests_util_certmgr, test_util_certmgr_WriteThingsboardRootCert2Modem)
{
	int iRet; 
	gbRetVal = true;

	iRet = certmgr::inst()->WriteThingsboardRootCert2Modem();
	zassert_equal(iRet, 0, "iRet != 0");
}

// test function WriteDeviceCert2Modem
ZTEST(tests_util_certmgr, test_util_certmgr_WriteDeviceCert2Modem)
{
	int iRet; 
	gbRetVal = true;

	iRet = certmgr::inst()->WriteDeviceCert2Modem();
	zassert_equal(iRet, 0, "iRet != 0");
}

// test function WriteDeviceKey2Modem
ZTEST(tests_util_certmgr, test_util_certmgr_WriteDeviceKey2Modem)
{
	int iRet; 
	gbRetVal = true;

	iRet = certmgr::inst()->WriteDeviceKey2Modem();
	zassert_equal(iRet, 0, "iRet != 0");
}


// test function AllPrecondFullFilled
ZTEST(tests_util_certmgr, test_util_certmgr_AllPrecondFullFilled)
{
	bool bRet; 

	gIntRetVal = 1;
	bRet = certmgr::inst()->AllPrecondFullFilled();
	zassert_false(bRet, "bRet != false");

	gIntRetVal = 2;
	bRet = certmgr::inst()->AllPrecondFullFilled();
	zassert_false(bRet, "bRet != false");

	gIntRetVal = 3;
	bRet = certmgr::inst()->AllPrecondFullFilled();
	zassert_false(bRet, "bRet != false");

	gIntRetVal = 10;
	bRet = certmgr::inst()->AllPrecondFullFilled();
	zassert_false(bRet, "bRet != false");

	gIntRetVal = 11;
	bRet = certmgr::inst()->AllPrecondFullFilled();
	zassert_false(bRet, "bRet != false");

	gIntRetVal = 12;
	bRet = certmgr::inst()->AllPrecondFullFilled();
	zassert_false(bRet, "bRet != false");

	gIntRetVal = 13;
	bRet = certmgr::inst()->AllPrecondFullFilled();
	zassert_false(bRet, "bRet != false");

	gIntRetVal = 14;
	bRet = certmgr::inst()->AllPrecondFullFilled();
	zassert_true(bRet, "bRet != true");
}

/*/ test function readBrokerCfg
ZTEST(tests_util_certmgr, test_util_readBrokerCfg)
{
	stBrokerCfg cfg;

	gIntRetVal = 1;
	cfg = certmgr::inst()->readBrokerCfg();
	printk("current = %d, desired = %d\n", cfg.currentBroker, cfg.desiredBroker);
	//zassert_equal(cfg.currentBroker, 0, "currentBroker != 0");
	//zassert_equal(cfg.desiredBroker, 0, "desiredBroker != 0");
}

// test function readBrokerCfg
ZTEST(tests_util_certmgr, test_util_readBrokerCfg2)
{
	stBrokerCfg cfg;

	gIntRetVal = 4;
	strncpy(gBuf, "3,3", 1024);
	cfg = certmgr::inst()->readBrokerCfg();
	printk("current = %d, desired = %d\n", cfg.currentBroker, cfg.desiredBroker);
	//zassert_equal(cfg.currentBroker, 0, "currentBroker != 0");
	//zassert_equal(cfg.desiredBroker, 0, "desiredBroker != 0");
}

// test function readBrokerCfg
ZTEST(tests_util_certmgr, test_util_readBrokerCfg3)
{
	stBrokerCfg cfg;

	gIntRetVal = 4;
	strncpy(gBuf, "3,7", 1024);
	cfg = certmgr::inst()->readBrokerCfg();
	printk("current = %d, desired = %d\n", cfg.currentBroker, cfg.desiredBroker);
	//zassert_equal(cfg.currentBroker, 0, "currentBroker != 0");
	//zassert_equal(cfg.desiredBroker, 0, "desiredBroker != 0");
}

ZTEST(tests_util_certmgr, test_util_readBrokerCfg4)
{
	stBrokerCfg cfg;

	gIntRetVal = 4;
	// test function readBrokerCfg
	strncpy(gBuf, "7,3", 1024);
	cfg = certmgr::inst()->readBrokerCfg();
	printk("current = %d, desired = %d\n", cfg.currentBroker, cfg.desiredBroker);
	//zassert_equal(cfg.currentBroker, 0, "currentBroker != 0");
	//zassert_equal(cfg.desiredBroker, 0, "desiredBroker != 0");
}
*/

// test function writeBrokerCfg
ZTEST(tests_util_certmgr, test_util_writeBrokerCfg)
{
	gIntRetVal = 1;
	certmgr::inst()->writeBrokerCfg(eBroker_JOVIA, eBroker_JOVIA);

	gIntRetVal = 4;
	certmgr::inst()->writeBrokerCfg(eBroker_JOVIA, eBroker_JOVIA);
}

// test function getCurrentBroker
ZTEST(tests_util_certmgr, test_util_getCurrentBroker)
{
	enBroker eRet;

	certmgr::inst()->brokerCfg.currentBroker = eBroker_JOVIA;
	certmgr::inst()->brokerCfg.desiredBroker = eBroker_JOVIA;

	eRet = certmgr::inst()->getCurrentBroker();
	zassert_equal(eRet, eBroker_JOVIA, "eRet != eBroker_JOVIA");

	certmgr::inst()->brokerCfg.currentBroker = eBroker_THINGSBOARD;
	certmgr::inst()->brokerCfg.desiredBroker = eBroker_THINGSBOARD;

	eRet = certmgr::inst()->getCurrentBroker();
	zassert_equal(eRet, eBroker_THINGSBOARD, "eRet != eBroker_THINGSBOARD");

	certmgr::inst()->brokerCfg.currentBroker = eBroker_NONE;
	certmgr::inst()->brokerCfg.desiredBroker = eBroker_NONE;

	eRet = certmgr::inst()->getCurrentBroker();
	zassert_equal(eRet, eBroker_NONE, "eRet != eBroker_NONE");
}

// test function getDesiredBroker
ZTEST(tests_util_certmgr, test_util_getDesiredBroker)
{
	enBroker eRet;

	certmgr::inst()->brokerCfg.currentBroker = eBroker_JOVIA;
	certmgr::inst()->brokerCfg.desiredBroker = eBroker_JOVIA;

	eRet = certmgr::inst()->getDesiredBroker();
	zassert_equal(eRet, eBroker_JOVIA, "eRet != eBroker_JOVIA");

	certmgr::inst()->brokerCfg.currentBroker = eBroker_THINGSBOARD;
	certmgr::inst()->brokerCfg.desiredBroker = eBroker_THINGSBOARD;

	eRet = certmgr::inst()->getDesiredBroker();
	zassert_equal(eRet, eBroker_THINGSBOARD, "eRet != eBroker_THINGSBOARD");

	certmgr::inst()->brokerCfg.currentBroker = eBroker_NONE;
	certmgr::inst()->brokerCfg.desiredBroker = eBroker_NONE;

	eRet = certmgr::inst()->getDesiredBroker();
	zassert_equal(eRet, eBroker_NONE, "eRet != eBroker_NONE");
}

// test function writeDeviceCert
ZTEST(tests_util_certmgr, test_util_writeDeviceCert)
{
	gIntRetVal = 2;
	certmgr::inst()->writeDeviceCert(eBroker_JOVIA, NULL);

	gIntRetVal = 4;
	certmgr::inst()->writeDeviceCert(eBroker_JOVIA, NULL);

	gIntRetVal = 3;
	certmgr::inst()->writeDeviceCert(eBroker_THINGSBOARD, NULL);

	gIntRetVal = 4;
	certmgr::inst()->writeDeviceCert(eBroker_THINGSBOARD, NULL);

	gIntRetVal = 3;
	certmgr::inst()->writeDeviceCert(eBroker_NONE, NULL);
}

// test function appendDeviceCert
ZTEST(tests_util_certmgr, test_util_appendDeviceCert)
{
	gIntRetVal = 2;
	certmgr::inst()->appendDeviceCert(eBroker_JOVIA, NULL);

	gIntRetVal = 4;
	certmgr::inst()->appendDeviceCert(eBroker_JOVIA, NULL);

	gIntRetVal = 3;
	certmgr::inst()->appendDeviceCert(eBroker_THINGSBOARD, NULL);

	gIntRetVal = 4;
	certmgr::inst()->appendDeviceCert(eBroker_THINGSBOARD, NULL);

	gIntRetVal = 3;
	certmgr::inst()->appendDeviceCert(eBroker_NONE, NULL);
}

// test function appendDeviceKey
ZTEST(tests_util_certmgr, test_util_appendDeviceKey)
{
	gIntRetVal = 2;
	certmgr::inst()->appendDeviceKey(eBroker_JOVIA, NULL);
}

// test function printCert
ZTEST(tests_util_certmgr, test_util_printCert)
{
	char s[32] = "1234567890";
	certmgr::inst()->printCert(s);
}

// test function readDeviceCertAndKey
ZTEST(tests_util_certmgr, test_util_readDeviceCertAndKey)
{
	strncpy(gBuf, "sdfsdfsdfssdsdfdf", 1024);
	
	gIntRetVal = 2;
	certmgr::inst()->readDeviceCertAndKey(eBroker_JOVIA);

	gIntRetVal = 4;
	certmgr::inst()->readDeviceCertAndKey(eBroker_JOVIA);

	char s[1024] = "{\"cmd\" : \"CERT_JOVIA1\",\"cert\" : \"-----BEGIN CERTIFICATE-----MIICQTGz8Mt+s=-----END CERTIFICATE----- -----BEGIN PRIVATE KEY-----MIG2Ag-----END PRIVATE KEY-----\"}";
	strncpy(gBuf, s, 1024);

	gIntRetVal = 3;
	certmgr::inst()->readDeviceCertAndKey(eBroker_THINGSBOARD);

	gIntRetVal = 4;
	certmgr::inst()->readDeviceCertAndKey(eBroker_THINGSBOARD);

	gIntRetVal = 3;
	certmgr::inst()->readDeviceCertAndKey(eBroker_NONE);
}

// test function parseCert
ZTEST(tests_util_certmgr, test_util_parseCert)
{
	certmgr::inst()->root_obj = NULL;
	char s[1024] = "{\"cmd\" : \"CERT_JOVIA1\",\"cert\" : \"-----BEGIN CERTIFICATE-----MIICQTGz8Mt+s=-----END CERTIFICATE-----\"}";
	certmgr::inst()->parseCert(s, (char*)"CERT_JOVIA1");
	zassert_not_null(certmgr::inst()->root_obj, "root_obj = NULL");

	certmgr::inst()->root_obj = cJSON_Parse((const char*)s);
	certmgr::inst()->parseCert(s, (char*)"CERT_JOVIA1");
	zassert_not_null(certmgr::inst()->root_obj, "root_obj = NULL");
}

// test function storeJoviaCert1
ZTEST(tests_util_certmgr, test_util_storeJoviaCert1)
{
	char s[1024] = "{\"cmd\" : \"CERT_JOVIA1\",\"cert\" : \"-----BEGIN CERTIFICATE-----MIICQTGz8Mt+s=-----END CERTIFICATE-----\"}";
	certmgr::inst()->storeJoviaCert1(s);
	zassert_not_null(certmgr::inst()->root_obj, "root_obj = NULL");
}

// test function storeJoviaCert2
ZTEST(tests_util_certmgr, test_util_storeJoviaCert2)
{
	char s[1024] = "{\"cmd\" : \"CERT_JOVIA2\",\"cert\" : \"-----BEGIN CERTIFICATE-----MIICQTGz8Mt+s=-----END CERTIFICATE-----\"}";
	certmgr::inst()->storeJoviaCert2(s);
	zassert_not_null(certmgr::inst()->root_obj, "root_obj = NULL");
}

// test function storeJoviaKey
ZTEST(tests_util_certmgr, test_util_storeJoviaKey)
{
	char s[1024] = "{\"cmd\" : \"KEY_JOVIA\",\"cert\" : \"-----BEGIN PRIVATE KEY-----MIICQTGz8Mt+s=-----END PRIVATE KEY-----\"}";
	certmgr::inst()->storeJoviaKey(s);
	zassert_not_null(certmgr::inst()->root_obj, "root_obj = NULL");
}

// test function storeThingsboardCert1
ZTEST(tests_util_certmgr, test_util_storeThingsboardCert1)
{
	char s[1024] = "{\"cmd\" : \"CERT_THINGSBOARD1\",\"cert\" : \"-----BEGIN CERTIFICATE-----MIICQTGz8Mt+s=-----END CERTIFICATE-----\"}";
	certmgr::inst()->storeThingsboardCert1(s);
	zassert_not_null(certmgr::inst()->root_obj, "root_obj = NULL");
}

// test function storeThingsboardCert2
ZTEST(tests_util_certmgr, test_util_storeThingsboardCert2)
{
	char s[1024] = "{\"cmd\" : \"CERT_THINGSBOARD2\",\"cert\" : \"-----BEGIN CERTIFICATE-----MIICQTGz8Mt+s=-----END CERTIFICATE-----\"}";
	certmgr::inst()->storeThingsboardCert2(s);
	zassert_not_null(certmgr::inst()->root_obj, "root_obj = NULL");
}

// test function storeThingsboardKey
ZTEST(tests_util_certmgr, test_util_storeThingsboardKey)
{
	char s[1024] = "{\"cmd\" : \"KEY_THINGSBOARD\",\"cert\" : \"-----BEGIN PRIVATE KEY-----MIICQTGz8Mt+s=-----END PRIVATE KEY-----\"}";
	certmgr::inst()->storeThingsboardKey(s);
	zassert_not_null(certmgr::inst()->root_obj, "root_obj = NULL");
}

// test function GetFsStat ...
ZTEST(tests_util_certmgr, test_util_certmgr_GetFsStat)
{
	certmgr::inst()->GetFsStat();
	certmgr::inst()->EraseFs();
	certmgr::inst()->ListFs();
}


