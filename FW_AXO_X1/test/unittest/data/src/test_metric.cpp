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
#include "modem/lte_monitor.h"
#include "drv/adc/adconverter.h"

#define private public
#include "data/metric.h"

#include "stubs.h"


using fs = FlashFS;

// -------------------------------------------------------------------------



ZTEST_SUITE(tests_metric, NULL, NULL, NULL, NULL, NULL);

// test constructor and destructor, including functions reset() and setFloatStrings()
ZTEST(tests_metric, test_metric_constructor)
{
	MetricsBuilder* pMetricsBuilder = new MetricsBuilder();

	zassert_not_null(pMetricsBuilder, "memory alloc failed");
    zassert(strcmp(pMetricsBuilder->payload.c_str(), "{")==0, 
                   "payload %s not correct", pMetricsBuilder->payload.c_str());

	delete pMetricsBuilder; 
}

// MISSING for 100% branch coverage static instantiation does not deliver NULL
// here code line 
// are not covered completly.

// test function isComplete()
ZTEST(tests_metric, test_metric_isComplete)
{
    bool bRet;

	MetricsBuilder* pMetricsBuilder = new MetricsBuilder();

    pMetricsBuilder->mdm_batv = "17";
    pMetricsBuilder->mdm_temp = "17";
    pMetricsBuilder->vdd_int = "17";
    pMetricsBuilder->temperature = "17";
    pMetricsBuilder->humidity = "17";

    bRet = pMetricsBuilder->isComplete();
    zassert_true(bRet, "bRet != true");

    pMetricsBuilder->mdm_batv = "";
    pMetricsBuilder->mdm_temp = "17";
    pMetricsBuilder->vdd_int = "17";
    pMetricsBuilder->temperature = "17";
    pMetricsBuilder->humidity = "17";

    bRet = pMetricsBuilder->isComplete();
    zassert_false(bRet, "bRet != false");

    pMetricsBuilder->mdm_batv = "17";
    pMetricsBuilder->mdm_temp = "";
    pMetricsBuilder->vdd_int = "17";
    pMetricsBuilder->temperature = "17";
    pMetricsBuilder->humidity = "17";

    bRet = pMetricsBuilder->isComplete();
    zassert_false(bRet, "bRet != false");

    pMetricsBuilder->mdm_batv = "17";
    pMetricsBuilder->mdm_temp = "17";
    pMetricsBuilder->vdd_int = "";
    pMetricsBuilder->temperature = "17";
    pMetricsBuilder->humidity = "17";

    bRet = pMetricsBuilder->isComplete();
    zassert_false(bRet, "bRet != false");

    pMetricsBuilder->mdm_batv = "17";
    pMetricsBuilder->mdm_temp = "17";
    pMetricsBuilder->vdd_int = "17";
    pMetricsBuilder->temperature = "";
    pMetricsBuilder->humidity = "17";

    bRet = pMetricsBuilder->isComplete();
    zassert_false(bRet, "bRet != false");

    pMetricsBuilder->mdm_batv = "17";
    pMetricsBuilder->mdm_temp = "17";
    pMetricsBuilder->vdd_int = "17";
    pMetricsBuilder->temperature = "17";
    pMetricsBuilder->humidity = "";

    bRet = pMetricsBuilder->isComplete();
    zassert_false(bRet, "bRet != false");

	delete pMetricsBuilder; 
}

// test function setConfigVersion()
ZTEST(tests_metric, test_metric_setConfigVersion)
{
	std::string s = "5.55";
    zassert(strcmp(s.c_str(), "5.55")==0, "s not 5.55");
    
    MetricsBuilder* pMetricsBuilder = new MetricsBuilder();

    pMetricsBuilder->setConfigVersion(s);
    zassert(strcmp(pMetricsBuilder->config_version.c_str(), "5.55")==0, "config_version not 5.55");

    delete pMetricsBuilder; 
}

// test function setActivationThres()
ZTEST(tests_metric, test_metric_setActivationThres)
{
	std::string s = "777";
    zassert(strcmp(s.c_str(), "777")==0, "s not 777");
    
    MetricsBuilder* pMetricsBuilder = new MetricsBuilder();

    pMetricsBuilder->setActivationThres(s);
    zassert(strcmp(pMetricsBuilder->act_thres.c_str(), "777")==0, "act_thres not 777");

    delete pMetricsBuilder; 
}

// test function setDeactivationThres()
ZTEST(tests_metric, test_metric_setDeactivationThres)
{
	std::string s = "777";
    zassert(strcmp(s.c_str(), "777")==0, "s not 777");
    
    MetricsBuilder* pMetricsBuilder = new MetricsBuilder();

    pMetricsBuilder->setDeactivationThres(s);
    zassert(strcmp(pMetricsBuilder->deact_thres.c_str(), "777")==0, "deact_thres not 777");

    delete pMetricsBuilder; 
}

// test function setDispModeOffFlag()
ZTEST(tests_metric, test_metric_setDispModeOffFlag)
{
	std::string s = "true";
    zassert(strcmp(s.c_str(), "true")==0, "s not true");
    
    MetricsBuilder* pMetricsBuilder = new MetricsBuilder();

    pMetricsBuilder->setDispModeOffFlag(s);
    zassert(strcmp(pMetricsBuilder->disp_mode_off.c_str(), "true")==0, "disp_mode_off not true");

    delete pMetricsBuilder; 
}

// test function setRawTransferOnFlag()
ZTEST(tests_metric, test_metric_setRawTransferOnFlag)
{
	std::string s = "true";
    zassert(strcmp(s.c_str(), "true")==0, "s not true");
    
    MetricsBuilder* pMetricsBuilder = new MetricsBuilder();

    pMetricsBuilder->setRawTransferOnFlag(s);
    zassert(strcmp(pMetricsBuilder->raw_transfer_on.c_str(), "true")==0, "raw_transfer_on not true");

    delete pMetricsBuilder; 
}

// test function setDispTransferOnFlag()
ZTEST(tests_metric, test_metric_setDispTransferOnFlag)
{
	std::string s = "true";
    zassert(strcmp(s.c_str(), "true")==0, "s not true");
    
    MetricsBuilder* pMetricsBuilder = new MetricsBuilder();

    pMetricsBuilder->setDispTransferOnFlag(s);
    zassert(strcmp(pMetricsBuilder->disp_transfer_on.c_str(), "true")==0, "disp_transfer_on not true");

    delete pMetricsBuilder; 
}

// test function setPublicKey()
ZTEST(tests_metric, test_metric_setPublicKey)
{
	std::string s = "#######--KEY--######";
    zassert(strcmp(s.c_str(), "#######--KEY--######")==0, "s not #######--KEY--######");
    
    MetricsBuilder* pMetricsBuilder = new MetricsBuilder();

    pMetricsBuilder->setPublicKey(s);
    zassert(strcmp(pMetricsBuilder->pubkey.c_str(), "#######--KEY--######")==0, "dipubkeysp_transfer_on not #######--KEY--######");

    delete pMetricsBuilder; 
}

// test function setMetricsInterval()
ZTEST(tests_metric, test_metric_setMetricsInterval)
{
	std::string s = "120";
    zassert(strcmp(s.c_str(), "120")==0, "s not 120");
    
    MetricsBuilder* pMetricsBuilder = new MetricsBuilder();

    pMetricsBuilder->setMetricsInterval(s);
    zassert(strcmp(pMetricsBuilder->interval.c_str(), "120")==0, "interval not 120");

    delete pMetricsBuilder; 
}

// test function set_Z_RmsThres()
ZTEST(tests_metric, test_metric_set_Z_RmsThres)
{
	std::string s = "120";
    zassert(strcmp(s.c_str(), "120")==0, "s not 120");
    
    MetricsBuilder* pMetricsBuilder = new MetricsBuilder();

    pMetricsBuilder->set_Z_RmsThres(s);
    zassert(strcmp(pMetricsBuilder->rms_thres.c_str(), "120")==0, "rms_thres not 120");

    delete pMetricsBuilder; 
}

// test function set_max_raw_per_day()
ZTEST(tests_metric, test_metric_set_max_raw_per_day)
{
	std::string s = "120";
    zassert(strcmp(s.c_str(), "120")==0, "s not 120");
    
    MetricsBuilder* pMetricsBuilder = new MetricsBuilder();

    pMetricsBuilder->set_max_raw_per_day(s);
    zassert(strcmp(pMetricsBuilder->max_raw_per_day.c_str(), "120")==0, "max_raw_per_day not 120");

    delete pMetricsBuilder; 
}

// test function add () and addModemBattV()
ZTEST(tests_metric, test_metric_addModemBattV)
{
    int32_t val = 27;
    
    PoolElem* msg = new PoolElem(eLteBattVoltage, Mbox::MboxId::eMetrics); 
    zassert_not_null(msg, "msg alloc failed");
    msg->putData((uint8_t*) &val, sizeof(val));

	MetricsBuilder* pMetricsBuilder = new MetricsBuilder();

    pMetricsBuilder->mdm_batv = "117";
    zassert(strcmp(pMetricsBuilder->mdm_batv.c_str(), "117")==0, "mdm_batv not 117");

    pMetricsBuilder->add(msg);
    zassert(strcmp(pMetricsBuilder->mdm_batv.c_str(), "27")==0, "mdm_batv %s incorrect", 
            pMetricsBuilder->mdm_batv.c_str());

	msg->free();
    delete pMetricsBuilder; 
}

// test function add and addModemTemp()
ZTEST(tests_metric, test_metric_addModemTemp)
{
    int32_t val = 27;
    
    PoolElem* msg = new PoolElem(eLteTemperature, Mbox::MboxId::eMetrics); 
    zassert_not_null(msg, "msg alloc failed");
    msg->putData((uint8_t*) &val, sizeof(val));

	MetricsBuilder* pMetricsBuilder = new MetricsBuilder();

    pMetricsBuilder->mdm_temp = "117";
    zassert(strcmp(pMetricsBuilder->mdm_temp.c_str(), "117")==0, "mdm_temp not 117");

    pMetricsBuilder->add(msg);
    //zassert(strcmp(pMetricsBuilder->mdm_temp.c_str(), "27")==0, "mdm_temp %s incorrect", 
    //        pMetricsBuilder->mdm_temp.c_str());
    printk("mdm_temp = %s", pMetricsBuilder->mdm_temp.c_str());

	msg->free();
    delete pMetricsBuilder; 
}

// test function add () and addModemLinkQuality()
ZTEST(tests_metric, test_metric_addModemLinkQuality)
{
    int8_t val = 27;
    
    PoolElem* msg = new PoolElem(eLteLinkQuality, Mbox::MboxId::eMetrics); 
    zassert_not_null(msg, "msg alloc failed");
    msg->p2 = val;

	MetricsBuilder* pMetricsBuilder = new MetricsBuilder();

    pMetricsBuilder->mdm_link = "11";
    zassert(strcmp(pMetricsBuilder->mdm_link.c_str(), "11")==0, "mdm_link not 11");

    pMetricsBuilder->add(msg);
    zassert(strcmp(pMetricsBuilder->mdm_link.c_str(), "27")==0, "mdm_link %s incorrect", 
            pMetricsBuilder->mdm_link.c_str());

	msg->free();
    delete pMetricsBuilder; 
}

// test function add () and addModemPsmAct()
ZTEST(tests_metric, test_metric_addModemPsmAct)
{
    struct psm_timer tim = {17,18};

    PoolElem* msg = new PoolElem(eLtePsmAct, Mbox::MboxId::eMetrics); 
    zassert_not_null(msg, "msg alloc failed");
    msg->putData((uint8_t*) &tim, sizeof(tim));

	MetricsBuilder* pMetricsBuilder = new MetricsBuilder();

    pMetricsBuilder->psm_tau = "22";
    pMetricsBuilder->psm_active  = "33";
    zassert(strcmp(pMetricsBuilder->psm_tau.c_str(), "22")==0, "psm_tau not 22");
    zassert(strcmp(pMetricsBuilder->psm_active.c_str(), "33")==0, "psm_active not 33");

    pMetricsBuilder->add(msg);
    zassert(strcmp(pMetricsBuilder->psm_tau.c_str(), "17")==0, "psm_tau not 17");
    zassert(strcmp(pMetricsBuilder->psm_active.c_str(), "18")==0, "psm_active not 18");

	msg->free();
    delete pMetricsBuilder; 
}

// test function add () and addIMSI()
ZTEST(tests_metric, test_metric_addIMSI)
{
    std::string str = "77777777777777";
    
    PoolElem* msg = new PoolElem(eLteIMSI, Mbox::MboxId::eMetrics); 
    zassert_not_null(msg, "msg alloc failed");
    msg->putString(str.c_str());

	MetricsBuilder* pMetricsBuilder = new MetricsBuilder();

    pMetricsBuilder->imsi = "111111111111111";
    zassert(strcmp(pMetricsBuilder->imsi.c_str(), "111111111111111")==0, "imsi not 111111111111111");

    pMetricsBuilder->add(msg);
    zassert(strcmp(pMetricsBuilder->imsi.c_str(), "77777777777777")==0, "imsi %s incorrect", 
            pMetricsBuilder->imsi.c_str());

	msg->free();
    delete pMetricsBuilder; 
}

// test function add () and case eLteFwVersion:
ZTEST(tests_metric, test_metric_case_eLteFwVersion)
{
    PoolElem* msg = new PoolElem(eLteFwVersion, Mbox::MboxId::eMetrics); 
    zassert_not_null(msg, "msg alloc failed");

	MetricsBuilder* pMetricsBuilder = new MetricsBuilder();

    pMetricsBuilder->add(msg);

	msg->free();
    delete pMetricsBuilder; 
}

// test function add () and addHwMetrics() mit hw_m->hw_status == eHwFailure
ZTEST(tests_metric, test_metric_addHwMetrics)
{
    struct hw_metrics hw_metrics = {7, eHwFailure};

    PoolElem* msg = new PoolElem(eHwMetrics, Mbox::MboxId::eMetrics); 
    zassert_not_null(msg, "msg alloc failed");
    msg->putData((uint8_t*) &hw_metrics, sizeof(hw_metrics));

	MetricsBuilder* pMetricsBuilder = new MetricsBuilder();

    pMetricsBuilder->num_hw_errors = "2";
    pMetricsBuilder->adxl_status   = "\"adxl37x_ok\"";
    zassert(strcmp(pMetricsBuilder->num_hw_errors.c_str(), "2")==0, "num_hw_errors not 2");
    zassert(strcmp(pMetricsBuilder->adxl_status.c_str(), "\"adxl37x_ok\"")==0, "adxl_status not adxl37x_ok");

    pMetricsBuilder->add(msg);
    zassert(strcmp(pMetricsBuilder->num_hw_errors.c_str(), "7")==0, "num_hw_errors not 7");
    zassert(strcmp(pMetricsBuilder->adxl_status.c_str(), "\"adxl37x_fail\"")==0, "adxl_status not adxl37x_fail");

	msg->free();
    delete pMetricsBuilder; 
}

// test function add () and addHwMetrics() mit hw_m->hw_status == eHwSelftestFailed
ZTEST(tests_metric, test_metric_addHwMetrics2)
{
    struct hw_metrics hw_metrics = {7, eHwSelftestFailed};

    PoolElem* msg = new PoolElem(eHwMetrics, Mbox::MboxId::eMetrics); 
    zassert_not_null(msg, "msg alloc failed");
    msg->putData((uint8_t*) &hw_metrics, sizeof(hw_metrics));

	MetricsBuilder* pMetricsBuilder = new MetricsBuilder();

    pMetricsBuilder->num_hw_errors = "2";
    pMetricsBuilder->adxl_status   = "\"adxl37x_ok\"";
    zassert(strcmp(pMetricsBuilder->num_hw_errors.c_str(), "2")==0, "num_hw_errors not 2");
    zassert(strcmp(pMetricsBuilder->adxl_status.c_str(), "\"adxl37x_ok\"")==0, "adxl_status not adxl37x_ok");

    pMetricsBuilder->add(msg);
    zassert(strcmp(pMetricsBuilder->num_hw_errors.c_str(), "7")==0, "num_hw_errors not 7");
    zassert(strcmp(pMetricsBuilder->adxl_status.c_str(), "\"adxl37x_selftest_err\"")==0, "adxl_status not adxl37x_selftest_err");

	msg->free();
    delete pMetricsBuilder; 
}

// test function add () and addHwMetrics() mit hw_m->hw_status == eHwOK
ZTEST(tests_metric, test_metric_addHwMetrics3)
{
    struct hw_metrics hw_metrics = {7, eHwOK};

    PoolElem* msg = new PoolElem(eHwMetrics, Mbox::MboxId::eMetrics); 
    zassert_not_null(msg, "msg alloc failed");
    msg->putData((uint8_t*) &hw_metrics, sizeof(hw_metrics));

	MetricsBuilder* pMetricsBuilder = new MetricsBuilder();

    pMetricsBuilder->num_hw_errors = "2";
    pMetricsBuilder->adxl_status   = "\"adxl37x_fail\"";
    zassert(strcmp(pMetricsBuilder->num_hw_errors.c_str(), "2")==0, "num_hw_errors not 2");
    zassert(strcmp(pMetricsBuilder->adxl_status.c_str(), "\"adxl37x_fail\"")==0, "adxl_status not adxl37x_fail");

    pMetricsBuilder->add(msg);
    zassert(strcmp(pMetricsBuilder->num_hw_errors.c_str(), "7")==0, "num_hw_errors not 7");
    zassert(strcmp(pMetricsBuilder->adxl_status.c_str(), "\"adxl37x_ok\"")==0, "adxl_status not adxl37x_ok");

	msg->free();
    delete pMetricsBuilder; 
}

// test function add () and addBattV()
ZTEST(tests_metric, test_metric_addBattV)
{
    struct ADInput ad_in = {1, 17, 17};

    PoolElem* msg = new PoolElem(eAdcData, Mbox::MboxId::eMetrics); 
    zassert_not_null(msg, "msg alloc failed");
    msg->putData((uint8_t*) &ad_in, sizeof(ad_in));

	MetricsBuilder* pMetricsBuilder = new MetricsBuilder();

    pMetricsBuilder->add(msg);
    zassert_equal(ad_in.channel, 1, "ad_in.channel != 1");
    zassert(strcmp(pMetricsBuilder->vdd_det.c_str(), "53")==0, "vdd_det not 53");
    msg->free();

    ad_in.channel = 2;

    PoolElem* msg2 = new PoolElem(eAdcData, Mbox::MboxId::eMetrics); 
    zassert_not_null(msg, "msg alloc failed");
    msg2->putData((uint8_t*) &ad_in, sizeof(ad_in));
    pMetricsBuilder->add(msg2);
    zassert_equal(ad_in.channel, 2, "ad_in.channel != 2");
    zassert(strcmp(pMetricsBuilder->vbat2.c_str(), "53")==0, "vbat2 not 53");
    msg2->free();

    ad_in.channel = 3;

    PoolElem* msg3 = new PoolElem(eAdcData, Mbox::MboxId::eMetrics); 
    zassert_not_null(msg, "msg alloc failed");
    msg3->putData((uint8_t*) &ad_in, sizeof(ad_in));
    pMetricsBuilder->add(msg3);
    zassert_equal(ad_in.channel, 3, "ad_in.channel != 3");
    zassert(strcmp(pMetricsBuilder->vbat1.c_str(), "53")==0, "vbat1 not 53");
    msg3->free();

    ad_in.channel = 7;

    PoolElem* msg4 = new PoolElem(eAdcData, Mbox::MboxId::eMetrics); 
    zassert_not_null(msg, "msg alloc failed");
    msg4->putData((uint8_t*) &ad_in, sizeof(ad_in));
    pMetricsBuilder->add(msg4);
    zassert_equal(ad_in.channel, 7, "ad_in.channel != 7");
    zassert(strcmp(pMetricsBuilder->vdd_int.c_str(), "17")==0, "vdd_int not 17");
    msg4->free();

    ad_in.channel = 5;

    PoolElem* msg5 = new PoolElem(eAdcData, Mbox::MboxId::eMetrics); 
    zassert_not_null(msg, "msg alloc failed");
    msg5->putData((uint8_t*) &ad_in, sizeof(ad_in));
    pMetricsBuilder->add(msg5);
    zassert_equal(ad_in.channel, 5, "ad_in.channel != 5");
    msg5->free();

	delete pMetricsBuilder; 
}

// test function add () and addResetCause()
ZTEST(tests_metric, test_metric_addResetCause)
{
    std::string cause = "qwertz";
    
    PoolElem* msg = new PoolElem(eResetCause, Mbox::MboxId::eMetrics); 
    zassert_not_null(msg, "msg alloc failed");
    msg->putString(cause.c_str());

	MetricsBuilder* pMetricsBuilder = new MetricsBuilder();

    pMetricsBuilder->reset_cause = "117";
    zassert(strcmp(pMetricsBuilder->reset_cause.c_str(), "117")==0, "reset_cause not 117");

    pMetricsBuilder->add(msg);
    zassert(strcmp(pMetricsBuilder->reset_cause.c_str(), "qwertz")==0, "reset_cause %s incorrect", 
            pMetricsBuilder->reset_cause.c_str());

	msg->free();
    delete pMetricsBuilder; 
}

// test function add () and addBootCnt()
ZTEST(tests_metric, test_metric_addBootCnt)
{
    int32_t val = 27;
    
    PoolElem* msg = new PoolElem(eBootCnt, Mbox::MboxId::eMetrics); 
    zassert_not_null(msg, "msg alloc failed");
    msg->putData((uint8_t*) &val, sizeof(val));

	MetricsBuilder* pMetricsBuilder = new MetricsBuilder();

    pMetricsBuilder->boot_cnt = "117";
    zassert(strcmp(pMetricsBuilder->boot_cnt.c_str(), "117")==0, "boot_cnt not 117");

    pMetricsBuilder->add(msg);
    zassert(strcmp(pMetricsBuilder->boot_cnt.c_str(), "27")==0, "boot_cnt %s incorrect", 
            pMetricsBuilder->boot_cnt.c_str());

	msg->free();
    delete pMetricsBuilder; 
}

// test function add () and addTemperature()
ZTEST(tests_metric, test_metric_addTemperature)
{
    struct sensor_value hum = {23, 15};
    
    PoolElem* msg = new PoolElem(eShtcxTemperature, Mbox::MboxId::eMetrics); 
    zassert_not_null(msg, "msg alloc failed");
    msg->putData((uint8_t*) &hum, sizeof(hum));

	MetricsBuilder* pMetricsBuilder = new MetricsBuilder();

    pMetricsBuilder->temperature = "12345";
    zassert(strcmp(pMetricsBuilder->temperature.c_str(), "12345")==0, "temperature not 12345");

    pMetricsBuilder->add(msg);
    zassert(strcmp(pMetricsBuilder->temperature.c_str(), "23.00")==0, "temperature %s incorrect", 
            pMetricsBuilder->temperature.c_str());

	msg->free();
    delete pMetricsBuilder; 
}

// test function add () and addHumidity()
ZTEST(tests_metric, test_metric_addHumidity)
{
    struct sensor_value hum = {23, 15};
    
    PoolElem* msg = new PoolElem(eShtcxHumidity, Mbox::MboxId::eMetrics); 
    zassert_not_null(msg, "msg alloc failed");
    msg->putData((uint8_t*) &hum, sizeof(hum));

	MetricsBuilder* pMetricsBuilder = new MetricsBuilder();

    pMetricsBuilder->humidity = "12345";
    zassert(strcmp(pMetricsBuilder->humidity.c_str(), "12345")==0, "humidity not 12345");

    pMetricsBuilder->add(msg);
    zassert(strcmp(pMetricsBuilder->humidity.c_str(), "23.00")==0, "humidity %s incorrect", 
            pMetricsBuilder->humidity.c_str());

	msg->free();
    delete pMetricsBuilder; 
}

// test function add () and default case
ZTEST(tests_metric, test_metric_default_case)
{
    PoolElem* msg = new PoolElem(eAdxl37xIdle, Mbox::MboxId::eMetrics); 
    zassert_not_null(msg, "msg alloc failed");

	MetricsBuilder* pMetricsBuilder = new MetricsBuilder();

    pMetricsBuilder->add(msg);

	msg->free();
    delete pMetricsBuilder; 
}

// test function finalize() with fs::inst().file_exists(BOOST_ENABLED_FILENAME) = true
ZTEST(tests_metric, test_metric_finalize)
{
	MetricsBuilder* pMetricsBuilder = new MetricsBuilder();

    zassert(strcmp(pMetricsBuilder->payload.c_str(), "{")==0, 
                "payload %s not correct", pMetricsBuilder->payload.c_str());

    pMetricsBuilder->reset_cause = "POR";
    pMetricsBuilder->boot_cnt = "7";

    pMetricsBuilder->vbat1 = "17";
    pMetricsBuilder->vbat2 = "17";
    pMetricsBuilder->vdd_det = "17";

    pMetricsBuilder->temperature = "27";
    pMetricsBuilder->humidity = "27";

    pMetricsBuilder->mdm_batv = "17";
    pMetricsBuilder->mdm_temp = "17";

    pMetricsBuilder->mdm_link = "2";
    pMetricsBuilder->psm_tau = "2";
    pMetricsBuilder->psm_active = "2";

    pMetricsBuilder->imsi = "111111111111111";

    pMetricsBuilder->config_version = "1.77";
    pMetricsBuilder->act_thres = "170";
    pMetricsBuilder->deact_thres = "100";
    pMetricsBuilder->disp_mode_off = "true";
    pMetricsBuilder->raw_transfer_on = "false";
    pMetricsBuilder->disp_transfer_on = "false";
    pMetricsBuilder->pubkey = "#####";
    pMetricsBuilder->interval = "60";
    pMetricsBuilder->rms_thres = "17";

    pMetricsBuilder->num_hw_errors = "7";
    pMetricsBuilder->adxl_status = "1";

    gbRetVal = true;

    pMetricsBuilder->finalize();

    std::string s = "{\"uptime\":0,\"reset_reason\":\"POR\",\"boot_cnt\":7,\"vbat1\":17,\"vbat2\":17,"
                   "\"vdd_det\":17,\"temp\":27,\"humidity\":27,\"mdm_batv\":17,\"mdm_temp\":17,"
                   "\"mdm_link\":2,\"psm_tau\":2,\"psm_active\":2,\"fw_ver\":9.77,\"imsi\":111111111111111,"
                   "\"conf_ver\":\"1.77\",\"act_thres\":170,\"deact_thres\":100,\"disp_mode_off\":\"true\","
                   "\"raw_transfer_on\":\"false\",\"disp_transfer_on\":\"false\",\"pub_key\":\"#####\","
                   "\"metrics_interval\":60,\"rms_thres\":17,\"device_errors\":7,\"device_states\":1,"
                   "\"boost_in_use\":1}";

    zassert(strcmp(pMetricsBuilder->payload.c_str(), s.c_str())==0, 
                   "payload %s %s not correct", pMetricsBuilder->payload.c_str(), s.c_str());

	delete pMetricsBuilder; 
}

// test function finalize() with fs::inst().file_exists(BOOST_ENABLED_FILENAME) = false
ZTEST(tests_metric, test_metric_finalize2)
{
	MetricsBuilder* pMetricsBuilder = new MetricsBuilder();

    zassert(strcmp(pMetricsBuilder->payload.c_str(), "{")==0, 
                "payload %s not correct", pMetricsBuilder->payload.c_str());

    pMetricsBuilder->reset_cause = "POR";
    pMetricsBuilder->boot_cnt = "7";

    pMetricsBuilder->vbat1 = "17";
    pMetricsBuilder->vbat2 = "17";
    pMetricsBuilder->vdd_det = "17";

    pMetricsBuilder->temperature = "27";
    pMetricsBuilder->humidity = "27";

    pMetricsBuilder->mdm_batv = "17";
    pMetricsBuilder->mdm_temp = "17";

    pMetricsBuilder->mdm_link = "";
    pMetricsBuilder->psm_tau = "";
    pMetricsBuilder->psm_active = "";

    pMetricsBuilder->imsi = "111111111111111";

    pMetricsBuilder->config_version = "1.77";
    pMetricsBuilder->act_thres = "170";
    pMetricsBuilder->deact_thres = "100";
    pMetricsBuilder->disp_mode_off = "true";
    pMetricsBuilder->raw_transfer_on = "false";
    pMetricsBuilder->disp_transfer_on = "false";
    pMetricsBuilder->pubkey = "#####";
    pMetricsBuilder->interval = "60";
    pMetricsBuilder->rms_thres = "17";

    pMetricsBuilder->num_hw_errors = "7";
    pMetricsBuilder->adxl_status = "1";

    gbRetVal = false;

    pMetricsBuilder->finalize();

    std::string s = "{\"uptime\":0,\"reset_reason\":\"POR\",\"boot_cnt\":7,\"vbat1\":17,\"vbat2\":17,"
                   "\"vdd_det\":17,\"temp\":27,\"humidity\":27,\"mdm_batv\":17,\"mdm_temp\":17,"
                   "\"mdm_link\":0,\"psm_tau\":0,\"psm_active\":0,\"fw_ver\":9.77,\"imsi\":111111111111111,"
                   "\"conf_ver\":\"1.77\",\"act_thres\":170,\"deact_thres\":100,\"disp_mode_off\":\"true\","
                   "\"raw_transfer_on\":\"false\",\"disp_transfer_on\":\"false\",\"pub_key\":\"#####\","
                   "\"metrics_interval\":60,\"rms_thres\":17,\"device_errors\":7,\"device_states\":1,"
                   "\"boost_in_use\":0}";

    zassert(strcmp(pMetricsBuilder->payload.c_str(), s.c_str())==0, 
                   "payload %s %s not correct", pMetricsBuilder->payload.c_str(), s.c_str());

	delete pMetricsBuilder; 
}

// test function getPayload()
ZTEST(tests_metric, test_metric_getPayload)
{
	MetricsBuilder* pMetricsBuilder = new MetricsBuilder();

    zassert(strcmp(pMetricsBuilder->payload.c_str(), "{")==0, 
                "payload %s not correct", pMetricsBuilder->payload.c_str());

    std::string s = pMetricsBuilder->getPayload();

    zassert(strcmp(s.c_str(), "{")==0, 
                "string s %s not correct", s.c_str());

	delete pMetricsBuilder; 
}

// test function print()
ZTEST(tests_metric, test_metric_print)
{
	MetricsBuilder* pMetricsBuilder = new MetricsBuilder();

    pMetricsBuilder->print();

	delete pMetricsBuilder; 
}

// test function reset()
ZTEST(tests_metric, test_metric_reset)
{
	MetricsBuilder* pMetricsBuilder = new MetricsBuilder();

    zassert(strcmp(pMetricsBuilder->payload.c_str(), "{")==0, 
                "payload %s not correct", pMetricsBuilder->payload.c_str());

    pMetricsBuilder->vbat1 = "abcd";
    pMetricsBuilder->vbat2 = "abcd";
    pMetricsBuilder->vdd_det = "abcd";
    pMetricsBuilder->vdd_int = "abcd";
    pMetricsBuilder->mdm_batv = "abcd";
    pMetricsBuilder->mdm_temp = "abcd";
    pMetricsBuilder->mdm_link = "abcd";
    pMetricsBuilder->psm_tau = "abcd";
    pMetricsBuilder->temperature = "abcd";
    pMetricsBuilder->humidity = "abcd";
    pMetricsBuilder->payload = "{abcd";

    zassert(strcmp(pMetricsBuilder->vbat1.c_str(), "abcd")==0, 
                "vbat1 %s not correct", pMetricsBuilder->vbat1.c_str());
    zassert(strcmp(pMetricsBuilder->vbat2.c_str(), "abcd")==0, 
                "vbat2 %s not correct", pMetricsBuilder->vbat2.c_str());
    zassert(strcmp(pMetricsBuilder->vdd_det.c_str(), "abcd")==0, 
                "vdd_det %s not correct", pMetricsBuilder->vdd_det.c_str());
    zassert(strcmp(pMetricsBuilder->vdd_int.c_str(), "abcd")==0, 
                "vdd_int %s not correct", pMetricsBuilder->vdd_int.c_str());
    zassert(strcmp(pMetricsBuilder->mdm_batv.c_str(), "abcd")==0, 
                "mdm_batv %s not correct", pMetricsBuilder->mdm_batv.c_str());
    zassert(strcmp(pMetricsBuilder->mdm_temp.c_str(), "abcd")==0, 
                "mdm_temp %s not correct", pMetricsBuilder->mdm_temp.c_str());
    zassert(strcmp(pMetricsBuilder->mdm_link.c_str(), "abcd")==0, 
                "mdm_link %s not correct", pMetricsBuilder->mdm_link.c_str());
    zassert(strcmp(pMetricsBuilder->psm_tau.c_str(), "abcd")==0, 
                "psm_tau %s not correct", pMetricsBuilder->psm_tau.c_str());
    zassert(strcmp(pMetricsBuilder->temperature.c_str(), "abcd")==0, 
                "temperature %s not correct", pMetricsBuilder->temperature.c_str());
    zassert(strcmp(pMetricsBuilder->humidity.c_str(), "abcd")==0, 
                "humidity %s not correct", pMetricsBuilder->humidity.c_str());
    zassert(strcmp(pMetricsBuilder->payload.c_str(), "{abcd")==0, 
                "payload %s not correct", pMetricsBuilder->payload.c_str());

    pMetricsBuilder->reset();

    zassert(strcmp(pMetricsBuilder->vbat1.c_str(), "")==0, 
                "vbat1 %s not correct", pMetricsBuilder->vbat1.c_str());
    zassert(strcmp(pMetricsBuilder->vbat2.c_str(), "")==0, 
                "vbat2 %s not correct", pMetricsBuilder->vbat2.c_str());
    zassert(strcmp(pMetricsBuilder->vdd_det.c_str(), "")==0, 
                "vdd_det %s not correct", pMetricsBuilder->vdd_det.c_str());
    zassert(strcmp(pMetricsBuilder->vdd_int.c_str(), "")==0, 
                "vdd_int %s not correct", pMetricsBuilder->vdd_int.c_str());
    zassert(strcmp(pMetricsBuilder->mdm_batv.c_str(), "")==0, 
                "mdm_batv %s not correct", pMetricsBuilder->mdm_batv.c_str());
    zassert(strcmp(pMetricsBuilder->mdm_temp.c_str(), "")==0, 
                "mdm_temp %s not correct", pMetricsBuilder->mdm_temp.c_str());
    zassert(strcmp(pMetricsBuilder->mdm_link.c_str(), "abcd")==0, 
                "mdm_link %s not correct", pMetricsBuilder->mdm_link.c_str());
    zassert(strcmp(pMetricsBuilder->psm_tau.c_str(), "abcd")==0, 
                "psm_tau %s not correct", pMetricsBuilder->psm_tau.c_str());
    zassert(strcmp(pMetricsBuilder->temperature.c_str(), "")==0, 
                "temperature %s not correct", pMetricsBuilder->temperature.c_str());
    zassert(strcmp(pMetricsBuilder->humidity.c_str(), "")==0, 
                "humidity %s not correct", pMetricsBuilder->humidity.c_str());
    zassert(strcmp(pMetricsBuilder->payload.c_str(), "{")==0, 
                "payload %s not correct", pMetricsBuilder->payload.c_str());

	delete pMetricsBuilder; 
}

