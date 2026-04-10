#define private public
#include "rtos/mbox.h"
#include "rtos/mpelem.h"
#include "rtos/mpool.h"
#include "rtos/mqueue.h"
#include "rtos/task.h"
#include "rtos/task.h"
#include "update/fsm.h"
#include "update/update_ctrl.h"

#include "bus/spi/spibus.h"
#include "drv/spi/fram.h"
#include "drv/spi/adxl37x.h"
#include "conf/config.h"
#include "./version.h"


#include "stubs.h"


// -------------------------------------------------------------------------

bool gbRetVal;
int gIntVal;

// -------------------------------------------------------------------------
std::allocator<uint8_t> allocBytes;


void Mbox::sendMsg(enum msgType mType, MboxId senderId, MboxId receiverId) {
    PoolElem* e1 = new PoolElem(mType, senderId);
    e1->buf = NULL;
    e1->bufLen = 0;
    Mbox::send(e1, receiverId);
}

void Mbox::sendMsg(enum msgType mType, MboxId senderId, MboxId receiverId,
                   uint8_t u8DataByte) {
    PoolElem* e1 = new PoolElem(mType, senderId);
    e1->p1 = u8DataByte;
    e1->buf = NULL;
    e1->bufLen = 0;
    Mbox::send(e1, receiverId);
}

void Mbox::sendMsg(enum msgType mType, MboxId senderId, MboxId receiverId,
                   int8_t s8DataByte) {
    PoolElem* e1 = new PoolElem(mType, senderId);
    e1->p2 = s8DataByte;
    e1->buf = NULL;
    e1->bufLen = 0;
    Mbox::send(e1, receiverId);
}

void Mbox::sendMsg(enum msgType mType, MboxId senderId, MboxId receiverId,
                        uint8_t* pData, uint16_t dataLen) {
    PoolElem* e1 = new PoolElem(mType, senderId);
        
    uint8_t* b = allocBytes.allocate(2048);
    memcpy(b, pData, dataLen);
    e1->buf = reinterpret_cast<uint8_t (*)[2048]>(b);
    //printk("sendMsg():  e1->buf = 0x%X\n\n", (unsigned int)e1->buf);
    e1->bufLen = dataLen;
    e1->mtype = mType;
    e1->from = senderId;
    Mbox::send(e1, receiverId);
}

void Mbox::sendMsg(enum msgType mType, MboxId senderId, MboxId receiverId,
                   const std::string& str) {
    PoolElem* e1 = new PoolElem(mType, senderId);
    size_t dataLen = str.size() + 1;
    uint8_t* b = allocBytes.allocate(2048);
    auto rc = snprintf(reinterpret_cast<char*>(b), dataLen, "%s", str.c_str());
    int strLen = strlen(reinterpret_cast<const char*>(b));
    dataLen = strLen ? strLen + 1 : 0;
    e1->buf = (uint8_t (*)[2048])(b);
    e1->bufLen = dataLen;
    e1->mtype = mType;
    e1->from = senderId;
    Mbox::send(e1, receiverId);
}

// -------------------------------------------------------------------------

uint8_t* PoolElem::getBuffer(void) { return *buf; }

void PoolElem::putString(const char* s) {
    std::string str = s;
    size_t dataLen = str.size() + 1;
    uint8_t* b = allocBytes.allocate(2048);
    auto rc = snprintf(reinterpret_cast<char*>(b), dataLen, "%s", str.c_str());
    int strLen = strlen(reinterpret_cast<const char*>(b));
    dataLen = strLen ? strLen + 1 : 0;
    this->buf = (uint8_t (*)[2048])(b);
    this->bufLen = dataLen;
}

void PoolElem::putData(const void* pdata, size_t len) {
    uint8_t* b = allocBytes.allocate(2048);
    memcpy(b, pdata, len);
    this->buf = reinterpret_cast<uint8_t (*)[2048]>(b);
    this->bufLen = len;
}

void PoolElem::Dump() { }

void PoolElem::free() {
    uint8_t* b = reinterpret_cast<uint8_t*>(this->buf);
    //printk("free():  this->buf = 0x%X\n\n", (unsigned int)this->buf);
    //printk("free():  0x%.2X%.2X%.2X%.2X\n\n", (*(uint8_t*)this->buf[0]), 
    //                                  (*(uint8_t*)this->buf[1]),
    //                                  (*(uint8_t*)this->buf[2]), 
    //                                  (*(uint8_t*)this->buf[3]));
    if (this->buf != NULL) {
        allocBytes.deallocate(reinterpret_cast<uint8_t*>(this->buf), 2048);
        this->buf = NULL;
        this->bufLen = 0;
    }
    delete this;
}

// -------------------------------------------------------------------------

void* my_k_timer_user_data_get(k_timer* timer) {
    return reinterpret_cast<void*>(timer);
}

void sys_reboot (int type) { }

int64_t my_k_uptime_get (void) {
    if (gbRetVal) {
        return 7000000;
    }
    return 0;
}

// -------------------------------------------------------------------------

int fota_download_init(fota_download_callback_t client_callback) {
    return 0;
}

int fota_download_start(const char *host, const char *file, int sec_tag,
            uint8_t pdn_id, size_t fragment_size) {
    return gIntVal;
}

void fota_download_set_expected(int32_t size, uint8_t *sha1) { }

int boot_write_img_confirmed (void) {
    return 0;
}

// -------------------------------------------------------------------------


FwState* FwStateList::createIdle(void* pd) { return new FwIdle(pd); }

FwState* FwStateList::createTriggered(void* pd) { return new FwTriggered(pd); }

FwState* FwStateList::createDownload(void* pd) { return new FwDownload(pd); }

FwState* FwStateList::createReady(void* pd) { return new FwReady(pd); }

FwFsm::FwFsm(void* context, FwFsm::StateId initialState) : pd(context) {
    myStateId = initialState;
    stateTbl[eIdle] = FwStateList::createIdle;
    stateTbl[eTriggered] = FwStateList::createTriggered;
    stateTbl[eDownload] = FwStateList::createDownload;
    stateTbl[eReady] = FwStateList::createReady;
    myState = stateTbl[initialState](pd);
}

FwFsm::~FwFsm() {
    delete myState;
}

void FwFsm::setState(FwFsm::StateId newStateId) {
    if (newStateId != eNoChange) {
        delete myState;
        myState = stateTbl[myStateId = newStateId](pd);
        __ASSERT_NO_MSG(myState);
        if (newStateId != eIdle) {
            ReportOtaUpdateActive();
        }
    }
}

int FwFsm::getStateId(void) { return myStateId; }

void FwFsm::ReportOtaUpdateActive(void) {
    Mbox::sendMsg(msgType::eFwUpdateActive, 
                  Mbox::MboxId::eFwUpdCtrl,
                  Mbox::MboxId::eAdxl37xTask);
}

void FwFsm::handle_OtaStart() {
    setState(myState->handle_OtaStart());
}

void FwFsm::handle_FwInfo(PoolElem** pElem) {
    setState(myState->handle_FwInfo(pElem));
}

void FwFsm::handle_DlComplete(PoolElem** pElem) {
    setState(myState->handle_DlComplete(pElem));
}

void FwFsm::handle_Restart() {
    setState(myState->handle_OtaStart());
}

void FwFsm::handle_DlFailed(PoolElem** pElem) {
    setState(myState->handle_DlFailed(pElem));
}

void FwFsm::handle_timer_exp() {
    setState(myState->handle_timer_exp());
}

// ################# state base class #####################

FwState::FwState(void* pData) : pd(pData) {
}

FwState::~FwState(void) {  }

FwFsm::StateId FwState::handle_OtaStart() {
    // notify platform on cmd rejected
    reinterpret_cast<FwUpdCtrl*>(pd)->otaStartReject();
    return FwFsm::eNoChange;
}

FwFsm::StateId FwState::handle_FwInfo(PoolElem** pElem) {
    // notify platform on firmware rejected
    reinterpret_cast<FwUpdCtrl*>(pd)->otaFwReject();
    return FwFsm::eNoChange;
}

FwFsm::StateId FwState::handle_DlComplete(PoolElem** pElem) {
     return FwFsm::eNoChange;
}

FwFsm::StateId FwState::handle_DlFailed(PoolElem** pElem) {
   return FwFsm::eNoChange;
}

FwFsm::StateId FwState::handle_timer_exp() {
    return FwFsm::eNoChange;
}

// ################# idle state class #####################

FwIdle::FwIdle(void* pData) : FwState(pData) {
}

FwIdle::~FwIdle() {  }

FwFsm::StateId FwIdle::handle_OtaStart() {
    // notify platform on cmd accepted
    reinterpret_cast<FwUpdCtrl*>(pd)->otaStartConfirm();
    // Initiating the OTA start timer and logging the event
    // Start the trigger timeout upon receiving OTA start
    reinterpret_cast<FwUpdCtrl*>(pd)->startTimeout(FwFsm::getTriggerTimeout());
    reinterpret_cast<FwUpdCtrl*>(pd)->startTimeout(FwFsm::getDownloadTimeout());
    return FwFsm::eTriggered;
}

// ################# triggered state class #####################

FwTriggered::FwTriggered(void* pData) : FwState(pData) {
}

FwTriggered::~FwTriggered() {
}

FwFsm::StateId FwTriggered::handle_timer_exp() {
    Mbox::sendMsg(msgType::eAdxlResumeMeas,
                  Mbox::MboxId::eFwUpdCtrl, Mbox::MboxId::eAdxl37xTask);
    return FwFsm::eIdle;
}

FwFsm::StateId FwTriggered::handle_FwInfo(PoolElem** pElem) {
    // todo: check firmware info

    // for now: always accept and notify platform
    reinterpret_cast<FwUpdCtrl*>(pd)->otaFwConfirm();

    // change message sender
    (*pElem)->from = Mbox::MboxId::eFwUpdCtrl;

    // forward message to update task
    Mbox::send(*pElem, Mbox::eFwUpd);

    // prevent pElem->free() on return
    *pElem = 0;

    // Transition to the Download state and start the download timeout timer.
    // The firmware information has been accepted, and the system is now ready
    // to download the firmware. The download timeout timer is started to
    // monitor the duration of the firmware download process.
    reinterpret_cast<FwUpdCtrl*>(pd)->startTimeout(FwFsm::getDownloadTimeout());
    startDownloadTimestamp = (uint32_t)(k_uptime_get() / 1000);
    return FwFsm::eDownload;
}

// ################# firmware download state class #####################

FwDownload::FwDownload(void* pData) : FwState(pData) {
}

FwDownload::~FwDownload() { }

FwFsm::StateId FwDownload::handle_DlComplete(PoolElem** pElem) {
    // change message sender
    (*pElem)->from = Mbox::MboxId::eFwUpdCtrl;
    
    // forward message to mqtt task
    Mbox::send(*pElem, Mbox::eMqttConnector);

    // prevent pElem->free() on return
    *pElem = 0;
    return FwFsm::eReady;
}

FwFsm::StateId FwDownload::handle_DlFailed(PoolElem** pElem) {
    // Check for maximum retries(2) and transition back to idle if needed
    if (reinterpret_cast<FwUpdCtrl*>(pd)->retryCnt >=
        reinterpret_cast<FwUpdCtrl*>(pd)->maxRetries) {
       reinterpret_cast<FwUpdCtrl*>(pd)->retryCnt = 0;  // Reset retry count
        // __ASSERT(false, "Exceeded maximum download retries");

        // notify platform via device event
        // change message sender
        (*pElem)->from = Mbox::MboxId::eFwUpdCtrl;

        // forward message to mqtt task
        Mbox::send(*pElem, Mbox::eMqttConnector);

        // prevent pElem->free() on return
        *pElem = 0;

        k_sleep(K_SECONDS(2));
        sys_reboot(SYS_REBOOT_COLD);

        return FwFsm::eIdle; // never reached
    }

    // Increment the retry count(Not needed here because its incrmented in
    // startRetryTimer (But Don't remove later will be used for testing))
    // ++reinterpret_cast<FwUpdCtrl*>(pd)->retryCnt;

    // start a one-shot timer to schedule a dl retry
    reinterpret_cast<FwUpdCtrl*>(pd)->startRetryTimer();

    // stay in dl
    return FwFsm::eNoChange;
}

FwFsm::StateId FwDownload::handle_timer_exp() {
    // calculate time since download has started
    uint32_t now = (uint32_t)(k_uptime_get() / 1000);
    uint32_t diff = now - startDownloadTimestamp;

    if ((diff > maxDownloadDelay) || 
        (reinterpret_cast<FwUpdCtrl*>(pd)->retryCnt >=
        reinterpret_cast<FwUpdCtrl*>(pd)->maxRetries)) {
        //LOG_ERR("handle_timer_exp: %s", "Download failed, perform Reset!");
        std::string msg("{\"event\":\"CODE3\"}");
        Mbox::sendMsg(msgType::eFwDlFailed, Mbox::MboxId::eFwUpdCtrl,
                      Mbox::MboxId::eMqttConnector, msg);
        k_sleep(K_SECONDS(2));
        sys_reboot(SYS_REBOOT_COLD);
    } else {
        // request dl again
        reinterpret_cast<FwUpdCtrl*>(pd)->requestDownload();
        // start a one-shot timer to schedule a dl retry
        reinterpret_cast<FwUpdCtrl*>(pd)->startRetryTimer();
        return FwFsm::eNoChange;
    }
    return FwFsm::eNoChange; // to avoid warning -Wreturn-type
}

// ################# ready state class #####################

FwReady::FwReady(void* pData) : FwState(pData) {
    // start a one-shot timer, we reboot when the timer expires
    reinterpret_cast<FwUpdCtrl*>(pd)->startRebootTimer();
}

FwReady::~FwReady() {  }

FwFsm::StateId FwReady::handle_timer_exp() {
    k_sleep(K_SECONDS(3));  // otherwise we won't see the final trace messages
    sys_reboot(SYS_REBOOT_WARM);
    return FwFsm::eIdle;
}
