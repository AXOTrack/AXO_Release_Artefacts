#define private public
#include "rtos/mbox.h"
#include "rtos/mpelem.h"
#include "rtos/mpool.h"
#include "rtos/mqueue.h"
#include "rtos/task.h"
#include "rtos/timer.h"

/*#include "bus/spi/spibus.h"
#include "drv/spi/fram.h"
#include "drv/spi/adxl37x.h"
#include "conf/config.h"
#include "./version.h"*/


#include "stubs.h"


// -------------------------------------------------------------------------

uint8_t gbU8;

int modem_info_init(void) {
    return 0;
}

int	modem_info_get_batt_voltage(int *val) {
    *val = 20;
    return 0;
}

int modem_info_get_temperature(int *val) {
    *val = 25;
    return 0;
}

int modem_info_get_fw_version (char *buf, size_t buf_size) {
    return 0;
}

int modem_info_string_get(enum modem_info info, char *buf, const size_t buf_size) {
    if (gbU8 == 0) {
            return -1;
    }
    if (info == MODEM_INFO_IMSI) {
        //char imsi_buf[15 + 6 + 1]; /* Add 6 for \r\nOK\r\n and 1 for \0 */
        strncpy(buf, "123456789012345\r\nOK\r\n\0", buf_size);
    } else if (info == MODEM_INFO_IMEI) {
        //char imei_buf[15 + 6 + 1]; /* Add 6 for \r\nOK\r\n and 1 for \0 */
        strncpy(buf, "098765432109876\r\nOK\r\n\0", buf_size);
    }
    return 0;
}

int nrf_modem_at_scanf(const char* cmd, const char* fmt, ...) {
    switch (gbU8) {
        case 0:     return -1;    break;
        case 1:     return  1;    break;
        case 2:     return  2;    break;
        case 3:     return  3;    break;
        case 0xFF:  return  0;    break;
    }
    return 0;
}

int nrf_modem_at_printf(const char* fmt, ...) {
    return 0;
}

void at_monitor_resume(struct at_monitor_entry* mon) { 
    //struct at_monitor_entry m1;
    mon->filter = NULL;
    //mon->handler = at_ecsq_handler;
    mon->flags.direct = 1;
    mon->flags.paused = 1;
}

int lte_lc_psm_get(int *tau, int *active_time) {
	if (gbU8 == 3) {
        return -1;
    }
    return 0;
}

int RSRP_IDX_TO_DBM(int inout) {
	if (gbU8 == 0) {
        return 0;
    }
    return 10;
}


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

//Timer::Timer(k_timer_expiry_t cb, void* pUser, uint32_t duration,uint32_t period) {}
//void Timer::start() {}

//void* k_timer_user_data_get(k_timer* timer) {
//    return reinterpret_cast<void*>(timer);
//}
void* my_k_timer_user_data_get(k_timer* timer) {
    return reinterpret_cast<void*>(timer);
}

/*/ -------------------------------------------------------------------

spibus::spibus() { }
spibus::~spibus() { }

spibus* spibus::inst() {
    static spibus myInstance;
    return &myInstance;
}

void spibus::init() { }

// -------------------------------------------------------------------------

spifram::spifram() : dpd_active(false) { }
spifram::~spifram() { }

spifram* spifram::inst() {
    static spifram myInstance;
    return &myInstance;
}

void spifram::init() { }

int spifram::wakeup() { return 0; }

int spifram::sleep() { return 0; }

int spifram::writeBytes(uint32_t addr, uint8_t* data, uint32_t num_bytes) { return 0; }

int spifram::readBytes(uint32_t addr, uint8_t* data, uint32_t num_bytes) {
    for (uint32_t i=0; i<num_bytes; i++) {
        data[i] = i;
    }
    return 0;
}

// -------------------------------------------------------------------------

void Adxl37x::convert(struct sensor_value* val, int16_t value) {
    int32_t micro_ms2 = value * (SENSOR_G / (16 * 1000 / 100));
    val->val1 = micro_ms2 / 1000000;
    val->val2 = micro_ms2 % 1000000;
}

// -------------------------------------------------------------------------

Config* Config::inst() {
    static Config myInstance;
    return &myInstance;
}

Config::Config() { }

Config::~Config() { }

stCfgGeneral* Config::getCfgGeneral() { 
    // 	std::string name;    //!< Name of the device class
	// std::string type;    //!< Type of the device
	// std::string version; //!< Version of the configuration 
    static stCfgGeneral ret = {"a","a","a"};
    return &ret;
}

stCfgMetrics* Config::getCfgMetrics() { 
    // std::string interval;
    static stCfgMetrics ret = {"a"};
    return &ret;
}

stCfgDatalog* Config::getCfgDatalog() {
    // std::string dispmodeoff; 
    // std::string rawtransferon; 
    // std::string disptransferon;  
    // std::string rms_thres;   
    // std::string max_raw_per_day;
    static stCfgDatalog ret_true = {"true","true","true","25","5"};
    static stCfgDatalog ret_false = {"false","false","false","25","5"};
    if (gbRetVal) {
        return &ret_true;
    } else {
        return &ret_false;
    }
}

stCfgDisplacement* Config::getCfgDisplacement() { 
    // std::string b1;
    static stCfgDisplacement ret = {"0.0"};
    return &ret;
}

stCfgSensor*  Config::getCfgSensor(enCfgSensorId id) { 
    // std::string name;       //!< Name of the sensor
	// std::string type;       //!< Type of the sensor
	// std::string actthres;   //!< Activation threshold
	// std::string deactthres; //!< Deactivation threshold
    static stCfgSensor ret = {"a","a","a","a"};
    return &ret;
}

// -------------------------------------------------------------------------

const char* getFwVersion() { return "9.77"; }

// -------------------------------------------------------------------------
*/

/*char gBuf[1024];

static struct fs_file_t stFile;

FlashFS::FlashFS(const char* name) { }

FlashFS& FlashFS::inst() {
    static FlashFS myInstance("fs");
    return myInstance;
}

FlashFS::~FlashFS() { }


bool FlashFS::file_exists(const char* path) {
    return gbRetVal;
}

int FlashFS::file_sync(fs_file_t* file) {
    return 0;
}

off_t FlashFS::file_seek(fs_file_t* file, off_t offset, int whence) {
    return 0;
}

fs_file_t* FlashFS::file_open(const char* path, int flags) {
    return reinterpret_cast<fs_file_t*>(this);
}

int FlashFS::file_close(fs_file_t* file) {
    return 0;
}

ssize_t FlashFS::file_read(fs_file_t* file, void* buffer, size_t len) {
    strncpy((char*)buffer, gBuf, 1024);
    return len;
}

ssize_t FlashFS::file_write(fs_file_t* file, const void* buffer, size_t len) {
    strncpy(gBuf, (char*)buffer, 1024);
    return len;
}

int FlashFS::remove(const char* filename) {
    return 0; 
}*/
