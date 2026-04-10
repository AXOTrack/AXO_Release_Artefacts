#include <zephyr/ztest.h>
#include <zephyr/kernel.h>
#include <vector>
#include <algorithm>

#include "rtos/mbox.h"
#include "rtos/mpelem.h"
#include "rtos/mpool.h"
#include "rtos/mqueue.h"
#include "rtos/task.h"
#include "data/rms_store.h"
#include "data/adxl_store.h"
#include "disp/disptask.h"

#define private public
#include "data/measurement.h"

ZTEST_SUITE(tests_measurement, NULL, NULL, NULL, NULL, NULL);

// test constructor and destructor, including functions reset() and setFloatStrings()
ZTEST(tests_measurement, test_measurement_constructor)
{
	MeasurementBuilder* pMeasurementBuilder = new MeasurementBuilder(true);

	zassert_not_null(pMeasurementBuilder, "memory alloc failed");
    zassert_equal(pMeasurementBuilder->med_hit_cnt.size(), 0, "med_hit_cnt.size != 0");
    zassert_equal(pMeasurementBuilder->crit_hit_cnt.size(), 0, "crit_hit_cnt.size != 0");
    zassert_equal(pMeasurementBuilder->med_hit_cnt.capacity(), 20, "med_hit_cnt.max_size != 20");
    zassert_equal(pMeasurementBuilder->crit_hit_cnt.capacity(), 20, "crit_hit_cnt.max_size != 20");

    zassert_equal(pMeasurementBuilder->max_rms_z, 0, "max_rms_z != 0");
    zassert(strcmp(pMeasurementBuilder->payload.c_str(), "{\"time_interval\":1000")==0, 
                   "payload %s not correct", pMeasurementBuilder->payload.c_str());
    zassert(strcmp(pMeasurementBuilder->timestamp.c_str(), "")==0, "timestamp not empty");
    zassert(strcmp(pMeasurementBuilder->sDispMin.c_str(), "-99999.0")==0, "sDispMin not -99999.0");
    zassert(strcmp(pMeasurementBuilder->sDispMax.c_str(), "-99999.0")==0, "sDispMax not -99999.0");
    zassert(strcmp(pMeasurementBuilder->sDispP2P.c_str(), "-99999.0")==0, "sDispP2P not -99999.0");
    zassert(strcmp(pMeasurementBuilder->sMaxX.c_str(), "-99999.0")==0, "sMaxX not -99999.0");
    zassert(strcmp(pMeasurementBuilder->sMaxY.c_str(), "-99999.0")==0, "sMaxY not -99999.0");
    zassert(strcmp(pMeasurementBuilder->sMaxZ.c_str(), "-99999.0")==0, "sMaxZ not -99999.0");

	delete pMeasurementBuilder; 
}

// MISSING for 100% branch coverage static instantiation does not deliver NULL
// here code line 
// are not covered completly.

// test function isComplete()
ZTEST(tests_measurement, test_measurement_isComplete)
{
    bool bRet;
    char buf[32];

 	MeasurementBuilder* pMeasurementBuilder = new MeasurementBuilder(true);

    zassert_equal(pMeasurementBuilder->max_rms_z, 0, "max_rms_z != 0");
    zassert_true(pMeasurementBuilder->payload.size() < 30, "error payload.size() < 30");

    pMeasurementBuilder->max_rms_z = 5.0;
    
    // if (disp_mode_off || (max_rms_z <= rmsZThres)) && if (payload.size() > 30)
    // true / false / false
    bool disp_mode_off = true;
    double rmsZThres = 4.0;
    bRet = pMeasurementBuilder->isComplete(disp_mode_off, rmsZThres);
    zassert_false(bRet, "bRet not false");

    // false / false / false
    disp_mode_off = false;
    rmsZThres = 6.0;
    bRet = pMeasurementBuilder->isComplete(disp_mode_off, rmsZThres);
    zassert_false(bRet, "bRet not false");

    // false / true / true
    pMeasurementBuilder->payload.append(",\"timestamp\":" + std::to_string(12345678));
    pMeasurementBuilder->payload.append(",\"rms_value\":[");
    snprintf(buf, sizeof(buf), "[7.1,17.2,27.3]");
    pMeasurementBuilder->payload.append(buf);
    bRet = pMeasurementBuilder->isComplete(disp_mode_off, rmsZThres);
    zassert_true(bRet, "bRet not true");
    zassert_true(pMeasurementBuilder->payload.size() > 30, "error payload.size() > 30");

    // true / true / true
    disp_mode_off = true;
    bRet = pMeasurementBuilder->isComplete(disp_mode_off, rmsZThres);
    zassert_true(bRet, "bRet not true");

    // else
    disp_mode_off = false;
    rmsZThres = 4.0;
    pMeasurementBuilder->sDispP2P = "-99999.0";
    bRet = pMeasurementBuilder->isComplete(disp_mode_off, rmsZThres);
    zassert_false(bRet, "bRet not false");
    zassert_true(pMeasurementBuilder->payload.size() > 30, "error payload.size() > 30");

    pMeasurementBuilder->sDispP2P = "66.7";
    bRet = pMeasurementBuilder->isComplete(disp_mode_off, rmsZThres);
    zassert_true(bRet, "bRet not true");
    zassert_true(pMeasurementBuilder->payload.size() > 30, "error payload.size() > 30");

    pMeasurementBuilder->payload = "{\"time_interval\":1000";
    bRet = pMeasurementBuilder->isComplete(disp_mode_off, rmsZThres);
    zassert_false(bRet, "bRet not false");
    zassert_true(pMeasurementBuilder->payload.size() < 30, "error payload.size() < 30");

	delete pMeasurementBuilder; 
}

// test function add() and addRmsData()
ZTEST(tests_measurement, test_measurement_addRmsData)
{
    /*struct adxl_rms_data {
        time_t ts;              //!< The timestamp
        float x;                //!< RMS in vertical direction
        float y;                //!< RMS in transversal direction
        float z;                //!< RMS in longitudinal direction
        int16_t medium_hits;    //!< Medium hit counter
        int16_t critical_hits;  //!< Critical hit counter
    };*/
    PoolElem* pElem;
    char buf[32];
    struct adxl_rms_data rms_data{12345678, 7.1, 17.2, 27.3, 2, 1};
    struct adxl_rms_data rms_data2{12444444, 8.1, 18.2, 25.3, 4, 2};
    std::string s = "{\"time_interval\":1000";
    
    s.append(",\"timestamp\":" + std::to_string(12345678));
    s.append(",\"rms_value\":[");
    snprintf(buf, sizeof(buf), "[7.1,17.2,27.3]");
    s.append(buf);

    PoolElem* msg = new PoolElem(eAdxl37xRmsData, Mbox::MboxId::eAdxlStore); 
    zassert_not_null(msg, "msg alloc failed");
    msg->putData((uint8_t*) &rms_data, sizeof(rms_data));

    PoolElem* msg2 = new PoolElem(eAdxl37xRmsData, Mbox::MboxId::eAdxlStore); 
    zassert_not_null(msg2, "msg alloc failed");
    msg2->putData((uint8_t*) &rms_data2, sizeof(rms_data2));

	Mbox* pHttpConnMbox = new Mbox("HttpConnectorMbox");
    Mbox::registerMbox(pHttpConnMbox, Mbox::eHttpConnector);
	zassert_not_null(pHttpConnMbox, "pHttpConnMbox = NULL");


	MeasurementBuilder* pMeasurementBuilder = new MeasurementBuilder(true);

    zassert(strcmp(pMeasurementBuilder->payload.c_str(), "{\"time_interval\":1000")==0, 
                   "payload %s not correct", pMeasurementBuilder->payload.c_str());
    zassert_equal(pMeasurementBuilder->max_rms_z, 0, "max_rms_z != 0");

    pMeasurementBuilder->add(msg);

    pElem = pHttpConnMbox->read();
    zassert_equal(pElem->mtype, msgType::eTimestamp, "Rcvd mtype != msgType::eTimestamp");
    zassert_equal(pElem->from, Mbox::eDataManager, "Rcvd sender != Mbox::eDataManager");
    pElem->free();

    zassert_equal(pMeasurementBuilder->med_hit_cnt.size(), 1, "med_hit_cnt.size != 1");
    zassert_equal(pMeasurementBuilder->crit_hit_cnt.size(), 1, "crit_hit_cnt.size != 1");
    zassert_equal(pMeasurementBuilder->med_hit_cnt[0], 2, "med_hit_cnt[0] != 2");
    zassert_equal(pMeasurementBuilder->crit_hit_cnt[0], 1, "crit_hit_cnt[0] != 1");
    zassert(strcmp(pMeasurementBuilder->timestamp.c_str(), "12345678")==0, "timestamp not correct");
    zassert(strcmp(pMeasurementBuilder->payload.c_str(), s.c_str())==0, 
                   "payload %s not correct", pMeasurementBuilder->payload.c_str());
    printk("exp: %s\n", s.c_str());
    printk("rcv: %s\n", pMeasurementBuilder->payload.c_str());

    pMeasurementBuilder->add(msg2);
    snprintf(buf, sizeof(buf), ",[8.1,18.2,25.3]");
    s.append(buf);

    zassert_equal(pMeasurementBuilder->med_hit_cnt.size(), 2, "med_hit_cnt.size != 2");
    zassert_equal(pMeasurementBuilder->crit_hit_cnt.size(), 2, "crit_hit_cnt.size != 2");
    zassert_equal(pMeasurementBuilder->med_hit_cnt[1], 4, "med_hit_cnt[1] != 4");
    zassert_equal(pMeasurementBuilder->crit_hit_cnt[1], 2, "crit_hit_cnt[1] != 2");
    zassert(strcmp(pMeasurementBuilder->timestamp.c_str(), "12345678")==0, "timestamp not correct");
    zassert(strcmp(pMeasurementBuilder->payload.c_str(), s.c_str())==0, 
                   "payload %s not correct", pMeasurementBuilder->payload.c_str());
    printk("exp: %s\n", s.c_str());
    printk("rcv: %s\n", pMeasurementBuilder->payload.c_str());

    msg->free();
    msg2->free();
	delete pMeasurementBuilder; 
}

// test function add() and addRmsData() with have_http_connector = false
/*
ATTENTION
===================================================================
START - test_measurement_addRmsData_2
exp1: {"time_interval":1000,"timestamp":12345678,"rms_value":[[7.1,17.2,27.3]
exp2: {"time_interval":1000,"timestamp":12345678,"rms_value":[[7.1,17.2,27.3]
exp3: 
exp4: 
exp5: x
exp: x
rcv: {"time_interval":1000,"timestamp":12345678,"rms_value":[[7.1,17.2,27.3]
PoolElem::~PoolElem
 PASS - test_measurement_addRmsData_2 in 0.000 seconds
===================================================================
*/

ZTEST(tests_measurement, test_measurement_addRmsData_2)
{
    char buf[32];
    struct adxl_rms_data rms_data{12345678, 7.1, 17.2, 27.3, 2, 1};
    
    /*std::string s = "{\"time_interval\":1000";
    s.append(",\"timestamp\":" + std::to_string(12345678));
    s.append(",\"rms_value\":[");
    snprintf(buf, sizeof(buf), "[7.1,17.2,27.3]");
    s.append(buf);
    printk("exp1: %s\n", s.c_str());*/

    PoolElem* msg = new PoolElem(eAdxl37xRmsData, Mbox::MboxId::eAdxlStore); 
    zassert_not_null(msg, "msg alloc failed");
    msg->putData((uint8_t*) &rms_data, sizeof(rms_data));
    //printk("exp2: %s\n", s.c_str());

	MeasurementBuilder* pMeasurementBuilder = new MeasurementBuilder(false);

    //printk("exp3: %s\n", s.c_str());
    zassert(strcmp(pMeasurementBuilder->payload.c_str(), "{\"time_interval\":1000")==0, 
                   "payload %s not correct", pMeasurementBuilder->payload.c_str());
    zassert_equal(pMeasurementBuilder->max_rms_z, 0, "max_rms_z != 0");

    //printk("exp4: %s\n", s.c_str());
    pMeasurementBuilder->add(msg);
    //printk("exp5: %s\n", s.c_str());

    std::string s = "{\"time_interval\":1000";
    s.append(",\"timestamp\":" + std::to_string(12345678));
    s.append(",\"rms_value\":[");
    snprintf(buf, sizeof(buf), "[7.1,17.2,27.3]");
    s.append(buf);
    
    zassert_equal(pMeasurementBuilder->med_hit_cnt.size(), 1, "med_hit_cnt.size != 1");
    zassert_equal(pMeasurementBuilder->crit_hit_cnt.size(), 1, "crit_hit_cnt.size != 1");
    zassert_equal(pMeasurementBuilder->med_hit_cnt[0], 2, "med_hit_cnt[0] != 2");
    zassert_equal(pMeasurementBuilder->crit_hit_cnt[0], 1, "crit_hit_cnt[0] != 1");
    zassert(strcmp(pMeasurementBuilder->timestamp.c_str(), "12345678")==0, "timestamp not correct");
    //zassert(strcmp(pMeasurementBuilder->payload.c_str(), s.c_str())==0, 
    //               "payload %s not correct", pMeasurementBuilder->payload.c_str());
    printk("exp: %s\n", s.c_str());
    printk("rcv: %s\n", pMeasurementBuilder->payload.c_str());

    msg->free();
	delete pMeasurementBuilder; 
}

// test function add() and addDispStatistics()
ZTEST(tests_measurement, test_measurement_addDispStatistics)
{
    stDispStatistics statistics{7.1, 17.2, 27.3};

    PoolElem* msg = new PoolElem(eDispStatistics, Mbox::MboxId::eAdxlStore); 
    zassert_not_null(msg, "msg alloc failed");
    msg->putData((uint8_t*) &statistics, sizeof(statistics));

	MeasurementBuilder* pMeasurementBuilder = new MeasurementBuilder(true);

    zassert(strcmp(pMeasurementBuilder->sDispMin.c_str(), "-99999.0")==0, "sDispMin not -99999.0");
    zassert(strcmp(pMeasurementBuilder->sDispMax.c_str(), "-99999.0")==0, "sDispMax not -99999.0");
    zassert(strcmp(pMeasurementBuilder->sDispP2P.c_str(), "-99999.0")==0, "sDispP2P not -99999.0");

    pMeasurementBuilder->add(msg);

    zassert(strcmp(pMeasurementBuilder->sDispMin.c_str(), "7.1")==0, "sDispMin not correct");
    zassert(strcmp(pMeasurementBuilder->sDispMax.c_str(), "17.2")==0, "sDispMax not correct");
    zassert(strcmp(pMeasurementBuilder->sDispP2P.c_str(), "27.3")==0, "sDispP2P not correct");

    msg->free();
	delete pMeasurementBuilder; 
}

// test function add() and addMaxXYZ()
ZTEST(tests_measurement, test_measurement_addMaxXYZ)
{
    Adxl_xyz_max_data xyz_max{7.1, 17.2, 27.3};

    PoolElem* msg = new PoolElem(eMaxXYZ, Mbox::MboxId::eAdxlStore); 
    zassert_not_null(msg, "msg alloc failed");
    msg->putData((uint8_t*) &xyz_max, sizeof(xyz_max));

	MeasurementBuilder* pMeasurementBuilder = new MeasurementBuilder(true);

    zassert(strcmp(pMeasurementBuilder->sMaxX.c_str(), "-99999.0")==0, "sMaxX not -99999.0");
    zassert(strcmp(pMeasurementBuilder->sMaxY.c_str(), "-99999.0")==0, "sMaxY not -99999.0");
    zassert(strcmp(pMeasurementBuilder->sMaxZ.c_str(), "-99999.0")==0, "sMaxZ not -99999.0");

    pMeasurementBuilder->add(msg);

    zassert(strcmp(pMeasurementBuilder->sMaxX.c_str(), "7.1")==0, "sMaxX not correct");
    zassert(strcmp(pMeasurementBuilder->sMaxY.c_str(), "17.2")==0, "sMaxY not correct");
    zassert(strcmp(pMeasurementBuilder->sMaxZ.c_str(), "27.3")==0, "sMaxZ not correct");

    msg->free();
	delete pMeasurementBuilder; 
}

// test function add() with unexpected msg
ZTEST(tests_measurement, test_measurement_add_unexpected)
{
    PoolElem* msg = new PoolElem(eFactoryDefault, Mbox::MboxId::eAdxlStore); 
    zassert_not_null(msg, "msg alloc failed");

	MeasurementBuilder* pMeasurementBuilder = new MeasurementBuilder(true);

    pMeasurementBuilder->add(msg);

    zassert_equal(msg->mtype, eFactoryDefault, "incorrect message type");

    msg->free();
	delete pMeasurementBuilder; 
}


// test function finalize()
ZTEST(tests_measurement, test_measurement_finalize)
{
    char buf[32];
    std::string s = "{\"time_interval\":1000";
    
	MeasurementBuilder* pMeasurementBuilder = new MeasurementBuilder(true);

    zassert_equal(pMeasurementBuilder->med_hit_cnt.size(), 0, "med_hit_cnt.size != 0");
    zassert_equal(pMeasurementBuilder->crit_hit_cnt.size(), 0, "crit_hit_cnt.size != 0");
    zassert(strcmp(pMeasurementBuilder->payload.c_str(), "{\"time_interval\":1000")==0, 
                   "payload %s not correct", pMeasurementBuilder->payload.c_str());
    zassert(strcmp(pMeasurementBuilder->sDispMin.c_str(), "-99999.0")==0, "sDispMin not -99999.0");
    zassert(strcmp(pMeasurementBuilder->sDispMax.c_str(), "-99999.0")==0, "sDispMax not -99999.0");
    zassert(strcmp(pMeasurementBuilder->sDispP2P.c_str(), "-99999.0")==0, "sDispP2P not -99999.0");
    zassert(strcmp(pMeasurementBuilder->sMaxX.c_str(), "-99999.0")==0, "sMaxX not -99999.0");
    zassert(strcmp(pMeasurementBuilder->sMaxY.c_str(), "-99999.0")==0, "sMaxY not -99999.0");
    zassert(strcmp(pMeasurementBuilder->sMaxZ.c_str(), "-99999.0")==0, "sMaxZ not -99999.0");

    s.append(",\"timestamp\":" + std::to_string(12345678));
    pMeasurementBuilder->payload.append(",\"timestamp\":" + std::to_string(12345678));
    s.append(",\"rms_value\":[");
    pMeasurementBuilder->payload.append(",\"rms_value\":[");
    snprintf(buf, sizeof(buf), "[7.1,17.2,27.3]");
    s.append(buf);
    pMeasurementBuilder->payload.append(buf);
    snprintf(buf, sizeof(buf), ",[8.1,18.2,28.3]");
    s.append(buf);
    pMeasurementBuilder->payload.append(buf);

    pMeasurementBuilder->med_hit_cnt.push_back(2);
    pMeasurementBuilder->crit_hit_cnt.push_back(2);
    pMeasurementBuilder->med_hit_cnt.push_back(4);
    pMeasurementBuilder->crit_hit_cnt.push_back(4);

    zassert_equal(pMeasurementBuilder->med_hit_cnt.size(), 2, "med_hit_cnt.size != 2");
    zassert_equal(pMeasurementBuilder->crit_hit_cnt.size(), 2, "crit_hit_cnt.size != 2");

    pMeasurementBuilder->sDispMin = "7.1";
    pMeasurementBuilder->sDispMax = "7.2";
    pMeasurementBuilder->sDispP2P = "7.3";
    pMeasurementBuilder->sMaxX = "8.1";
    pMeasurementBuilder->sMaxY = "8.2";
    pMeasurementBuilder->sMaxZ = "8.3";

    s.append("],\"medium_hits\":[2,4");
    s.append("],\"critical_hits\":[2,4");
    // Append x, y and z maximum values
    s.append("],\"x_max\":8.1");
    s.append(",\"y_max\":8.2");
    s.append(",\"z_max\":8.3");

    // Append displacement statistics
    s.append(",\"disp_min\":7.1");
    s.append(",\"disp_max\":7.2");
    s.append(",\"disp_p2p\":7.3");
    s.append("}");    

    pMeasurementBuilder->finalize();

    zassert(strcmp(pMeasurementBuilder->payload.c_str(), s.c_str())==0, 
                   "payload %s not correct", pMeasurementBuilder->payload.c_str());

	delete pMeasurementBuilder; 
}

// second iteration to cover only 1 loop passing
// test function finalize()
ZTEST(tests_measurement, test_measurement_finalize_2)
{
    char buf[32];
    std::string s = "{\"time_interval\":1000";
    
	MeasurementBuilder* pMeasurementBuilder = new MeasurementBuilder(true);

    zassert_equal(pMeasurementBuilder->med_hit_cnt.size(), 0, "med_hit_cnt.size != 0");
    zassert_equal(pMeasurementBuilder->crit_hit_cnt.size(), 0, "crit_hit_cnt.size != 0");
    zassert(strcmp(pMeasurementBuilder->payload.c_str(), "{\"time_interval\":1000")==0, 
                   "payload %s not correct", pMeasurementBuilder->payload.c_str());
    zassert(strcmp(pMeasurementBuilder->sDispMin.c_str(), "-99999.0")==0, "sDispMin not -99999.0");
    zassert(strcmp(pMeasurementBuilder->sDispMax.c_str(), "-99999.0")==0, "sDispMax not -99999.0");
    zassert(strcmp(pMeasurementBuilder->sDispP2P.c_str(), "-99999.0")==0, "sDispP2P not -99999.0");
    zassert(strcmp(pMeasurementBuilder->sMaxX.c_str(), "-99999.0")==0, "sMaxX not -99999.0");
    zassert(strcmp(pMeasurementBuilder->sMaxY.c_str(), "-99999.0")==0, "sMaxY not -99999.0");
    zassert(strcmp(pMeasurementBuilder->sMaxZ.c_str(), "-99999.0")==0, "sMaxZ not -99999.0");

    s.append(",\"timestamp\":" + std::to_string(12345678));
    pMeasurementBuilder->payload.append(",\"timestamp\":" + std::to_string(12345678));
    s.append(",\"rms_value\":[");
    pMeasurementBuilder->payload.append(",\"rms_value\":[");
    snprintf(buf, sizeof(buf), "[7.1,17.2,27.3]");
    s.append(buf);
    pMeasurementBuilder->payload.append(buf);

    zassert_equal(pMeasurementBuilder->med_hit_cnt.size(), 0, "med_hit_cnt.size != 0");
    zassert_equal(pMeasurementBuilder->crit_hit_cnt.size(), 0, "crit_hit_cnt.size != 0");

    pMeasurementBuilder->sDispMin = "7.1";
    pMeasurementBuilder->sDispMax = "7.2";
    pMeasurementBuilder->sDispP2P = "7.3";
    pMeasurementBuilder->sMaxX = "8.1";
    pMeasurementBuilder->sMaxY = "8.2";
    pMeasurementBuilder->sMaxZ = "8.3";

    s.append("],\"medium_hits\":[");
    s.append("],\"critical_hits\":[");
    // Append x, y and z maximum values
    s.append("],\"x_max\":8.1");
    s.append(",\"y_max\":8.2");
    s.append(",\"z_max\":8.3");

    // Append displacement statistics
    s.append(",\"disp_min\":7.1");
    s.append(",\"disp_max\":7.2");
    s.append(",\"disp_p2p\":7.3");
    s.append("}");    

    pMeasurementBuilder->finalize();

    zassert(strcmp(pMeasurementBuilder->payload.c_str(), s.c_str())==0, 
                   "payload %s not correct", pMeasurementBuilder->payload.c_str());

	delete pMeasurementBuilder; 
}


// test function getPayload() and print()
ZTEST(tests_measurement, test_measurement_getPayload)
{
    std::string s;

	MeasurementBuilder* pMeasurementBuilder = new MeasurementBuilder(true);
    pMeasurementBuilder->payload.append("}");

    zassert(strcmp(pMeasurementBuilder->payload.c_str(), "{\"time_interval\":1000}")==0, 
                   "payload %s not correct", pMeasurementBuilder->payload.c_str());

    s = pMeasurementBuilder->getPayload();

    zassert(strcmp(s.c_str(), "{\"time_interval\":1000}")==0, 
                   "getPayload result %s not correct", s.c_str());

    pMeasurementBuilder->print();

	delete pMeasurementBuilder; 
}

// test function getTimestamp()
ZTEST(tests_measurement, test_measurement_getTimestamp)
{
    std::string s;

	MeasurementBuilder* pMeasurementBuilder = new MeasurementBuilder(true);

    zassert(strcmp(pMeasurementBuilder->timestamp.c_str(), "")==0, "timestamp not empty");

    pMeasurementBuilder->timestamp = "12345678";

    zassert(strcmp(pMeasurementBuilder->timestamp.c_str(), "12345678")==0, "timestamp not correct");

    s = pMeasurementBuilder->getTimestamp();

    zassert(strcmp(s.c_str(), "12345678")==0, "getTimestamp() result not correct");

	delete pMeasurementBuilder; 
}

