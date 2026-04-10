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

#include "conf/config.h"
#include "bus/spi/spibus.h"
#include "drv/spi/fram.h"

#include "data/adxl_store.h"

#include "stubs.h"



ZTEST_SUITE(tests_adxl_store, NULL, NULL, NULL, NULL, NULL);

// test constructor and destructor
ZTEST(tests_adxl_store, test_adxl_store_constructor)
{
	AdxlDataStore* pAdxlStore = new AdxlDataStore("adxlstore", 5, 4400, true);

	zassert_not_null(pAdxlStore, "memory alloc failed");
	zassert_not_null(pAdxlStore->pInMbox, "pInMbox = NULL");   
    zassert_equal(pAdxlStore->pInMbox->count(), 0, "pInMbox->count() != 0");
    zassert_equal(pAdxlStore->pInMbox, Mbox::getMboxById(Mbox::eAdxlStore),
                  "Error getting mailbox by id");

    zassert_equal(pAdxlStore->samples_per_sec, 4400, "samples_per_sec != 4400");
    zassert_equal(pAdxlStore->rd, 0, "rd != 0");
    zassert_equal(pAdxlStore->wr, 0, "wr != 0");
    zassert_equal(pAdxlStore->level, 0, "level != 0");
    zassert_equal(pAdxlStore->full, false, "full != false");
    zassert_equal(pAdxlStore->idle, true, "idle != true");
    zassert_equal(pAdxlStore->allow_wrap_around, false, "allow_wrap_around != false");
    zassert_equal(pAdxlStore->pm_state, PowerMgr::PmState::eOff, "pm_state != PowerMgr::PmState::eOff");
    zassert_equal(pAdxlStore->have_http_connector, true, "have_http_connector != true");
    zassert_equal(pAdxlStore->x_max, 0.0, "x_max != 0.0");
    zassert_equal(pAdxlStore->y_max, 0.0, "x_max != 0.0");
    zassert_equal(pAdxlStore->z_max, 0.0, "x_max != 0.0");
    zassert_equal(pAdxlStore->capa, 132000, "capa != 132000");
    zassert_equal(pAdxlStore->bytes_per_sec, 26400, "bytes_per_sec != 26400");

	delete pAdxlStore; 
}

// MISSING for 100% branch coverage static instantiation does not deliver NULL
// here code line 
// pInMbox = new Mbox(name); 
// are not covered completly.


// Test function store with full = true
ZTEST(tests_adxl_store, test_adxl_store_store)
{
	AdxlDataStore* pAdxlStore = new AdxlDataStore("adxlstore", 5, 4400, true);

	zassert_not_null(pAdxlStore, "memory alloc failed");
	zassert_not_null(pAdxlStore->pInMbox, "pInMbox = NULL");   
    zassert_equal(pAdxlStore->pInMbox->count(), 0, "pInMbox->count() != 0");
    zassert_equal(pAdxlStore->pInMbox, Mbox::getMboxById(Mbox::eAdxlStore),
                  "Error getting mailbox by id");

    pAdxlStore->full = true;

    //struct adxl372_xyz_accel_data {
    //    int16_t x;  
    //    int16_t y;  
    //    int16_t z;  };
    struct adxl372_xyz_accel_data data[2] = { {1,2,3},{1,2,3} };
    pAdxlStore->store(&data[0], 2);

	delete pAdxlStore; 
}

// Test function store with full = false and wr + len >= capa and allow_wrap_around = false
ZTEST(tests_adxl_store, test_adxl_store_store2)
{
    PoolElem* pElem;

	Mbox* pAdxl37xMbox = new Mbox("Adxl37xMbox");
    Mbox::registerMbox(pAdxl37xMbox, Mbox::eAdxl37xTask);
	zassert_not_null(pAdxl37xMbox, "pAdxl37xMbox = NULL");

	AdxlDataStore* pAdxlStore = new AdxlDataStore("adxlstore", 5, 4400, true);

	zassert_not_null(pAdxlStore, "memory alloc failed");
	zassert_not_null(pAdxlStore->pInMbox, "pInMbox = NULL");   
    zassert_equal(pAdxlStore->pInMbox->count(), 0, "pInMbox->count() != 0");
    zassert_equal(pAdxlStore->pInMbox, Mbox::getMboxById(Mbox::eAdxlStore),
                  "Error getting mailbox by id");

    pAdxlStore->full = false;
    pAdxlStore->capa = 30;
    pAdxlStore->wr = 24;
    pAdxlStore->rd = 12;
    pAdxlStore->allow_wrap_around = false;
    
    //struct adxl372_xyz_accel_data {
    //    int16_t x;  
    //    int16_t y;  
    //    int16_t z;  };
    struct adxl372_xyz_accel_data data[2] = { {1,2,3}, {1,2,3}};

    zassert_equal(pAdxl37xMbox->count(), 0, "pAdxl37xMbox->count() != 0");

    pAdxlStore->store(&data[0], 2);

    zassert_equal(pAdxl37xMbox->count(), 1, "pAdxl37xMbox->count() != 1");
    zassert_equal(pAdxlStore->wr, 0, "wr != 0");
    zassert_equal(pAdxlStore->level, 18, "level != 18");

    pElem = pAdxl37xMbox->read();
    zassert_equal(pElem->mtype, msgType::eAdxlStorageFull, "Rcvd mtype != msgType::eAdxlStorageFull");
    zassert_equal(pElem->from, Mbox::eAdxlStore, "Rcvd sender != Mbox::eAdxlStore");

    pElem->free();
    delete pAdxl37xMbox;
	delete pAdxlStore; 
}

// Test function store with full = false and wr + len >= capa and allow_wrap_around = true
ZTEST(tests_adxl_store, test_adxl_store_store3)
{
    PoolElem* pElem;

	Mbox* pAdxl37xMbox = new Mbox("Adxl37xMbox");
    Mbox::registerMbox(pAdxl37xMbox, Mbox::eAdxl37xTask);
	zassert_not_null(pAdxl37xMbox, "pAdxl37xMbox = NULL");

	AdxlDataStore* pAdxlStore = new AdxlDataStore("adxlstore", 5, 4400, true);

	zassert_not_null(pAdxlStore, "memory alloc failed");
	zassert_not_null(pAdxlStore->pInMbox, "pInMbox = NULL");   
    zassert_equal(pAdxlStore->pInMbox->count(), 0, "pInMbox->count() != 0");
    zassert_equal(pAdxlStore->pInMbox, Mbox::getMboxById(Mbox::eAdxlStore),
                  "Error getting mailbox by id");

    pAdxlStore->full = false;
    pAdxlStore->capa = 30;
    pAdxlStore->wr = 24;
    pAdxlStore->rd = 24;
    pAdxlStore->bytes_per_sec = 6;
    pAdxlStore->allow_wrap_around = true;
    
    //struct adxl372_xyz_accel_data {
    //    int16_t x;  
    //    int16_t y;  
    //    int16_t z;  };
    struct adxl372_xyz_accel_data data[2] = { {1,2,3}, {1,2,3}};

    zassert_equal(pAdxl37xMbox->count(), 0, "pAdxl37xMbox->count() != 0");

    pAdxlStore->store(&data[0], 2);

    zassert_equal(pAdxl37xMbox->count(), 1, "pAdxl37xMbox->count() != 1");
    //printk("pAdxlStore->rd = %d\n", pAdxlStore->rd);
    //printk("pAdxlStore->wr = %d\n", pAdxlStore->wr);
    //printk("pAdxlStore->level = %d\n", pAdxlStore->level);
    zassert_equal(pAdxlStore->wr, 6, "wr != 6");
    zassert_equal(pAdxlStore->level, 12, "level != 12");
    zassert_equal(pAdxlStore->rd, 0, "rd != 0");

    pElem = pAdxl37xMbox->read();
    zassert_equal(pElem->mtype, msgType::eAdxlStorageFull, "Rcvd mtype != msgType::eAdxlStorageFull");
    zassert_equal(pElem->from, Mbox::eAdxlStore, "Rcvd sender != Mbox::eAdxlStore");

    pElem->free();
    delete pAdxl37xMbox;
	delete pAdxlStore; 
}

// Test function store with full = false and wr + len >= capa and allow_wrap_around = true
ZTEST(tests_adxl_store, test_adxl_store_store4)
{
    PoolElem* pElem;

	Mbox* pAdxl37xMbox = new Mbox("Adxl37xMbox");
    Mbox::registerMbox(pAdxl37xMbox, Mbox::eAdxl37xTask);
	zassert_not_null(pAdxl37xMbox, "pAdxl37xMbox = NULL");

	AdxlDataStore* pAdxlStore = new AdxlDataStore("adxlstore", 5, 4400, true);

	zassert_not_null(pAdxlStore, "memory alloc failed");
	zassert_not_null(pAdxlStore->pInMbox, "pInMbox = NULL");   
    zassert_equal(pAdxlStore->pInMbox->count(), 0, "pInMbox->count() != 0");
    zassert_equal(pAdxlStore->pInMbox, Mbox::getMboxById(Mbox::eAdxlStore),
                  "Error getting mailbox by id");

    pAdxlStore->full = false;
    pAdxlStore->capa = 30;
    pAdxlStore->wr = 24;
    pAdxlStore->rd = 12;
    pAdxlStore->bytes_per_sec = 6;
    pAdxlStore->allow_wrap_around = true;
    
    //struct adxl372_xyz_accel_data {
    //    int16_t x;  
    //    int16_t y;  
    //    int16_t z;  };
    struct adxl372_xyz_accel_data data[2] = { {1,2,3}, {1,2,3}};

    zassert_equal(pAdxl37xMbox->count(), 0, "pAdxl37xMbox->count() != 0");

    pAdxlStore->store(&data[0], 2);

    //printk("pAdxl37xMbox->count() = %d\n", pAdxl37xMbox->count());
    //printk("pAdxlStore->rd = %d\n", pAdxlStore->rd);
    //printk("pAdxlStore->wr = %d\n", pAdxlStore->wr);
    //printk("pAdxlStore->level = %d\n", pAdxlStore->level);
    zassert_equal(pAdxl37xMbox->count(), 1, "pAdxl37xMbox->count() != 1");
    zassert_equal(pAdxlStore->wr, 6, "wr != 6");
    zassert_equal(pAdxlStore->level, 24, "wr != 24");
    zassert_equal(pAdxlStore->rd, 18, "rd != 18");

    pElem = pAdxl37xMbox->read();
    zassert_equal(pElem->mtype, msgType::eAdxlStorageFull, "Rcvd mtype != msgType::eAdxlStorageFull");
    zassert_equal(pElem->from, Mbox::eAdxlStore, "Rcvd sender != Mbox::eAdxlStore");

    pElem->free();
    delete pAdxl37xMbox;
	delete pAdxlStore; 
}

// Test function store with len = 0
ZTEST(tests_adxl_store, test_adxl_store_store5)
{
	AdxlDataStore* pAdxlStore = new AdxlDataStore("adxlstore", 5, 4400, true);

	zassert_not_null(pAdxlStore, "memory alloc failed");
	zassert_not_null(pAdxlStore->pInMbox, "pInMbox = NULL");   
    zassert_equal(pAdxlStore->pInMbox->count(), 0, "pInMbox->count() != 0");
    zassert_equal(pAdxlStore->pInMbox, Mbox::getMboxById(Mbox::eAdxlStore),
                  "Error getting mailbox by id");

    pAdxlStore->full = false;
    pAdxlStore->capa = 30;
    pAdxlStore->wr = 24;
    pAdxlStore->rd = 12;
    pAdxlStore->bytes_per_sec = 6;
    pAdxlStore->allow_wrap_around = true;
    
    //struct adxl372_xyz_accel_data {
    //    int16_t x;  
    //    int16_t y;  
    //    int16_t z;  };
    struct adxl372_xyz_accel_data data[2] = { {1,2,3}, {1,2,3}};

    pAdxlStore->store(&data[0], 0);

	delete pAdxlStore; 
}

//Test receive msg eAdxl37xSensorData with idle = true and pm_state = eOff
ZTEST(tests_adxl_store, test_adxl_store_rcv_msg_eAdxl37xSensorData)
{
    PoolElem* pElem;
    PoolElem* pElem2;

	Mbox* pRmsMbox = new Mbox("RmsMbox");
    Mbox::registerMbox(pRmsMbox, Mbox::eAdxlRms);
	zassert_not_null(pRmsMbox, "pRmsMbox = NULL");

	Mbox* pAdxl37xMbox = new Mbox("Adxl37xMbox");
    Mbox::registerMbox(pAdxl37xMbox, Mbox::eAdxl37xTask);
	zassert_not_null(pAdxl37xMbox, "pAdxl37xMbox = NULL");

	AdxlDataStore* pAdxlStore = new AdxlDataStore("adxlstore", 5, 4400, true);
	zassert_not_null(pAdxlStore, "memory alloc failed");
	zassert_not_null(pAdxlStore->pInMbox, "pInMbox = NULL");   
    zassert_equal(pAdxlStore->pInMbox->count(), 0, "pInMbox->count() != 0");
    zassert_equal(pAdxlStore->pInMbox, Mbox::getMboxById(Mbox::eAdxlStore),
                  "Error getting mailbox by id");

    pAdxlStore->idle = true;

    //  struct adxl372_xyz_accel_data {
    //  int16_t x;  //!< measured x value of the acceleration data
    //  int16_t y;  //!< measured y value of the acceleration data
    //  int16_t z;  //!< measured z value of the acceleration data}; 
    struct adxl372_xyz_accel_data xyz[3] = {{ 7, 8, 9 },{ 7, 8, 9 },{ 7, 8, 9 }};
    // struct adxl372_xyz_accel_data* samples;   //!< x, y, z axis measured acceleration data
    //   uint16_t cnt;  //!< The sample counter
    //   time_t ts;     //!< The timestamp of the measurement};
    struct adxl37x_sensor_value val = { &xyz[0], 3, 12345678 };

    pAdxlStore->rd = 4;
    pAdxlStore->wr = 3;
    pAdxlStore->full = true;
    pAdxlStore->x_max = 20.0;
    pAdxlStore->y_max = 20.0;
    pAdxlStore->z_max = 20.0;    
    pAdxlStore->pm_state = PowerMgr::PmState::eOff;
    zassert_equal(pAdxlStore->rd, 4, "rd != 4");
    zassert_equal(pAdxlStore->wr, 3, "wr != 3");
    zassert_equal(pAdxlStore->full, true, "full != true");
    zassert_equal(pAdxlStore->x_max, 20.0, "x_max != 20.0");
    zassert_equal(pAdxlStore->y_max, 20.0, "x_max != 20.0");
    zassert_equal(pAdxlStore->z_max, 20.0, "x_max != 20.0");
    zassert_equal(pAdxlStore->pm_state, PowerMgr::PmState::eOff, "pm_state != PowerMgr::PmState::eOff");

    Mbox::sendMsg(msgType::eAdxl37xSensorData, Mbox::eAdxlStore, Mbox::eAdxlStore, (uint8_t*)&val, sizeof(val));

    zassert_equal(pAdxlStore->pInMbox->count(), 1, "pInMbox->count() != 1");
    //printk("pAdxlStore->pInMbox->count() = %d\n\n", pAdxlStore->pInMbox->count());

    pAdxlStore->task();  

    zassert_equal(pAdxlStore->pInMbox->count(), 0, "pInMbox->count() != 0");
    zassert_equal(pRmsMbox->count(), 1, "pRmsMbox->count() != 1");
    zassert_equal(pAdxl37xMbox->count(), 1, "pAdxl37xMbox->count() != 1");

    zassert_equal(pAdxlStore->idle, false, "pAdxlStore->idle != false");
    zassert_equal(pAdxlStore->rd, 0, "rd != 0");
    zassert_equal(pAdxlStore->wr, 18, "wr != 18");
    zassert_equal(pAdxlStore->full, false, "full != false");
    //printk("pAdxlStore->rd = %d\n", pAdxlStore->rd);
    //printk("pAdxlStore->wr = %d\n", pAdxlStore->wr);
    //printk("pAdxlStore->x_max = %f\n", pAdxlStore->x_max);
    //printk("pAdxlStore->y_max = %f\n", pAdxlStore->y_max);
    //printk("pAdxlStore->z_max = %f\n", pAdxlStore->z_max);
    zassert_equal(pAdxlStore->pm_state, PowerMgr::PmState::eOn, "pm_state != PowerMgr::PmState::eOn");
    zassert_true(pAdxlStore->x_max > 0.4, "x_max  > 0.4");
    zassert_true(pAdxlStore->y_max > 0.4, "y_max  > 0.4");
    zassert_true(pAdxlStore->z_max > 0.4, "z_max  > 0.4");

    pElem = pRmsMbox->read();
    zassert_equal(pElem->mtype, msgType::eAdxl37xSensorData, "Rcvd mtype != msgType::eAdxl37xSensorData");
    zassert_equal(pElem->from, Mbox::eAdxlStore, "Rcvd sender != Mbox::eAdxlStore");

    pElem2 = pAdxl37xMbox->read();
    zassert_equal(pElem2->mtype, msgType::eAdxlStorageDone, "Rcvd mtype != msgType::eAdxlStorageDone");
    zassert_equal(pElem2->from, Mbox::eAdxlStore, "Rcvd sender != Mbox::eAdxlStore");

    pElem->free();
    pElem2->free();
    delete pRmsMbox;
    delete pAdxl37xMbox;
	delete pAdxlStore; 
}

//Test receive msg eAdxl37xSensorData with idle = false and pm_state = eOn
ZTEST(tests_adxl_store, test_adxl_store_rcv_msg_eAdxl37xSensorData2)
{
    PoolElem* pElem;
    PoolElem* pElem2;

	Mbox* pRmsMbox = new Mbox("RmsMbox");
    Mbox::registerMbox(pRmsMbox, Mbox::eAdxlRms);
	zassert_not_null(pRmsMbox, "pRmsMbox = NULL");

	Mbox* pAdxl37xMbox = new Mbox("Adxl37xMbox");
    Mbox::registerMbox(pAdxl37xMbox, Mbox::eAdxl37xTask);
	zassert_not_null(pAdxl37xMbox, "pAdxl37xMbox = NULL");

	AdxlDataStore* pAdxlStore = new AdxlDataStore("adxlstore", 5, 4400, true);
	zassert_not_null(pAdxlStore, "memory alloc failed");
	zassert_not_null(pAdxlStore->pInMbox, "pInMbox = NULL");   
    zassert_equal(pAdxlStore->pInMbox->count(), 0, "pInMbox->count() != 0");
    zassert_equal(pAdxlStore->pInMbox, Mbox::getMboxById(Mbox::eAdxlStore),
                  "Error getting mailbox by id");

    pAdxlStore->idle = false;

    //  struct adxl372_xyz_accel_data {
    //  int16_t x;  //!< measured x value of the acceleration data
    //  int16_t y;  //!< measured y value of the acceleration data
    //  int16_t z;  //!< measured z value of the acceleration data}; 
    struct adxl372_xyz_accel_data xyz[3] = {{ 7, 8, 9 },{ 7, 8, 9 },{ 7, 8, 9 }};
    // struct adxl372_xyz_accel_data* samples;   //!< x, y, z axis measured acceleration data
    //   uint16_t cnt;  //!< The sample counter
    //   time_t ts;     //!< The timestamp of the measurement};
    struct adxl37x_sensor_value val = { &xyz[0], 3, 12345678 };

    pAdxlStore->rd = 4;
    pAdxlStore->wr = 3;
    pAdxlStore->full = true;
    pAdxlStore->x_max = 20.0;
    pAdxlStore->y_max = 20.0;
    pAdxlStore->z_max = 20.0;    
    pAdxlStore->pm_state = PowerMgr::PmState::eOn;
    zassert_equal(pAdxlStore->rd, 4, "rd != 4");
    zassert_equal(pAdxlStore->wr, 3, "wr != 3");
    zassert_equal(pAdxlStore->full, true, "full != true");
    zassert_equal(pAdxlStore->x_max, 20.0, "x_max != 20.0");
    zassert_equal(pAdxlStore->y_max, 20.0, "x_max != 20.0");
    zassert_equal(pAdxlStore->z_max, 20.0, "x_max != 20.0");
    zassert_equal(pAdxlStore->pm_state, PowerMgr::PmState::eOn, "pm_state != PowerMgr::PmState::eOn");

    Mbox::sendMsg(msgType::eAdxl37xSensorData, Mbox::eAdxlStore, Mbox::eAdxlStore, (uint8_t*)&val, sizeof(val));

    zassert_equal(pAdxlStore->pInMbox->count(), 1, "pInMbox->count() != 1");
    //printk("pAdxlStore->pInMbox->count() = %d\n\n", pAdxlStore->pInMbox->count());

    pAdxlStore->task();  

    zassert_equal(pAdxlStore->pInMbox->count(), 0, "pInMbox->count() != 0");
    zassert_equal(pRmsMbox->count(), 1, "pRmsMbox->count() != 1");
    zassert_equal(pAdxl37xMbox->count(), 1, "pAdxl37xMbox->count() != 1");

    zassert_equal(pAdxlStore->idle, false, "pAdxlStore->idle != false");
    zassert_equal(pAdxlStore->rd, 4, "rd != 4");
    zassert_equal(pAdxlStore->wr, 3, "wr != 3");
    zassert_equal(pAdxlStore->full, true, "full != true");
    //printk("pAdxlStore->rd = %d\n", pAdxlStore->rd);
    //printk("pAdxlStore->wr = %d\n", pAdxlStore->wr);
    //printk("pAdxlStore->x_max = %f\n", pAdxlStore->x_max);
    //printk("pAdxlStore->y_max = %f\n", pAdxlStore->y_max);
    //printk("pAdxlStore->z_max = %f\n", pAdxlStore->z_max);
    zassert_equal(pAdxlStore->pm_state, PowerMgr::PmState::eOn, "pm_state != PowerMgr::PmState::eOn");
    zassert_true(pAdxlStore->x_max > 0.4, "x_max  > 0.4");
    zassert_true(pAdxlStore->y_max > 0.4, "y_max  > 0.4");
    zassert_true(pAdxlStore->z_max > 0.4, "z_max  > 0.4");

    pElem = pRmsMbox->read();
    zassert_equal(pElem->mtype, msgType::eAdxl37xSensorData, "Rcvd mtype != msgType::eAdxl37xSensorData");
    zassert_equal(pElem->from, Mbox::eAdxlStore, "Rcvd sender != Mbox::eAdxlStore");

    pElem2 = pAdxl37xMbox->read();
    zassert_equal(pElem2->mtype, msgType::eAdxlStorageDone, "Rcvd mtype != msgType::eAdxlStorageDone");
    zassert_equal(pElem2->from, Mbox::eAdxlStore, "Rcvd sender != Mbox::eAdxlStore");

    pElem->free();
    pElem2->free();
    delete pRmsMbox;
    delete pAdxl37xMbox;
	delete pAdxlStore; 
}

//Test receive msg eAdxl37xIdle with idle = false
ZTEST(tests_adxl_store, test_adxl_store_rcv_msg_eAdxl37xIdle)
{
    PoolElem* pElem;
    PoolElem* pElem2;
    PoolElem* pElem3;

	Mbox* pDataMgrMbox = new Mbox("DataMgrMbox");
    Mbox::registerMbox(pDataMgrMbox, Mbox::eDataManager);
	zassert_not_null(pDataMgrMbox, "pRmsMbox = NULL");

	Mbox* pHttpConnector = new Mbox("HttpConnectorMbox");
    Mbox::registerMbox(pHttpConnector, Mbox::eHttpConnector);
	zassert_not_null(pHttpConnector, "pAdxl37xMbox = NULL");

	Mbox* pRmsStoreMbox = new Mbox("RmsStoreMbox");
    Mbox::registerMbox(pRmsStoreMbox, Mbox::eRmsStore);
	zassert_not_null(pRmsStoreMbox, "pRmsStoreMbox = NULL");

	AdxlDataStore* pAdxlStore = new AdxlDataStore("adxlstore", 5, 4400, true);
	zassert_not_null(pAdxlStore, "memory alloc failed");
	zassert_not_null(pAdxlStore->pInMbox, "pInMbox = NULL");   
    zassert_equal(pAdxlStore->pInMbox->count(), 0, "pInMbox->count() != 0");
    zassert_equal(pAdxlStore->pInMbox, Mbox::getMboxById(Mbox::eAdxlStore),
                  "Error getting mailbox by id");

    pAdxlStore->idle = false;

    pAdxlStore->pm_state = PowerMgr::PmState::eOn;
    pAdxlStore->have_http_connector = true;
    zassert_equal(pAdxlStore->idle, false, "pAdxlStore->idle != false");
    zassert_equal(pAdxlStore->pm_state, PowerMgr::PmState::eOn, "pm_state != PowerMgr::PmState::eOn");
    zassert_true(pAdxlStore->have_http_connector, "have_http_connector != true");

    Mbox::sendMsg(msgType::eAdxl37xIdle, Mbox::eAdxlStore, Mbox::eAdxlStore);

    zassert_equal(pAdxlStore->pInMbox->count(), 1, "pInMbox->count() != 1");

    pAdxlStore->task();  

    zassert_equal(pAdxlStore->pInMbox->count(), 0, "pInMbox->count() != 0");
    zassert_equal(pDataMgrMbox->count(), 1, "pDataMgrMbox->count() != 1");
    zassert_equal(pHttpConnector->count(), 1, "pHttpConnector->count() != 1");
    zassert_equal(pRmsStoreMbox->count(), 1, "pRmsStoreMbox->count() != 1");

    zassert_equal(pAdxlStore->idle, true, "pAdxlStore->idle != true");
    zassert_equal(pAdxlStore->pm_state, PowerMgr::PmState::eOff, "pm_state != PowerMgr::PmState::eOff");

    pElem3 = pRmsStoreMbox->read();
    zassert_equal(pElem3->mtype, msgType::eAdxl37xIdle, "Rcvd mtype != msgType::eAdxl37xIdle");
    zassert_equal(pElem3->from, Mbox::eAdxlStore, "Rcvd sender != Mbox::eAdxlStore");

    pElem = pDataMgrMbox->read();
    zassert_equal(pElem->mtype, msgType::eMaxXYZ, "Rcvd mtype != msgType::eMaxXYZ");
    zassert_equal(pElem->from, Mbox::eAdxlStore, "Rcvd sender != Mbox::eAdxlStore");

    pElem2 = pHttpConnector->read();
    zassert_equal(pElem2->mtype, msgType::eAdxlIdleInd, "Rcvd mtype != msgType::eAdxlIdleInd");
    zassert_equal(pElem2->from, Mbox::eAdxlStore, "Rcvd sender != Mbox::eAdxlStore");

    pElem->free();
    pElem2->free();
    pElem3->free();
    delete pDataMgrMbox;
    delete pHttpConnector;
    delete pRmsStoreMbox;
	delete pAdxlStore; 
}

//Test receive msg eAdxl37xIdle with have_http_connector = false
ZTEST(tests_adxl_store, test_adxl_store_rcv_msg_eAdxl37xIdle2)
{
    PoolElem* pElem;

	Mbox* pRmsStoreMbox = new Mbox("RmsStoreMbox");
    Mbox::registerMbox(pRmsStoreMbox, Mbox::eRmsStore);
	zassert_not_null(pRmsStoreMbox, "pRmsStoreMbox = NULL");

	AdxlDataStore* pAdxlStore = new AdxlDataStore("adxlstore", 5, 4400, true);
	zassert_not_null(pAdxlStore, "memory alloc failed");
	zassert_not_null(pAdxlStore->pInMbox, "pInMbox = NULL");   
    zassert_equal(pAdxlStore->pInMbox->count(), 0, "pInMbox->count() != 0");
    zassert_equal(pAdxlStore->pInMbox, Mbox::getMboxById(Mbox::eAdxlStore),
                  "Error getting mailbox by id");

    pAdxlStore->pm_state = PowerMgr::PmState::eOn;
    pAdxlStore->have_http_connector = false;
    zassert_equal(pAdxlStore->idle, true, "pAdxlStore->idle != true");
    zassert_equal(pAdxlStore->pm_state, PowerMgr::PmState::eOn, "pm_state != PowerMgr::PmState::eOn");
    zassert_false(pAdxlStore->have_http_connector, "have_http_connector != false");

    Mbox::sendMsg(msgType::eAdxl37xIdle, Mbox::eAdxlStore, Mbox::eAdxlStore);

    zassert_equal(pAdxlStore->pInMbox->count(), 1, "pInMbox->count() != 1");

    pAdxlStore->task();  

    zassert_equal(pAdxlStore->pInMbox->count(), 0, "pInMbox->count() != 0");
    zassert_equal(pRmsStoreMbox->count(), 1, "pRmsStoreMbox->count() != 1");

    zassert_equal(pAdxlStore->idle, true, "pAdxlStore->idle != true");
    zassert_equal(pAdxlStore->pm_state, PowerMgr::PmState::eOff, "pm_state != PowerMgr::PmState::eOff");

    pElem = pRmsStoreMbox->read();
    zassert_equal(pElem->mtype, msgType::eAdxl37xIdle, "Rcvd mtype != msgType::eAdxl37xIdle");
    zassert_equal(pElem->from, Mbox::eAdxlStore, "Rcvd sender != Mbox::eAdxlStore");

    pElem->free();
    delete pRmsStoreMbox;
	delete pAdxlStore; 
}

//Test receive msg eAdxlIdleReq with have_http_connector = false
ZTEST(tests_adxl_store, test_adxl_store_rcv_msg_eAdxlIdleReq)
{
	AdxlDataStore* pAdxlStore = new AdxlDataStore("adxlstore", 5, 4400, true);
	zassert_not_null(pAdxlStore, "memory alloc failed");
	zassert_not_null(pAdxlStore->pInMbox, "pInMbox = NULL");   
    zassert_equal(pAdxlStore->pInMbox->count(), 0, "pInMbox->count() != 0");
    zassert_equal(pAdxlStore->pInMbox, Mbox::getMboxById(Mbox::eAdxlStore),
                  "Error getting mailbox by id");

    pAdxlStore->have_http_connector = false;
    zassert_false(pAdxlStore->have_http_connector, "have_http_connector != false");

    Mbox::sendMsg(msgType::eAdxlIdleReq, Mbox::eAdxlStore, Mbox::eAdxlStore);
    zassert_equal(pAdxlStore->pInMbox->count(), 1, "pInMbox->count() != 1");

    pAdxlStore->task();  
    zassert_equal(pAdxlStore->pInMbox->count(), 0, "pInMbox->count() != 0");

	delete pAdxlStore; 
}

//Test receive msg eAdxlIdleReq with have_http_connector = true
ZTEST(tests_adxl_store, test_adxl_store_rcv_msg_eAdxlIdleReq2)
{
    PoolElem* pElem;

	Mbox* pHttpConnector = new Mbox("HttpConnectorMbox");
    Mbox::registerMbox(pHttpConnector, Mbox::eHttpConnector);
	zassert_not_null(pHttpConnector, "pAdxl37xMbox = NULL");

	AdxlDataStore* pAdxlStore = new AdxlDataStore("adxlstore", 5, 4400, true);
	zassert_not_null(pAdxlStore, "memory alloc failed");
	zassert_not_null(pAdxlStore->pInMbox, "pInMbox = NULL");   
    zassert_equal(pAdxlStore->pInMbox->count(), 0, "pInMbox->count() != 0");
    zassert_equal(pAdxlStore->pInMbox, Mbox::getMboxById(Mbox::eAdxlStore),
                  "Error getting mailbox by id");

    pAdxlStore->have_http_connector = true;
    zassert_true(pAdxlStore->have_http_connector, "have_http_connector != true");

    Mbox::sendMsg(msgType::eAdxlIdleReq, Mbox::eAdxlStore, Mbox::eAdxlStore);
    zassert_equal(pAdxlStore->pInMbox->count(), 1, "pInMbox->count() != 1");

    pAdxlStore->task();  
    zassert_equal(pAdxlStore->pInMbox->count(), 0, "pInMbox->count() != 0");
    zassert_equal(pHttpConnector->count(), 1, "pHttpConnector->count() != 1");

    pElem = pHttpConnector->read();
    zassert_equal(pElem->mtype, msgType::eAdxlIdleInd, "Rcvd mtype != msgType::eAdxlIdleInd");
    zassert_equal(pElem->from, Mbox::eAdxlStore, "Rcvd sender != Mbox::eAdxlStore");

    pElem->free();
    delete pHttpConnector;
	delete pAdxlStore; 
}

//Test receive msg eRawDataStatus with full = false
ZTEST(tests_adxl_store, test_adxl_store_rcv_msg_eRawDataStatus)
{
    PoolElem* pElem;

	Mbox* pDispTaskMbox = new Mbox("DispTaskMbox");
    Mbox::registerMbox(pDispTaskMbox, Mbox::eDispTask);
	zassert_not_null(pDispTaskMbox, "pDispTaskMbox = NULL");

	AdxlDataStore* pAdxlStore = new AdxlDataStore("adxlstore", 5, 4400, true);
	zassert_not_null(pAdxlStore, "memory alloc failed");
	zassert_not_null(pAdxlStore->pInMbox, "pInMbox = NULL");   
    zassert_equal(pAdxlStore->pInMbox->count(), 0, "pInMbox->count() != 0");
    zassert_equal(pAdxlStore->pInMbox, Mbox::getMboxById(Mbox::eAdxlStore),
                  "Error getting mailbox by id");

    Mbox::sendMsg(msgType::eRawDataStatus, Mbox::eDispTask, Mbox::eAdxlStore);
    zassert_equal(pAdxlStore->pInMbox->count(), 1, "pInMbox->count() != 1");

    pAdxlStore->full = false;
    pAdxlStore->capa = 30;
    pAdxlStore->wr = 24;
    pAdxlStore->allow_wrap_around = false;

    pAdxlStore->task();  
    //printk("pAdxlStore->pInMbox->count() = %d\n\n", pAdxlStore->pInMbox->count());
    zassert_equal(pAdxlStore->pInMbox->count(), 0, "pInMbox->count() != 0");
    zassert_equal(pDispTaskMbox->count(), 1, "pDispTaskMbox->count() != 1");

    pElem = pDispTaskMbox->read();
    zassert_equal(pElem->mtype, msgType::eRawDataStatus, "Rcvd mtype != msgType::eRawDataStatus");
    zassert_equal(pElem->from, Mbox::eAdxlStore, "Rcvd sender != Mbox::eAdxlStore");

    pElem->free();
    delete pDispTaskMbox;
	delete pAdxlStore; 
}

//Test receive msg eRawDataStatus with full = true and allow_wrap_around = false
ZTEST(tests_adxl_store, test_adxl_store_rcv_msg_eRawDataStatus2)
{
    PoolElem* pElem;

	Mbox* pDispTaskMbox = new Mbox("DispTaskMbox");
    Mbox::registerMbox(pDispTaskMbox, Mbox::eDispTask);
	zassert_not_null(pDispTaskMbox, "pDispTaskMbox = NULL");

	AdxlDataStore* pAdxlStore = new AdxlDataStore("adxlstore", 5, 4400, true);
	zassert_not_null(pAdxlStore, "memory alloc failed");
	zassert_not_null(pAdxlStore->pInMbox, "pInMbox = NULL");   
    zassert_equal(pAdxlStore->pInMbox->count(), 0, "pInMbox->count() != 0");
    zassert_equal(pAdxlStore->pInMbox, Mbox::getMboxById(Mbox::eAdxlStore),
                  "Error getting mailbox by id");

    Mbox::sendMsg(msgType::eRawDataStatus, Mbox::eDispTask, Mbox::eAdxlStore);
    zassert_equal(pAdxlStore->pInMbox->count(), 1, "pInMbox->count() != 1");

    pAdxlStore->full = true;
    pAdxlStore->capa = 30;
    pAdxlStore->wr = 24;
    pAdxlStore->allow_wrap_around = false;

    pAdxlStore->task();  
    //printk("pAdxlStore->pInMbox->count() = %d\n\n", pAdxlStore->pInMbox->count());
    zassert_equal(pAdxlStore->pInMbox->count(), 0, "pInMbox->count() != 0");
    zassert_equal(pDispTaskMbox->count(), 1, "pDispTaskMbox->count() != 1");

    pElem = pDispTaskMbox->read();
    zassert_equal(pElem->mtype, msgType::eRawDataStatus, "Rcvd mtype != msgType::eRawDataStatus");
    zassert_equal(pElem->from, Mbox::eAdxlStore, "Rcvd sender != Mbox::eAdxlStore");

    pElem->free();
    delete pDispTaskMbox;
	delete pAdxlStore; 
}

//Test receive msg eRawDataStatus with full = true and allow_wrap_around = true
ZTEST(tests_adxl_store, test_adxl_store_rcv_msg_eRawDataStatus3)
{
    PoolElem* pElem;

	Mbox* pDispTaskMbox = new Mbox("DispTaskMbox");
    Mbox::registerMbox(pDispTaskMbox, Mbox::eDispTask);
	zassert_not_null(pDispTaskMbox, "pDispTaskMbox = NULL");

	AdxlDataStore* pAdxlStore = new AdxlDataStore("adxlstore", 5, 4400, true);
	zassert_not_null(pAdxlStore, "memory alloc failed");
	zassert_not_null(pAdxlStore->pInMbox, "pInMbox = NULL");   
    zassert_equal(pAdxlStore->pInMbox->count(), 0, "pInMbox->count() != 0");
    zassert_equal(pAdxlStore->pInMbox, Mbox::getMboxById(Mbox::eAdxlStore),
                  "Error getting mailbox by id");

    Mbox::sendMsg(msgType::eRawDataStatus, Mbox::eDispTask, Mbox::eAdxlStore);
    zassert_equal(pAdxlStore->pInMbox->count(), 1, "pInMbox->count() != 1");

    pAdxlStore->full = true;
    pAdxlStore->capa = 30;
    pAdxlStore->wr = 24;
    pAdxlStore->allow_wrap_around = true;

    pAdxlStore->task();  
    //printk("pAdxlStore->pInMbox->count() = %d\n\n", pAdxlStore->pInMbox->count());
    zassert_equal(pAdxlStore->pInMbox->count(), 0, "pInMbox->count() != 0");
    zassert_equal(pDispTaskMbox->count(), 1, "pDispTaskMbox->count() != 1");

    pElem = pDispTaskMbox->read();
    zassert_equal(pElem->mtype, msgType::eRawDataStatus, "Rcvd mtype != msgType::eRawDataStatus");
    zassert_equal(pElem->from, Mbox::eAdxlStore, "Rcvd sender != Mbox::eAdxlStore");

    pElem->free();
    delete pDispTaskMbox;
	delete pAdxlStore; 
}

//Test receive msg eRawData 
ZTEST(tests_adxl_store, test_adxl_store_rcv_msg_eRawData)
{
    PoolElem* pElem;

	Mbox* pDispTaskMbox = new Mbox("DispTaskMbox");
    Mbox::registerMbox(pDispTaskMbox, Mbox::eDispTask);
	zassert_not_null(pDispTaskMbox, "pDispTaskMbox = NULL");

	AdxlDataStore* pAdxlStore = new AdxlDataStore("adxlstore", 5, 4400, true);
	zassert_not_null(pAdxlStore, "memory alloc failed");
	zassert_not_null(pAdxlStore->pInMbox, "pInMbox = NULL");   
    zassert_equal(pAdxlStore->pInMbox->count(), 0, "pInMbox->count() != 0");
    zassert_equal(pAdxlStore->pInMbox, Mbox::getMboxById(Mbox::eAdxlStore),
                  "Error getting mailbox by id");

    //struct AdxlRawDataProgress {
    //    uint32_t startRd;      
    //    uint32_t rcvdEntries;  
    //    uint32_t requestedEntries;  };
    struct AdxlRawDataProgress p = {0, 0, 20};
    Mbox::sendMsg(msgType::eRawData, Mbox::eDispTask, Mbox::eAdxlStore, (uint8_t*)&p, sizeof(p));
    zassert_equal(pAdxlStore->pInMbox->count(), 1, "pInMbox->count() != 1");

    pAdxlStore->task();  
    //printk("pAdxlStore->pInMbox->count() = %d\n\n", pAdxlStore->pInMbox->count());
    zassert_equal(pAdxlStore->pInMbox->count(), 0, "pInMbox->count() != 0");
    zassert_equal(pDispTaskMbox->count(), 1, "pDispTaskMbox->count() != 1");

    pElem = pDispTaskMbox->read();
    zassert_equal(pElem->mtype, msgType::eRawData, "Rcvd mtype != msgType::eRawData");
    zassert_equal(pElem->from, Mbox::eAdxlStore, "Rcvd sender != Mbox::eAdxlStore");

    pElem->free();
    delete pDispTaskMbox;
	delete pAdxlStore; 
}

//Test receive msg eDispFirstEntries 
ZTEST(tests_adxl_store, test_adxl_store_rcv_msg_eDispFirstEntries)
{
    PoolElem* pElem;

	Mbox* pDispTaskMbox = new Mbox("DispTaskMbox");
    Mbox::registerMbox(pDispTaskMbox, Mbox::eDispTask);
	zassert_not_null(pDispTaskMbox, "pDispTaskMbox = NULL");

	AdxlDataStore* pAdxlStore = new AdxlDataStore("adxlstore", 5, 4400, true);
	zassert_not_null(pAdxlStore, "memory alloc failed");
	zassert_not_null(pAdxlStore->pInMbox, "pInMbox = NULL");   
    zassert_equal(pAdxlStore->pInMbox->count(), 0, "pInMbox->count() != 0");
    zassert_equal(pAdxlStore->pInMbox, Mbox::getMboxById(Mbox::eAdxlStore),
                  "Error getting mailbox by id");

    //struct AdxlRawDataProgress {
    //    uint32_t startRd;      
    //    uint32_t rcvdEntries;  
    //    uint32_t requestedEntries;  };
    struct AdxlRawDataProgress p = {0, 0, 20};
    Mbox::sendMsg(msgType::eDispFirstEntries, Mbox::eDispTask, Mbox::eAdxlStore, (uint8_t*)&p, sizeof(p));
    zassert_equal(pAdxlStore->pInMbox->count(), 1, "pInMbox->count() != 1");

    pAdxlStore->task();  
    //printk("pAdxlStore->pInMbox->count() = %d\n\n", pAdxlStore->pInMbox->count());
    zassert_equal(pAdxlStore->pInMbox->count(), 0, "pInMbox->count() != 0");
    zassert_equal(pDispTaskMbox->count(), 1, "pDispTaskMbox->count() != 1");

    pElem = pDispTaskMbox->read();
    zassert_equal(pElem->mtype, msgType::eDispFirstEntries, "Rcvd mtype != msgType::eDispFirstEntries");
    zassert_equal(pElem->from, Mbox::eAdxlStore, "Rcvd sender != Mbox::eAdxlStore");

    pElem->free();
    delete pDispTaskMbox;
	delete pAdxlStore; 
}

//Test receive msg eDispLastEntries 
ZTEST(tests_adxl_store, test_adxl_store_rcv_msg_eDispLastEntries)
{
    PoolElem* pElem;

	Mbox* pDispTaskMbox = new Mbox("DispTaskMbox");
    Mbox::registerMbox(pDispTaskMbox, Mbox::eDispTask);
	zassert_not_null(pDispTaskMbox, "pDispTaskMbox = NULL");

	AdxlDataStore* pAdxlStore = new AdxlDataStore("adxlstore", 5, 4400, true);
	zassert_not_null(pAdxlStore, "memory alloc failed");
	zassert_not_null(pAdxlStore->pInMbox, "pInMbox = NULL");   
    zassert_equal(pAdxlStore->pInMbox->count(), 0, "pInMbox->count() != 0");
    zassert_equal(pAdxlStore->pInMbox, Mbox::getMboxById(Mbox::eAdxlStore),
                  "Error getting mailbox by id");

    //struct AdxlRawDataProgress {
    //    uint32_t startRd;      
    //    uint32_t rcvdEntries;  
    //    uint32_t requestedEntries;  };
    struct AdxlRawDataProgress p = {0, 0, 20};
    Mbox::sendMsg(msgType::eDispLastEntries, Mbox::eDispTask, Mbox::eAdxlStore, (uint8_t*)&p, sizeof(p));
    zassert_equal(pAdxlStore->pInMbox->count(), 1, "pInMbox->count() != 1");

    pAdxlStore->task();  
    //printk("pAdxlStore->pInMbox->count() = %d\n\n", pAdxlStore->pInMbox->count());
    zassert_equal(pAdxlStore->pInMbox->count(), 0, "pInMbox->count() != 0");
    zassert_equal(pDispTaskMbox->count(), 1, "pDispTaskMbox->count() != 1");

    pElem = pDispTaskMbox->read();
    zassert_equal(pElem->mtype, msgType::eDispLastEntries, "Rcvd mtype != msgType::eDispLastEntries");
    zassert_equal(pElem->from, Mbox::eAdxlStore, "Rcvd sender != Mbox::eAdxlStore");

    pElem->free();
    delete pDispTaskMbox;
	delete pAdxlStore; 
}

//Test receive msg eFwFiltData 
ZTEST(tests_adxl_store, test_adxl_store_rcv_msg_eFwFiltData)
{
    PoolElem* pElem;

	Mbox* pDispTaskMbox = new Mbox("DispTaskMbox");
    Mbox::registerMbox(pDispTaskMbox, Mbox::eDispTask);
	zassert_not_null(pDispTaskMbox, "pDispTaskMbox = NULL");

	AdxlDataStore* pAdxlStore = new AdxlDataStore("adxlstore", 5, 4400, true);
	zassert_not_null(pAdxlStore, "memory alloc failed");
	zassert_not_null(pAdxlStore->pInMbox, "pInMbox = NULL");   
    zassert_equal(pAdxlStore->pInMbox->count(), 0, "pInMbox->count() != 0");
    zassert_equal(pAdxlStore->pInMbox, Mbox::getMboxById(Mbox::eAdxlStore),
                  "Error getting mailbox by id");

    //struct AdxlRawDataProgress {
    //    uint32_t startRd;      
    //    uint32_t rcvdEntries;  
    //    uint32_t requestedEntries;  };
    struct AdxlRawDataProgress p = {0, 0, 20};
    Mbox::sendMsg(msgType::eFwFiltData, Mbox::eDispTask, Mbox::eAdxlStore, (uint8_t*)&p, sizeof(p));
    zassert_equal(pAdxlStore->pInMbox->count(), 1, "pInMbox->count() != 1");

    pAdxlStore->task();  
    //printk("pAdxlStore->pInMbox->count() = %d\n\n", pAdxlStore->pInMbox->count());
    zassert_equal(pAdxlStore->pInMbox->count(), 0, "pInMbox->count() != 0");
    zassert_equal(pDispTaskMbox->count(), 1, "pDispTaskMbox->count() != 1");

    pElem = pDispTaskMbox->read();
    zassert_equal(pElem->mtype, msgType::eFwFiltData, "Rcvd mtype != msgType::eFwFiltData");
    zassert_equal(pElem->from, Mbox::eAdxlStore, "Rcvd sender != Mbox::eAdxlStore");

    pElem->free();
    delete pDispTaskMbox;
	delete pAdxlStore; 
}

//Test receive msg eBwFiltData 
ZTEST(tests_adxl_store, test_adxl_store_rcv_msg_eBwFiltData)
{
    PoolElem* pElem;

	Mbox* pDispTaskMbox = new Mbox("DispTaskMbox");
    Mbox::registerMbox(pDispTaskMbox, Mbox::eDispTask);
	zassert_not_null(pDispTaskMbox, "pDispTaskMbox = NULL");

	AdxlDataStore* pAdxlStore = new AdxlDataStore("adxlstore", 5, 4400, true);
	zassert_not_null(pAdxlStore, "memory alloc failed");
	zassert_not_null(pAdxlStore->pInMbox, "pInMbox = NULL");   
    zassert_equal(pAdxlStore->pInMbox->count(), 0, "pInMbox->count() != 0");
    zassert_equal(pAdxlStore->pInMbox, Mbox::getMboxById(Mbox::eAdxlStore),
                  "Error getting mailbox by id");

    //struct AdxlRawDataProgress {
    //    uint32_t startRd;      
    //    uint32_t rcvdEntries;  
    //    uint32_t requestedEntries;  };
    struct AdxlRawDataProgress p = {0, 0, 20};
    Mbox::sendMsg(msgType::eBwFiltData, Mbox::eDispTask, Mbox::eAdxlStore, (uint8_t*)&p, sizeof(p));
    zassert_equal(pAdxlStore->pInMbox->count(), 1, "pInMbox->count() != 1");

    pAdxlStore->task();  
    //printk("pAdxlStore->pInMbox->count() = %d\n\n", pAdxlStore->pInMbox->count());
    zassert_equal(pAdxlStore->pInMbox->count(), 0, "pInMbox->count() != 0");
    zassert_equal(pDispTaskMbox->count(), 1, "pDispTaskMbox->count() != 1");

    pElem = pDispTaskMbox->read();
    zassert_equal(pElem->mtype, msgType::eBwFiltData, "Rcvd mtype != msgType::eBwFiltData");
    zassert_equal(pElem->from, Mbox::eAdxlStore, "Rcvd sender != Mbox::eAdxlStore");

    pElem->free();
    delete pDispTaskMbox;
	delete pAdxlStore; 
}

//Test receive msg eSResultData 
ZTEST(tests_adxl_store, test_adxl_store_rcv_msg_eSResultData)
{
    PoolElem* pElem;

	Mbox* pDispTaskMbox = new Mbox("DispTaskMbox");
    Mbox::registerMbox(pDispTaskMbox, Mbox::eDispTask);
	zassert_not_null(pDispTaskMbox, "pDispTaskMbox = NULL");

	AdxlDataStore* pAdxlStore = new AdxlDataStore("adxlstore", 5, 4400, true);
	zassert_not_null(pAdxlStore, "memory alloc failed");
	zassert_not_null(pAdxlStore->pInMbox, "pInMbox = NULL");   
    zassert_equal(pAdxlStore->pInMbox->count(), 0, "pInMbox->count() != 0");
    zassert_equal(pAdxlStore->pInMbox, Mbox::getMboxById(Mbox::eAdxlStore),
                  "Error getting mailbox by id");

    //struct AdxlRawDataProgress {
    //    uint32_t startRd;      
    //    uint32_t rcvdEntries;  
    //    uint32_t requestedEntries;  };
    struct AdxlRawDataProgress p = {0, 0, 20};
    Mbox::sendMsg(msgType::eSResultData, Mbox::eDispTask, Mbox::eAdxlStore, (uint8_t*)&p, sizeof(p));
    zassert_equal(pAdxlStore->pInMbox->count(), 1, "pInMbox->count() != 1");

    pAdxlStore->task();  
    //printk("pAdxlStore->pInMbox->count() = %d\n\n", pAdxlStore->pInMbox->count());
    zassert_equal(pAdxlStore->pInMbox->count(), 0, "pInMbox->count() != 0");
    zassert_equal(pDispTaskMbox->count(), 1, "pDispTaskMbox->count() != 1");

    pElem = pDispTaskMbox->read();
    zassert_equal(pElem->mtype, msgType::eSResultData, "Rcvd mtype != msgType::eSResultData");
    zassert_equal(pElem->from, Mbox::eAdxlStore, "Rcvd sender != Mbox::eAdxlStore");

    pElem->free();
    delete pDispTaskMbox;
	delete pAdxlStore; 
}

//Test receive msg eStoreFwFiltResult with all forward filtered values are stored
ZTEST(tests_adxl_store, test_adxl_store_rcv_msg_eStoreFwFiltResult)
{
    PoolElem* pElem;

	Mbox* pDispTaskMbox = new Mbox("DispTaskMbox");
    Mbox::registerMbox(pDispTaskMbox, Mbox::eDispTask);
	zassert_not_null(pDispTaskMbox, "pDispTaskMbox = NULL");

	AdxlDataStore* pAdxlStore = new AdxlDataStore("adxlstore", 5, 4400, true);
	zassert_not_null(pAdxlStore, "memory alloc failed");
	zassert_not_null(pAdxlStore->pInMbox, "pInMbox = NULL");   
    zassert_equal(pAdxlStore->pInMbox->count(), 0, "pInMbox->count() != 0");
    zassert_equal(pAdxlStore->pInMbox, Mbox::getMboxById(Mbox::eAdxlStore),
                  "Error getting mailbox by id");

    pAdxlStore->num_samples = 10;

    //struct AdxlFilterDataProgress {
    //    uint32_t startWr;           
    //    uint32_t sentEntries;       
    //    uint32_t requestedEntries;  
    //    uint8_t data[sizeof(double) * MAX_NUM_REQ_ENTRIES];};
    struct AdxlFilterDataProgress p = {0, 0, 10, {1,2,3,4,5,6,7,8,9,0}};
    Mbox::sendMsg(msgType::eStoreFwFiltResult, Mbox::eDispTask, Mbox::eAdxlStore, (uint8_t*)&p, sizeof(p));
    zassert_equal(pAdxlStore->pInMbox->count(), 1, "pInMbox->count() != 1");

    pAdxlStore->task();  
    //printk("pAdxlStore->pInMbox->count() = %d\n\n", pAdxlStore->pInMbox->count());
    zassert_equal(pAdxlStore->pInMbox->count(), 0, "pInMbox->count() != 0");
    zassert_equal(pDispTaskMbox->count(), 1, "pDispTaskMbox->count() != 1");

    pElem = pDispTaskMbox->read();
    zassert_equal(pElem->mtype, msgType::eStoredFwFiltVals, "Rcvd mtype != msgType::eStoredFwFiltVals");
    zassert_equal(pElem->from, Mbox::eAdxlStore, "Rcvd sender != Mbox::eAdxlStore");

    pElem->free();
    delete pDispTaskMbox;
	delete pAdxlStore; 
}


//Test receive msg eStoreFwFiltResult with NOT all forward filtered values are stored
ZTEST(tests_adxl_store, test_adxl_store_rcv_msg_eStoreFwFiltResult2)
{
	AdxlDataStore* pAdxlStore = new AdxlDataStore("adxlstore", 5, 4400, true);
	zassert_not_null(pAdxlStore, "memory alloc failed");
	zassert_not_null(pAdxlStore->pInMbox, "pInMbox = NULL");   
    zassert_equal(pAdxlStore->pInMbox->count(), 0, "pInMbox->count() != 0");
    zassert_equal(pAdxlStore->pInMbox, Mbox::getMboxById(Mbox::eAdxlStore),
                  "Error getting mailbox by id");

    pAdxlStore->num_samples = 20;

    //struct AdxlFilterDataProgress {
    //    uint32_t startWr;           
    //    uint32_t sentEntries;       
    //    uint32_t requestedEntries;  
    //    uint8_t data[sizeof(double) * MAX_NUM_REQ_ENTRIES];};
    struct AdxlFilterDataProgress p = {0, 0, 10, {1,2,3,4,5,6,7,8,9,0}};
    Mbox::sendMsg(msgType::eStoreFwFiltResult, Mbox::eDispTask, Mbox::eAdxlStore, (uint8_t*)&p, sizeof(p));
    zassert_equal(pAdxlStore->pInMbox->count(), 1, "pInMbox->count() != 1");

    pAdxlStore->task();  
    printk("pAdxlStore->pInMbox->count() = %d\n\n", pAdxlStore->pInMbox->count());

    zassert_equal(pAdxlStore->pInMbox->count(), 0, "pInMbox->count() != 0");

	delete pAdxlStore; 
}

//Test receive msg eStoreBwFiltResult with all backward filtered values are stored
ZTEST(tests_adxl_store, test_adxl_store_rcv_msg_eStoreBwFiltResult)
{
    PoolElem* pElem;

	Mbox* pDispTaskMbox = new Mbox("DispTaskMbox");
    Mbox::registerMbox(pDispTaskMbox, Mbox::eDispTask);
	zassert_not_null(pDispTaskMbox, "pDispTaskMbox = NULL");

	AdxlDataStore* pAdxlStore = new AdxlDataStore("adxlstore", 5, 4400, true);
	zassert_not_null(pAdxlStore, "memory alloc failed");
	zassert_not_null(pAdxlStore->pInMbox, "pInMbox = NULL");   
    zassert_equal(pAdxlStore->pInMbox->count(), 0, "pInMbox->count() != 0");
    zassert_equal(pAdxlStore->pInMbox, Mbox::getMboxById(Mbox::eAdxlStore),
                  "Error getting mailbox by id");

    pAdxlStore->num_samples = 10;

    //struct AdxlFilterDataProgress {
    //    uint32_t startWr;           
    //    uint32_t sentEntries;       
    //    uint32_t requestedEntries;  
    //    uint8_t data[sizeof(double) * MAX_NUM_REQ_ENTRIES];};
    struct AdxlFilterDataProgress p = {0, 0, 10, {1,2,3,4,5,6,7,8,9,0}};
    Mbox::sendMsg(msgType::eStoreBwFiltResult, Mbox::eDispTask, Mbox::eAdxlStore, (uint8_t*)&p, sizeof(p));
    zassert_equal(pAdxlStore->pInMbox->count(), 1, "pInMbox->count() != 1");

    pAdxlStore->task();  
    //printk("pAdxlStore->pInMbox->count() = %d\n\n", pAdxlStore->pInMbox->count());
    zassert_equal(pAdxlStore->pInMbox->count(), 0, "pInMbox->count() != 0");
    zassert_equal(pDispTaskMbox->count(), 1, "pDispTaskMbox->count() != 1");

    pElem = pDispTaskMbox->read();
    zassert_equal(pElem->mtype, msgType::eStoredBwFiltVals, "Rcvd mtype != msgType::eStoredBwFiltVals");
    zassert_equal(pElem->from, Mbox::eAdxlStore, "Rcvd sender != Mbox::eAdxlStore");

    pElem->free();
    delete pDispTaskMbox;
	delete pAdxlStore; 
}


//Test receive msg eStoreBwFiltResult with NOT all backward filtered values are stored
ZTEST(tests_adxl_store, test_adxl_store_rcv_msg_eStoreBwFiltResult2)
{
	AdxlDataStore* pAdxlStore = new AdxlDataStore("adxlstore", 5, 4400, true);
	zassert_not_null(pAdxlStore, "memory alloc failed");
	zassert_not_null(pAdxlStore->pInMbox, "pInMbox = NULL");   
    zassert_equal(pAdxlStore->pInMbox->count(), 0, "pInMbox->count() != 0");
    zassert_equal(pAdxlStore->pInMbox, Mbox::getMboxById(Mbox::eAdxlStore),
                  "Error getting mailbox by id");

    pAdxlStore->num_samples = 20;

    //struct AdxlFilterDataProgress {
    //    uint32_t startWr;           
    //    uint32_t sentEntries;       
    //    uint32_t requestedEntries;  
    //    uint8_t data[sizeof(double) * MAX_NUM_REQ_ENTRIES];};
    struct AdxlFilterDataProgress p = {0, 0, 10, {1,2,3,4,5,6,7,8,9,0}};
    Mbox::sendMsg(msgType::eStoreBwFiltResult, Mbox::eDispTask, Mbox::eAdxlStore, (uint8_t*)&p, sizeof(p));
    zassert_equal(pAdxlStore->pInMbox->count(), 1, "pInMbox->count() != 1");

    pAdxlStore->task();  
    printk("pAdxlStore->pInMbox->count() = %d\n\n", pAdxlStore->pInMbox->count());

    zassert_equal(pAdxlStore->pInMbox->count(), 0, "pInMbox->count() != 0");

	delete pAdxlStore; 
}

//Test receive msg eStoreSResult with all S result values are stored
ZTEST(tests_adxl_store, test_adxl_store_rcv_msg_eStoreSResult)
{
    PoolElem* pElem;

	Mbox* pDispTaskMbox = new Mbox("DispTaskMbox");
    Mbox::registerMbox(pDispTaskMbox, Mbox::eDispTask);
	zassert_not_null(pDispTaskMbox, "pDispTaskMbox = NULL");

	AdxlDataStore* pAdxlStore = new AdxlDataStore("adxlstore", 5, 4400, true);
	zassert_not_null(pAdxlStore, "memory alloc failed");
	zassert_not_null(pAdxlStore->pInMbox, "pInMbox = NULL");   
    zassert_equal(pAdxlStore->pInMbox->count(), 0, "pInMbox->count() != 0");
    zassert_equal(pAdxlStore->pInMbox, Mbox::getMboxById(Mbox::eAdxlStore),
                  "Error getting mailbox by id");

    pAdxlStore->num_samples = 10;

    //struct AdxlFilterDataProgress {
    //    uint32_t startWr;           
    //    uint32_t sentEntries;       
    //    uint32_t requestedEntries;  
    //    uint8_t data[sizeof(double) * MAX_NUM_REQ_ENTRIES];};
    struct AdxlFilterDataProgress p = {0, 0, 10, {1,2,3,4,5,6,7,8,9,0}};
    Mbox::sendMsg(msgType::eStoreSResult, Mbox::eDispTask, Mbox::eAdxlStore, (uint8_t*)&p, sizeof(p));
    zassert_equal(pAdxlStore->pInMbox->count(), 1, "pInMbox->count() != 1");

    pAdxlStore->task();  
    //printk("pAdxlStore->pInMbox->count() = %d\n\n", pAdxlStore->pInMbox->count());
    zassert_equal(pAdxlStore->pInMbox->count(), 0, "pInMbox->count() != 0");
    zassert_equal(pDispTaskMbox->count(), 1, "pDispTaskMbox->count() != 1");

    pElem = pDispTaskMbox->read();
    zassert_equal(pElem->mtype, msgType::eStoredSResultVals, "Rcvd mtype != msgType::eStoredSResultVals");
    zassert_equal(pElem->from, Mbox::eAdxlStore, "Rcvd sender != Mbox::eAdxlStore");

    pElem->free();
    delete pDispTaskMbox;
	delete pAdxlStore; 
}


//Test receive msg eStoreSResult with NOT all S result values are stored
ZTEST(tests_adxl_store, test_adxl_store_rcv_msg_eStoreSResult2)
{
	AdxlDataStore* pAdxlStore = new AdxlDataStore("adxlstore", 5, 4400, true);
	zassert_not_null(pAdxlStore, "memory alloc failed");
	zassert_not_null(pAdxlStore->pInMbox, "pInMbox = NULL");   
    zassert_equal(pAdxlStore->pInMbox->count(), 0, "pInMbox->count() != 0");
    zassert_equal(pAdxlStore->pInMbox, Mbox::getMboxById(Mbox::eAdxlStore),
                  "Error getting mailbox by id");

    pAdxlStore->num_samples = 20;

    //struct AdxlFilterDataProgress {
    //    uint32_t startWr;           
    //    uint32_t sentEntries;       
    //    uint32_t requestedEntries;  
    //    uint8_t data[sizeof(double) * MAX_NUM_REQ_ENTRIES];};
    struct AdxlFilterDataProgress p = {0, 0, 10, {1,2,3,4,5,6,7,8,9,0}};
    Mbox::sendMsg(msgType::eStoreSResult, Mbox::eDispTask, Mbox::eAdxlStore, (uint8_t*)&p, sizeof(p));
    zassert_equal(pAdxlStore->pInMbox->count(), 1, "pInMbox->count() != 1");

    pAdxlStore->task();  
    printk("pAdxlStore->pInMbox->count() = %d\n\n", pAdxlStore->pInMbox->count());

    zassert_equal(pAdxlStore->pInMbox->count(), 0, "pInMbox->count() != 0");

	delete pAdxlStore; 
}

//Test receive msg eStoreDResult with all D result values are stored
ZTEST(tests_adxl_store, test_adxl_store_rcv_msg_eStoreDResult)
{
    PoolElem* pElem;

	Mbox* pDispTaskMbox = new Mbox("DispTaskMbox");
    Mbox::registerMbox(pDispTaskMbox, Mbox::eDispTask);
	zassert_not_null(pDispTaskMbox, "pDispTaskMbox = NULL");

	AdxlDataStore* pAdxlStore = new AdxlDataStore("adxlstore", 5, 4400, true);
	zassert_not_null(pAdxlStore, "memory alloc failed");
	zassert_not_null(pAdxlStore->pInMbox, "pInMbox = NULL");   
    zassert_equal(pAdxlStore->pInMbox->count(), 0, "pInMbox->count() != 0");
    zassert_equal(pAdxlStore->pInMbox, Mbox::getMboxById(Mbox::eAdxlStore),
                  "Error getting mailbox by id");

    pAdxlStore->num_samples = 10;

    //struct AdxlFilterDataProgress {
    //    uint32_t startWr;           
    //    uint32_t sentEntries;       
    //    uint32_t requestedEntries;  
    //    uint8_t data[sizeof(double) * MAX_NUM_REQ_ENTRIES];};
    struct AdxlFilterDataProgress p = {0, 0, 10, {1,2,3,4,5,6,7,8,9,0}};
    Mbox::sendMsg(msgType::eStoreDResult, Mbox::eDispTask, Mbox::eAdxlStore, (uint8_t*)&p, sizeof(p));
    zassert_equal(pAdxlStore->pInMbox->count(), 1, "pInMbox->count() != 1");

    pAdxlStore->task();  
    //printk("pAdxlStore->pInMbox->count() = %d\n\n", pAdxlStore->pInMbox->count());
    zassert_equal(pAdxlStore->pInMbox->count(), 0, "pInMbox->count() != 0");
    zassert_equal(pDispTaskMbox->count(), 1, "pDispTaskMbox->count() != 1");

    pElem = pDispTaskMbox->read();
    zassert_equal(pElem->mtype, msgType::eStoredDResultVals, "Rcvd mtype != msgType::eStoredDResultVals");
    zassert_equal(pElem->from, Mbox::eAdxlStore, "Rcvd sender != Mbox::eAdxlStore");

    pElem->free();
    delete pDispTaskMbox;
	delete pAdxlStore; 
}


//Test receive msg eStoreDResult with NOT all D result values are stored
ZTEST(tests_adxl_store, test_adxl_store_rcv_msg_eStoreDResult2)
{
	AdxlDataStore* pAdxlStore = new AdxlDataStore("adxlstore", 5, 4400, true);
	zassert_not_null(pAdxlStore, "memory alloc failed");
	zassert_not_null(pAdxlStore->pInMbox, "pInMbox = NULL");   
    zassert_equal(pAdxlStore->pInMbox->count(), 0, "pInMbox->count() != 0");
    zassert_equal(pAdxlStore->pInMbox, Mbox::getMboxById(Mbox::eAdxlStore),
                  "Error getting mailbox by id");

    pAdxlStore->num_samples = 20;

    //struct AdxlFilterDataProgress {
    //    uint32_t startWr;           
    //    uint32_t sentEntries;       
    //    uint32_t requestedEntries;  
    //    uint8_t data[sizeof(double) * MAX_NUM_REQ_ENTRIES];};
    struct AdxlFilterDataProgress p = {0, 0, 10, {1,2,3,4,5,6,7,8,9,0}};
    Mbox::sendMsg(msgType::eStoreDResult, Mbox::eDispTask, Mbox::eAdxlStore, (uint8_t*)&p, sizeof(p));
    zassert_equal(pAdxlStore->pInMbox->count(), 1, "pInMbox->count() != 1");

    pAdxlStore->task();  
    printk("pAdxlStore->pInMbox->count() = %d\n\n", pAdxlStore->pInMbox->count());

    zassert_equal(pAdxlStore->pInMbox->count(), 0, "pInMbox->count() != 0");

	delete pAdxlStore; 
}

//Test receive msg eDispData
ZTEST(tests_adxl_store, test_adxl_store_rcv_msg_eDispData)
{
    PoolElem* pElem;

	Mbox* pDispTaskMbox = new Mbox("DispTaskMbox");
    Mbox::registerMbox(pDispTaskMbox, Mbox::eDispTask);
	zassert_not_null(pDispTaskMbox, "pDispTaskMbox = NULL");

	AdxlDataStore* pAdxlStore = new AdxlDataStore("adxlstore", 5, 4400, true);
	zassert_not_null(pAdxlStore, "memory alloc failed");
	zassert_not_null(pAdxlStore->pInMbox, "pInMbox = NULL");   
    zassert_equal(pAdxlStore->pInMbox->count(), 0, "pInMbox->count() != 0");
    zassert_equal(pAdxlStore->pInMbox, Mbox::getMboxById(Mbox::eAdxlStore),
                  "Error getting mailbox by id");

    pAdxlStore->num_samples = 10;

    //struct AdxlRawDataProgress {
    //    uint32_t startRd;      
    //    uint32_t rcvdEntries;  
    //    uint32_t requestedEntries;  };
    struct AdxlRawDataProgress p = {0, 0, 20};
    Mbox::sendMsg(msgType::eDispData, Mbox::eDispTask, Mbox::eAdxlStore, (uint8_t*)&p, sizeof(p));
    zassert_equal(pAdxlStore->pInMbox->count(), 1, "pInMbox->count() != 1");

    pAdxlStore->task();  
    //printk("pAdxlStore->pInMbox->count() = %d\n\n", pAdxlStore->pInMbox->count());
    zassert_equal(pAdxlStore->pInMbox->count(), 0, "pInMbox->count() != 0");
    zassert_equal(pDispTaskMbox->count(), 1, "pDispTaskMbox->count() != 1");

    pElem = pDispTaskMbox->read();
    zassert_equal(pElem->mtype, msgType::eDispData, "Rcvd mtype != msgType::eDispData");
    zassert_equal(pElem->from, Mbox::eAdxlStore, "Rcvd sender != Mbox::eAdxlStore");

    pElem->free();
    delete pDispTaskMbox;
	delete pAdxlStore; 
}

//Test receive unexpected msg 
ZTEST(tests_adxl_store, test_adxl_store_rcv_msg_unexpected)
{
	AdxlDataStore* pAdxlStore = new AdxlDataStore("adxlstore", 5, 4400, true);

	zassert_not_null(pAdxlStore, "memory alloc failed");
	zassert_not_null(pAdxlStore->pInMbox, "pInMbox = NULL");   
    zassert_equal(pAdxlStore->pInMbox->count(), 0, "pInMbox->count() != 0");
    zassert_equal(pAdxlStore->pInMbox, Mbox::getMboxById(Mbox::eAdxlStore),
                  "Error getting mailbox by id");

    Mbox::sendMsg(msgType::eResetCause, Mbox::eAdxlStore, Mbox::eAdxlStore);

    zassert_equal(pAdxlStore->pInMbox->count(), 1, "pInMbox->count() != 1");

    pAdxlStore->task();  

    zassert_equal(pAdxlStore->pInMbox->count(), 0, "pInMbox->count() != 0");

	delete pAdxlStore; 
}

//Test receive no msg 
ZTEST(tests_adxl_store, test_adxl_store_rcv_msg_none)
{
	AdxlDataStore* pAdxlStore = new AdxlDataStore("adxlstore", 5, 4400, true);

	zassert_not_null(pAdxlStore, "memory alloc failed");
	zassert_not_null(pAdxlStore->pInMbox, "pInMbox = NULL");   
    zassert_equal(pAdxlStore->pInMbox->count(), 0, "pInMbox->count() != 0");
    zassert_equal(pAdxlStore->pInMbox, Mbox::getMboxById(Mbox::eAdxlStore),
                  "Error getting mailbox by id");

    pAdxlStore->pInMbox->write(NULL);
    
    pAdxlStore->task();  

    zassert_equal(pAdxlStore->pInMbox->count(), 0, "pInMbox->count() != 0");
	delete pAdxlStore; 
}

//Test function dump
ZTEST(tests_adxl_store, test_adxl_store_dump)
{
	AdxlDataStore* pAdxlStore = new AdxlDataStore("adxlstore", 5, 4400, true);

	zassert_not_null(pAdxlStore, "memory alloc failed");
	zassert_not_null(pAdxlStore->pInMbox, "pInMbox = NULL");   
    zassert_equal(pAdxlStore->pInMbox->count(), 0, "pInMbox->count() != 0");
    zassert_equal(pAdxlStore->pInMbox, Mbox::getMboxById(Mbox::eAdxlStore),
                  "Error getting mailbox by id");

    pAdxlStore->wr = 30;
    pAdxlStore->rd = 24;

    pAdxlStore->dump();

	delete pAdxlStore; 
}

ZTEST(tests_adxl_store, test_adxl_store_dump2)
{
	AdxlDataStore* pAdxlStore = new AdxlDataStore("adxlstore", 5, 4400, true);

	zassert_not_null(pAdxlStore, "memory alloc failed");
	zassert_not_null(pAdxlStore->pInMbox, "pInMbox = NULL");   
    zassert_equal(pAdxlStore->pInMbox->count(), 0, "pInMbox->count() != 0");
    zassert_equal(pAdxlStore->pInMbox, Mbox::getMboxById(Mbox::eAdxlStore),
                  "Error getting mailbox by id");

    pAdxlStore->wr = 24;
    pAdxlStore->rd = 30;

    pAdxlStore->dump();

	delete pAdxlStore; 
}

//Test function shell_dump
ZTEST(tests_adxl_store, test_adxl_store_shell_dump)
{
    PoolElem* pElem;
    PoolElem* pElem2;

	Mbox* pAdxl37xMbox = new Mbox("Adxl37xMbox");
    Mbox::registerMbox(pAdxl37xMbox, Mbox::eAdxl37xTask);
	zassert_not_null(pAdxl37xMbox, "pAdxl37xMbox = NULL");

	AdxlDataStore* pAdxlStore = new AdxlDataStore("adxlstore", 5, 4400, true);

	zassert_not_null(pAdxlStore, "memory alloc failed");
	zassert_not_null(pAdxlStore->pInMbox, "pInMbox = NULL");   
    zassert_equal(pAdxlStore->pInMbox->count(), 0, "pInMbox->count() != 0");
    zassert_equal(pAdxlStore->pInMbox, Mbox::getMboxById(Mbox::eAdxlStore),
                  "Error getting mailbox by id");

    pAdxlStore->shell_dump(NULL, 1, NULL);
    pAdxlStore->task();  
    
    pElem = pAdxl37xMbox->read();
    zassert_equal(pElem->mtype, msgType::eAdxlStorageDumpActive, "Rcvd mtype != msgType::eAdxlStorageDumpActive");
    zassert_equal(pElem->from, Mbox::eAdxlStore, "Rcvd sender != Mbox::eAdxlStore");

    pElem2 = pAdxl37xMbox->read();
    zassert_equal(pElem2->mtype, msgType::eAdxlStorageDumpComplete, "Rcvd mtype != msgType::eAdxlStorageDumpComplete");
    zassert_equal(pElem2->from, Mbox::eAdxlStore, "Rcvd sender != Mbox::eAdxlStore");

    pElem->free();
    pElem2->free();
    delete pAdxl37xMbox;
	delete pAdxlStore; 
}

//Test function dumpAll mit full = true
ZTEST(tests_adxl_store, test_adxl_store_dumpAll)
{
	AdxlDataStore* pAdxlStore = new AdxlDataStore("adxlstore", 5, 4400, true);

	zassert_not_null(pAdxlStore, "memory alloc failed");
	zassert_not_null(pAdxlStore->pInMbox, "pInMbox = NULL");   
    zassert_equal(pAdxlStore->pInMbox->count(), 0, "pInMbox->count() != 0");
    zassert_equal(pAdxlStore->pInMbox, Mbox::getMboxById(Mbox::eAdxlStore),
                  "Error getting mailbox by id");

    pAdxlStore->full = true;
    pAdxlStore->capa = 30;
    pAdxlStore->wr = 24;
    //pAdxlStore->rd = 24;
    //pAdxlStore->bytes_per_sec = 6;
    pAdxlStore->allow_wrap_around = false;

    pAdxlStore->dumpAll();

	delete pAdxlStore; 
}

//Test function dumpAll mit full = false and allow_wrap_around = false
ZTEST(tests_adxl_store, test_adxl_store_dumpAll2)
{
	AdxlDataStore* pAdxlStore = new AdxlDataStore("adxlstore", 5, 4400, true);

	zassert_not_null(pAdxlStore, "memory alloc failed");
	zassert_not_null(pAdxlStore->pInMbox, "pInMbox = NULL");   
    zassert_equal(pAdxlStore->pInMbox->count(), 0, "pInMbox->count() != 0");
    zassert_equal(pAdxlStore->pInMbox, Mbox::getMboxById(Mbox::eAdxlStore),
                  "Error getting mailbox by id");

    pAdxlStore->full = false;
    pAdxlStore->capa = 30;
    pAdxlStore->wr = 24;
    //pAdxlStore->rd = 24;
    //pAdxlStore->bytes_per_sec = 6;
    pAdxlStore->allow_wrap_around = false;

    pAdxlStore->dumpAll();

	delete pAdxlStore; 
}

//Test function dumpAll mit full = false and allow_wrap_around = true
ZTEST(tests_adxl_store, test_adxl_store_dumpAll3)
{
	AdxlDataStore* pAdxlStore = new AdxlDataStore("adxlstore", 5, 4400, true);

	zassert_not_null(pAdxlStore, "memory alloc failed");
	zassert_not_null(pAdxlStore->pInMbox, "pInMbox = NULL");   
    zassert_equal(pAdxlStore->pInMbox->count(), 0, "pInMbox->count() != 0");
    zassert_equal(pAdxlStore->pInMbox, Mbox::getMboxById(Mbox::eAdxlStore),
                  "Error getting mailbox by id");

    pAdxlStore->full = true;
    pAdxlStore->capa = 30;
    pAdxlStore->wr = 6;
    //pAdxlStore->rd = 24;
    //pAdxlStore->bytes_per_sec = 6;
    pAdxlStore->allow_wrap_around = true;

    pAdxlStore->dumpAll();

	delete pAdxlStore; 
}

//Test function dumpAllBytes mit full = true
ZTEST(tests_adxl_store, test_adxl_store_dumpAllBytes)
{
	AdxlDataStore* pAdxlStore = new AdxlDataStore("adxlstore", 5, 4400, true);

	zassert_not_null(pAdxlStore, "memory alloc failed");
	zassert_not_null(pAdxlStore->pInMbox, "pInMbox = NULL");   
    zassert_equal(pAdxlStore->pInMbox->count(), 0, "pInMbox->count() != 0");
    zassert_equal(pAdxlStore->pInMbox, Mbox::getMboxById(Mbox::eAdxlStore),
                  "Error getting mailbox by id");

    pAdxlStore->full = true;
    pAdxlStore->capa = 30;
    pAdxlStore->wr = 24;
    //pAdxlStore->rd = 24;
    //pAdxlStore->bytes_per_sec = 6;
    pAdxlStore->allow_wrap_around = false;

    pAdxlStore->dumpAllBytes();

	delete pAdxlStore; 
}

//Test function dumpAllBytes mit full = false and allow_wrap_around = false
ZTEST(tests_adxl_store, test_adxl_store_dumpAllBytes2)
{
	AdxlDataStore* pAdxlStore = new AdxlDataStore("adxlstore", 5, 4400, true);

	zassert_not_null(pAdxlStore, "memory alloc failed");
	zassert_not_null(pAdxlStore->pInMbox, "pInMbox = NULL");   
    zassert_equal(pAdxlStore->pInMbox->count(), 0, "pInMbox->count() != 0");
    zassert_equal(pAdxlStore->pInMbox, Mbox::getMboxById(Mbox::eAdxlStore),
                  "Error getting mailbox by id");

    pAdxlStore->full = false;
    pAdxlStore->capa = 30;
    pAdxlStore->wr = 24;
    //pAdxlStore->rd = 24;
    //pAdxlStore->bytes_per_sec = 6;
    pAdxlStore->allow_wrap_around = false;

    pAdxlStore->dumpAllBytes();

	delete pAdxlStore; 
}

//Test function dumpAllBytes mit full = false and allow_wrap_around = true
ZTEST(tests_adxl_store, test_adxl_store_dumpAllBytes3)
{
	AdxlDataStore* pAdxlStore = new AdxlDataStore("adxlstore", 5, 4400, true);

	zassert_not_null(pAdxlStore, "memory alloc failed");
	zassert_not_null(pAdxlStore->pInMbox, "pInMbox = NULL");   
    zassert_equal(pAdxlStore->pInMbox->count(), 0, "pInMbox->count() != 0");
    zassert_equal(pAdxlStore->pInMbox, Mbox::getMboxById(Mbox::eAdxlStore),
                  "Error getting mailbox by id");

    pAdxlStore->full = true;
    pAdxlStore->capa = 30;
    pAdxlStore->wr = 6;
    //pAdxlStore->rd = 24;
    //pAdxlStore->bytes_per_sec = 6;
    pAdxlStore->allow_wrap_around = true;

    pAdxlStore->dumpAllBytes();

	delete pAdxlStore; 
}

//Test function dumpAllFilt mit full = false and allow_wrap_around = true
ZTEST(tests_adxl_store, test_adxl_store_dumpAllFilt)
{
	AdxlDataStore* pAdxlStore = new AdxlDataStore("adxlstore", 5, 4400, true);

	zassert_not_null(pAdxlStore, "memory alloc failed");
	zassert_not_null(pAdxlStore->pInMbox, "pInMbox = NULL");   
    zassert_equal(pAdxlStore->pInMbox->count(), 0, "pInMbox->count() != 0");
    zassert_equal(pAdxlStore->pInMbox, Mbox::getMboxById(Mbox::eAdxlStore),
                  "Error getting mailbox by id");

    pAdxlStore->num_samples = 5;

    pAdxlStore->dumpAllFilt();

	delete pAdxlStore; 
}

//Test function dumpAllDisp mit full = false and allow_wrap_around = true
ZTEST(tests_adxl_store, test_adxl_store_dumpAllDisp)
{
	AdxlDataStore* pAdxlStore = new AdxlDataStore("adxlstore", 5, 4400, true);

	zassert_not_null(pAdxlStore, "memory alloc failed");
	zassert_not_null(pAdxlStore->pInMbox, "pInMbox = NULL");   
    zassert_equal(pAdxlStore->pInMbox->count(), 0, "pInMbox->count() != 0");
    zassert_equal(pAdxlStore->pInMbox, Mbox::getMboxById(Mbox::eAdxlStore),
                  "Error getting mailbox by id");

    pAdxlStore->num_samples = 5;

    pAdxlStore->dumpAllDisp();

	delete pAdxlStore; 
}


//Test function shell_dump_bytes
ZTEST(tests_adxl_store, test_adxl_store_shell_dump_bytes)
{
    PoolElem* pElem;
    PoolElem* pElem2;

	Mbox* pAdxl37xMbox = new Mbox("Adxl37xMbox");
    Mbox::registerMbox(pAdxl37xMbox, Mbox::eAdxl37xTask);
	zassert_not_null(pAdxl37xMbox, "pAdxl37xMbox = NULL");

	AdxlDataStore* pAdxlStore = new AdxlDataStore("adxlstore", 5, 4400, true);

	zassert_not_null(pAdxlStore, "memory alloc failed");
	zassert_not_null(pAdxlStore->pInMbox, "pInMbox = NULL");   
    zassert_equal(pAdxlStore->pInMbox->count(), 0, "pInMbox->count() != 0");
    zassert_equal(pAdxlStore->pInMbox, Mbox::getMboxById(Mbox::eAdxlStore),
                  "Error getting mailbox by id");

    pAdxlStore->shell_dump_bytes(NULL, 1, NULL);
    pAdxlStore->task();  
    
    pElem = pAdxl37xMbox->read();
    zassert_equal(pElem->mtype, msgType::eAdxlStorageDumpActive, "Rcvd mtype != msgType::eAdxlStorageDumpActive");
    zassert_equal(pElem->from, Mbox::eAdxlStore, "Rcvd sender != Mbox::eAdxlStore");

    pElem2 = pAdxl37xMbox->read();
    zassert_equal(pElem2->mtype, msgType::eAdxlStorageDumpComplete, "Rcvd mtype != msgType::eAdxlStorageDumpComplete");
    zassert_equal(pElem2->from, Mbox::eAdxlStore, "Rcvd sender != Mbox::eAdxlStore");

    pElem->free();
    pElem2->free();
    delete pAdxl37xMbox;
	delete pAdxlStore; 
}

//Test function shell_dump_filt
ZTEST(tests_adxl_store, test_adxl_store_shell_dump_filt)
{
    PoolElem* pElem;
    PoolElem* pElem2;

	Mbox* pAdxl37xMbox = new Mbox("Adxl37xMbox");
    Mbox::registerMbox(pAdxl37xMbox, Mbox::eAdxl37xTask);
	zassert_not_null(pAdxl37xMbox, "pAdxl37xMbox = NULL");

	AdxlDataStore* pAdxlStore = new AdxlDataStore("adxlstore", 5, 4400, true);

	zassert_not_null(pAdxlStore, "memory alloc failed");
	zassert_not_null(pAdxlStore->pInMbox, "pInMbox = NULL");   
    zassert_equal(pAdxlStore->pInMbox->count(), 0, "pInMbox->count() != 0");
    zassert_equal(pAdxlStore->pInMbox, Mbox::getMboxById(Mbox::eAdxlStore),
                  "Error getting mailbox by id");

    pAdxlStore->shell_dump_filt(NULL, 1, NULL);
    pAdxlStore->task();  
    
    pElem = pAdxl37xMbox->read();
    zassert_equal(pElem->mtype, msgType::eAdxlStorageDumpActive, "Rcvd mtype != msgType::eAdxlStorageDumpActive");
    zassert_equal(pElem->from, Mbox::eAdxlStore, "Rcvd sender != Mbox::eAdxlStore");

    pElem2 = pAdxl37xMbox->read();
    zassert_equal(pElem2->mtype, msgType::eAdxlStorageDumpComplete, "Rcvd mtype != msgType::eAdxlStorageDumpComplete");
    zassert_equal(pElem2->from, Mbox::eAdxlStore, "Rcvd sender != Mbox::eAdxlStore");

    pElem->free();
    pElem2->free();
    delete pAdxl37xMbox;
	delete pAdxlStore; 
}

//Test function shell_dump_disp
ZTEST(tests_adxl_store, test_adxl_store_shell_dump_disp)
{
    PoolElem* pElem;
    PoolElem* pElem2;

	Mbox* pAdxl37xMbox = new Mbox("Adxl37xMbox");
    Mbox::registerMbox(pAdxl37xMbox, Mbox::eAdxl37xTask);
	zassert_not_null(pAdxl37xMbox, "pAdxl37xMbox = NULL");

	AdxlDataStore* pAdxlStore = new AdxlDataStore("adxlstore", 5, 4400, true);

	zassert_not_null(pAdxlStore, "memory alloc failed");
	zassert_not_null(pAdxlStore->pInMbox, "pInMbox = NULL");   
    zassert_equal(pAdxlStore->pInMbox->count(), 0, "pInMbox->count() != 0");
    zassert_equal(pAdxlStore->pInMbox, Mbox::getMboxById(Mbox::eAdxlStore),
                  "Error getting mailbox by id");

    pAdxlStore->shell_dump_disp(NULL, 1, NULL);
    pAdxlStore->task();  
    
    pElem = pAdxl37xMbox->read();
    zassert_equal(pElem->mtype, msgType::eAdxlStorageDumpActive, "Rcvd mtype != msgType::eAdxlStorageDumpActive");
    zassert_equal(pElem->from, Mbox::eAdxlStore, "Rcvd sender != Mbox::eAdxlStore");

    pElem2 = pAdxl37xMbox->read();
    zassert_equal(pElem2->mtype, msgType::eAdxlStorageDumpComplete, "Rcvd mtype != msgType::eAdxlStorageDumpComplete");
    zassert_equal(pElem2->from, Mbox::eAdxlStore, "Rcvd sender != Mbox::eAdxlStore");

    pElem->free();
    pElem2->free();
    delete pAdxl37xMbox;
	delete pAdxlStore; 
}

//Test function configure
ZTEST(tests_adxl_store, test_adxl_store_configure)
{
	AdxlDataStore* pAdxlStore = new AdxlDataStore("adxlstore", 5, 4400, true);

	zassert_not_null(pAdxlStore, "memory alloc failed");
	zassert_not_null(pAdxlStore->pInMbox, "pInMbox = NULL");   
    zassert_equal(pAdxlStore->pInMbox->count(), 0, "pInMbox->count() != 0");
    zassert_equal(pAdxlStore->pInMbox, Mbox::getMboxById(Mbox::eAdxlStore),
                  "Error getting mailbox by id");

    gbRetVal = true;
    pAdxlStore->configure();  
    
	delete pAdxlStore; 
}
ZTEST(tests_adxl_store, test_adxl_store_configure2)
{
	AdxlDataStore* pAdxlStore = new AdxlDataStore("adxlstore", 5, 4400, true);

	zassert_not_null(pAdxlStore, "memory alloc failed");
	zassert_not_null(pAdxlStore->pInMbox, "pInMbox = NULL");   
    zassert_equal(pAdxlStore->pInMbox->count(), 0, "pInMbox->count() != 0");
    zassert_equal(pAdxlStore->pInMbox, Mbox::getMboxById(Mbox::eAdxlStore),
                  "Error getting mailbox by id");

    gbRetVal = false;
    pAdxlStore->configure();  
    
	delete pAdxlStore; 
}

#define USE_TEST_DATA
// Test function storeSampleData with full = false
ZTEST(tests_adxl_store, test_adxl_store_storeSampleData)
{
	AdxlDataStore* pAdxlStore = new AdxlDataStore("adxlstore", 5, 4400, true);

	zassert_not_null(pAdxlStore, "memory alloc failed");
	zassert_not_null(pAdxlStore->pInMbox, "pInMbox = NULL");   
    zassert_equal(pAdxlStore->pInMbox->count(), 0, "pInMbox->count() != 0");
    zassert_equal(pAdxlStore->pInMbox, Mbox::getMboxById(Mbox::eAdxlStore),
                  "Error getting mailbox by id");

    pAdxlStore->full = false;

    pAdxlStore->storeSampleData();

	delete pAdxlStore; 
}

// Test function storeSampleData with full = true
ZTEST(tests_adxl_store, test_adxl_store_storeSampleData3)
{
	AdxlDataStore* pAdxlStore = new AdxlDataStore("adxlstore", 5, 4400, true);

	zassert_not_null(pAdxlStore, "memory alloc failed");
	zassert_not_null(pAdxlStore->pInMbox, "pInMbox = NULL");   
    zassert_equal(pAdxlStore->pInMbox->count(), 0, "pInMbox->count() != 0");
    zassert_equal(pAdxlStore->pInMbox, Mbox::getMboxById(Mbox::eAdxlStore),
                  "Error getting mailbox by id");

    pAdxlStore->full = true;

    pAdxlStore->storeSampleData();

	delete pAdxlStore; 
}
#undef USE_TEST_DATA
