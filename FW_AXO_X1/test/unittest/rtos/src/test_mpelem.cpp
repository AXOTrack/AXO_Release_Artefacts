#include <zephyr/ztest.h>
//#include <vector>
//#include <algorithm>

#define private public
#include "rtos/mempool.h"
#include "rtos/mpool.h"
#include "rtos/mpelem.h"


ZTEST_SUITE(mpelem_tests, NULL, NULL, NULL, NULL, NULL);

ZTEST(mpelem_tests, test_mpelem_constructor) {
    MsgPool* p = MsgPool::inst();
    PoolElem* pMsg;
    uint16_t size = 20;

    pMsg = p->getNewElem(size);
    zassert_not_null(pMsg, "pMsg == NULL");

    pMsg->free();
}

ZTEST(mpelem_tests, test_mpelem_putString) {
    MsgPool* p = MsgPool::inst();
    PoolElem* pMsg;
    uint16_t size = 20;
    uint16_t bufsize;
    char* pStr;

    pMsg = p->getNewElem(size);
    zassert_not_null(pMsg, "pMsg == NULL");
    zassert_false(pMsg->inPool, "inpool == true");
    pMsg->putString("123-Test");
    zassert_equal(pMsg->bufLen, 32, "bufLen != 32");
    bufsize = pMsg->getBufferSize();
    zassert_equal(bufsize, 32, "bufsize != 32");
    zassert_equal(pMsg->dataLen, 9, "dataLen != 9");
    pStr = (char*)pMsg->getBuffer();
    zassert(strcmp(pStr, "123-Test")==0, "wrong string");

    pMsg->DumpAsString();
    pMsg->DumpAsStringHex(pMsg->bufLen);
    pMsg->DumpAsStringHex();

    pMsg->free();
    zassert_true(pMsg->inPool, "inpool == false");
}

ZTEST(mpelem_tests, test_mpelem_putString2) {
    MsgPool* p = MsgPool::inst();
    PoolElem* pMsg;
    uint16_t size = 20;
    uint16_t bufsize;
    char* pStr;

    pMsg = p->getNewElem(size);
    zassert_not_null(pMsg, "pMsg == NULL");
    zassert_false(pMsg->inPool, "inpool == true");
    pMsg->putString("");
    zassert_equal(pMsg->bufLen, 32, "bufLen != 32");
    bufsize = pMsg->getBufferSize();
    zassert_equal(bufsize, 32, "bufsize != 32");
    zassert_equal(pMsg->dataLen, 0, "dataLen != 0");
    pStr = (char*)pMsg->getBuffer();
    zassert(strcmp(pStr, "")==0, "wrong string");

    pMsg->DumpAsString();
    pMsg->DumpAsStringHex(pMsg->bufLen);
    pMsg->DumpAsStringHex();

    pMsg->free();
    zassert_true(pMsg->inPool, "inpool == false");
}

ZTEST(mpelem_tests, test_mpelem_putData) {
    MsgPool* p = MsgPool::inst();
    PoolElem* pMsg;
    uint16_t size = 4;
    uint16_t bufsize;
    uint8_t* pData;
    char data[4] = {1,2,3,4};

    pMsg = p->getNewElem(size);
    zassert_not_null(pMsg, "pMsg == NULL");
    zassert_false(pMsg->inPool, "inpool == true");
    pMsg->putData(data, 4);
    zassert_equal(pMsg->bufLen, 32, "bufLen != 32");
    bufsize = pMsg->getBufferSize();
    zassert_equal(bufsize, 32, "bufsize != 32");
    zassert_equal(pMsg->dataLen, 4, "dataLen != 4");
    pData = pMsg->getBuffer();
    zassert_equal(pData[0], 1, "pData[0] != 1");
    zassert_equal(pData[1], 2, "pData[0] != 2");
    zassert_equal(pData[2], 3, "pData[0] != 3");
    zassert_equal(pData[3], 4, "pData[0] != 4");

    pMsg->Dump();

    pMsg->clearBuffer();
    zassert_equal(pMsg->dataLen, 0, "dataLen != 0");

    pMsg->free();
    zassert_true(pMsg->inPool, "inpool == false");
}