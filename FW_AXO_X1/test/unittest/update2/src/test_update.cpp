#include <zephyr/ztest.h>

#define private public
#include "update/update.h"

#include "stubs.h"

ZTEST_SUITE(tests_update_FwUpd, NULL, NULL, NULL, NULL, NULL);

// test constructor and destructor
ZTEST(tests_update_FwUpd, test_update_FwUpd_constructor)
{
	FwUpd* pFwUpd = new FwUpd("fwupd");
	zassert_not_null(pFwUpd, "memory alloc failed");
	zassert_not_null(pFwUpd->pInMbox, "pInMbox = NULL");
    zassert_equal(pFwUpd->pInMbox->count(), 0, "pInMbox->count() != 0");
    zassert_equal(pFwUpd->pInMbox, Mbox::getMboxById(Mbox::eFwUpd),
                  "Error getting mailbox by id");

	pFwUpd->setHostname("Test-Hostname");				  
	pFwUpd->setFilename("Test-Filename");
	zassert(strcmp(pFwUpd->hostname, "Test-Hostname")==0, "hostname not correct");
	zassert(strcmp(pFwUpd->filename, "Test-Filename")==0, "filename not correct");

	delete pFwUpd;
}

// MISSING for 100% branch coverage operator new does not deliver NULL
// test constructor and destructor
// here code lines 
// pInMbox = new Mbox(name);
// are not covered completly.


// test function fota_dl_handler
ZTEST(tests_update_FwUpd, test_update_FwUpd_fota_dl_handler)
{
    PoolElem* pElem;

	Mbox* pUpdCtrlMbox = new Mbox("UpdCtrlMbox");
    Mbox::registerMbox(pUpdCtrlMbox, Mbox::eFwUpdCtrl);
	zassert_not_null(pUpdCtrlMbox, "pUpdCtrlMbox = NULL");

	FwUpd* pFwUpd = new FwUpd("fwupd");
	zassert_not_null(pFwUpd, "memory alloc failed");
	zassert_not_null(pFwUpd->pInMbox, "pInMbox = NULL");
    zassert_equal(pFwUpd->pInMbox->count(), 0, "pInMbox->count() != 0");
    zassert_equal(pFwUpd->pInMbox, Mbox::getMboxById(Mbox::eFwUpd),
                  "Error getting mailbox by id");

	struct fota_download_evt ev;

	ev.id = FOTA_DOWNLOAD_EVT_ERROR;
	pFwUpd->fota_dl_handler(&ev);	

	pElem = pUpdCtrlMbox->read();
    zassert_equal(pElem->mtype, msgType::eFwDlFailed, "Rcvd mtype != msgType::eFwDlFailed");
    zassert_equal(pElem->from, Mbox::eFwUpd, "Rcvd sender != Mbox::eFwUpd");
	std::string s = std::string(reinterpret_cast<char*>(pElem->getBuffer()));
	printk("s = %s\n\n", s.c_str());
	zassert(strcmp(s.c_str(), "{\"event\":\"CODE3\"}")==0, "payload not correct");
	pElem->free();

	delete pUpdCtrlMbox;
	delete pFwUpd;
}

// test function fota_dl_handler
ZTEST(tests_update_FwUpd, test_update_FwUpd_fota_dl_handler2)
{
    PoolElem* pElem;

	Mbox* pUpdCtrlMbox = new Mbox("UpdCtrlMbox");
    Mbox::registerMbox(pUpdCtrlMbox, Mbox::eFwUpdCtrl);
	zassert_not_null(pUpdCtrlMbox, "pUpdCtrlMbox = NULL");

	FwUpd* pFwUpd = new FwUpd("fwupd");
	zassert_not_null(pFwUpd, "memory alloc failed");
	zassert_not_null(pFwUpd->pInMbox, "pInMbox = NULL");
    zassert_equal(pFwUpd->pInMbox->count(), 0, "pInMbox->count() != 0");
    zassert_equal(pFwUpd->pInMbox, Mbox::getMboxById(Mbox::eFwUpd),
                  "Error getting mailbox by id");

	struct fota_download_evt ev;

	ev.id = FOTA_DOWNLOAD_EVT_FINISHED;
	pFwUpd->fota_dl_handler(&ev);	

	pElem = pUpdCtrlMbox->read();
    zassert_equal(pElem->mtype, msgType::eFwDlSuccess, "Rcvd mtype != msgType::eFwDlSuccess");
    zassert_equal(pElem->from, Mbox::eFwUpd, "Rcvd sender != Mbox::eFwUpd");
	std::string s = std::string(reinterpret_cast<char*>(pElem->getBuffer()));
	printk("s = %s\n\n", s.c_str());
	zassert(strcmp(s.c_str(), "{\"event\":\"CODE2\"}")==0, "payload not correct");
	pElem->free();

	delete pUpdCtrlMbox;
	delete pFwUpd;
}

// test function fota_dl_handler
ZTEST(tests_update_FwUpd, test_update_FwUpd_fota_dl_handler3)
{
    PoolElem* pElem;

	Mbox* pUpdCtrlMbox = new Mbox("UpdCtrlMbox");
    Mbox::registerMbox(pUpdCtrlMbox, Mbox::eFwUpdCtrl);
	zassert_not_null(pUpdCtrlMbox, "pUpdCtrlMbox = NULL");

	FwUpd* pFwUpd = new FwUpd("fwupd");
	zassert_not_null(pFwUpd, "memory alloc failed");
	zassert_not_null(pFwUpd->pInMbox, "pInMbox = NULL");
    zassert_equal(pFwUpd->pInMbox->count(), 0, "pInMbox->count() != 0");
    zassert_equal(pFwUpd->pInMbox, Mbox::getMboxById(Mbox::eFwUpd),
                  "Error getting mailbox by id");

	struct fota_download_evt ev;

	ev.id = FOTA_DOWNLOAD_EVT_CANCELLED;
	pFwUpd->fota_dl_handler(&ev);	

	pElem = pUpdCtrlMbox->read();
    zassert_equal(pElem->mtype, msgType::eFwDlFailed, "Rcvd mtype != msgType::eFwDlFailed");
    zassert_equal(pElem->from, Mbox::eFwUpd, "Rcvd sender != Mbox::eFwUpd");
	std::string s = std::string(reinterpret_cast<char*>(pElem->getBuffer()));
	printk("s = %s\n\n", s.c_str());
	zassert(strcmp(s.c_str(), "{\"event\":\"CODE4\"}")==0, "payload not correct");
	pElem->free();

	delete pUpdCtrlMbox;
	delete pFwUpd;
}

// test function fota_dl_handler
ZTEST(tests_update_FwUpd, test_update_FwUpd_fota_dl_handler4)
{
	FwUpd* pFwUpd = new FwUpd("fwupd");
	zassert_not_null(pFwUpd, "memory alloc failed");
	zassert_not_null(pFwUpd->pInMbox, "pInMbox = NULL");
    zassert_equal(pFwUpd->pInMbox->count(), 0, "pInMbox->count() != 0");
    zassert_equal(pFwUpd->pInMbox, Mbox::getMboxById(Mbox::eFwUpd),
                  "Error getting mailbox by id");

	struct fota_download_evt ev;

	ev.id = FOTA_DOWNLOAD_EVT_ERASE_PENDING;
	pFwUpd->fota_dl_handler(&ev);	

	delete pFwUpd;
}

// test function download_start
ZTEST(tests_update_FwUpd, test_update_FwUpd_download_start)
{
    PoolElem* pElem;

	Mbox* pUpdCtrlMbox = new Mbox("UpdCtrlMbox");
    Mbox::registerMbox(pUpdCtrlMbox, Mbox::eFwUpdCtrl);
	zassert_not_null(pUpdCtrlMbox, "pUpdCtrlMbox = NULL");

	FwUpd* pFwUpd = new FwUpd("fwupd");
	zassert_not_null(pFwUpd, "memory alloc failed");
	zassert_not_null(pFwUpd->pInMbox, "pInMbox = NULL");
    zassert_equal(pFwUpd->pInMbox->count(), 0, "pInMbox->count() != 0");
    zassert_equal(pFwUpd->pInMbox, Mbox::getMboxById(Mbox::eFwUpd),
                  "Error getting mailbox by id");

	gIntVal = 0;
	pFwUpd->download_start();	

	gIntVal = 17;
	pFwUpd->download_start();	

	pElem = pUpdCtrlMbox->read();
    zassert_equal(pElem->mtype, msgType::eFwDlFailed, "Rcvd mtype != msgType::eFwDlFailed");
    zassert_equal(pElem->from, Mbox::eFwUpd, "Rcvd sender != Mbox::eFwUpd");
	std::string s = std::string(reinterpret_cast<char*>(pElem->getBuffer()));
	printk("s = %s\n\n", s.c_str());
	zassert(strcmp(s.c_str(), "{\"event\":\"CODE3\"}")==0, "payload not correct");
	pElem->free();

	delete pUpdCtrlMbox;
	delete pFwUpd;
}

//Test receive msg eFwInfo
ZTEST(tests_update_FwUpd, test_update_rcv_msg_eFwInfo)
{
	FwUpd* pFwUpd = new FwUpd("fwupd");
	zassert_not_null(pFwUpd, "memory alloc failed");
	zassert_not_null(pFwUpd->pInMbox, "pInMbox = NULL");
    zassert_equal(pFwUpd->pInMbox->count(), 0, "pInMbox->count() != 0");
    zassert_equal(pFwUpd->pInMbox, Mbox::getMboxById(Mbox::eFwUpd),
                  "Error getting mailbox by id");

	std::string str = "{\"url\":\"https://gomera3.ddns.net/stg/" 
			"app_update.bin\",\"fw_version\":\"0.1\",\"size\":\"247996\",\"sha1\":"
			"\"3db38b191b\"}";
    Mbox::sendMsg(msgType::eFwInfo, Mbox::eFwUpd, Mbox::eFwUpd, str);

    zassert_equal(pFwUpd->pInMbox->count(), 1, "pInMbox->count() != 1");

    pFwUpd->task(); 
    zassert_equal(pFwUpd->pInMbox->count(), 0, "pInMbox->count() != 0");
    zassert_equal(pFwUpd->size_expected, 247996, "size_expected != 247996");
	zassert(strcmp((char*)pFwUpd->sha1_expected, "3db38b191b")==0, "sha1_expected not correct");
	zassert(strcmp(pFwUpd->hostname, "https://gomera3.ddns.net")==0, "hostname not correct");
	zassert(strcmp(pFwUpd->filename, "stg/app_update.bin")==0, "filename not correct");

	delete pFwUpd; 
}

//Test receive msg eFwDownload
ZTEST(tests_update_FwUpd, test_update_rcv_msg_eFwDownload)
{
	FwUpd* pFwUpd = new FwUpd("fwupd");
	zassert_not_null(pFwUpd, "memory alloc failed");
	zassert_not_null(pFwUpd->pInMbox, "pInMbox = NULL");
    zassert_equal(pFwUpd->pInMbox->count(), 0, "pInMbox->count() != 0");
    zassert_equal(pFwUpd->pInMbox, Mbox::getMboxById(Mbox::eFwUpd),
                  "Error getting mailbox by id");

    Mbox::sendMsg(msgType::eFwDownload, Mbox::eFwUpd, Mbox::eFwUpd);

    zassert_equal(pFwUpd->pInMbox->count(), 1, "pInMbox->count() != 1");

    pFwUpd->task(); 
    zassert_equal(pFwUpd->pInMbox->count(), 0, "pInMbox->count() != 0");

	delete pFwUpd; 
}

//Test receive unexpected msg 
ZTEST(tests_update_FwUpd, test_update_rcv_msg_unexpected)
{
	FwUpd* pFwUpd = new FwUpd("fwupd");
	zassert_not_null(pFwUpd, "memory alloc failed");
	zassert_not_null(pFwUpd->pInMbox, "pInMbox = NULL");
    zassert_equal(pFwUpd->pInMbox->count(), 0, "pInMbox->count() != 0");
    zassert_equal(pFwUpd->pInMbox, Mbox::getMboxById(Mbox::eFwUpd),
                  "Error getting mailbox by id");

    Mbox::sendMsg(msgType::eResetCause, Mbox::eFwUpd, Mbox::eFwUpd);

    zassert_equal(pFwUpd->pInMbox->count(), 1, "pInMbox->count() != 1");

    pFwUpd->task();  

    zassert_equal(pFwUpd->pInMbox->count(), 0, "pInMbox->count() != 0");
	delete pFwUpd; 
}

//Test receive no msg 
ZTEST(tests_update_FwUpd, test_update_rcv_no_msg)
{
	FwUpd* pFwUpd = new FwUpd("fwupd");
	zassert_not_null(pFwUpd, "memory alloc failed");
	zassert_not_null(pFwUpd->pInMbox, "pInMbox = NULL");
    zassert_equal(pFwUpd->pInMbox->count(), 0, "pInMbox->count() != 0");
    zassert_equal(pFwUpd->pInMbox, Mbox::getMboxById(Mbox::eFwUpd),
                  "Error getting mailbox by id");

    pFwUpd->pInMbox->write(NULL);
    
    pFwUpd->task();  

    zassert_equal(pFwUpd->pInMbox->count(), 0, "pInMbox->count() != 0");
	delete pFwUpd; 
}
