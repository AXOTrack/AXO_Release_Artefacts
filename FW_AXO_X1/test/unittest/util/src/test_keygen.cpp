#include <zephyr/ztest.h>


#define private public
#include "util/keygen.h"

#include "stubs.h"

extern int entropy_func2(void* ctx, unsigned char* buf, size_t len);

ZTEST_SUITE(tests_util_keygen, NULL, NULL, NULL, NULL, NULL);

// test constructor and destructor
ZTEST(tests_util_keygen, test_util_keygen_constructor)
{
	KeyGen* pKeyGen = new KeyGen();
	zassert_not_null(pKeyGen, "memory alloc failed");
	zassert_false(pKeyGen->keygen_done, "keygen_done != false");
	zassert_false(pKeyGen->transfer_done, "transfer_done != false");
	delete pKeyGen;
}

// MISSING for 100% branch coverage static instantiation does not deliver NULL
// here code line 
// static KeyGen myInstance;
// are not covered completly.


// test function entropy_func2
ZTEST(tests_util_keygen, test_util_keygen_entropy_func2)
{
	int iRet = entropy_func2(NULL, NULL, 1);
	zassert_equal(iRet, 1, "iRet != 1");
}

// test function init
ZTEST(tests_util_keygen, test_util_keygen_init)
{
	gIntRetVal = 20;
	gbReady  = true;
	KeyGen::inst()->init();
	zassert_false(KeyGen::inst()->keygen_done, "keygen_done != false");
	zassert_false(KeyGen::inst()->transfer_done, "transfer_done != false");

	gIntRetVal = 20;
	gbReady  = false;
	KeyGen::inst()->init();
	zassert_false(KeyGen::inst()->keygen_done, "keygen_done != false");
	zassert_false(KeyGen::inst()->transfer_done, "transfer_done != false");

	gIntRetVal = 23;
	gbRetVal = true;
	KeyGen::inst()->init();
	zassert_true(KeyGen::inst()->keygen_done, "keygen_done != true");
	zassert_true(KeyGen::inst()->transfer_done, "transfer_done != true");

	gIntRetVal = 20;
	gbReady  = false;
	KeyGen::inst()->init();
	zassert_true(KeyGen::inst()->keygen_done, "keygen_done != true");
	zassert_true(KeyGen::inst()->transfer_done, "transfer_done != true");

	gIntRetVal = 21;
	gbRetVal = false;
	KeyGen::inst()->init();
	zassert_true(KeyGen::inst()->keygen_done, "keygen_done != true");
	zassert_true(KeyGen::inst()->transfer_done, "transfer_done != true");

	gIntRetVal = 22;
	gbRetVal = false;
	KeyGen::inst()->init();
	zassert_true(KeyGen::inst()->keygen_done, "keygen_done != true");
	zassert_true(KeyGen::inst()->transfer_done, "transfer_done != true");

	gIntRetVal = 2000;
	gbRetVal = false;
	KeyGen::inst()->init();
	zassert_true(KeyGen::inst()->keygen_done, "keygen_done != true");
	zassert_true(KeyGen::inst()->transfer_done, "transfer_done != true");
}

// test function bIsKeyTransfered
ZTEST(tests_util_keygen, test_util_keygen_bIsKeyTransfered)
{
	bool bRet;

	KeyGen::inst()->transfer_done = true;
	bRet = KeyGen::inst()->bIsKeyTransfered();
	zassert_true(KeyGen::inst()->transfer_done, "transfer_done != true");

	KeyGen::inst()->transfer_done = false;
	bRet = KeyGen::inst()->bIsKeyTransfered();
	zassert_false(KeyGen::inst()->transfer_done, "transfer_done != false");
}

// test function setKeyTransfered
ZTEST(tests_util_keygen, test_util_keygen_setKeyTransfered)
{
	KeyGen::inst()->setKeyTransfered();
	zassert_true(KeyGen::inst()->transfer_done, "transfer_done != true");
	zassert(strcmp(gBuf, "1")==0, "gBuf %s not correct", gBuf);
}

// test function getPublicKey
ZTEST(tests_util_keygen, test_util_keygen_getPublicKey)
{
	uint16_t uRet; 
	char buf[1024];

	strncpy(gBuf, "1234567890", 32);
	gbReady = true;

	uRet = KeyGen::inst()->getPublicKey((unsigned char*)buf, 10);
	zassert_equal(uRet, 10, "uRet != 10");
	zassert(strcmp((const char*)gBuf, (const char*)buf)==0, "buf %s not correct", buf);
}

// test function getPrivateKey, printKey and deleteKeyFiles
ZTEST(tests_util_keygen, test_util_keygen_getPrivateKey)
{
	std::string s;

	strncpy(gBuf, "1234567890", 32);
	gbReady = true;

	s = KeyGen::inst()->getPrivateKey();
	zassert_equal(s.length(), 10, "s.length() != 10");
	zassert(strcmp((const char*)gBuf, (const char*)s.c_str())==0, "string %s not correct", s.c_str());

	KeyGen::inst()->printKey((unsigned char*)s.c_str(), 10);
	KeyGen::inst()->printKey(s);

	gIntRetVal = 20;
	KeyGen::inst()->deleteKeyFiles();
	gIntRetVal = 2000;
	KeyGen::inst()->deleteKeyFiles();
}


