#include <zephyr/ztest.h>

#define private public
#define protected public
#include "update/fwinfo.h"

#include "stubs.h"

ZTEST_SUITE(tests_update_FwInfo, NULL, NULL, NULL, NULL, NULL);

// test constructor and destructor
ZTEST(tests_update_FwInfo, test_update_FwInfo_constructor)
{
	char str[] = "Teststring";

	FwInfo* pFwInfo = new FwInfo(str);
	zassert_not_null(pFwInfo, "memory alloc failed");
	zassert(strcmp(pFwInfo->input.c_str(), "Teststring")==0, "input wrong");

	delete pFwInfo;
}

// MISSING for 100% branch coverage operator new does not deliver NULL
// test constructor and destructor
// here code lines 
// are not covered completly.


// test constructor and destructor
ZTEST(tests_update_FwInfo, test_update_FwInfo_constructor2)
{
	std::string str = "Teststring";
	PoolElem* pElem = new PoolElem();
	pElem->putString(str.c_str());

	FwInfo* pFwInfo = new FwInfo(pElem);
	zassert_not_null(pFwInfo, "memory alloc failed");
	zassert(strcmp(pFwInfo->input.c_str(), "Teststring")==0, "input wrong");

	delete pFwInfo;
}

// test function validate
ZTEST(tests_update_FwInfo, test_update_FwInfo_validate_nok)
{
	char json[] = "test";
	int rc;

	FwInfo* fwinfo = new FwInfo(json);

	fwinfo->url = "";
	fwinfo->sha1 = "12345";
	fwinfo->fw_version = "12345";
	fwinfo->filesize = 110000;

	rc = fwinfo->validate();
	zassert_equal(rc, -22, "Failed to validate the message");

	fwinfo->url = "12345";
	fwinfo->sha1 = "";
	fwinfo->fw_version = "12345";
	fwinfo->filesize = 1110000;

	rc = fwinfo->validate();
	zassert_equal(rc, -22, "Failed to validate the message");

	fwinfo->url = "12345";
	fwinfo->sha1 = "12345";
	fwinfo->fw_version = "";
	fwinfo->filesize = 110000;

	rc = fwinfo->validate();
	zassert_equal(rc, -22, "Failed to validate the message");

	fwinfo->url = "12345";
	fwinfo->sha1 = "12345";
	fwinfo->fw_version = "12345";
	fwinfo->filesize = 1;

	rc = fwinfo->validate();
	zassert_equal(rc, -22, "Failed to validate the message");

	delete fwinfo;
}

// test function validate
ZTEST(tests_update_FwInfo, test_update_FwInfo_validate_ok)
{
	char json[] = { "{\"url\":\"https://gomera3.ddns.net/stg/" 
			"app_update.bin\",\"fw_version\":\"0.1\",\"size\":\"247996\",\"sha1\":"
			"\"3db38b191b\"}"
			};

	FwInfo* fwinfo = new FwInfo(json);

	auto rc = fwinfo->parse();
	zassert_equal(rc, 0, "Failed to parse the JSON");

	fwinfo->splitUrl();

	rc = fwinfo->validate();
	zassert_equal(rc, 0, "Failed to validate the message");

	zassert_equal(fwinfo->getProto(), "https://", "Proto mismatch");
	zassert_equal(fwinfo->getFwVersion(), "0.1", "Failed to get Fw Version");
	zassert_equal(fwinfo->getFileSize(), 247996, "Failed to get Filesize");
	zassert_equal(fwinfo->getHost(), "gomera3.ddns.net", "Incorrect Host");
	zassert_equal(fwinfo->getFileName(), "stg/app_update.bin", "Incorrect Filename");
	zassert_equal(fwinfo->getUrl(), "https://gomera3.ddns.net/stg/app_update.bin",
			"Incorrect Url");
	zassert_equal(fwinfo->getSha1(), "3db38b191b", "Incorrect sha");

	delete fwinfo;
}

// test function validate
ZTEST(tests_update_FwInfo, test_update_FwInfo_validate_size_type)
{
	char json[] = { "{\"url\":\"https://gomera3.ddns.net/stg/" 
			"app_update.bin\",\"fw_version\":\"0.1\",\"size\":null,\"sha1\":"
			"\"3db38b191b\"}"
			};

	FwInfo* fwinfo = new FwInfo(json);

	auto rc = fwinfo->parse();
	zassert_equal(rc, 0, "Failed to parse the JSON");

	fwinfo->splitUrl();

	rc = fwinfo->validate();
	zassert_equal(rc, 0, "Failed to validate the message");

	zassert_equal(fwinfo->getProto(), "https://", "Proto mismatch");
	zassert_equal(fwinfo->getFwVersion(), "0.1", "Failed to get Fw Version");
	zassert_equal(fwinfo->getHost(), "gomera3.ddns.net", "Incorrect Host");
	zassert_equal(fwinfo->getFileName(), "stg/app_update.bin", "Incorrect Filename");
	zassert_equal(fwinfo->getUrl(), "https://gomera3.ddns.net/stg/app_update.bin",
			"Incorrect Url");
	zassert_equal(fwinfo->getSha1(), "3db38b191b", "Incorrect sha");

	delete fwinfo;
}

// test function validate
ZTEST(tests_update_FwInfo, test_update_FwInfo_validate_sha1_invalid)
{
	char json[] = { "{\"url\":\"https://gomera3.ddns.net/stg/app_update.bin\",\"fw_version\":\"0.1\",\"size\":247996}" };

	FwInfo* fwinfo = new FwInfo(json);

	auto rc = fwinfo->parse();
	zassert_equal(rc, 0, "Failed to parse the JSON");

	fwinfo->splitUrl();

	rc = fwinfo->validate();
	zassert_equal(rc, 0, "Failed to validate the message");

	zassert_equal(fwinfo->getHost(), "gomera3.ddns.net", "Incorrect Host");
	zassert_equal(fwinfo->getUrl(), "https://gomera3.ddns.net/stg/app_update.bin",
			"Incorrect Url");
	zassert_equal(fwinfo->getFwVersion(), "0.1", "Failed to get Fw Version");
	zassert_equal(fwinfo->getFileSize(), 247996, "Failed to get Filesize");

	delete fwinfo;
}

// test function validate
ZTEST(tests_update_FwInfo, test_update_FwInfo_size_invalid)
{
	char json[] = { "{\"url\":\"https://gomera3.ddns.net/stg/" 
			"app_update.bin\",\"fw_version\":\"0.1\",\"size\":\"7996\",\"sha1\":"
			"\"3db38b191b\"}"
			};

	FwInfo* fwinfo = new FwInfo(json);

	auto rc = fwinfo->parse();
	zassert_equal(rc, 0, "Failed to parse the JSON");

	fwinfo->splitUrl();

	rc = fwinfo->validate();
	zassert_equal(rc, -22, "Failed to validate the message");

	zassert_equal(fwinfo->getProto(), "https://", "Proto mismatch");
	zassert_equal(fwinfo->getFwVersion(), "0.1", "Failed to get Fw Version");
	zassert_equal(fwinfo->getFileSize(), 7996, "Failed to get Filesize");
	zassert_equal(fwinfo->getHost(), "gomera3.ddns.net", "Incorrect Host");
	zassert_equal(fwinfo->getFileName(), "stg/app_update.bin", "Incorrect Filename");
	zassert_equal(fwinfo->getUrl(), "https://gomera3.ddns.net/stg/app_update.bin",
			"Incorrect Url");
	zassert_equal(fwinfo->getSha1(), "3db38b191b", "Incorrect sha");

	delete fwinfo;
}

// test function validate
ZTEST(tests_update_FwInfo, test_update_FwInfo_validate_fwversion_invalid)
{
	char json[] = { "{\"url\":\"https://gomera3.ddns.net/stg/" 
			"app_update.bin\",\"size\":247996,\"sha1\":"
			"\"3db38b191b\"}"
			};

	FwInfo* fwinfo = new FwInfo(json);

	auto rc = fwinfo->parse();
	zassert_equal(rc, 0, "Failed to parse the JSON");

	fwinfo->splitUrl();
	
	zassert_equal(fwinfo->filename, "stg/app_update.bin", "Filename mismatch");
	zassert_equal(fwinfo->host, "gomera3.ddns.net", "host name mismatch");
	zassert_equal(fwinfo->proto, "https://", "Filename mismatch");

	rc = fwinfo->validate();
	zassert_equal(rc, 0, "Failed to validate the message");

	zassert_equal(fwinfo->getSha1(), "3db38b191b", "Incorrect sha");

	delete fwinfo;
}

ZTEST(tests_update_FwInfo, test_update_FwInfo_validate_size_invalid)
{
	char json[] = { "{\"url\":\"https://gomera3.ddns.net/stg/" 
			"app_update.bin\",\"fw_version\": \"1.2\",\"sha1\":"
			"\"3db38b191b\"}"
			};

	FwInfo* fwinfo = new FwInfo(json);

	auto rc = fwinfo->parse();
	zassert_equal(rc, 0, "Failed to parse the JSON");

	fwinfo->splitUrl();

	rc = fwinfo->validate();
	zassert_equal(rc, 0, "Failed to validate the message");

	zassert_equal(fwinfo->getFwVersion(), "1.2", "Failed to get Fw Version");
	zassert_equal(fwinfo->getHost(), "gomera3.ddns.net", "Incorrect Host");
	zassert_equal(fwinfo->getFileName(), "stg/app_update.bin", "Incorrect Filename");
	zassert_equal(fwinfo->getUrl(), "https://gomera3.ddns.net/stg/app_update.bin", "Incorrect Url");
	zassert_equal(fwinfo->getSha1(), "3db38b191b", "Incorrect sha");

	delete fwinfo;
}

ZTEST(tests_update_FwInfo, test_update_FwInfo_validate_url)
{
	char json[] = { "{\"url\":\"http://www.example.com/path/to/resource/" 
			"file%20with%20spaces.bin\",\"fw_version\":\"0.1\",\"size\":247996,\"sha1\":"
			"\"3db38b191b\"}"
			};

	FwInfo* fwinfo = new FwInfo(json);

	auto rc = fwinfo->parse();
	zassert_equal(rc, 0, "Failed to parse the JSON");

	fwinfo->splitUrl();

	rc = fwinfo->validate();
	zassert_equal(rc, 0, "Failed to validate the message");

	zassert_equal(fwinfo->getProto(), "http://", "Proto mismatch");
	zassert_equal(fwinfo->getFwVersion(), "0.1", "Failed to get Fw Version");
	zassert_equal(fwinfo->getFileSize(), 247996, "Failed to get Filesize");
	zassert_equal(fwinfo->getHost(), "www.example.com", "Incorrect Host");
	zassert_equal(fwinfo->getFileName(), "path/to/resource/file%20with%20spaces.bin", "Incorrect Filename");
	zassert_equal(fwinfo->getUrl(), "http://www.example.com/path/to/resource/file%20with%20spaces.bin",
			"Incorrect Url");
	zassert_equal(fwinfo->getSha1(), "3db38b191b", "Incorrect sha");

	delete fwinfo;
}

ZTEST(tests_update_FwInfo, test_update_FwInfo_validate_url_invalid)
{
	char json[] = { "{\"url\":\"coap://www.example.com/path/to/resource/" 
			"file%20with%20spaces.bin\",\"fw_version\":\"0.1\",\"size\":247996,\"sha1\":"
			"\"3db38b191b\"}"
			};

	FwInfo* fwinfo = new FwInfo(json);

	auto rc = fwinfo->parse();
	zassert_equal(rc, 0, "Failed to parse the JSON");

	fwinfo->splitUrl();

	rc = fwinfo->validate();
	zassert_equal(rc, 0, "Failed to validate the message");

	zassert_equal(fwinfo->getFwVersion(), "0.1", "Failed to get Fw Version");
	zassert_equal(fwinfo->getFileSize(), 247996, "Failed to get Filesize");
	zassert_equal(fwinfo->getSha1(), "3db38b191b", "Incorrect sha");

	delete fwinfo;
}

ZTEST(tests_update_FwInfo, test_update_FwInfo_invalid_json)
{
	char json[] = { "{\"url\":\"coap://www.example.com/path/to/resource/" 
			"file%20with%20spaces.bin\",\"f0.1\",\"size\":247996,\"sha1\":"
			"\"3db38b19b\"}"
			};

	FwInfo* fwinfo = new FwInfo(json);

	auto rc = fwinfo->parse();
	zassert_equal(rc, -1, "Failed to parse the JSON");

	delete fwinfo;
}
