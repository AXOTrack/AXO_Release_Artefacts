#include <zephyr/ztest.h>
#include <vector>
#include <algorithm>

#define private public
#include "rtos/mempool.h"
#include "rtos/mpool.h"
#include "rtos/mpelem.h"

ZTEST_SUITE(tests_msgpool, NULL, NULL, NULL, NULL, NULL);

ZTEST(tests_msgpool, msgpool_test_constructor) {
    MsgPool* pMsgPool;
    pMsgPool = new MsgPool();
    zassert_not_null(pMsgPool);
    delete pMsgPool;
}

// MISSING for 100% branch coverage operator new does not deliver NULL
// here code lines 
// static MsgPool myInstance;
// are not covered completly.

ZTEST(tests_msgpool, msgpool_test_0)
{
    MsgPool* p = MsgPool::inst();
    PoolElem* pMsg[17];

    zassert_equal(p->size(), 16, "size() != 16");

    for (int i = 0; i < 16; i++) {
        pMsg[i] = p->getNewElem();
        printk("size = %d, used = %d, max used = %d, available = %d\n\n", 
               p->size(), p->used(), p->maxused(), p->available());
        zassert_equal(p->used(), i+1, "used() != %d", i+1);
        zassert_equal(p->maxused(), i+1, "maxused() != %d", i+1);
        zassert_equal(p->available(), 16-(i+1), "available() != %d", 16-(i+1));
    }
    
    pMsg[16] = p->getNewElem();
    printk("MsgPool empty, pMsg = NULL\n\n");
    zassert_is_null(pMsg[16]);
    zassert_equal(p->used(), 16, "used() != 16");
    zassert_equal(p->maxused(), 16, "maxused() != 16");
    zassert_equal(p->available(), 0, "available() != 0");

    for (int i = 0; i < 16; i++) {
        pMsg[i]->free();
        printk("size = %d, used = %d, max used = %d, available = %d\n\n", 
                p->size(), p->used(), p->maxused(), p->available());
        zassert_equal(p->used(), 16-(i+1), "used() != %d", 16-(i+1));
        zassert_equal(p->maxused(), 16, "maxused() != 16");
        zassert_equal(p->available(), i+1, "available() != %d", i+1);
    }
}

ZTEST(tests_msgpool, msgpool_test_32)
{
    MsgPool* p = MsgPool::inst();
    PoolElem* pMsg[9];
    uint16_t size = 30;

    zassert_equal(p->bufpool_size(size), 8, "bufpool_size() != 8");

    for (int i = 0; i < 8; i++) {
        pMsg[i] = p->getNewElem(size);
        printk("MsgPool: size = %d, used = %d, max used = %d, available = %d\n", 
            p->size(), p->used(), p->maxused(), p->available());
        printk("BufPool: size = %d, used = %d, max used = %d, available = %d\n\n", 
            p->bufpool_size(size), p->bufpool_used(size), p->bufpool_maxused(size), 
            p->bufpool_available(size));
        zassert_equal(p->bufpool_used(size), i+1, "bufpool_used() != %d", i+1);
        zassert_equal(p->bufpool_maxused(size), i+1, "bufpool_maxused() != %d", i+1);
        zassert_equal(p->bufpool_available(size), 8-(i+1), "bufpool_available() != %d", 8-(i+1));
    }
    
    pMsg[9] = p->getNewElem();
    printk("BufferPool empty, pMsg->buffer = NULL\n\n");
    zassert_is_null(pMsg[9]->getBuffer());
    zassert_equal(p->bufpool_used(size), 8, "bufpool_used() != 8");
    zassert_equal(p->bufpool_maxused(size), 8, "bufpool_maxused() != 8");
    zassert_equal(p->bufpool_available(size), 0, "bufpool_available() != 0");
    pMsg[9]->free();

    for (int i = 0; i < 8; i++) {
        pMsg[i]->free();
        printk("BufPool: size = %d, used = %d, max used = %d, available = %d\n\n", 
            p->bufpool_size(size), p->bufpool_used(size), p->bufpool_maxused(size), 
            p->bufpool_available(size));
        zassert_equal(p->bufpool_used(size), 8-(i+1), "bufpool_used() != %d", 8-(i+1));
        zassert_equal(p->bufpool_maxused(size), 8, "bufpool_maxused() != 8");
        zassert_equal(p->bufpool_available(size), i+1, "bufpool_available() != %d", i+1);
    }
}

ZTEST(tests_msgpool, msgpool_test_128)
{
    MsgPool* p = MsgPool::inst();
    PoolElem* pMsg[5];
    uint16_t size = 120;

    zassert_equal(p->bufpool_size(size), 4, "bufpool_size() != 4");

    for (int i = 0; i < 4; i++) {
        pMsg[i] = p->getNewElem(size);
        printk("MsgPool: size = %d, used = %d, max used = %d, available = %d\n", 
            p->size(), p->used(), p->maxused(), p->available());
        printk("BufPool: size = %d, used = %d, max used = %d, available = %d\n\n", 
            p->bufpool_size(size), p->bufpool_used(size), p->bufpool_maxused(size), 
            p->bufpool_available(size));
        zassert_equal(p->bufpool_used(size), i+1, "bufpool_used() != %d", i+1);
        zassert_equal(p->bufpool_maxused(size), i+1, "bufpool_maxused() != %d", i+1);
        zassert_equal(p->bufpool_available(size), 4-(i+1), "bufpool_available() != %d", 4-(i+1));
    }
    
    pMsg[5] = p->getNewElem();
    printk("BufferPool empty, pMsg->buffer = NULL\n\n");
    zassert_is_null(pMsg[5]->getBuffer());
    zassert_equal(p->bufpool_used(size), 4, "bufpool_used() != 4");
    zassert_equal(p->bufpool_maxused(size), 4, "bufpool_maxused() != 4");
    zassert_equal(p->bufpool_available(size), 0, "bufpool_available() != 0");
    pMsg[5]->free();

    for (int i = 0; i < 4; i++) {
        pMsg[i]->free();
        printk("BufPool: size = %d, used = %d, max used = %d, available = %d\n\n", 
            p->bufpool_size(size), p->bufpool_used(size), p->bufpool_maxused(size), 
            p->bufpool_available(size));
        zassert_equal(p->bufpool_used(size),4-(i+1), "bufpool_used() != %d", 4-(i+1));
        zassert_equal(p->bufpool_maxused(size), 4, "bufpool_maxused() != 4");
        zassert_equal(p->bufpool_available(size), i+1, "bufpool_available() != %d", i+1);
    }
}

ZTEST(tests_msgpool, msgpool_test_256)
{
    MsgPool* p = MsgPool::inst();
    PoolElem* pMsg[5];
    uint16_t size = 250;

    zassert_equal(p->bufpool_size(size), 4, "bufpool_size() != 4");

    for (int i = 0; i < 4; i++) {
        pMsg[i] = p->getNewElem(size);
        printk("MsgPool: size = %d, used = %d, max used = %d, available = %d\n", 
            p->size(), p->used(), p->maxused(), p->available());
            printk("BufPool: size = %d, used = %d, max used = %d, available = %d\n\n", 
                p->bufpool_size(size), p->bufpool_used(size), p->bufpool_maxused(size), 
            p->bufpool_available(size));
        zassert_equal(p->bufpool_used(size), i+1, "bufpool_used() != %d", i+1);
        zassert_equal(p->bufpool_maxused(size), i+1, "bufpool_maxused() != %d", i+1);
        zassert_equal(p->bufpool_available(size), 4-(i+1), "bufpool_available() != %d", 4-(i+1));
    }
    
    pMsg[5] = p->getNewElem();
    printk("BufferPool empty, pMsg->buffer = NULL\n\n");
    zassert_is_null(pMsg[5]->getBuffer());
    zassert_equal(p->bufpool_used(size), 4, "bufpool_used() != 4");
    zassert_equal(p->bufpool_maxused(size), 4, "bufpool_maxused() != 4");
    zassert_equal(p->bufpool_available(size), 0, "bufpool_available() != 0");
    pMsg[5]->free();

    for (int i = 0; i < 4; i++) {
        pMsg[i]->free();
        printk("BufPool: size = %d, used = %d, max used = %d, available = %d\n\n", 
            p->bufpool_size(size), p->bufpool_used(size), p->bufpool_maxused(size), 
            p->bufpool_available(size));
        zassert_equal(p->bufpool_used(size), 4-(i+1), "bufpool_used() != %d", 4-(i+1));
        zassert_equal(p->bufpool_maxused(size), 4, "bufpool_maxused() != 4");
        zassert_equal(p->bufpool_available(size), i+1, "bufpool_available() != %d", i+1);
    }
}

ZTEST(tests_msgpool, msgpool_test_512)
{
    MsgPool* p = MsgPool::inst();
    PoolElem* pMsg[3];
    uint16_t size = 500;

    zassert_equal(p->bufpool_size(size), 2, "bufpool_size() != 2");

    for (int i = 0; i < 2; i++) {
        pMsg[i] = p->getNewElem(size);
        printk("MsgPool: size = %d, used = %d, max used = %d, available = %d\n", 
            p->size(), p->used(), p->maxused(), p->available());
        printk("BufPool: size = %d, used = %d, max used = %d, available = %d\n\n", 
            p->bufpool_size(size), p->bufpool_used(size), p->bufpool_maxused(size), 
            p->bufpool_available(size));
        zassert_equal(p->bufpool_used(size), i+1, "bufpool_used() != %d", i+1);
        zassert_equal(p->bufpool_maxused(size), i+1, "bufpool_maxused() != %d", i+1);
        zassert_equal(p->bufpool_available(size), 2-(i+1), "bufpool_available() != %d", 2-(i+1));
    }
    
    pMsg[3] = p->getNewElem();
    printk("BufferPool empty, pMsg->buffer = NULL\n\n");
    zassert_is_null(pMsg[3]->getBuffer());
    zassert_equal(p->bufpool_used(size), 2, "bufpool_used() != 2");
    zassert_equal(p->bufpool_maxused(size), 2, "bufpool_maxused() != 2");
    zassert_equal(p->bufpool_available(size), 0, "bufpool_available() != 0");
    pMsg[3]->free();

    for (int i = 0; i < 2; i++) {
        pMsg[i]->free();
        printk("BufPool: size = %d, used = %d, max used = %d, available = %d\n\n", 
            p->bufpool_size(size), p->bufpool_used(size), p->bufpool_maxused(size), 
            p->bufpool_available(size));
        zassert_equal(p->bufpool_used(size), 2-(i+1), "bufpool_used() != %d", 2-(i+1));
        zassert_equal(p->bufpool_maxused(size), 2, "bufpool_maxused() != 2");
        zassert_equal(p->bufpool_available(size), i+1, "bufpool_available() != %d", i+1);
    }
}


ZTEST(tests_msgpool, msgpool_test_1024)
{
    MsgPool* p = MsgPool::inst();
    PoolElem* pMsg[5];
    uint16_t size = 1000;

    zassert_equal(p->bufpool_size(size), 4, "bufpool_size() != 4");

    for (int i = 0; i < 4; i++) {
        pMsg[i] = p->getNewElem(size);
        printk("MsgPool: size = %d, used = %d, max used = %d, available = %d\n", 
            p->size(), p->used(), p->maxused(), p->available());
        printk("BufPool: size = %d, used = %d, max used = %d, available = %d\n\n", 
            p->bufpool_size(size), p->bufpool_used(size), p->bufpool_maxused(size), 
            p->bufpool_available(size));
        zassert_equal(p->bufpool_used(size), i+1, "bufpool_used() != %d", i+1);
        zassert_equal(p->bufpool_maxused(size), i+1, "bufpool_maxused() != %d", i+1);
        zassert_equal(p->bufpool_available(size), 4-(i+1), "bufpool_available() != %d", 4-(i+1));
    }
    
    pMsg[5] = p->getNewElem();
    printk("BufferPool empty, pMsg->buffer = NULL\n\n");
    zassert_is_null(pMsg[5]->getBuffer());
    zassert_equal(p->bufpool_used(size), 4, "bufpool_used() != 4");
    zassert_equal(p->bufpool_maxused(size), 4, "bufpool_maxused() != 4");
    zassert_equal(p->bufpool_available(size), 0, "bufpool_available() != 0");
    pMsg[5]->free();

    for (int i = 0; i < 4; i++) {
        pMsg[i]->free();
        printk("BufPool: size = %d, used = %d, max used = %d, available = %d\n\n", 
            p->bufpool_size(size), p->bufpool_used(size), p->bufpool_maxused(size), 
            p->bufpool_available(size));
        zassert_equal(p->bufpool_used(size), 4-(i+1), "bufpool_used() != %d", 4-(i+1));
        zassert_equal(p->bufpool_maxused(size), 4, "bufpool_maxused() != 4");
        zassert_equal(p->bufpool_available(size), i+1, "bufpool_available() != %d", i+1);
    }
}

ZTEST(tests_msgpool, msgpool_test_1024_2)
{
    MsgPool* p = MsgPool::inst();
    PoolElem* pMsg[5];
    uint16_t size = 1024;

    zassert_equal(p->bufpool_size(size), 4, "bufpool_size() != 4");

    for (int i = 0; i < 4; i++) {
        pMsg[i] = p->getNewElem(size);
        printk("MsgPool: size = %d, used = %d, max used = %d, available = %d\n", 
            p->size(), p->used(), p->maxused(), p->available());
        printk("BufPool: size = %d, used = %d, max used = %d, available = %d\n\n", 
            p->bufpool_size(size), p->bufpool_used(size), p->bufpool_maxused(size), 
            p->bufpool_available(size));
        zassert_equal(p->bufpool_used(size), i+1, "bufpool_used() != %d", i+1);
        zassert_equal(p->bufpool_maxused(size), i+1, "bufpool_maxused() != %d", i+1);
        zassert_equal(p->bufpool_available(size), 4-(i+1), "bufpool_available() != %d", 4-(i+1));
    }
    
    pMsg[5] = p->getNewElem();
    printk("BufferPool empty, pMsg->buffer = NULL\n\n");
    zassert_is_null(pMsg[5]->getBuffer());
    zassert_equal(p->bufpool_used(size), 4, "bufpool_used() != 4");
    zassert_equal(p->bufpool_maxused(size), 4, "bufpool_maxused() != 4");
    zassert_equal(p->bufpool_available(size), 0, "bufpool_available() != 0");
    pMsg[5]->free();

    for (int i = 0; i < 4; i++) {
        pMsg[i]->free();
        printk("BufPool: size = %d, used = %d, max used = %d, available = %d\n\n", 
            p->bufpool_size(size), p->bufpool_used(size), p->bufpool_maxused(size), 
            p->bufpool_available(size));
        zassert_equal(p->bufpool_used(size), 4-(i+1), "bufpool_used() != %d", 4-(i+1));
        zassert_equal(p->bufpool_maxused(size), 4, "bufpool_maxused() != 4");
        zassert_equal(p->bufpool_available(size), i+1, "bufpool_available() != %d", i+1);
    }
}

ZTEST(tests_msgpool, msgpool_test_invalid_size)
{
    MsgPool* p = MsgPool::inst();
    PoolElem* pMsg;
    PoolElem* pMsg2;
    uint16_t size = 1070;

    pMsg = p->getNewElem(1020);
    pMsg2 = p->getNewElem(size);
    pMsg->bufLen = size;
    p->freeElem(pMsg);

    p->stats();

    p->bufpool_size(0);
    p->bufpool_size(size);
    p->bufpool_used(0);
    p->bufpool_used(size);
    p->bufpool_maxused(0);
    p->bufpool_maxused(size);
    p->bufpool_available(0);
    p->bufpool_available(size);
}

