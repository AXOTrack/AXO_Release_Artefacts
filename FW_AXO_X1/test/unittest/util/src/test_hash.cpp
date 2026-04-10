#include <zephyr/ztest.h>


#define private public
#include "util/hash.h"

#include "stubs.h"

ZTEST_SUITE(tests_util_hash, NULL, NULL, NULL, NULL, NULL);

// test constructor and destructor
ZTEST(tests_util_hash, test_util_hash_constructor)
{
	Hash* pHash = new Hash();
	zassert_not_null(pHash, "memory alloc failed");
	delete pHash;
}

// MISSING for 100% branch coverage static instantiation does not deliver NULL
// here code line 
// static Hash myInstance;
// are not covered completly.


// test function init
ZTEST(tests_util_hash, test_util_hash_init)
{
	Hash::inst()->init();
	printk("state = %d\n\n", Hash::inst()->sha_ctx.state);
	//zassert_equal(Hash::inst()->sha_ctx.state, 17, "sha_ctx.state != 17");
}

// test function update
ZTEST(tests_util_hash, test_util_hash_update)
{
	Hash::inst()->update(NULL, 23);
	zassert_equal(Hash::inst()->sha_ctx.state, 27, "sha_ctx.state != 27");
}

// test function finalize and print
ZTEST(tests_util_hash, test_util_hash_finalize)
{
	uint8_t* pBuf;
	uint16_t len;
	int i;

	Hash::inst()->finalize();
	//printk("state = %d\n\n", Hash::inst()->sha_ctx.state);
	zassert_equal(Hash::inst()->sha_ctx.state, 77, "sha_ctx.state != 77");
	for (i = 0; i < 10; i++) {
		zassert_equal(Hash::inst()->hash[i], 1, "hash[i] != 1");
		zassert_equal(Hash::inst()->hash[i+10], 2, "hash[i+10] != 2");
	}

	Hash::inst()->print();

	pBuf = Hash::inst()->getHash();
	for (i = 0; i < 10; i++) {
		zassert_equal(pBuf[i], 1, "pBuf[i] != 1");
		zassert_equal(pBuf[i+10], 2, "pBuf[i+10] != 2");
	}

	len = Hash::inst()->getHashLen();
	zassert_equal(len, SHA_HASH_SIZE, "len != SHA_HASH_SIZE");
}
