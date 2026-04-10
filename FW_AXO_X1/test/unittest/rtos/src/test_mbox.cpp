#include <stddef.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
//#include <syscalls/rand32.h>
#include <zephyr/kernel.h>
#include <zephyr/ztest.h>

#include "rtos/mbox.h"
#include "rtos/mpelem.h"
#include "rtos/mpool.h"
#include "rtos/mqueue.h"

ZTEST_SUITE(mbox_tests, NULL, NULL, NULL, NULL, NULL);

ZTEST(mbox_tests, test_mbox_constructor) {
    Mbox* pInMbox;
    pInMbox = new Mbox("test");
    zassert_equal(pInMbox->count(), 0, "Error creating mailbox");
    delete pInMbox;
}

ZTEST(mbox_tests, test_mbox_registerMBox) {
    Mbox* pInMbox;
    pInMbox = new Mbox("test");
    zassert_equal(pInMbox->count(), 0, "Error creating mailbox");

    // Generate random mailbox id to test
    Mbox::MboxId id = Mbox::MboxId::eDemoTask; //(Mbox::MboxId)(sys_rand32_get() % Mbox::MboxId::eMboxMax);

    Mbox::registerMbox(pInMbox, id);
    zassert_equal(pInMbox, Mbox::getMboxById(id),
                  "Error getting mailbox by id");

    delete pInMbox;
}

ZTEST(mbox_tests, test_mbox_write) {
    Mbox* pInMbox;
    pInMbox = new Mbox("test");
    zassert_equal(pInMbox->count(), 0, "Error creating mailbox");

    // Get memory pool element
    PoolElem* e1 = MsgPool::inst()->getNewElem(15);
    zassert_not_null(e1, "Failed to get new pool element");

    // Write max slot number of elements to the mailbox
    e1->putString("test_string");
    for (uint32_t i = 1; i <= pInMbox->getMaxSlots(); i++) {
        pInMbox->write(e1);
        zassert_equal(pInMbox->count(), i, "Error writing to the mailbox");
    }

    // Try to add element greater than max slots, it should fail
    pInMbox->write(e1);
    zassert_not_equal(pInMbox->count(), 9, "Error writing to the mailbox");

    // Free the memory pool element
    e1->free();
    delete pInMbox;
}

ZTEST(mbox_tests, test_mbox_send) {
    Mbox* pInMbox;
    pInMbox = new Mbox("test");
    zassert_equal(pInMbox->count(), 0, "Error creating mailbox");
    Mbox::registerMbox(pInMbox, Mbox::MboxId::eDemoTask);

    // Get memory pool element
    PoolElem* e1 = MsgPool::inst()->getNewElem(15);
    zassert_not_null(e1, "Failed to get new pool element");

    // Write max slot number of elements to the mailbox
    e1->putString("test_string");

    // Try to add element greater than max slots, it should fail
    pInMbox->send(e1, Mbox::MboxId::eDemoTask);
    zassert_not_equal(pInMbox->count(), 9, "Error writing to the mailbox");

    pInMbox->stats();

    // Free the memory pool element
    e1->free();
    delete pInMbox;
}

ZTEST(mbox_tests, test_mbox_read) {
    Mbox* pInMbox;
    pInMbox = new Mbox("test");
    zassert_equal(pInMbox->count(), 0, "Error creating mailbox");

    // Read mailbox when no element is there, should return null
    // PoolElem* er = pInMbox->read();
    // zassert_is_null(er, "Failed to get new pool element");

    // Get memory pool element
    PoolElem* e1 = MsgPool::inst()->getNewElem(15);
    zassert_not_null(e1, "Failed to get new pool element");

    // Write mem pool element to the mailbox
    e1->putString("test_string");
    pInMbox->write(e1);

    // Read mailbox when no element is there, should return null
    PoolElem* er = pInMbox->read();
    zassert_not_null(er, "Failed to read mailbox");
    zassert_equal(e1, er, "Error reading the mem pool element");

    // Free the memory pool element
    er->free();
    delete pInMbox;
}

// test function sendMsg(enum msgType mType, MboxId senderId, MboxId receiverId);
ZTEST(mbox_tests, test_mbox_sendMsg_0) {
    Mbox* pInMbox;
    pInMbox = new Mbox("test");
    Mbox::registerMbox(pInMbox, Mbox::eRmsStore);
    zassert_not_null(pInMbox, "pInMbox = NULL");
    zassert_equal(pInMbox->count(), 0, "Error creating mailbox");

    Mbox::sendMsg(msgType::eAdxlStorageDumpAll, Mbox::MboxId::eMain, Mbox::MboxId::eRmsStore);

    // Read mailbox when no element is there, should return null
    PoolElem* e1 = pInMbox->read();
    zassert_not_null(e1, "Failed to read mailbox");
    zassert_equal(e1->mtype, msgType::eAdxlStorageDumpAll, "Wrong msgType");
    zassert_equal(e1->from, Mbox::MboxId::eMain, "Wrong msg sender");

    // Free the memory pool element
    e1->free();
    delete pInMbox;
}

// test function static void sendMsg(enum msgType mType, MboxId senderId, MboxId receiverId, uint8_t u8DataByte);
ZTEST(mbox_tests, test_mbox_sendMsg_u8) {
    Mbox* pInMbox;
    pInMbox = new Mbox("test");
    Mbox::registerMbox(pInMbox, Mbox::eRmsStore);
    zassert_not_null(pInMbox, "pInMbox = NULL");
    zassert_equal(pInMbox->count(), 0, "Error creating mailbox");

    Mbox::sendMsg(msgType::eAdxlStorageDumpAll, Mbox::MboxId::eMain, Mbox::MboxId::eRmsStore, (uint8_t)17);

    // Read mailbox when no element is there, should return null
    PoolElem* e1 = pInMbox->read();
    zassert_not_null(e1, "Failed to read mailbox");
    zassert_equal(e1->mtype, msgType::eAdxlStorageDumpAll, "Wrong msgType");
    zassert_equal(e1->from, Mbox::MboxId::eMain, "Wrong msg sender");
    zassert_equal(e1->p1, 17, "Wrong u8 content");

    // Free the memory pool element
    e1->free();
    delete pInMbox;
}

// test function  sendMsg(enum msgType mType, MboxId senderId, MboxId receiverId, int8_t s8DataByte);
ZTEST(mbox_tests, test_mbox_sendMsg_s8) {
    Mbox* pInMbox;
    pInMbox = new Mbox("test");
    Mbox::registerMbox(pInMbox, Mbox::eRmsStore);
    zassert_not_null(pInMbox, "pInMbox = NULL");
    zassert_equal(pInMbox->count(), 0, "Error creating mailbox");

    Mbox::sendMsg(msgType::eAdxlStorageDumpAll, Mbox::MboxId::eMain, Mbox::MboxId::eRmsStore, (int8_t)-17);

    // Read mailbox when no element is there, should return null
    PoolElem* e1 = pInMbox->read();
    zassert_not_null(e1, "Failed to read mailbox");
    zassert_equal(e1->mtype, msgType::eAdxlStorageDumpAll, "Wrong msgType");
    zassert_equal(e1->from, Mbox::MboxId::eMain, "Wrong msg sender");
    zassert_equal(e1->p2, -17, "Wrong s8 content");

    // Free the memory pool element
    e1->free();
    delete pInMbox;
}

// test function  sendMsg(enum msgType mType, MboxId senderId, MboxId receiverId,uint8_t* pData, uint16_t dataLen);
ZTEST(mbox_tests, test_mbox_sendMsg_buf) {
    Mbox* pInMbox;
    uint8_t buf[4] = {17,18,19,20};
    uint16_t datalen = 4;
    uint8_t* pBuf;

    pInMbox = new Mbox("test");
    Mbox::registerMbox(pInMbox, Mbox::eRmsStore);
    zassert_not_null(pInMbox, "pInMbox = NULL");
    zassert_equal(pInMbox->count(), 0, "Error creating mailbox");

    Mbox::sendMsg(msgType::eAdxlStorageDumpAll, Mbox::MboxId::eMain, Mbox::MboxId::eRmsStore, buf, datalen);

    // Read mailbox when no element is there, should return null
    PoolElem* e1 = pInMbox->read();
    pBuf = e1->getBuffer();
    zassert_not_null(e1, "Failed to read mailbox");
    zassert_equal(e1->mtype, msgType::eAdxlStorageDumpAll, "Wrong msgType");
    zassert_equal(e1->from, Mbox::MboxId::eMain, "Wrong msg sender");
    printk("bufsize = %d\n", e1->getBufferSize());
    //zassert_equal(e1->getBufferSize(), 32, "Wrong buffer size");
    zassert_equal(pBuf[0], 17, "Wrong pBuf[0] content");
    zassert_equal(pBuf[1], 18, "Wrong pBuf[1] content");
    zassert_equal(pBuf[2], 19, "Wrong pBuf[2] content");
    zassert_equal(pBuf[3], 20, "Wrong pBuf[3] content");

    // Free the memory pool element
    e1->free();
    delete pInMbox;
}

// test function  sendMsg(enum msgType mType, MboxId senderId, MboxId receiverId,const std::string& str);
ZTEST(mbox_tests, test_mbox_sendMsg_string) {
    Mbox* pInMbox;
    std::string s = "testTestTest";
    std::string s2;
    unsigned char* pBuf;

    pInMbox = new Mbox("test");
    Mbox::registerMbox(pInMbox, Mbox::eRmsStore);
    zassert_not_null(pInMbox, "pInMbox = NULL");
    zassert_equal(pInMbox->count(), 0, "Error creating mailbox");

    Mbox::sendMsg(msgType::eAdxlStorageDumpAll, Mbox::MboxId::eMain, Mbox::MboxId::eRmsStore, s);

    // Read mailbox when no element is there, should return null
    PoolElem* e1 = pInMbox->read();
    pBuf = e1->getBuffer();
    zassert_not_null(e1, "Failed to read mailbox");
    zassert_equal(e1->mtype, msgType::eAdxlStorageDumpAll, "Wrong msgType");
    zassert_equal(e1->from, Mbox::MboxId::eMain, "Wrong msg sender");
    printk("bufsize = %d\n", e1->getBufferSize());
    //zassert_equal(e1->getBufferSize(), 13, "Wrong buffer size");
    zassert(strcmp(s.c_str(), "testTestTest")==0, "wrong string content");

    // Free the memory pool element
    e1->free();
    delete pInMbox;
}

