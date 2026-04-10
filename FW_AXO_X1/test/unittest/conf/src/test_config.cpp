#include <zephyr/ztest.h>
#include <vector>
#include <algorithm>

#define private public
#include "conf/config.h"

#include "stubs.h"

ZTEST_SUITE(tests_conf_config, NULL, NULL, NULL, NULL, NULL);

// test constructor and destructor
ZTEST(tests_conf_config, test_conf_config_constructor)
{
	Config* pConfig = new Config();

	zassert_not_null(pConfig, "memory alloc failed");

	delete pConfig;
}

// MISSING for 100% branch coverage static instantiation does not deliver NULL
// here code line 
// static Config myInstance;
// LOG_DBG("%s", "Config::~Config");
// are not covered completly.

// test function loadFromFile()
ZTEST(tests_conf_config, test_conf_config_loadFromFile)
{
	int rc;

	gbRetVal = true;
	strcpy(gBuf, "not_default: metrics:123");

	std::string strConfig = "default: metrics:123"; 
	rc = Config::inst()->loadFromFile(strConfig);
	zassert_equal(rc, 0, "rc != 0");
    zassert(strcmp(Config::inst()->input.c_str(), "not_default: metrics:123")==0, "input %s not correct", Config::inst()->input.c_str());

	gbRetVal = false;

	rc = Config::inst()->loadFromFile(strConfig);
	zassert_equal(rc, 0, "rc != 0");
    zassert(strcmp(Config::inst()->input.c_str(), "default: metrics:123")==0, "input %s not correct", Config::inst()->input.c_str());
}

// test function saveToFile(...)
ZTEST(tests_conf_config, test_conf_config_saveToFile1)
{
	int rc;

	gbRetVal = true;
	strcpy(gBuf, "abcdefg");

	std::string strConfig = "123  456\n\n"; 
	
	rc = Config::inst()->saveToFile(strConfig);
	zassert_equal(rc, 0, "rc != 0");
    zassert(strcmp(gBuf, "abcdefg")==0, "gBuf %s not correct", gBuf);

	gbRetVal = false;
	rc = Config::inst()->saveToFile(strConfig);
	zassert_equal(rc, 0, "rc != 0");
    zassert(strcmp(gBuf, "123456")==0, "gBuf %s not correct", gBuf);
}

//"{\"general\":[{\"name\":\"axo2\",\"type\":\"9160\",\"version\":\"0.8.0\"}]}";
// test function parse()
ZTEST(tests_conf_config, test_conf_config_parse)
{
	int rc;

	Config::inst()->input = "{\"general\":[{\"name\":\"axo2\",\"type\":\"9160\",\"version\":\"0.8.0\"}]}";

	rc = Config::inst()->parse(&Config::inst()->root_obj, Config::inst()->input);
	zassert_equal(rc, 0, "rc != 0");
	zassert_not_null(Config::inst()->root_obj, "root_obj is NULL");

	rc = Config::inst()->parse(&Config::inst()->root_obj, Config::inst()->input);
	zassert_equal(rc, 0, "rc != 0");
	zassert_not_null(Config::inst()->root_obj, "root_obj is NULL");
}

// test function saveToFile()
ZTEST(tests_conf_config, test_conf_config_saveToFile2)
{
	int rc;

	gbRetVal = false;
	Config::inst()->input = "{\"general\":[{\"name\":\"axo2\",\"type\":\"9160\",\"version\":\"0.8.0\"}]}";

	rc = Config::inst()->parse(&Config::inst()->root_obj, Config::inst()->input);
	zassert_equal(rc, 0, "rc != 0");
	zassert_not_null(Config::inst()->root_obj, "root_obj is NULL");

	rc = Config::inst()->saveToFile();
	zassert_equal(rc, 0, "rc != 0");
    zassert(strcmp(gBuf, "{\"general\":[{\"name\":\"axo2\",\"type\":\"9160\",\"version\":\"0.8.0\"}]}")==0, 
	        "gBuf %s not correct", gBuf);
}

// test function deleteConfigFile
ZTEST(tests_conf_config, test_conf_config_deleteConfigFile)
{
	gbRetVal = true;
	Config::inst()->deleteConfigFile();

	gbRetVal = false;
	Config::inst()->deleteConfigFile();
}

// test function save
ZTEST(tests_conf_config, test_conf_config_save)
{
	int rc;

	gbRetVal = false;
	Config::inst()->input = "{\"general\":[{\"name\":\"axo2\",\"type\":\"9160\",\"version\":\"0.8.0\"}]}";

	rc = Config::inst()->parse(&Config::inst()->root_obj, Config::inst()->input);
	zassert_equal(rc, 0, "rc != 0");
	zassert_not_null(Config::inst()->root_obj, "root_obj is NULL");

	Config::inst()->input = "12345";
    zassert(strcmp(Config::inst()->input.c_str(), "12345")==0, 
	        "input %s not correct", Config::inst()->input.c_str());

	rc = Config::inst()->save();
	zassert_equal(rc, 0, "rc != 0");
    zassert(strcmp(Config::inst()->input.c_str(), "{\"general\":[{\"name\":\"axo2\",\"type\":\"9160\",\"version\":\"0.8.0\"}]}")==0, 
	        "input %s not correct", Config::inst()->input.c_str());
}

// test function dump
ZTEST(tests_conf_config, test_conf_config_dump)
{
	int rc;

	gbRetVal = false;
	Config::inst()->input = "{\"general\":[{\"name\":\"axo2\",\"type\":\"9160\",\"version\":\"0.8.0\"}]}";

	rc = Config::inst()->parse(&Config::inst()->root_obj, Config::inst()->input);
	zassert_equal(rc, 0, "rc != 0");
	zassert_not_null(Config::inst()->root_obj, "root_obj is NULL");

	Config::inst()->dump();
}

// test function getArraySize
ZTEST(tests_conf_config, test_conf_config_getArraySize)
{
	int rc;

	gbRetVal = false;
	Config::inst()->input = "{\"general\":[{\"name\":\"axo2\",\"type\":\"9160\",\"version\":\"0.8.0\"}]}";

	rc = Config::inst()->parse(&Config::inst()->root_obj, Config::inst()->input);
	zassert_equal(rc, 0, "rc != 0");
	zassert_not_null(Config::inst()->root_obj, "root_obj is NULL");

	rc = Config::inst()->getArraySize(Config::inst()->root_obj);
	printk("rc = %d\n", rc);
	zassert_equal(rc, 1, "rc != 1");
}

// test function groupExists
ZTEST(tests_conf_config, test_conf_config_groupExists)
{
	int rc;
	bool bRet;

	gbRetVal = false;
	Config::inst()->input = "{\"general\":[{\"name\":\"axo2\",\"type\":\"9160\",\"version\":\"0.8.0\"}]}";

	rc = Config::inst()->parse(&Config::inst()->root_obj, Config::inst()->input);
	zassert_equal(rc, 0, "rc != 0");
	zassert_not_null(Config::inst()->root_obj, "root_obj is NULL");

	bRet = Config::inst()->groupExists(Config::inst()->root_obj, "general");
	zassert_true(bRet, "bRet != true");
	bRet = Config::inst()->groupExists(Config::inst()->root_obj, "general2");
	zassert_false(bRet, "bRet != false");
}

// test function getGroupEntries
ZTEST(tests_conf_config, test_conf_config_getGroupEntries)
{
	int rc;

	gbRetVal = false;
	Config::inst()->input = "{\"general\":[{\"name\":\"axo2\",\"type\":\"9160\",\"version\":\"0.8.0\"}]}";

	rc = Config::inst()->parse(&Config::inst()->root_obj, Config::inst()->input);
	zassert_equal(rc, 0, "rc != 0");
	zassert_not_null(Config::inst()->root_obj, "root_obj is NULL");

	rc = Config::inst()->getGroupEntries("general");
	printk("rc = %d\n", rc);
	zassert_equal(rc, 1, "rc != 1");
	rc = Config::inst()->getGroupEntries("general2");
	zassert_equal(rc, 0, "rc != 0");
}

// test function parseAllValues
ZTEST(tests_conf_config, test_conf_config_parseAllValues)
{
	int rc;

	gbRetVal = false;
	Config::inst()->input = "{\"general\":[{\"name\":\"axo2\",\"type\":\"9160\",\"version\":\"0.8.0\"}],\"sensors\":[{\"name\":\"adxl372\",\"type\":\"accel\",\"actthres\":\"200\",\"deactthres\":\"500\"}],\"metrics\":[{\"interval\":\"120\"}],\"displacement\":[{\"b1\":\"1.0\"}],\"datalog\":[{\"dispmodeoff\":\"false\",\"rawtransferon\":\"true\",\"disptransferon\":\"true\",\"rms_thres\":\"65\",\"max_raw_per_day\":\"5\"}]}";

	rc = Config::inst()->parse(&Config::inst()->root_obj, Config::inst()->input);
	zassert_equal(rc, 0, "rc != 0");
	zassert_not_null(Config::inst()->root_obj, "root_obj is NULL");

	Config::inst()->parseAllValues();
    zassert(strcmp(Config::inst()->cfgGeneral.name.c_str(), "axo2")==0, 
	        "cfgGeneral.name %s not correct", Config::inst()->cfgGeneral.name.c_str());
    zassert(strcmp(Config::inst()->cfgGeneral.type.c_str(), "9160")==0, 
	        "cfgGeneral.type %s not correct", Config::inst()->cfgGeneral.type.c_str());
    zassert(strcmp(Config::inst()->cfgGeneral.version.c_str(), "0.8.0")==0, 
	        "cfgGeneral.version %s not correct", Config::inst()->cfgGeneral.version.c_str());

    zassert(strcmp(Config::inst()->cfgSensor[eSensor_adxl372].name.c_str(), "adxl372")==0, 
	        "cfgSensor[eSensor_adxl372].name %s not correct", 
			Config::inst()->cfgSensor[eSensor_adxl372].name.c_str());
    zassert(strcmp(Config::inst()->cfgSensor[eSensor_adxl372].type.c_str(), "accel")==0, 
	        "cfgSensor[eSensor_adxl372].type %s not correct", 
			Config::inst()->cfgSensor[eSensor_adxl372].type.c_str());
    zassert(strcmp(Config::inst()->cfgSensor[eSensor_adxl372].actthres.c_str(), "200")==0, 
	        "cfgSensor[eSensor_adxl372].actthres %s not correct", 
			Config::inst()->cfgSensor[eSensor_adxl372].actthres.c_str());
    zassert(strcmp(Config::inst()->cfgSensor[eSensor_adxl372].deactthres.c_str(), "500")==0, 
	        "cfgSensor[eSensor_adxl372].deactthres %s not correct", 
			Config::inst()->cfgSensor[eSensor_adxl372].deactthres.c_str());

    zassert(strcmp(Config::inst()->cfgMetrics.interval.c_str(), "120")==0, 
	        "cfgMetrics.interval %s not correct", 
			Config::inst()->cfgMetrics.interval.c_str());

    zassert(strcmp(Config::inst()->cfgDisplacement.b1.c_str(), "1.0")==0, 
	        "cfgDisplacement.b1 %s not correct", 
			Config::inst()->cfgDisplacement.b1.c_str());

    zassert(strcmp(Config::inst()->cfgDatalog.dispmodeoff.c_str(), "false")==0, 
	        "cfgDatalog.dispmodeoff %s not correct", 
			Config::inst()->cfgDatalog.dispmodeoff.c_str());
    zassert(strcmp(Config::inst()->cfgDatalog.rawtransferon.c_str(), "true")==0, 
	        "cfgDatalog.rawtransferon %s not correct", 
			Config::inst()->cfgDatalog.rawtransferon.c_str());
    zassert(strcmp(Config::inst()->cfgDatalog.disptransferon.c_str(), "true")==0, 
	        "cfgDatalog.disptransferon %s not correct", 
			Config::inst()->cfgDatalog.disptransferon.c_str());
	printk("%s\n\n", Config::inst()->cfgDatalog.rms_thres.c_str());
    zassert(strcmp(Config::inst()->cfgDatalog.rms_thres.c_str(), "65")==0, 
	        "cfgDatalog.rms_thres %s not correct", 
			Config::inst()->cfgDatalog.rms_thres.c_str());
    zassert(strcmp(Config::inst()->cfgDatalog.max_raw_per_day.c_str(), "5")==0, 
	        "cfgDatalog.max_raw_per_day %s not correct", 
			Config::inst()->cfgDatalog.max_raw_per_day.c_str());
}

// test function parseAllValues
ZTEST(tests_conf_config, test_conf_config_parseAllValues2)
{
	int rc;

	gbRetVal = false;
	Config::inst()->input = "{\"general\":[{\"name\":\"INVALID\",\"type\":\"9160\",\"version\":\"0.1.0\"}],\"sensors\":[{\"name\":\"adxl372\",\"type\":\"accel\",\"actthres\":\"20\",\"deactthres\":\"51100\"}],\"metrics\":[{\"interval\":\"2\"}],\"displacement\":[{\"b1\":\"1.0\"}],\"datalog\":[{\"dispmodeoff\":\"true\",\"rawtransferon\":\"true\",\"disptransferon\":\"true\",\"rms_thres\":\"65\",\"max_raw_per_day\":\"5\"}]}";

	rc = Config::inst()->parse(&Config::inst()->root_obj, Config::inst()->input);
	zassert_equal(rc, 0, "rc != 0");
	zassert_not_null(Config::inst()->root_obj, "root_obj is NULL");

	Config::inst()->parseAllValues();
    zassert(strcmp(Config::inst()->cfgGeneral.name.c_str(), "INVALID")==0, 
	        "cfgGeneral.name %s not correct", Config::inst()->cfgGeneral.name.c_str());
    zassert(strcmp(Config::inst()->cfgGeneral.type.c_str(), "9160")==0, 
	        "cfgGeneral.type %s not correct", Config::inst()->cfgGeneral.type.c_str());
    zassert(strcmp(Config::inst()->cfgGeneral.version.c_str(), "0.1.0")==0, 
	        "cfgGeneral.version %s not correct", Config::inst()->cfgGeneral.version.c_str());

    zassert(strcmp(Config::inst()->cfgSensor[eSensor_adxl372].name.c_str(), "adxl372")==0, 
	        "cfgSensor[eSensor_adxl372].name %s not correct", 
			Config::inst()->cfgSensor[eSensor_adxl372].name.c_str());
    zassert(strcmp(Config::inst()->cfgSensor[eSensor_adxl372].type.c_str(), "accel")==0, 
	        "cfgSensor[eSensor_adxl372].type %s not correct", 
			Config::inst()->cfgSensor[eSensor_adxl372].type.c_str());
    zassert(strcmp(Config::inst()->cfgSensor[eSensor_adxl372].actthres.c_str(), "200")==0, 
	        "cfgSensor[eSensor_adxl372].actthres %s not correct", 
			Config::inst()->cfgSensor[eSensor_adxl372].actthres.c_str());
    zassert(strcmp(Config::inst()->cfgSensor[eSensor_adxl372].deactthres.c_str(), "50000")==0, 
	        "cfgSensor[eSensor_adxl372].deactthres %s not correct", 
			Config::inst()->cfgSensor[eSensor_adxl372].deactthres.c_str());

    zassert(strcmp(Config::inst()->cfgMetrics.interval.c_str(), "5")==0, 
	        "cfgMetrics.interval %s not correct", 
			Config::inst()->cfgMetrics.interval.c_str());

    zassert(strcmp(Config::inst()->cfgDisplacement.b1.c_str(), "1.0")==0, 
	        "cfgDisplacement.b1 %s not correct", 
			Config::inst()->cfgDisplacement.b1.c_str());

    zassert(strcmp(Config::inst()->cfgDatalog.dispmodeoff.c_str(), "true")==0, 
	        "cfgDatalog.dispmodeoff %s not correct", 
			Config::inst()->cfgDatalog.dispmodeoff.c_str());
    zassert(strcmp(Config::inst()->cfgDatalog.rawtransferon.c_str(), "true")==0, 
	        "cfgDatalog.rawtransferon %s not correct", 
			Config::inst()->cfgDatalog.rawtransferon.c_str());
    zassert(strcmp(Config::inst()->cfgDatalog.disptransferon.c_str(), "false")==0, 
	        "cfgDatalog.disptransferon %s not correct", 
			Config::inst()->cfgDatalog.disptransferon.c_str());
	printk("%s\n\n", Config::inst()->cfgDatalog.rms_thres.c_str());
    zassert(strcmp(Config::inst()->cfgDatalog.rms_thres.c_str(), "65")==0, 
	        "cfgDatalog.rms_thres %s not correct", 
			Config::inst()->cfgDatalog.rms_thres.c_str());
    zassert(strcmp(Config::inst()->cfgDatalog.max_raw_per_day.c_str(), "5")==0, 
	        "cfgDatalog.max_raw_per_day %s not correct", 
			Config::inst()->cfgDatalog.max_raw_per_day.c_str());
}

// test function parseAllValues
ZTEST(tests_conf_config, test_conf_config_parseAllValues3)
{
	int rc;

	gbRetVal = false;
	Config::inst()->input = "{\"general\":[{\"name\":\"axo2\",\"type\":\"9160\",\"version\":\"0.8.0\"}],\"sensors\":[{\"name\":\"adxl372\",\"type\":\"accel\",\"actthres\":\"50800\",\"deactthres\":\"500\"}],\"metrics\":[{\"interval\":\"20080\"}],\"displacement\":[{\"b1\":\"1.0\"}],\"datalog\":[{\"dispmodeoff\":\"true\",\"rawtransferon\":\"true\",\"disptransferon\":\"false\",\"rms_thres\":\"65\",\"max_raw_per_day\":\"5\"}]}";

	rc = Config::inst()->parse(&Config::inst()->root_obj, Config::inst()->input);
	zassert_equal(rc, 0, "rc != 0");
	zassert_not_null(Config::inst()->root_obj, "root_obj is NULL");

	Config::inst()->parseAllValues();
    zassert(strcmp(Config::inst()->cfgGeneral.name.c_str(), "axo2")==0, 
	        "cfgGeneral.name %s not correct", Config::inst()->cfgGeneral.name.c_str());
    zassert(strcmp(Config::inst()->cfgGeneral.type.c_str(), "9160")==0, 
	        "cfgGeneral.type %s not correct", Config::inst()->cfgGeneral.type.c_str());
    zassert(strcmp(Config::inst()->cfgGeneral.version.c_str(), "0.8.0")==0, 
	        "cfgGeneral.version %s not correct", Config::inst()->cfgGeneral.version.c_str());

    zassert(strcmp(Config::inst()->cfgSensor[eSensor_adxl372].name.c_str(), "adxl372")==0, 
	        "cfgSensor[eSensor_adxl372].name %s not correct", 
			Config::inst()->cfgSensor[eSensor_adxl372].name.c_str());
    zassert(strcmp(Config::inst()->cfgSensor[eSensor_adxl372].type.c_str(), "accel")==0, 
	        "cfgSensor[eSensor_adxl372].type %s not correct", 
			Config::inst()->cfgSensor[eSensor_adxl372].type.c_str());

    zassert(strcmp(Config::inst()->cfgSensor[eSensor_adxl372].actthres.c_str(), "49800")==0, 
	        "cfgSensor[eSensor_adxl372].actthres %s not correct", 
			Config::inst()->cfgSensor[eSensor_adxl372].actthres.c_str());
			//HIER HIER HIER FEHLER !!!!!!!!!!!!
    zassert(strcmp(Config::inst()->cfgSensor[eSensor_adxl372].deactthres.c_str(), "50000")==0, 
	        "cfgSensor[eSensor_adxl372].deactthres %s not correct", 
			Config::inst()->cfgSensor[eSensor_adxl372].deactthres.c_str());

    zassert(strcmp(Config::inst()->cfgMetrics.interval.c_str(), "10080")==0, 
	        "cfgMetrics.interval %s not correct", 
			Config::inst()->cfgMetrics.interval.c_str());

    zassert(strcmp(Config::inst()->cfgDisplacement.b1.c_str(), "1.0")==0, 
	        "cfgDisplacement.b1 %s not correct", 
			Config::inst()->cfgDisplacement.b1.c_str());

    //zassert(strcmp(Config::inst()->cfgDatalog.dispmodeoff.c_str(), "false")==0, 
    zassert(strcmp(Config::inst()->cfgDatalog.dispmodeoff.c_str(), "true")==0, 
	        "cfgDatalog.dispmodeoff %s not correct", 
			Config::inst()->cfgDatalog.dispmodeoff.c_str());
    zassert(strcmp(Config::inst()->cfgDatalog.rawtransferon.c_str(), "true")==0, 
	        "cfgDatalog.rawtransferon %s not correct", 
			Config::inst()->cfgDatalog.rawtransferon.c_str());
    zassert(strcmp(Config::inst()->cfgDatalog.disptransferon.c_str(), "false")==0, 
	        "cfgDatalog.disptransferon %s not correct", 
			Config::inst()->cfgDatalog.disptransferon.c_str());
	printk("%s\n\n", Config::inst()->cfgDatalog.rms_thres.c_str());
    zassert(strcmp(Config::inst()->cfgDatalog.rms_thres.c_str(), "65")==0, 
	        "cfgDatalog.rms_thres %s not correct", 
			Config::inst()->cfgDatalog.rms_thres.c_str());
    zassert(strcmp(Config::inst()->cfgDatalog.max_raw_per_day.c_str(), "5")==0, 
	        "cfgDatalog.max_raw_per_day %s not correct", 
			Config::inst()->cfgDatalog.max_raw_per_day.c_str());
}

// test function keyExists
ZTEST(tests_conf_config, test_conf_config_keyExists)
{
	int rc;
	bool bRet;

	Config::inst()->input = "{\"general\":[{\"name\":\"axo2\",\"type\":\"9160\",\"version\":\"0.8.0\"}]}";

	rc = Config::inst()->parse(&Config::inst()->root_obj, Config::inst()->input);
	zassert_equal(rc, 0, "rc != 0");
	zassert_not_null(Config::inst()->root_obj, "root_obj is NULL");

	bRet = Config::inst()->keyExists(Config::inst()->root_obj, "general", 0, "name");
	zassert_true(bRet, "bRet != true");
	bRet = Config::inst()->keyExists(Config::inst()->root_obj, "general", 0, "nixname");
	zassert_false(bRet, "bRet != false");
	bRet = Config::inst()->keyExists(Config::inst()->root_obj, "general2", 0, "name");
	zassert_false(bRet, "bRet != false");
}

// test function getValue() and getValue(...)
ZTEST(tests_conf_config, test_conf_config_getValue)
{
	int rc;
	std::string sRet;

	Config::inst()->input = "{\"general\":[{\"name\":\"axo2\",\"type\":\"9160\",\"version\":\"0.8.0\"}]}";

	rc = Config::inst()->parse(&Config::inst()->root_obj, Config::inst()->input);
	zassert_equal(rc, 0, "rc != 0");
	zassert_not_null(Config::inst()->root_obj, "root_obj is NULL");

	sRet = Config::inst()->getValue("general", 0, "name");
    zassert(strcmp(sRet.c_str(), "axo2")==0, "sRet %s not correct", sRet.c_str());
	sRet = Config::inst()->getValue(Config::inst()->root_obj, "general", 0, "version");
    zassert(strcmp(sRet.c_str(), "0.8.0")==0, "sRet %s not correct", sRet.c_str());
	sRet = Config::inst()->getValue(Config::inst()->root_obj, "general2", 0, "version");
    zassert(strcmp(sRet.c_str(), "")==0, "sRet %s not correct", sRet.c_str());
	sRet = Config::inst()->getValue(Config::inst()->root_obj, "general", 0, "version2");
    zassert(strcmp(sRet.c_str(), "")==0, "sRet %s not correct", sRet.c_str());
}

// test function setValue() and setValue(...)
ZTEST(tests_conf_config, test_conf_config_setValue)
{
	int rc;
	std::string sRet;

	Config::inst()->input = "{\"general\":[{\"name\":\"axo2\",\"type\":\"9160\",\"version\":\"0.8.0\"}]}";

	rc = Config::inst()->parse(&Config::inst()->root_obj, Config::inst()->input);
	zassert_equal(rc, 0, "rc != 0");
	zassert_not_null(Config::inst()->root_obj, "root_obj is NULL");

	rc = Config::inst()->setValue("general", 0, "name", "newname");
	zassert_equal(rc, 0, "rc != 0");
	rc = Config::inst()->setValue(Config::inst()->root_obj, "general", 0, "version", "9.9.9");
	zassert_equal(rc, 0, "rc != 0");

	sRet = Config::inst()->getValue("general", 0, "name");
    zassert(strcmp(sRet.c_str(), "newname")==0, "sRet %s not correct", sRet.c_str());
	sRet = Config::inst()->getValue("general", 0, "version");
    zassert(strcmp(sRet.c_str(), "9.9.9")==0, "sRet %s not correct", sRet.c_str());

	rc = Config::inst()->setValue(Config::inst()->root_obj, "general2", 0, "version", "9.9.9");
	zassert_equal(rc, -1, "rc != -1");
	rc = Config::inst()->setValue(Config::inst()->root_obj, "general", 0, "version2", "9.9.9");
	zassert_equal(rc, -1, "rc != -1");
}

// test function getCfgGeneral, getCfgSensor, getCfgMetrics, getCfgDisplacement, getCfgDatalog
ZTEST(tests_conf_config, test_conf_config_getCfgGeneral)
{
	int rc;

	gbRetVal = false;
	Config::inst()->input = "{\"general\":[{\"name\":\"axo2\",\"type\":\"9160\",\"version\":\"0.8.0\"}],\"sensors\":[{\"name\":\"adxl372\",\"type\":\"accel\",\"actthres\":\"200\",\"deactthres\":\"500\"}],\"metrics\":[{\"interval\":\"120\"}],\"displacement\":[{\"b1\":\"1.0\"}],\"datalog\":[{\"dispmodeoff\":\"false\",\"rawtransferon\":\"true\",\"disptransferon\":\"true\",\"rms_thres\":\"65\",\"max_raw_per_day\":\"5\"}]}";

	rc = Config::inst()->parse(&Config::inst()->root_obj, Config::inst()->input);
	zassert_equal(rc, 0, "rc != 0");
	zassert_not_null(Config::inst()->root_obj, "root_obj is NULL");

	Config::inst()->parseAllValues();
    zassert(strcmp(Config::inst()->cfgGeneral.name.c_str(), "axo2")==0, 
	        "cfgGeneral.name %s not correct", Config::inst()->cfgGeneral.name.c_str());
    zassert(strcmp(Config::inst()->cfgGeneral.type.c_str(), "9160")==0, 
	        "cfgGeneral.type %s not correct", Config::inst()->cfgGeneral.type.c_str());
    zassert(strcmp(Config::inst()->cfgGeneral.version.c_str(), "0.8.0")==0, 
	        "cfgGeneral.version %s not correct", Config::inst()->cfgGeneral.version.c_str());

    zassert(strcmp(Config::inst()->cfgSensor[eSensor_adxl372].name.c_str(), "adxl372")==0, 
	        "cfgSensor[eSensor_adxl372].name %s not correct", 
			Config::inst()->cfgSensor[eSensor_adxl372].name.c_str());
    zassert(strcmp(Config::inst()->cfgSensor[eSensor_adxl372].type.c_str(), "accel")==0, 
	        "cfgSensor[eSensor_adxl372].type %s not correct", 
			Config::inst()->cfgSensor[eSensor_adxl372].type.c_str());
    zassert(strcmp(Config::inst()->cfgSensor[eSensor_adxl372].actthres.c_str(), "200")==0, 
	        "cfgSensor[eSensor_adxl372].actthres %s not correct", 
			Config::inst()->cfgSensor[eSensor_adxl372].actthres.c_str());
    zassert(strcmp(Config::inst()->cfgSensor[eSensor_adxl372].deactthres.c_str(), "500")==0, 
	        "cfgSensor[eSensor_adxl372].deactthres %s not correct", 
			Config::inst()->cfgSensor[eSensor_adxl372].deactthres.c_str());

    zassert(strcmp(Config::inst()->cfgMetrics.interval.c_str(), "120")==0, 
	        "cfgMetrics.interval %s not correct", 
			Config::inst()->cfgMetrics.interval.c_str());

    zassert(strcmp(Config::inst()->cfgDisplacement.b1.c_str(), "1.0")==0, 
	        "cfgDisplacement.b1 %s not correct", 
			Config::inst()->cfgDisplacement.b1.c_str());

    zassert(strcmp(Config::inst()->cfgDatalog.dispmodeoff.c_str(), "false")==0, 
	        "cfgDatalog.dispmodeoff %s not correct", 
			Config::inst()->cfgDatalog.dispmodeoff.c_str());
    zassert(strcmp(Config::inst()->cfgDatalog.rawtransferon.c_str(), "true")==0, 
	        "cfgDatalog.rawtransferon %s not correct", 
			Config::inst()->cfgDatalog.rawtransferon.c_str());
    zassert(strcmp(Config::inst()->cfgDatalog.disptransferon.c_str(), "true")==0, 
	        "cfgDatalog.disptransferon %s not correct", 
			Config::inst()->cfgDatalog.disptransferon.c_str());
    zassert(strcmp(Config::inst()->cfgDatalog.rms_thres.c_str(), "65")==0, 
	        "cfgDatalog.rms_thres %s not correct", 
			Config::inst()->cfgDatalog.rms_thres.c_str());
    zassert(strcmp(Config::inst()->cfgDatalog.max_raw_per_day.c_str(), "5")==0, 
	        "cfgDatalog.max_raw_per_day %s not correct", 
			Config::inst()->cfgDatalog.max_raw_per_day.c_str());

	stCfgGeneral* p = Config::inst()->getCfgGeneral();
    zassert(strcmp(p->name.c_str(), "axo2")==0, "p->name %s not correct", p->name.c_str());
    zassert(strcmp(p->type.c_str(), "9160")==0, "p->type %s not correct", p->type.c_str());
    zassert(strcmp(p->version.c_str(), "0.8.0")==0, "p->version %s not correct", p->version.c_str());

	stCfgSensor* p2 = Config::inst()->getCfgSensor(eSensor_adxl372);
    zassert(strcmp(p2->name.c_str(), "adxl372")==0, "p2->name %s not correct", p2->name.c_str());
    zassert(strcmp(p2->type.c_str(), "accel")==0, "p2->type %s not correct", p2->type.c_str());
    zassert(strcmp(p2->actthres.c_str(), "200")==0,  "p2->actthres %s not correct", p2->actthres.c_str());
    zassert(strcmp(p2->deactthres.c_str(), "500")==0, "p2->deactthres %s not correct", p2->deactthres.c_str());

	stCfgMetrics* p3 = Config::inst()->getCfgMetrics();
    zassert(strcmp(p3->interval.c_str(), "120")==0, "p3->interval %s not correct", p3->interval.c_str());

	stCfgDisplacement* p4 = Config::inst()->getCfgDisplacement();
    zassert(strcmp(p4->b1.c_str(), "1.0")==0, "p4->b1 %s not correct", p4->b1.c_str());

	stCfgDatalog* p5 = Config::inst()->getCfgDatalog();
    zassert(strcmp(p5->dispmodeoff.c_str(), "false")==0, "p5->dispmodeoff %s not correct", p5->dispmodeoff.c_str());
    zassert(strcmp(p5->rawtransferon.c_str(), "true")==0, "p5->rawtransferon %s not correct", p5->rawtransferon.c_str());
    zassert(strcmp(p5->disptransferon.c_str(), "true")==0, "p5->disptransferon %s not correct", p5->disptransferon.c_str());
    zassert(strcmp(p5->rms_thres.c_str(), "65")==0, "p5->rms_thres %s not correct", p5->rms_thres.c_str());
    zassert(strcmp(p5->max_raw_per_day.c_str(), "5")==0, "p5->max_raw_per_day %s not correct", p5->max_raw_per_day.c_str());
}

// test function getConfigJson
ZTEST(tests_conf_config, test_conf_config_getConfigJson)
{
	int rc;
	std::string sRet;

	Config::inst()->input = "{\"general\":[{\"name\":\"axo2\",\"type\":\"9160\",\"version\":\"0.8.0\"}]}";

	rc = Config::inst()->parse(&Config::inst()->root_obj, Config::inst()->input);
	zassert_equal(rc, 0, "rc != 0");
	zassert_not_null(Config::inst()->root_obj, "root_obj is NULL");

	sRet = (char*)Config::inst()->getConfigJson();
    zassert(strcmp(sRet.c_str(), "{\"general\":[{\"name\":\"axo2\",\"type\":\"9160\",\"version\":\"0.8.0\"}]}")==0, 
	        "sRet %s not correct", sRet.c_str());
}

// test function areValuesStrings
ZTEST(tests_conf_config, test_conf_config_areValuesStrings)
{
	int rc;
	bool bRet;

	Config::inst()->input = "{\"general\":[{\"name\":\"axo2\",\"type\":\"9160\",\"version\":\"0.8.0\"}]}";

	rc = Config::inst()->parse(&Config::inst()->root_obj, Config::inst()->input);
	zassert_equal(rc, 0, "rc != 0");
	zassert_not_null(Config::inst()->root_obj, "root_obj is NULL");

	bRet = Config::inst()->areValuesStrings("general");
    zassert_true(bRet, "bRet != true");
	bRet = Config::inst()->areValuesStrings("general2");
    zassert_false(bRet, "bRet != false");
}

// test function areValuesStrings
ZTEST(tests_conf_config, test_conf_config_areValuesStrings2)
{
	int rc;
	bool bRet;

	Config::inst()->input = "{\"general\":[{\"name\":\"axo2\",\"type\":9160,\"version\":\"0.8.0\"}]}";

	rc = Config::inst()->parse(&Config::inst()->root_obj, Config::inst()->input);
	zassert_equal(rc, 0, "rc != 0");
	zassert_not_null(Config::inst()->root_obj, "root_obj is NULL");

	bRet = Config::inst()->areValuesStrings("general");
    zassert_false(bRet, "bRet != false");
}

// test function mergeConfig
ZTEST(tests_conf_config, test_conf_config_mergeConfig)
{
	int rc;
	std::string sRet;
	struct cJSON* load_obj = NULL;

	Config::inst()->input = "{\"general\":[{\"name\":\"axo2\",\"type\":9160,\"version\":\"0.8.0\"}]}";

	rc = Config::inst()->parse(&Config::inst()->root_obj, Config::inst()->input);
	zassert_equal(rc, 0, "rc != 0");
	zassert_not_null(Config::inst()->root_obj, "root_obj is NULL");

	rc = Config::inst()->parse(&load_obj, Config::inst()->input);
	zassert_equal(rc, 0, "rc != 0");
	zassert_not_null(load_obj, "load_obj is NULL");

	Config::inst()->mergeConfig(Config::inst()->root_obj, load_obj);

	sRet = (char*)Config::inst()->getConfigJson();

    zassert(strcmp(Config::inst()->input.c_str(), sRet.c_str())==0, "sRet %s not correct", sRet.c_str());
}

// test function mergeConfig
ZTEST(tests_conf_config, test_conf_config_mergeConfig2)
{
	int rc;
	std::string sRet;
	struct cJSON* load_obj = NULL;

	std::string new_input = "{\"general\":[{\"name\":\"axo2\",\"type\":\"9160\",\"version\":\"0.8.0\"}],\"sensors\":[{\"name\":\"adxl372\",\"type\":\"accel\",\"actthres\":\"217\",\"deactthres\":\"517\"}],\"metrics\":[{\"interval\":\"170\"}],\"displacement\":[{\"b1\":\"7.0\"}],\"datalog\":[{\"dispmodeoff\":\"true\",\"rawtransferon\":\"false\",\"disptransferon\":\"false\",\"rms_thres\":\"17\",\"max_raw_per_day\":\"7\"}]}";;
	Config::inst()->input = "{\"general\":[{\"name\":\"axo2\",\"type\":\"9160\",\"version\":\"0.8.0\"}],\"sensors\":[{\"name\":\"adxl372\",\"type\":\"accel\",\"actthres\":\"200\",\"deactthres\":\"500\"}],\"metrics\":[{\"interval\":\"120\"}],\"displacement\":[{\"b1\":\"1.0\"}],\"datalog\":[{\"dispmodeoff\":\"false\",\"rawtransferon\":\"true\",\"disptransferon\":\"true\",\"rms_thres\":\"65\",\"max_raw_per_day\":\"5\"}]}";;

	rc = Config::inst()->parse(&Config::inst()->root_obj, Config::inst()->input);
	zassert_equal(rc, 0, "rc != 0");
	zassert_not_null(Config::inst()->root_obj, "root_obj is NULL");

	rc = Config::inst()->parse(&load_obj, new_input);
	zassert_equal(rc, 0, "rc != 0");
	zassert_not_null(load_obj, "load_obj is NULL");

	Config::inst()->mergeConfig(Config::inst()->root_obj, load_obj);

	sRet = (char*)Config::inst()->getConfigJson();

    zassert(strcmp(new_input.c_str(), sRet.c_str())==0, "sRet %s not correct", sRet.c_str());
}

// test function mergeConfig
ZTEST(tests_conf_config, test_conf_config_mergeConfig3)
{
	int rc;
	std::string sRet;
	struct cJSON* load_obj = NULL;

	std::string new_input = "{\"general\":[{\"name\":\"axo2\",\"type\":\"9160\",\"version\":\"0.8.0\"}],\"sensors\":[{\"name\":\"adxl372\",\"type\":\"accel\",\"actthres2\":\"217\",\"deactthres2\":\"517\"}],\"metrics\":[{\"interval2\":\"170\"}],\"displacement\":[{\"b12\":\"7.0\"}],\"datalog\":[{\"dispmodeoff2\":\"true\",\"rawtransferon2\":\"false\",\"disptransferon2\":\"false\",\"rms_thres2\":\"17\",\"max_raw_per_day2\":\"7\"}]}";;
	Config::inst()->input = "{\"general\":[{\"name\":\"axo2\",\"type\":\"9160\",\"version\":\"0.8.0\"}],\"sensors\":[{\"name\":\"adxl372\",\"type\":\"accel\",\"actthres\":\"200\",\"deactthres\":\"500\"}],\"metrics\":[{\"interval\":\"120\"}],\"displacement\":[{\"b1\":\"1.0\"}],\"datalog\":[{\"dispmodeoff\":\"false\",\"rawtransferon\":\"true\",\"disptransferon\":\"true\",\"rms_thres\":\"65\",\"max_raw_per_day\":\"5\"}]}";;

	rc = Config::inst()->parse(&Config::inst()->root_obj, Config::inst()->input);
	zassert_equal(rc, 0, "rc != 0");
	zassert_not_null(Config::inst()->root_obj, "root_obj is NULL");

	rc = Config::inst()->parse(&load_obj, new_input);
	zassert_equal(rc, 0, "rc != 0");
	zassert_not_null(load_obj, "load_obj is NULL");

	Config::inst()->mergeConfig(Config::inst()->root_obj, load_obj);

	sRet = (char*)Config::inst()->getConfigJson();

    zassert(strcmp(Config::inst()->input.c_str(), sRet.c_str())==0, "sRet %s not correct", sRet.c_str());
}

// test function isValid
ZTEST(tests_conf_config, test_conf_config_isValid)
{
	bool bRet;
	// 1    1    1    1    1
	const char str[512] = "{\"general\":[{\"name\":\"axo2\",\"type\":\"9160\",\"version\":\"0.8.0\"}],\"sensors\":[{\"name\":\"adxl372\",\"type\":\"accel\",\"actthres\":\"200\",\"deactthres\":\"500\"}],\"metrics\":[{\"interval\":\"120\"}],\"displacement\":[{\"b1\":\"1.0\"}],\"datalog\":[{\"dispmodeoff\":\"false\",\"rawtransferon\":\"true\",\"disptransferon\":\"true\",\"rms_thres\":\"65\",\"max_raw_per_day\":\"5\"}]}";

	bRet = Config::inst()->isValid(str);
    zassert_true(bRet, "bRet != true");

	// 0    0    0    0    0
	const char str2[512] = "{\"general2\":[{\"name\":\"axo2\",\"type\":\"9160\",\"version\":\"0.8.0\"}],\"sensors2\":[{\"name\":\"adxl372\",\"type\":\"accel\",\"actthres\":\"200\",\"deactthres\":\"500\"}],\"metrics2\":[{\"interval\":\"120\"}],\"displacement2\":[{\"b1\":\"1.0\"}],\"datalog2\":[{\"dispmodeoff\":\"false\",\"rawtransferon\":\"true\",\"disptransferon\":\"true\",\"rms_thres\":\"65\",\"max_raw_per_day\":\"5\"}]}";

	bRet = Config::inst()->isValid(str2);
    zassert_false(bRet, "bRet != false");
}

// test function isValid
ZTEST(tests_conf_config, test_conf_config_isValid2)
{
	bool bRet;

	// 0    1    1    1    1
	const char str[512] = "{\"general2\":[{\"name\":\"axo2\",\"type\":\"9160\",\"version\":\"0.8.0\"}],\"sensors\":[{\"name\":\"adxl372\",\"type\":\"accel\",\"actthres\":\"200\",\"deactthres\":\"500\"}],\"metrics\":[{\"interval\":\"120\"}],\"displacement\":[{\"b1\":\"1.0\"}],\"datalog\":[{\"dispmodeoff\":\"false\",\"rawtransferon\":\"true\",\"disptransferon\":\"true\",\"rms_thres\":\"65\",\"max_raw_per_day\":\"5\"}]}";

	bRet = Config::inst()->isValid(str);
    zassert_false(bRet, "bRet != false");

	// 1    0    1    1    1
	const char str2[512] = "{\"general\":[{\"name\":\"axo2\",\"type\":\"9160\",\"version\":\"0.8.0\"}],\"sensors2\":[{\"name\":\"adxl372\",\"type\":\"accel\",\"actthres\":\"200\",\"deactthres\":\"500\"}],\"metrics\":[{\"interval\":\"120\"}],\"displacement\":[{\"b1\":\"1.0\"}],\"datalog\":[{\"dispmodeoff\":\"false\",\"rawtransferon\":\"true\",\"disptransferon\":\"true\",\"rms_thres\":\"65\",\"max_raw_per_day\":\"5\"}]}";

	bRet = Config::inst()->isValid(str2);
    zassert_false(bRet, "bRet != false");

	// 1    1    0    1    1
	const char str3[512] = "{\"general\":[{\"name\":\"axo2\",\"type\":\"9160\",\"version\":\"0.8.0\"}],\"sensors\":[{\"name\":\"adxl372\",\"type\":\"accel\",\"actthres\":\"200\",\"deactthres\":\"500\"}],\"metrics2\":[{\"interval\":\"120\"}],\"displacement\":[{\"b1\":\"1.0\"}],\"datalog\":[{\"dispmodeoff\":\"false\",\"rawtransferon\":\"true\",\"disptransferon\":\"true\",\"rms_thres\":\"65\",\"max_raw_per_day\":\"5\"}]}";

	bRet = Config::inst()->isValid(str3);
    zassert_false(bRet, "bRet != false");

	// 1    1    1    0    1
	const char str5[512] = "{\"general\":[{\"name\":\"axo2\",\"type\":\"9160\",\"version\":\"0.8.0\"}],\"sensors\":[{\"name\":\"adxl372\",\"type\":\"accel\",\"actthres\":\"200\",\"deactthres\":\"500\"}],\"metrics\":[{\"interval\":\"120\"}],\"displacement2\":[{\"b1\":\"1.0\"}],\"datalog\":[{\"dispmodeoff\":\"false\",\"rawtransferon\":\"true\",\"disptransferon\":\"true\",\"rms_thres\":\"65\",\"max_raw_per_day\":\"5\"}]}";

	bRet = Config::inst()->isValid(str5);
    zassert_false(bRet, "bRet != false");

	// 1    1    1    1    0
	const char str6[512] = "{\"general\":[{\"name\":\"axo2\",\"type\":\"9160\",\"version\":\"0.8.0\"}],\"sensors\":[{\"name\":\"adxl372\",\"type\":\"accel\",\"actthres\":\"200\",\"deactthres\":\"500\"}],\"metrics\":[{\"interval\":\"120\"}],\"displacement\":[{\"b1\":\"1.0\"}],\"datalog2\":[{\"dispmodeoff\":\"false\",\"rawtransferon\":\"true\",\"disptransferon\":\"true\",\"rms_thres\":\"65\",\"max_raw_per_day\":\"5\"}]}";

	bRet = Config::inst()->isValid(str6);
    zassert_false(bRet, "bRet != false");
}

// test function isValid, all values strings
ZTEST(tests_conf_config, test_conf_config_isValid3)
{
	bool bRet;

	const char str[512] = "{\"general\":[{\"name\":\"axo2\",\"type\":9160,\"version\":\"0.8.0\"}],\"sensors\":[{\"name\":\"adxl372\",\"type\":\"accel\",\"actthres\":200,\"deactthres\":\"500\"}],\"metrics\":[{\"interval\":120}],\"displacement\":[{\"b1\":1.0}],\"datalog\":[{\"dispmodeoff\":\"false\",\"rawtransferon\":\"true\",\"disptransferon\":\"true\",\"rms_thres\":65,\"max_raw_per_day\":\"5\"}]}";

	bRet = Config::inst()->isValid(str);
    zassert_false(bRet, "bRet != false");

	const char str2[512] = "{\"general\":[{\"name\":\"FALSCH\",\"type\":\"9160\",\"version\":\"0.8.0\"}],\"sensors\":[{\"name\":\"adxl372\",\"type\":\"accel\",\"actthres\":\"200\",\"deactthres\":\"500\"}],\"metrics\":[{\"interval\":\"120\"}],\"displacement\":[{\"b1\":\"1.0\"}],\"datalog\":[{\"dispmodeoff\":\"false\",\"rawtransferon\":\"true\",\"disptransferon\":\"true\",\"rms_thres\":\"65\",\"max_raw_per_day\":\"5\"}]}";

	bRet = Config::inst()->isValid(str2);
    zassert_false(bRet, "bRet != false");

	const char str3[512] = "{\"general\":[{\"name\":\"axo2\",\"type\":\"FALSCH\",\"version\":\"0.8.0\"}],\"sensors\":[{\"name\":\"adxl372\",\"type\":\"accel\",\"actthres\":\"200\",\"deactthres\":\"500\"}],\"metrics\":[{\"interval\":\"120\"}],\"displacement\":[{\"b1\":\"1.0\"}],\"datalog\":[{\"dispmodeoff\":\"false\",\"rawtransferon\":\"true\",\"disptransferon\":\"true\",\"rms_thres\":\"65\",\"max_raw_per_day\":\"5\"}]}";

	bRet = Config::inst()->isValid(str3);
    zassert_false(bRet, "bRet != false");

	const char str4[512] = "{\"general\":[{\"name\":\"axo2\",\"type\":\"9160\",\"version\":\"0.8.0\"}],\"sensors\":[{\"name\":\"FALSCH\",\"type\":\"accel\",\"actthres\":\"200\",\"deactthres\":\"500\"}],\"metrics\":[{\"interval\":\"120\"}],\"displacement\":[{\"b1\":\"1.0\"}],\"datalog\":[{\"dispmodeoff\":\"false\",\"rawtransferon\":\"true\",\"disptransferon\":\"true\",\"rms_thres\":\"65\",\"max_raw_per_day\":\"5\"}]}";

	bRet = Config::inst()->isValid(str4);
    zassert_false(bRet, "bRet != false");

	const char str5[512] = "{\"general\":[{\"name\":\"axo2\",\"type\":\"9160\",\"version\":\"0.8.0\"}],\"sensors\":[{\"name\":\"adxl372\",\"type\":\"FALSCH\",\"actthres\":\"200\",\"deactthres\":\"500\"}],\"metrics\":[{\"interval\":\"120\"}],\"displacement\":[{\"b1\":\"1.0\"}],\"datalog\":[{\"dispmodeoff\":\"false\",\"rawtransferon\":\"true\",\"disptransferon\":\"true\",\"rms_thres\":\"65\",\"max_raw_per_day\":\"5\"}]}";

	bRet = Config::inst()->isValid(str5);
    zassert_false(bRet, "bRet != false");
}

// test function isValid, version and string is int/float/bool
ZTEST(tests_conf_config, test_conf_config_isValid4)
{
	bool bRet;

	const char str[512] = "{\"general\":[{\"name\":\"axo2\",\"type\":\"9160\",\"version\":\"0.1.0\"}],\"sensors\":[{\"name\":\"adxl372\",\"type\":\"accel\",\"actthres\":\"200\",\"deactthres\":\"500\"}],\"metrics\":[{\"interval\":\"120\"}],\"displacement\":[{\"b1\":\"1.0\"}],\"datalog\":[{\"dispmodeoff\":\"false\",\"rawtransferon\":\"true\",\"disptransferon\":\"true\",\"rms_thres\":\"65\",\"max_raw_per_day\":\"5\"}]}";

	bRet = Config::inst()->isValid(str);
    zassert_false(bRet, "bRet != false");

	const char str2[512] = "{\"general\":[{\"name\":\"axo2\",\"type\":\"9160\",\"version\":\"0.8.0\"}],\"sensors\":[{\"name\":\"adxl372\",\"type\":\"accel\",\"actthres\":\"FALSCH\",\"deactthres\":\"FALSCH\"}],\"metrics\":[{\"interval\":\"FALSCH\"}],\"displacement\":[{\"b1\":\"FALSCH\"}],\"datalog\":[{\"dispmodeoff\":\"FALSCH\",\"rawtransferon\":\"FALSCH\",\"disptransferon\":\"FALSCH\",\"rms_thres\":\"FALSCH\",\"max_raw_per_day\":\"FALSCH\"}]}";

	bRet = Config::inst()->isValid(str2);
    zassert_false(bRet, "bRet != false");
}

// test function Autocorrection
ZTEST(tests_conf_config, test_conf_config_Autocorrection)
{
	int rc;

	std::string in_string = "{\"general\":[{\"name\":\"axo2\",\"type\":\"9160\",\"version\":\"0.8.0\"}],\"sensors\":[{\"name\":\"adxl372\",\"type\":\"accel\",\"actthres\":\"200\",\"deactthres\":\"500\"}],\"metrics\":[{\"interval\":\"120\"}],\"displacement\":[{\"b1\":\"1.0\"}],\"datalog\":[{\"dispmodeoff\":\"false\",\"rawtransferon\":\"true\",\"disptransferon\":\"true\",\"rms_thres\":\"65\",\"max_raw_per_day\":\"5\"}]}";
	std::string expected  = "{\"general\":[{\"name\":\"axo2\",\"type\":\"9160\",\"version\":\"0.8.0\"}],\"sensors\":[{\"name\":\"adxl372\",\"type\":\"accel\",\"actthres\":\"200\",\"deactthres\":\"500\"}],\"metrics\":[{\"interval\":\"120\"}],\"displacement\":[{\"b1\":\"1.0\"}],\"datalog\":[{\"dispmodeoff\":\"false\",\"rawtransferon\":\"true\",\"disptransferon\":\"true\",\"rms_thres\":\"65\",\"max_raw_per_day\":\"5\"}]}";

	rc = Config::inst()->parse(&Config::inst()->root_obj, in_string); //Config::inst()->input);
	zassert_equal(rc, 0, "rc != 0");
	zassert_not_null(Config::inst()->root_obj, "root_obj is NULL");

	Config::inst()->Autocorrection();
	zassert(strcmp(Config::inst()->input.c_str(), expected.c_str())==0, "expected %s not correct", expected.c_str());
}

// test function Autocorrection
ZTEST(tests_conf_config, test_conf_config_Autocorrection2)
{
	int rc;

	std::string in_string = "{\"general\":[{\"name\":\"axo2\",\"type\":\"9160\",\"version\":\"0.8.0\"}],\"sensors\":[{\"name\":\"adxl372\",\"type\":\"accel\",\"actthres\":\"20\",\"deactthres\":\"10\"}],\"metrics\":[{\"interval\":\"1\"}],\"displacement\":[{\"b1\":\"1.0\"}],\"datalog\":[{\"dispmodeoff\":\"true\",\"rawtransferon\":\"true\",\"disptransferon\":\"true\",\"rms_thres\":\"65\",\"max_raw_per_day\":\"5\"}]}";
	std::string expected  = "{\"general\":[{\"name\":\"axo2\",\"type\":\"9160\",\"version\":\"0.8.0\"}],\"sensors\":[{\"name\":\"adxl372\",\"type\":\"accel\",\"actthres\":\"200\",\"deactthres\":\"400\"}],\"metrics\":[{\"interval\":\"5\"}],\"displacement\":[{\"b1\":\"1.0\"}],\"datalog\":[{\"dispmodeoff\":\"true\",\"rawtransferon\":\"true\",\"disptransferon\":\"false\",\"rms_thres\":\"65\",\"max_raw_per_day\":\"5\"}]}";

	rc = Config::inst()->parse(&Config::inst()->root_obj, in_string); //Config::inst()->input);
	zassert_equal(rc, 0, "rc != 0");
	zassert_not_null(Config::inst()->root_obj, "root_obj is NULL");

	Config::inst()->Autocorrection();
	zassert(strcmp(Config::inst()->input.c_str(), expected.c_str())==0, "expected %s not correct", expected.c_str());
}

// test function Autocorrection
ZTEST(tests_conf_config, test_conf_config_Autocorrection3)
{
	int rc;

	std::string in_string = "{\"general\":[{\"name\":\"axo2\",\"type\":\"9160\",\"version\":\"0.8.0\"}],\"sensors\":[{\"name\":\"adxl372\",\"type\":\"accel\",\"actthres\":\"50000\",\"deactthres\":\"55500\"}],\"metrics\":[{\"interval\":\"20080\"}],\"displacement\":[{\"b1\":\"1.0\"}],\"datalog\":[{\"dispmodeoff\":\"true\",\"rawtransferon\":\"true\",\"disptransferon\":\"false\",\"rms_thres\":\"65\",\"max_raw_per_day\":\"5\"}]}";
	std::string expected  = "{\"general\":[{\"name\":\"axo2\",\"type\":\"9160\",\"version\":\"0.8.0\"}],\"sensors\":[{\"name\":\"adxl372\",\"type\":\"accel\",\"actthres\":\"49800\",\"deactthres\":\"50000\"}],\"metrics\":[{\"interval\":\"10080\"}],\"displacement\":[{\"b1\":\"1.0\"}],\"datalog\":[{\"dispmodeoff\":\"true\",\"rawtransferon\":\"true\",\"disptransferon\":\"false\",\"rms_thres\":\"65\",\"max_raw_per_day\":\"5\"}]}";

	rc = Config::inst()->parse(&Config::inst()->root_obj, in_string); //Config::inst()->input);
	zassert_equal(rc, 0, "rc != 0");
	zassert_not_null(Config::inst()->root_obj, "root_obj is NULL");

	Config::inst()->Autocorrection();
	zassert(strcmp(Config::inst()->input.c_str(), expected.c_str())==0, "expected %s not correct", expected.c_str());
}

// test function isVersionValid
ZTEST(tests_conf_config, test_conf_config_isVersionValid)
{
	int rc;
	bool bRet;

	Config::inst()->input = "{\"general\":[{\"name\":\"axo2\",\"type\":\"9160\",\"version\":\"0.8.0\"}],\"sensors\":[{\"name\":\"adxl372\",\"type\":\"accel\",\"actthres\":\"200\",\"deactthres\":\"500\"}],\"metrics\":[{\"interval\":\"120\"}],\"displacement\":[{\"b1\":\"1.0\"}],\"datalog\":[{\"dispmodeoff\":\"false\",\"rawtransferon\":\"true\",\"disptransferon\":\"true\",\"rms_thres\":\"65\",\"max_raw_per_day\":\"5\"}]}";

	rc = Config::inst()->parse(&Config::inst()->root_obj, Config::inst()->input);
	zassert_equal(rc, 0, "rc != 0");
	zassert_not_null(Config::inst()->root_obj, "root_obj is NULL");

	bRet = Config::inst()->isVersionValid(Config::inst()->root_obj);
	zassert_true(bRet, "bRet != true");
}

// test function isVersionValid
ZTEST(tests_conf_config, test_conf_config_isVersionValid2)
{
	int rc;
	bool bRet;

	Config::inst()->input = "{\"general2\":[{\"name\":\"axo2\",\"type\":\"9160\",\"version\":\"0.8.0\"}],\"sensors2\":[{\"name\":\"adxl372\",\"type\":\"accel\",\"actthres\":\"200\",\"deactthres\":\"500\"}],\"metrics2\":[{\"interval\":\"120\"}],\"displacement2\":[{\"b1\":\"1.0\"}],\"datalog2\":[{\"dispmodeoff\":\"false\",\"rawtransferon\":\"true\",\"disptransferon\":\"true\",\"rms_thres\":\"65\",\"max_raw_per_day\":\"5\"}]}";

	rc = Config::inst()->parse(&Config::inst()->root_obj, Config::inst()->input);
	zassert_equal(rc, 0, "rc != 0");
	zassert_not_null(Config::inst()->root_obj, "root_obj is NULL");

	bRet = Config::inst()->isVersionValid(Config::inst()->root_obj);
    zassert_false(bRet, "bRet != false");
}

// test function isVersionValid
ZTEST(tests_conf_config, test_conf_config_isVersionValid3)
{
	int rc;
	bool bRet;

	Config::inst()->input = "{\"general\":[{\"name\":\"axo2\",\"type\":\"9160\",\"version\":\"0.1.0\"}],\"sensors\":[{\"name\":\"adxl372\",\"type\":\"accel\",\"actthres\":\"200\",\"deactthres\":\"500\"}],\"metrics\":[{\"interval\":\"120\"}],\"displacement\":[{\"b1\":\"1.0\"}],\"datalog\":[{\"dispmodeoff\":\"false\",\"rawtransferon\":\"true\",\"disptransferon\":\"true\",\"rms_thres\":\"65\",\"max_raw_per_day\":\"5\"}]}";

	rc = Config::inst()->parse(&Config::inst()->root_obj, Config::inst()->input);
	zassert_equal(rc, 0, "rc != 0");
	zassert_not_null(Config::inst()->root_obj, "root_obj is NULL");

	bRet = Config::inst()->isVersionValid(Config::inst()->root_obj);
    zassert_false(bRet, "bRet != false");}


// test function strIsInt
ZTEST(tests_conf_config, test_conf_config_strIsInt)
{
	bool bRet;
	std::string in;

    in = "abc";
	bRet = Config::inst()->strIsInt(in);
    zassert_false(bRet, "bRet != false");
    in = "11";
	bRet = Config::inst()->strIsInt(in);
    zassert_true(bRet, "bRet != true");
    in = "-11";
	bRet = Config::inst()->strIsInt(in);
    zassert_true(bRet, "bRet != true");
    in = "0x01";
	bRet = Config::inst()->strIsInt(in);
    zassert_false(bRet, "bRet != false");
    in = "";
	bRet = Config::inst()->strIsInt(in);
    zassert_false(bRet, "bRet != false");
}

// test function strIsFloat
ZTEST(tests_conf_config, test_conf_config_strIsFloat)
{
	bool bRet;
	std::string in;

    in = "abc";
	bRet = Config::inst()->strIsFloat(in);
    zassert_false(bRet, "bRet != false");
    in = "0x01";
	bRet = Config::inst()->strIsFloat(in);
    zassert_true(bRet, "bRet != true");
    in = "11";
	bRet = Config::inst()->strIsFloat(in);
    zassert_true(bRet, "bRet != true");
    in = "-11";
	bRet = Config::inst()->strIsFloat(in);
    zassert_true(bRet, "bRet != true");
    in = "-11.22";
	bRet = Config::inst()->strIsFloat(in);
    zassert_true(bRet, "bRet != true");
    in = "";
	bRet = Config::inst()->strIsFloat(in);
    zassert_false(bRet, "bRet != false");
}

// test function strIsBool
ZTEST(tests_conf_config, test_conf_config_strIsBool)
{
	bool bRet;

	std::string in;

    in = "nö";
	bRet = Config::inst()->strIsBool(in);
    zassert_false(bRet, "bRet != false");
    in = "false";
	bRet = Config::inst()->strIsBool(in);
    zassert_true(bRet, "bRet != true");
    in = "true";
	bRet = Config::inst()->strIsBool(in);
    zassert_true(bRet, "bRet != true");
    in = "TRUE";
	bRet = Config::inst()->strIsBool(in);
    zassert_false(bRet, "bRet != false");
}

// test function init
ZTEST(tests_conf_config, test_conf_config_init)
{
	bool bRet;

	gbRetVal = true;

	strcpy(gBuf, "{\"general\":[{\"name\":\"axo2\",\"type\":\"9160\",\"version\":\"0.8.0\"}],\"sensors\":[{\"name\":\"adxl372\",\"type\":\"accel\",\"actthres\":\"200\",\"deactthres\":\"500\"}],\"metrics\":[{\"interval\":\"120\"}],\"displacement\":[{\"b1\":\"1.0\"}],\"datalog\":[{\"dispmodeoff\":\"false\",\"rawtransferon\":\"true\",\"disptransferon\":\"true\",\"rms_thres\":\"65\",\"max_raw_per_day\":\"5\"}]}");

	Config::inst()->init();
	zassert(strcmp(Config::inst()->input.c_str(), gBuf)==0, "expected %s not correct", gBuf);
}

// test function init
ZTEST(tests_conf_config, test_conf_config_init2)
{
	bool bRet;

	gbRetVal = false;

	strcpy(gBuf, "{\"general\":[{\"name\":\"axo2\",\"type\":\"9160\",\"version\":\"0.8.0\"}],\"sensors\":[{\"name\":\"adxl372\",\"type\":\"accel\",\"actthres\":\"200\",\"deactthres\":\"500\"}],\"metrics\":[{\"interval\":\"120\"}],\"displacement\":[{\"b1\":\"1.0\"}],\"datalog\":[{\"dispmodeoff\":\"false\",\"rawtransferon\":\"true\",\"disptransferon\":\"false\",\"rms_thres\":\"65\",\"max_raw_per_day\":\"10\"}]}");

	Config::inst()->init();
	zassert(strcmp(Config::inst()->input.c_str(), gBuf)==0, "expected %s not correct", gBuf);
}


