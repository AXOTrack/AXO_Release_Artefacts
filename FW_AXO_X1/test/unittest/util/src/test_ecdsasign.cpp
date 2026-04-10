#include <zephyr/ztest.h>


#define private public
#include "util/ecdsasign.h"

#include "stubs.h"

extern int entropy_func(void* ctx, unsigned char* buf, size_t len);

ZTEST_SUITE(tests_util_ecdsasign, NULL, NULL, NULL, NULL, NULL);

// test constructor and destructor
ZTEST(tests_util_ecdsasign, test_util_ecdsasign_constructor)
{
	gbReady = true;
	ECDSASigner* pECDSASigner = new ECDSASigner();
	zassert_not_null(pECDSASigner, "memory alloc failed");
	delete pECDSASigner;
}

// MISSING for 100% branch coverage static instantiation does not deliver NULL
// here code line 
// static ECDSASigner myInstance;
// are not covered completly.

// test constructor and destructor
ZTEST(tests_util_ecdsasign, test_util_ecdsasign_constructor2)
{
	gbReady = false;
	ECDSASigner* pECDSASigner = new ECDSASigner();
	zassert_not_null(pECDSASigner, "memory alloc failed");
	delete pECDSASigner;
}

// test function entropy_func
ZTEST(tests_util_ecdsasign, test_util_ecdsasign_entropy_func)
{
	unsigned char buf[16];
	int iRet = entropy_func(NULL, buf, 16);
	zassert_equal(iRet, 0, "iRet != 0");
}

// test function setPrivateKey
ZTEST(tests_util_ecdsasign, test_util_ecdsasign_setPrivateKey)
{
	ECDSASigner::inst()->setPrivateKey("qwertz");
}

// test function signHash
ZTEST(tests_util_ecdsasign, test_util_ecdsasign_signHash)
{
	std::string s; 
	s = ECDSASigner::inst()->signHash((const unsigned char*)"12345678980", 10);
	//printk("s = %s\n", s.c_str());
	zassert(strcmp(s.c_str(), "77777")==0, "string %s not correct", s.c_str());	
}

// test function string_to_hex
ZTEST(tests_util_ecdsasign, test_util_ecdsasign_string_to_hex)
{
	std::string s; 
	s = ECDSASigner::inst()->string_to_hex("64");
	//printk("s = %s\n", s.c_str());
	zassert(strcmp(s.c_str(), "3634")==0, "string %s not correct", s.c_str());	
}

// test function printSignature
ZTEST(tests_util_ecdsasign, test_util_ecdsasign_printSignature)
{
	ECDSASigner::inst()->printSignature("12345678980");
}

