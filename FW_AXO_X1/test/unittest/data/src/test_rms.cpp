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

#include "drv/spi/adxl37x.h"

#include "data/hit_detect.h"
#include "data/rms_store.h"
#include "data/rms.h"

#include "stubs.h"

ZTEST_SUITE(tests_rms, NULL, NULL, NULL, NULL, NULL);

// test constructor and destructor
ZTEST(tests_rms, test_rms_constructor)
{
	RmsTask* pRmsTask = new RmsTask("rmstask", 1, 4400);

	zassert_not_null(pRmsTask, "memory alloc failed");
	zassert_not_null(pRmsTask->pInMbox, "pInMbox = NULL");
    zassert_not_null(pRmsTask->myTimer, "myTimer = NULL");
    zassert_not_null(pRmsTask->myDetectorMed, "myDetectorMed = NULL");
    zassert_not_null(pRmsTask->myDetectorCrit, "myDetectorCrit = NULL");
    
    zassert_equal(pRmsTask->pInMbox->count(), 0, "pInMbox->count() != 0");
    zassert_equal(pRmsTask->pInMbox, Mbox::getMboxById(Mbox::eAdxlRms),
                  "Error getting mailbox by id");

    zassert_equal(pRmsTask->samples_per_sec, 4400, "samples_per_sec != 4400");
    zassert_equal(pRmsTask->n_seconds, 1, "n_seconds != 1");
    zassert_equal(pRmsTask->n_sample, 0, "n_sample != 0");
    zassert_equal(pRmsTask->x_square_sum, 0.0, "x_square_sum != 0.0");
    zassert_equal(pRmsTask->y_square_sum, 0.0, "y_square_sum != 0.0");
    zassert_equal(pRmsTask->z_square_sum, 0.0, "z_square_sum != 0.0");
    zassert_equal(pRmsTask->idle, true, "idle != true");
    zassert_equal(pRmsTask->rx_cnt, 0, "rx_cnt != 0");
    zassert_equal(pRmsTask->rx_cnt_prev, 0, "rx_cnt_prev != 0");

	delete pRmsTask; 
}

// MISSING for 100% branch coverage static instantiation does not deliver NULL
// here code line 
// pInMbox = new Mbox(name); 
// myTimer = new Timer(RmsTask::timer_cb, this, 1, 1);
// myDetectorMed = new HitDetector(samples_per_sec, 300, 8);
// myDetectorCrit = new HitDetector(samples_per_sec, 800, 2);
// static PoolElem e1 = {eTimerExpired, Mbox::MboxId::eAdxlRms, 'r', 'm'};
// are not covered completly.

// test function timer_cb and timer1_expired
ZTEST(tests_rms, test_rms_timer_cb)
{
    PoolElem* pElem;

    RmsTask* pRmsTask = new RmsTask("rmstask", 1, 4400);
	zassert_not_null(pRmsTask, "memory alloc failed");
	zassert_not_null(pRmsTask->pInMbox, "pInMbox = NULL");
    zassert_equal(pRmsTask->pInMbox->count(), 0, "pInMbox->count() != 0");
    zassert_equal(pRmsTask->pInMbox, Mbox::getMboxById(Mbox::eAdxlRms),
                  "Error getting mailbox by id");

    pRmsTask->timer_cb(reinterpret_cast<k_timer*>(pRmsTask));

    zassert_equal(pRmsTask->pInMbox->count(), 1, "pInMbox->count() != 1");
    pElem = pRmsTask->pInMbox->read();
    zassert_equal(pElem->mtype, msgType::eTimerExpired, "Rcvd mtype != msgType::eTimerExpired");

    delete pRmsTask;
}    

// test function init
ZTEST(tests_rms, test_rms_init)
{
    RmsTask* pRmsTask = new RmsTask("rmstask", 1, 4400);

    pRmsTask->n_sample = 7;
    pRmsTask->x_square_sum = 3.0;
    pRmsTask->y_square_sum = 3.0;
    pRmsTask->z_square_sum = 3.0;
    zassert_equal(pRmsTask->n_sample, 7, "n_sample != 7");
    zassert_equal(pRmsTask->x_square_sum, 3.0, "x_square_sum != 3.0");
    zassert_equal(pRmsTask->y_square_sum, 3.0, "y_square_sum != 3.0");
    zassert_equal(pRmsTask->z_square_sum, 3.0, "z_square_sum != 3.0");

    pRmsTask->myDetectorMed->n_cur = 3;
    pRmsTask->myDetectorMed->n_high = 3;
    pRmsTask->myDetectorMed->hit_cnt = 3;
    pRmsTask->myDetectorCrit->n_cur = 3;
    pRmsTask->myDetectorCrit->n_high = 3;
    pRmsTask->myDetectorCrit->hit_cnt = 3;
    zassert_equal(pRmsTask->myDetectorMed->n_cur, 3, "myDetectorMed->n_cur != 3");
    zassert_equal(pRmsTask->myDetectorMed->n_high, 3, "myDetectorMed->n_high != 3");
    zassert_equal(pRmsTask->myDetectorMed->hit_cnt, 3, "myDetectorMed->hit_cnt != 3");
    zassert_equal(pRmsTask->myDetectorCrit->n_cur, 3, "myDetectorCrit->n_cur != 3");
    zassert_equal(pRmsTask->myDetectorCrit->n_high, 3, "myDetectorCrit->n_high != 3");
    zassert_equal(pRmsTask->myDetectorCrit->hit_cnt, 3, "myDetectorCrit->hit_cnt != 3");

    pRmsTask->init();

    zassert_equal(pRmsTask->n_sample, 0, "n_sample != 0");
    zassert_equal(pRmsTask->x_square_sum, 0.0, "x_square_sum != 0.0");
    zassert_equal(pRmsTask->y_square_sum, 0.0, "y_square_sum != 0.0");
    zassert_equal(pRmsTask->z_square_sum, 0.0, "z_square_sum != 0.0");
    zassert_equal(pRmsTask->myDetectorMed->n_cur, 0, "myDetectorMed->n_cur != 0");
    zassert_equal(pRmsTask->myDetectorMed->n_high, 0, "myDetectorMed->n_high != 0");
    zassert_equal(pRmsTask->myDetectorMed->hit_cnt, 0, "myDetectorMed->hit_cnt != 0");
    zassert_equal(pRmsTask->myDetectorCrit->n_cur, 0, "myDetectorCrit->n_cur != 0");
    zassert_equal(pRmsTask->myDetectorCrit->n_high, 0, "myDetectorCrit->n_high != 0");
    zassert_equal(pRmsTask->myDetectorCrit->hit_cnt, 0, "myDetectorCrit->hit_cnt != 0");
	delete pRmsTask; 
}    

//Test receive msg eAdxl37xSensorData with idle = true
ZTEST(tests_rms, test_rms_rcv_msg_eAdxl37xSensorData)
{
    PoolElem* pElem;

	Mbox* pPowerMgrMbox = new Mbox("PowerMgrMbox");
    Mbox::registerMbox(pPowerMgrMbox, Mbox::ePowerManager);
	zassert_not_null(pPowerMgrMbox, "pPowerMgrMbox = NULL");

    RmsTask* pRmsTask = new RmsTask("rmstask", 1, 4400);
	zassert_not_null(pRmsTask, "memory alloc failed");
	zassert_not_null(pRmsTask->pInMbox, "pInMbox = NULL");
    zassert_equal(pRmsTask->pInMbox->count(), 0, "pInMbox->count() != 0");
    zassert_equal(pRmsTask->pInMbox, Mbox::getMboxById(Mbox::eAdxlRms),
                  "Error getting mailbox by id");

    pRmsTask->idle = true;

    /*  struct adxl372_xyz_accel_data {
        int16_t x;  //!< measured x value of the acceleration data
        int16_t y;  //!< measured y value of the acceleration data
        int16_t z;  //!< measured z value of the acceleration data
    }; */
    struct adxl372_xyz_accel_data xyz = { 7, 8, 9 };
    /*struct adxl372_xyz_accel_data* samples;   //!< x, y, z axis measured acceleration data
        uint16_t cnt;  //!< The sample counter
        time_t ts;     //!< The timestamp of the measurement
    };*/
    struct adxl37x_sensor_value val = { &xyz, 15, 12345678 };

    Mbox::sendMsg(msgType::eAdxl37xSensorData, Mbox::eAdxlRms, Mbox::eAdxlRms, (uint8_t*)&val, sizeof(val));

    zassert_equal(pRmsTask->pInMbox->count(), 1, "pInMbox->count() != 1");

    pRmsTask->task();  

    zassert_equal(pRmsTask->pInMbox->count(), 0, "pInMbox->count() != 0");
    zassert_equal(pPowerMgrMbox->count(), 1, "pPowerMgrMbox->count() != 1");

    pElem = pPowerMgrMbox->read();
    zassert_equal(pElem->mtype, msgType::eAdxl37xWake, "Rcvd mtype != msgType::eAdxl37xWake");
    zassert_equal(pElem->from, Mbox::eAdxlRms, "Rcvd sender != Mbox::eAdxlRms");

    pElem->free();
    delete pPowerMgrMbox;
	delete pRmsTask; 
}

//Test receive msg eAdxl37xSensorData with idle = false
ZTEST(tests_rms, test_rms_rcv_msg_eAdxl37xSensorData_2)
{
	Mbox* pPowerMgrMbox = new Mbox("PowerMgrMbox");
    Mbox::registerMbox(pPowerMgrMbox, Mbox::ePowerManager);
	zassert_not_null(pPowerMgrMbox, "pPowerMgrMbox = NULL");

    RmsTask* pRmsTask = new RmsTask("rmstask", 1, 4400);
	zassert_not_null(pRmsTask, "memory alloc failed");
	zassert_not_null(pRmsTask->pInMbox, "pInMbox = NULL");
    zassert_equal(pRmsTask->pInMbox->count(), 0, "pInMbox->count() != 0");
    zassert_equal(pRmsTask->pInMbox, Mbox::getMboxById(Mbox::eAdxlRms),
                  "Error getting mailbox by id");

    pRmsTask->idle = false;

    /*  struct adxl372_xyz_accel_data {
        int16_t x;  //!< measured x value of the acceleration data
        int16_t y;  //!< measured y value of the acceleration data
        int16_t z;  //!< measured z value of the acceleration data
    }; */
    struct adxl372_xyz_accel_data xyz = { 7, 8, 9 };
    /*struct adxl372_xyz_accel_data* samples;   //!< x, y, z axis measured acceleration data
        uint16_t cnt;  //!< The sample counter
        time_t ts;     //!< The timestamp of the measurement
    };*/
    struct adxl37x_sensor_value val = { &xyz, 15, 12345678 };

    Mbox::sendMsg(msgType::eAdxl37xSensorData, Mbox::eAdxlRms, Mbox::eAdxlRms, (uint8_t*)&val, sizeof(val));

    zassert_equal(pRmsTask->pInMbox->count(), 1, "pInMbox->count() != 1");

    pRmsTask->task();  

    zassert_equal(pRmsTask->pInMbox->count(), 0, "pInMbox->count() != 0");

    delete pPowerMgrMbox;
	delete pRmsTask; 
}

//Test receive msg eAdxl37xSensorData with idle = true 
ZTEST(tests_rms, test_rms_rcv_msg_eAdxl37xSensorData_3)
{
    PoolElem* pElem;

	Mbox* pRmsStoreMbox = new Mbox("RmsStoreMbox");
    Mbox::registerMbox(pRmsStoreMbox, Mbox::eRmsStore);
	zassert_not_null(pRmsStoreMbox, "pRmsStoreMbox = NULL");

    RmsTask* pRmsTask = new RmsTask("rmstask", 1, 20);
	zassert_not_null(pRmsTask, "memory alloc failed");
	zassert_not_null(pRmsTask->pInMbox, "pInMbox = NULL");
    zassert_equal(pRmsTask->pInMbox->count(), 0, "pInMbox->count() != 0");
    zassert_equal(pRmsTask->pInMbox, Mbox::getMboxById(Mbox::eAdxlRms),
                  "Error getting mailbox by id");

    pRmsTask->idle = true;
    //pRmsTask->n_sample = 18;
    pRmsTask->x_square_sum = 20;
    pRmsTask->y_square_sum = 20;
    pRmsTask->z_square_sum = 20;

    /*  struct adxl372_xyz_accel_data {
        int16_t x;  //!< measured x value of the acceleration data
        int16_t y;  //!< measured y value of the acceleration data
        int16_t z;  //!< measured z value of the acceleration data
    }; */
    struct adxl372_xyz_accel_data xyz[3] = { {7, 8, 9}, {7, 8, 9}, {7, 8, 9} };
    /*struct adxl372_xyz_accel_data* samples;   //!< x, y, z axis measured acceleration data
        uint16_t cnt;  //!< The sample counter
        time_t ts;     //!< The timestamp of the measurement
    };*/
    struct adxl37x_sensor_value val = { &xyz[0], 5, 12345678 };

    Mbox::sendMsg(msgType::eAdxl37xSensorData, Mbox::eAdxlRms, Mbox::eAdxlRms, (uint8_t*)&val, sizeof(val));

    zassert_equal(pRmsTask->pInMbox->count(), 1, "pInMbox->count() != 1");

    pRmsTask->task();  

    zassert_equal(pRmsTask->pInMbox->count(), 0, "pInMbox->count() != 0");
    zassert_true(pRmsStoreMbox->count() >= 1, "pRmsStoreMbox->count() == %d", pRmsStoreMbox->count());
    //printk("pRmsStoreMbox->count() = %d\n", pRmsStoreMbox->count());

    pElem = pRmsStoreMbox->read();
    zassert_equal(pElem->mtype, msgType::eAdxl37xRmsData, "Rcvd mtype != msgType::eAdxl37xRmsData");
    zassert_equal(pElem->from, Mbox::eAdxlRms, "Rcvd sender != Mbox::eAdxlRms");

    struct adxl_rms_data pData = *(struct adxl_rms_data*)pElem->getBuffer();
    zassert_true(pData.x >= 1.0, "error pData.x");
    zassert_true(pData.y >= 1.0, "error pData.y");
    zassert_true(pData.z >= 1.0, "error pData.z");
    zassert_equal(pData.medium_hits, 0, "pData.medium_hits != 0");
    zassert_equal(pData.critical_hits, 0, "pData.critical_hits != 0");
    /*printk("ts = %d\n", (unsigned int)pData.ts);
    printk("ts = %f\n", pData.x);
    printk("ts = %f\n", pData.y);
    printk("ts = %f\n", pData.z);
    printk("ts = %d\n", (unsigned int)pData.medium_hits);
    printk("ts = %d\n", (unsigned int)pData.critical_hits);*/
    
    pElem->free();
    delete pRmsStoreMbox;
	delete pRmsTask; 
}

//Test receive msg eAdxl37xSensorData with idle = false and ++n_sample == samples_per_sec * n_seconds
ZTEST(tests_rms, test_rms_rcv_msg_eAdxl37xSensorData_4)
{
    PoolElem* pElem;

	Mbox* pRmsStoreMbox = new Mbox("RmsStoreMbox");
    Mbox::registerMbox(pRmsStoreMbox, Mbox::eRmsStore);
	zassert_not_null(pRmsStoreMbox, "pRmsStoreMbox = NULL");

    RmsTask* pRmsTask = new RmsTask("rmstask", 1, 20);
	zassert_not_null(pRmsTask, "memory alloc failed");
	zassert_not_null(pRmsTask->pInMbox, "pInMbox = NULL");
    zassert_equal(pRmsTask->pInMbox->count(), 0, "pInMbox->count() != 0");
    zassert_equal(pRmsTask->pInMbox, Mbox::getMboxById(Mbox::eAdxlRms),
                  "Error getting mailbox by id");

    pRmsTask->idle = false;
    pRmsTask->n_sample = 18;
    pRmsTask->x_square_sum = 20;
    pRmsTask->y_square_sum = 20;
    pRmsTask->z_square_sum = 20;

    /*  struct adxl372_xyz_accel_data {
        int16_t x;  //!< measured x value of the acceleration data
        int16_t y;  //!< measured y value of the acceleration data
        int16_t z;  //!< measured z value of the acceleration data
    }; */
    struct adxl372_xyz_accel_data xyz[3] = { {7, 8, 9}, {7, 8, 9}, {7, 8, 9} };
    /*struct adxl372_xyz_accel_data* samples;   //!< x, y, z axis measured acceleration data
        uint16_t cnt;  //!< The sample counter
        time_t ts;     //!< The timestamp of the measurement
    };*/
    struct adxl37x_sensor_value val = { &xyz[0], 5, 12345678 };

    Mbox::sendMsg(msgType::eAdxl37xSensorData, Mbox::eAdxlRms, Mbox::eAdxlRms, (uint8_t*)&val, sizeof(val));

    zassert_equal(pRmsTask->pInMbox->count(), 1, "pInMbox->count() != 1");

    pRmsTask->task();  

    zassert_equal(pRmsTask->pInMbox->count(), 0, "pInMbox->count() != 0");
    zassert_true(pRmsStoreMbox->count() >= 1, "pRmsStoreMbox->count() == %d", pRmsStoreMbox->count());
    //printk("pRmsStoreMbox->count() = %d\n", pRmsStoreMbox->count());

    pElem = pRmsStoreMbox->read();
    zassert_equal(pElem->mtype, msgType::eAdxl37xRmsData, "Rcvd mtype != msgType::eAdxl37xRmsData");
    zassert_equal(pElem->from, Mbox::eAdxlRms, "Rcvd sender != Mbox::eAdxlRms");

    struct adxl_rms_data pData = *(struct adxl_rms_data*)pElem->getBuffer();
    zassert_true(pData.x >= 1.0, "error pData.x");
    zassert_true(pData.y >= 1.0, "error pData.y");
    zassert_true(pData.z >= 1.0, "error pData.z");
    zassert_equal(pData.medium_hits, 0, "pData.medium_hits != 0");
    zassert_equal(pData.critical_hits, 0, "pData.critical_hits != 0");
    /*printk("ts = %d\n", (unsigned int)pData.ts);
    printk("ts = %f\n", pData.x);
    printk("ts = %f\n", pData.y);
    printk("ts = %f\n", pData.z);
    printk("ts = %d\n", (unsigned int)pData.medium_hits);
    printk("ts = %d\n", (unsigned int)pData.critical_hits);*/
    
    pElem->free();
    delete pRmsStoreMbox;
	delete pRmsTask; 
}


//Test receive msg eTimerExpired with idle = false and rx_cnt == rx_cnt_prev
ZTEST(tests_rms, test_rms_rcv_msg_eTimerExpired)
{
    PoolElem* pElem;

	Mbox* pAdxl37xMbox = new Mbox("Adxl37xMbox");
    Mbox::registerMbox(pAdxl37xMbox, Mbox::eAdxl37xTask);
	zassert_not_null(pAdxl37xMbox, "pPowerMgrMbox = NULL");

    RmsTask* pRmsTask = new RmsTask("rmstask", 1, 4400);
	zassert_not_null(pRmsTask, "memory alloc failed");
	zassert_not_null(pRmsTask->pInMbox, "pInMbox = NULL");
    zassert_equal(pRmsTask->pInMbox->count(), 0, "pInMbox->count() != 0");
    zassert_equal(pRmsTask->pInMbox, Mbox::getMboxById(Mbox::eAdxlRms),
                  "Error getting mailbox by id");

    pRmsTask->idle = false;
    pRmsTask->rx_cnt = 10;
    pRmsTask->rx_cnt_prev = 10;

    Mbox::sendMsg(msgType::eTimerExpired, Mbox::eAdxlRms, Mbox::eAdxlRms);

    zassert_equal(pRmsTask->pInMbox->count(), 1, "pInMbox->count() != 1");

    pRmsTask->task();  

    zassert_equal(pRmsTask->idle, true, "pRmsTask->idle != true");
    zassert_equal(pRmsTask->pInMbox->count(), 0, "pInMbox->count() != 0");
    zassert_equal(pAdxl37xMbox->count(), 1, "pAdxl37xMbox->count() != 1");

    pElem = pAdxl37xMbox->read();
    zassert_equal(pElem->mtype, msgType::eAdxl37xIdle, "Rcvd mtype != msgType::eAdxl37xIdle");
    zassert_equal(pElem->from, Mbox::eAdxlRms, "Rcvd sender != Mbox::eAdxlRms");

    pElem->free();
    delete pAdxl37xMbox;
	delete pRmsTask; 
}

//Test receive msg eTimerExpired with idle = true and rx_cnt == rx_cnt_prev
ZTEST(tests_rms, test_rms_rcv_msg_eTimerExpired_2)
{
    RmsTask* pRmsTask = new RmsTask("rmstask", 1, 4400);
	zassert_not_null(pRmsTask, "memory alloc failed");
	zassert_not_null(pRmsTask->pInMbox, "pInMbox = NULL");
    zassert_equal(pRmsTask->pInMbox->count(), 0, "pInMbox->count() != 0");
    zassert_equal(pRmsTask->pInMbox, Mbox::getMboxById(Mbox::eAdxlRms),
                  "Error getting mailbox by id");

    pRmsTask->idle = true;
    pRmsTask->rx_cnt = 10;
    pRmsTask->rx_cnt_prev = 10;

    Mbox::sendMsg(msgType::eTimerExpired, Mbox::eAdxlRms, Mbox::eAdxlRms);

    zassert_equal(pRmsTask->pInMbox->count(), 1, "pInMbox->count() != 1");

    pRmsTask->task();  

    zassert_equal(pRmsTask->idle, true, "pRmsTask->idle != true");
    zassert_equal(pRmsTask->rx_cnt, 10, "pRmsTask->rx_cnt != 10");
    zassert_equal(pRmsTask->rx_cnt_prev, 10, "pRmsTask->rx_cnt_prev != 10");
    zassert_equal(pRmsTask->pInMbox->count(), 0, "pInMbox->count() != 0");

	delete pRmsTask; 
}

//Test receive msg eTimerExpired with idle = false and rx_cnt != rx_cnt_prev
ZTEST(tests_rms, test_rms_rcv_msg_eTimerExpired_3)
{
    RmsTask* pRmsTask = new RmsTask("rmstask", 1, 4400);
	zassert_not_null(pRmsTask, "memory alloc failed");
	zassert_not_null(pRmsTask->pInMbox, "pInMbox = NULL");
    zassert_equal(pRmsTask->pInMbox->count(), 0, "pInMbox->count() != 0");
    zassert_equal(pRmsTask->pInMbox, Mbox::getMboxById(Mbox::eAdxlRms),
                  "Error getting mailbox by id");

    pRmsTask->idle = false;
    pRmsTask->rx_cnt = 10;
    pRmsTask->rx_cnt_prev = 9;

    Mbox::sendMsg(msgType::eTimerExpired, Mbox::eAdxlRms, Mbox::eAdxlRms);

    zassert_equal(pRmsTask->pInMbox->count(), 1, "pInMbox->count() != 1");

    pRmsTask->task();  

    zassert_equal(pRmsTask->idle, false, "pRmsTask->idle != false");
    zassert_equal(pRmsTask->rx_cnt, 10, "pRmsTask->rx_cnt != 10");
    zassert_equal(pRmsTask->rx_cnt_prev, 10, "pRmsTask->rx_cnt_prev != 10");
    zassert_equal(pRmsTask->pInMbox->count(), 0, "pInMbox->count() != 0");

	delete pRmsTask; 
}

//Test receive msg eFwUpdateActive
ZTEST(tests_rms, test_rms_rcv_msg_eFwUpdateActive)
{
    RmsTask* pRmsTask = new RmsTask("rmstask", 1, 4400);
	zassert_not_null(pRmsTask, "memory alloc failed");
	zassert_not_null(pRmsTask->pInMbox, "pInMbox = NULL");
    zassert_equal(pRmsTask->pInMbox->count(), 0, "pInMbox->count() != 0");
    zassert_equal(pRmsTask->pInMbox, Mbox::getMboxById(Mbox::eAdxlRms),
                  "Error getting mailbox by id");

    pRmsTask->idle = false;
    pRmsTask->rx_cnt = 10;
    pRmsTask->rx_cnt_prev = 9;

    Mbox::sendMsg(msgType::eFwUpdateActive, Mbox::eAdxlRms, Mbox::eAdxlRms);

    zassert_equal(pRmsTask->pInMbox->count(), 1, "pInMbox->count() != 1");

    pRmsTask->task();  

    zassert_equal(pRmsTask->pInMbox->count(), 0, "pInMbox->count() != 0");

	delete pRmsTask; 
}

//Test receive unexpected msg 
ZTEST(tests_rms, test_rms_rcv_msg_unexpected)
{
    RmsTask* pRmsTask = new RmsTask("rmstask", 1, 4400);
	zassert_not_null(pRmsTask, "memory alloc failed");
	zassert_not_null(pRmsTask->pInMbox, "pInMbox = NULL");
    zassert_equal(pRmsTask->pInMbox->count(), 0, "pInMbox->count() != 0");
    zassert_equal(pRmsTask->pInMbox, Mbox::getMboxById(Mbox::eAdxlRms),
                  "Error getting mailbox by id");

    Mbox::sendMsg(msgType::eResetCause, Mbox::eAdxlRms, Mbox::eAdxlRms);

    zassert_equal(pRmsTask->pInMbox->count(), 1, "pInMbox->count() != 1");

    pRmsTask->task();  

    zassert_equal(pRmsTask->pInMbox->count(), 0, "pInMbox->count() != 0");
	delete pRmsTask; 
}

//Test receive no msg 
ZTEST(tests_rms, test_rms_rcv_msg_none)
{
    RmsTask* pRmsTask = new RmsTask("rmstask", 1, 4400);
	zassert_not_null(pRmsTask, "memory alloc failed");
	zassert_not_null(pRmsTask->pInMbox, "pInMbox = NULL");
    zassert_equal(pRmsTask->pInMbox->count(), 0, "pInMbox->count() != 0");
    zassert_equal(pRmsTask->pInMbox, Mbox::getMboxById(Mbox::eAdxlRms),
                  "Error getting mailbox by id");


    pRmsTask->pInMbox->write(NULL);
    
    pRmsTask->task();  

    zassert_equal(pRmsTask->pInMbox->count(), 0, "pInMbox->count() != 0");
	delete pRmsTask; 
}


