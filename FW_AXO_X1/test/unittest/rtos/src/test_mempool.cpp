#include <stddef.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
//#include <zephyr/kernel.h>
#include <zephyr/ztest.h>

#include "rtos/mbox.h"
#include "rtos/mpelem.h"
#include "rtos/mpool.h"
#include "rtos/mqueue.h"

#include "stubs.h"
 


ZTEST_SUITE(tests_mempool, NULL, NULL, NULL, NULL, NULL);

ZTEST(tests_mempool, mempool_test)
{
	int32_t rc;
    unsigned char (*out1)[16];
    unsigned char (*out2)[16];
    unsigned char (*out3)[16];
    unsigned char (*out4)[16];
    unsigned char (*out5)[16];
    uint16_t in;
	std::string s;
	std::string exp;

    MemoryPool<uint8_t[16], 4> p;

    out1 = p.alloc();
	zassert_equal(p.size(), 4, "size() != 4");
	zassert_equal(p.blocksize(), 16, "blocksize() != 16");
	zassert_equal(p.used(), 1, "used() != 1");
	zassert_equal(p.maxused(), 1, "maxused() != 1");
	zassert_equal(p.available(), 3, "available() != 3");
    p.stats();

    out2 = p.alloc();
	zassert_equal(p.used(), 2, "used() != 2");
	zassert_equal(p.maxused(), 2, "maxused() != 2");
	zassert_equal(p.available(), 2, "available() != 2");
    p.stats();

    out3 = p.alloc();
	zassert_equal(p.used(), 3, "used() != 3");
	zassert_equal(p.maxused(), 3, "maxused() != 3");
	zassert_equal(p.available(), 1, "available() != 1");
    p.stats();

    p.free(out2);
	zassert_equal(p.used(), 2, "used() != 2");
	zassert_equal(p.maxused(), 3, "maxused() != 3");
	zassert_equal(p.available(), 2, "available() != 2");
    p.stats();

    out2 = p.alloc();
	zassert_equal(p.used(), 3, "used() != 3");
	zassert_equal(p.maxused(), 3, "maxused() != 3");
	zassert_equal(p.available(), 1, "available() != 1");
    p.stats();

    out4 = p.alloc();
	zassert_equal(p.used(), 4, "used() != 4");
	zassert_equal(p.maxused(), 4, "maxused() != 4");
	zassert_equal(p.available(), 0, "available() != 0");
    p.stats();

    out5 = p.alloc();
    zassert_is_null(out5, "memory pool empty");
    p.stats();
}

ZTEST(tests_mempool, mempool_test2)
{
	int32_t rc;
    unsigned char (*out1)[64];
    unsigned char (*out2)[64];
    unsigned char (*out3)[64];
    unsigned char (*out4)[64];
    unsigned char (*out5)[64];
    uint16_t in;
	std::string s;
	std::string exp;

    MemoryPool<uint8_t[64], 4> p;

    out1 = p.alloc();
	zassert_equal(p.size(), 4, "size() != 4");
	zassert_equal(p.blocksize(), 64, "blocksize() != 64");
	zassert_equal(p.used(), 1, "used() != 1");
	zassert_equal(p.maxused(), 1, "maxused() != 1");
	zassert_equal(p.available(), 3, "available() != 3");
    p.stats();

    out2 = p.alloc();
	zassert_equal(p.used(), 2, "used() != 2");
	zassert_equal(p.maxused(), 2, "maxused() != 2");
	zassert_equal(p.available(), 2, "available() != 2");
    p.stats();

    out3 = p.alloc();
	zassert_equal(p.used(), 3, "used() != 3");
	zassert_equal(p.maxused(), 3, "maxused() != 3");
	zassert_equal(p.available(), 1, "available() != 1");
    p.stats();

    p.free(out2);
	zassert_equal(p.used(), 2, "used() != 2");
	zassert_equal(p.maxused(), 3, "maxused() != 3");
	zassert_equal(p.available(), 2, "available() != 2");
    p.stats();

    out2 = p.alloc();
	zassert_equal(p.used(), 3, "used() != 3");
	zassert_equal(p.maxused(), 3, "maxused() != 3");
	zassert_equal(p.available(), 1, "available() != 1");
    p.stats();

    out4 = p.alloc();
	zassert_equal(p.used(), 4, "used() != 4");
	zassert_equal(p.maxused(), 4, "maxused() != 4");
	zassert_equal(p.available(), 0, "available() != 0");
    p.stats();

    out5 = p.alloc();
    zassert_is_null(out5, "memory pool empty");
    p.stats();
}

/*ZTEST(mempool_tests, test_mempool) { zassert_true(1, "1 was false"); }

ZTEST(mempool_tests, test_mempool_init) {
    // Test if memory pool is initialized properly
}

ZTEST(mempool_tests, test_mempool_alloc) {
    // Test if different sized memory elements can be obtained from the memory
    // pool
}

ZTEST(mempool_tests, test_mempool_free) {
    // Test if obtained memory pool elements are freed properly
}*/