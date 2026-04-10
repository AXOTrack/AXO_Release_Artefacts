#include <zephyr/ztest.h>
#include <zephyr/kernel.h>
#include <zephyr/sys/printk.h>
#include <vector>
#include <algorithm>
#include <memory>

#define private public
#include "rtos/mbox.h"
#include "rtos/mpelem.h"
#include "rtos/mpool.h"
#include "rtos/mqueue.h"
#include "rtos/task.h"
#include "rtos/timer.h"

#include "data/rms_store.h"


ZTEST_SUITE(tests_rms_store, NULL, NULL, NULL, NULL, NULL);

// test constructor and destructor
ZTEST(tests_rms_store, test_rms_store_constructor)
{
	RmsStore* pRmsStore = new RmsStore("rmsstore", 5);

	zassert_not_null(pRmsStore, "memory alloc failed");
	zassert_not_null(pRmsStore->pInMbox, "pInMbox = NULL");   
    zassert_equal(pRmsStore->pInMbox->count(), 0, "pInMbox->count() != 0");
    zassert_equal(pRmsStore->pInMbox, Mbox::getMboxById(Mbox::eRmsStore),
                  "Error getting mailbox by id");

    zassert_equal(pRmsStore->capacity, 5, "capacity != 5");
    zassert_equal(pRmsStore->full, false, "full != false");
    zassert_equal(pRmsStore->idle, true, "idle != true");
    zassert_equal(pRmsStore->v.capacity(), 5, "v.capacity() != 5");
    zassert_equal(pRmsStore->v.size(), 0, "v.size() != 0");

	delete pRmsStore; 
}

// MISSING for 100% branch coverage static instantiation does not deliver NULL
// here code line 
// pInMbox = new Mbox(name);
// are not covered completly.

// test function store and reset
ZTEST(tests_rms_store, test_rms_store_store)
{
	RmsStore* pRmsStore = new RmsStore("rmsstore", 5);

	zassert_not_null(pRmsStore, "memory alloc failed");
	zassert_not_null(pRmsStore->pInMbox, "pInMbox = NULL");   
    zassert_equal(pRmsStore->pInMbox->count(), 0, "pInMbox->count() != 0");
    zassert_equal(pRmsStore->pInMbox, Mbox::getMboxById(Mbox::eRmsStore),
                  "Error getting mailbox by id");

    zassert_equal(pRmsStore->v.size(), 0, "v.size() != 0");

    struct adxl_rms_data data = {1234567, 7.1, 7.2, 7.3, 1, 2};
    pRmsStore->store(data);
    zassert_equal(pRmsStore->v.size(), 1, "v.size() != 1");
    zassert_equal(pRmsStore->full, false, "full != false");

    pRmsStore->store(data);
    zassert_equal(pRmsStore->v.size(), 2, "v.size() != 2");
    zassert_equal(pRmsStore->full, false, "full != false");

    pRmsStore->store(data);
    zassert_equal(pRmsStore->v.size(), 3, "v.size() != 3");
    zassert_equal(pRmsStore->full, false, "full != false");

    pRmsStore->store(data);
    zassert_equal(pRmsStore->v.size(), 4, "v.size() != 4");
    zassert_equal(pRmsStore->full, false, "full != false");

    pRmsStore->store(data);
    zassert_equal(pRmsStore->v.size(), 5, "v.size() != 5, %d", pRmsStore->v.size());
    zassert_equal(pRmsStore->full, false, "full != false");

    pRmsStore->store(data);
    zassert_equal(pRmsStore->v.size(), 5, "v.size() != 5, %d", pRmsStore->v.size());
    zassert_equal(pRmsStore->full, true, "full != true");

    pRmsStore->reset();
    zassert_equal(pRmsStore->v.size(), 0, "v.size() != 0");
    zassert_equal(pRmsStore->full, false, "full != false");

	delete pRmsStore; 
}

// test function dumpAll
ZTEST(tests_rms_store, test_rms_store_dumpAll)
{
	RmsStore* pRmsStore = new RmsStore("rmsstore", 5);

	zassert_not_null(pRmsStore, "memory alloc failed");
	zassert_not_null(pRmsStore->pInMbox, "pInMbox = NULL");   
    zassert_equal(pRmsStore->pInMbox->count(), 0, "pInMbox->count() != 0");
    zassert_equal(pRmsStore->pInMbox, Mbox::getMboxById(Mbox::eRmsStore),
                  "Error getting mailbox by id");

    pRmsStore->dumpAll();

    struct adxl_rms_data data = {1234567, 7.1, 7.2, 7.3, 1, 2};
    pRmsStore->store(data);
    zassert_equal(pRmsStore->v.size(), 1, "v.size() != 1");
    zassert_equal(pRmsStore->full, false, "full != false");

    pRmsStore->store(data);
    zassert_equal(pRmsStore->v.size(), 2, "v.size() != 2");
    zassert_equal(pRmsStore->full, false, "full != false");

    pRmsStore->dumpAll();
    delete pRmsStore; 
}

// test function dump
ZTEST(tests_rms_store, test_rms_store_dump)
{
	RmsStore* pRmsStore = new RmsStore("rmsstore", 5);
	zassert_not_null(pRmsStore, "memory alloc failed");
	zassert_not_null(pRmsStore->pInMbox, "pInMbox = NULL");   
    zassert_equal(pRmsStore->pInMbox->count(), 0, "pInMbox->count() != 0");
    zassert_equal(pRmsStore->pInMbox, Mbox::getMboxById(Mbox::eRmsStore),
                  "Error getting mailbox by id");

    pRmsStore->dump();

    struct adxl_rms_data data = {1234567, 7.1, 7.2, 7.3, 1, 2};
    pRmsStore->store(data);
    zassert_equal(pRmsStore->v.size(), 1, "v.size() != 1");
    zassert_equal(pRmsStore->full, false, "full != false");

    pRmsStore->store(data);
    zassert_equal(pRmsStore->v.size(), 2, "v.size() != 2");
    zassert_equal(pRmsStore->full, false, "full != false");

    pRmsStore->dump();
    delete pRmsStore; 
}

//Test receive msg eAdxl37xRmsData with idle = true and not full
ZTEST(tests_rms_store, test_rms_store_rcv_msg_eAdxl37xRmsData)
{
    PoolElem* pElem;

	Mbox* pHttpConnectorMbox = new Mbox("HttpConnectorMbox");
    Mbox::registerMbox(pHttpConnectorMbox, Mbox::eHttpConnector);
	zassert_not_null(pHttpConnectorMbox, "pHttpConnectorMbox = NULL");

	RmsStore* pRmsStore = new RmsStore("rmsstore", 5);
	zassert_not_null(pRmsStore, "memory alloc failed");
	zassert_not_null(pRmsStore->pInMbox, "pInMbox = NULL");   
    zassert_equal(pRmsStore->pInMbox->count(), 0, "pInMbox->count() != 0");
    zassert_equal(pRmsStore->pInMbox, Mbox::getMboxById(Mbox::eRmsStore),
                  "Error getting mailbox by id");

    pRmsStore->idle = true;

    /*struct adxl_rms_data {
        time_t ts;              //!< The timestamp
        float x;                //!< RMS in vertical direction
        float y;                //!< RMS in transversal direction
        float z;                //!< RMS in longitudinal direction
        int16_t medium_hits;    //!< Medium hit counter
        int16_t critical_hits;  //!< Critical hit counter
    };*/
    struct adxl_rms_data val = {1234567, 5.1,6.2,7.3,2,1};

    Mbox::sendMsg(msgType::eAdxl37xRmsData, Mbox::eRmsStore, Mbox::eRmsStore, (uint8_t*)&val, sizeof(val));

    zassert_equal(pRmsStore->pInMbox->count(), 1, "pInMbox->count() != 1");

    pRmsStore->task();  

    zassert_equal(pRmsStore->idle , false, "pRmsStore->idle  != false");
    zassert_equal(pRmsStore->pInMbox->count(), 0, "pInMbox->count() != 0");
    zassert_equal(pHttpConnectorMbox->count(), 1, "pHttpConnectorMbox->count() != 1");

    pElem = pHttpConnectorMbox->read();
    zassert_equal(pElem->mtype, msgType::eAdxl37xRmsData, "Rcvd mtype != msgType::eAdxl37xRmsData");
    zassert_equal(pElem->from, Mbox::eRmsStore, "Rcvd sender != Mbox::eRmsStore");

    pElem->free();
    delete pHttpConnectorMbox;
	delete pRmsStore; 
}

//Test receive msg eAdxl37xRmsData with idle = false and full
ZTEST(tests_rms_store, test_rms_store_rcv_msg_eAdxl37xRmsData_2)
{
	RmsStore* pRmsStore = new RmsStore("rmsstore", 5);
	zassert_not_null(pRmsStore, "memory alloc failed");
	zassert_not_null(pRmsStore->pInMbox, "pInMbox = NULL");   
    zassert_equal(pRmsStore->pInMbox->count(), 0, "pInMbox->count() != 0");
    zassert_equal(pRmsStore->pInMbox, Mbox::getMboxById(Mbox::eRmsStore),
                  "Error getting mailbox by id");

    pRmsStore->idle = false;

    struct adxl_rms_data data = {1234567, 7.1, 7.2, 7.3, 1, 2};
    pRmsStore->store(data);
    zassert_equal(pRmsStore->v.size(), 1, "v.size() != 1");
    zassert_equal(pRmsStore->full, false, "full != false");

    pRmsStore->store(data);
    zassert_equal(pRmsStore->v.size(), 2, "v.size() != 2");
    zassert_equal(pRmsStore->full, false, "full != false");

    pRmsStore->store(data);
    zassert_equal(pRmsStore->v.size(), 3, "v.size() != 3");
    zassert_equal(pRmsStore->full, false, "full != false");

    pRmsStore->store(data);
    zassert_equal(pRmsStore->v.size(), 4, "v.size() != 4");
    zassert_equal(pRmsStore->full, false, "full != false");

    pRmsStore->store(data);
    zassert_equal(pRmsStore->v.size(), 5, "v.size() != 5, %d", pRmsStore->v.size());
    zassert_equal(pRmsStore->full, false, "full != false");

    pRmsStore->store(data);
    zassert_equal(pRmsStore->v.size(), 5, "v.size() != 5, %d", pRmsStore->v.size());
    zassert_equal(pRmsStore->full, true, "full != true");

    struct adxl_rms_data val = {1234567, 5.1,6.2,7.3,2,1};

    Mbox::sendMsg(msgType::eAdxl37xRmsData, Mbox::eRmsStore, Mbox::eRmsStore, (uint8_t*)&val, sizeof(val));

    zassert_equal(pRmsStore->pInMbox->count(), 1, "pInMbox->count() != 1");

    pRmsStore->task();  

    zassert_equal(pRmsStore->pInMbox->count(), 0, "pInMbox->count() != 0");

	delete pRmsStore; 
}

//Test receive msg eAdxl37xIdle 
ZTEST(tests_rms_store, test_rms_store_rcv_msg_eAdxl37xIdle)
{
    PoolElem* pElem;

	Mbox* pHttpConnectorMbox = new Mbox("HttpConnectorMbox");
    Mbox::registerMbox(pHttpConnectorMbox, Mbox::eHttpConnector);
	zassert_not_null(pHttpConnectorMbox, "pHttpConnectorMbox = NULL");

	RmsStore* pRmsStore = new RmsStore("rmsstore", 5);
	zassert_not_null(pRmsStore, "memory alloc failed");
	zassert_not_null(pRmsStore->pInMbox, "pInMbox = NULL");   
    zassert_equal(pRmsStore->pInMbox->count(), 0, "pInMbox->count() != 0");
    zassert_equal(pRmsStore->pInMbox, Mbox::getMboxById(Mbox::eRmsStore),
                  "Error getting mailbox by id");

    pRmsStore->idle = false;

    struct adxl_rms_data val = {1234567, 5.1,6.2,7.3,2,1};

    Mbox::sendMsg(msgType::eAdxl37xIdle, Mbox::eRmsStore, Mbox::eRmsStore);

    zassert_equal(pRmsStore->pInMbox->count(), 1, "pInMbox->count() != 1");

    pRmsStore->task();  

    zassert_equal(pRmsStore->idle , true, "pRmsStore->idle  != true");
    zassert_equal(pRmsStore->pInMbox->count(), 0, "pInMbox->count() != 0");
    zassert_equal(pHttpConnectorMbox->count(), 1, "pHttpConnectorMbox->count() != 1");

    pElem = pHttpConnectorMbox->read();
    zassert_equal(pElem->mtype, msgType::eAdxl37xIdle, "Rcvd mtype != msgType::eAdxl37xIdle");
    zassert_equal(pElem->from, Mbox::eRmsStore, "Rcvd sender != Mbox::eRmsStore");

    pElem->free();
    delete pHttpConnectorMbox;
	delete pRmsStore; 
}

//Test receive msg eRmsReport 
ZTEST(tests_rms_store, test_rms_store_rcv_msg_eRmsReport)
{
    PoolElem* pElem;

	Mbox* pDataMgrMbox = new Mbox("DataMgrMbox");
    Mbox::registerMbox(pDataMgrMbox, Mbox::eDataManager);
	zassert_not_null(pDataMgrMbox, "pDataMgrMbox = NULL");

	RmsStore* pRmsStore = new RmsStore("rmsstore", 5);
	zassert_not_null(pRmsStore, "memory alloc failed");
	zassert_not_null(pRmsStore->pInMbox, "pInMbox = NULL");   
    zassert_equal(pRmsStore->pInMbox->count(), 0, "pInMbox->count() != 0");
    zassert_equal(pRmsStore->pInMbox, Mbox::getMboxById(Mbox::eRmsStore),
                  "Error getting mailbox by id");

    struct adxl_rms_data val = {1234567, 5.1,6.2,7.3,2,1};

    Mbox::sendMsg(msgType::eRmsReport, Mbox::eRmsStore, Mbox::eRmsStore);

    zassert_equal(pRmsStore->pInMbox->count(), 1, "pInMbox->count() != 1");

    pRmsStore->task();  

    zassert_equal(pRmsStore->pInMbox->count(), 0, "pInMbox->count() != 0");
    zassert_equal(pDataMgrMbox->count(), 1, "pDataMgrMbox->count() != 1");

    pElem = pDataMgrMbox->read();
    zassert_equal(pElem->mtype, msgType::eRmsReport, "Rcvd mtype != msgType::eRmsReport");
    zassert_equal(pElem->from, Mbox::eRmsStore, "Rcvd sender != Mbox::eRmsStore");

    pElem->free();
    delete pDataMgrMbox;
	delete pRmsStore; 
}

//Test receive msg eAdxlStorageDumpAll
ZTEST(tests_rms_store, test_rms_store_rcv_msg_eAdxlStorageDumpAll)
{
	RmsStore* pRmsStore = new RmsStore("rmsstore", 5);
	zassert_not_null(pRmsStore, "memory alloc failed");
	zassert_not_null(pRmsStore->pInMbox, "pInMbox = NULL");   
    zassert_equal(pRmsStore->pInMbox->count(), 0, "pInMbox->count() != 0");
    zassert_equal(pRmsStore->pInMbox, Mbox::getMboxById(Mbox::eRmsStore),
                  "Error getting mailbox by id");

    Mbox::sendMsg(msgType::eAdxlStorageDumpAll, Mbox::eRmsStore, Mbox::eRmsStore);

    zassert_equal(pRmsStore->pInMbox->count(), 1, "pInMbox->count() != 1");

    pRmsStore->task();  

    zassert_equal(pRmsStore->pInMbox->count(), 0, "pInMbox->count() != 0");

	delete pRmsStore; 
}

//Test receive unexpected msg 
ZTEST(tests_rms_store, test_rms_store_rcv_msg_unexpected)
{
	RmsStore* pRmsStore = new RmsStore("rmsstore", 5);
	zassert_not_null(pRmsStore, "memory alloc failed");
	zassert_not_null(pRmsStore->pInMbox, "pInMbox = NULL");   
    zassert_equal(pRmsStore->pInMbox->count(), 0, "pInMbox->count() != 0");
    zassert_equal(pRmsStore->pInMbox, Mbox::getMboxById(Mbox::eRmsStore),
                  "Error getting mailbox by id");


    Mbox::sendMsg(msgType::eResetCause, Mbox::eRmsStore, Mbox::eRmsStore);

    zassert_equal(pRmsStore->pInMbox->count(), 1, "pInMbox->count() != 1");

    pRmsStore->task();  

    zassert_equal(pRmsStore->pInMbox->count(), 0, "pInMbox->count() != 0");
	delete pRmsStore; 
}

//Test receive no msg 
ZTEST(tests_rms_store, test_rms_store_rcv_msg_none)
{
	RmsStore* pRmsStore = new RmsStore("rmsstore", 5);
	zassert_not_null(pRmsStore, "memory alloc failed");
	zassert_not_null(pRmsStore->pInMbox, "pInMbox = NULL");   
    zassert_equal(pRmsStore->pInMbox->count(), 0, "pInMbox->count() != 0");
    zassert_equal(pRmsStore->pInMbox, Mbox::getMboxById(Mbox::eRmsStore),
                  "Error getting mailbox by id");

    pRmsStore->pInMbox->write(NULL);
    
    pRmsStore->task();  

    zassert_equal(pRmsStore->pInMbox->count(), 0, "pInMbox->count() != 0");
	delete pRmsStore; 
}

//Test function shell_dump
ZTEST(tests_rms_store, test_rms_store_shell_dump)
{
	RmsStore* pRmsStore = new RmsStore("rmsstore", 5);
	zassert_not_null(pRmsStore, "memory alloc failed");
	zassert_not_null(pRmsStore->pInMbox, "pInMbox = NULL");   
    zassert_equal(pRmsStore->pInMbox->count(), 0, "pInMbox->count() != 0");
    zassert_equal(pRmsStore->pInMbox, Mbox::getMboxById(Mbox::eRmsStore),
                  "Error getting mailbox by id");

    pRmsStore->shell_dump(NULL, 1, NULL);
    
	delete pRmsStore; 
}

