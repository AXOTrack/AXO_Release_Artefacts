#define private public
#include "rtos/mbox.h"
#include "rtos/mpelem.h"
#include "rtos/mpool.h"
#include "rtos/mqueue.h"
#include "rtos/task.h"

#include "bus/spi/spibus.h"
#include "drv/spi/fram.h"
#include "drv/spi/adxl37x.h"
#include "conf/config.h"
#include "./version.h"


#include "stubs.h"


// -------------------------------------------------------------------------

bool gbRetVal;

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
    /*printk("free():  this->buf = 0x%X\n\n", (unsigned int)this->buf);
    printk("free():  0x%.2X%.2X%.2X%.2X\n\n", (*(uint8_t*)this->buf[0]), 
                                      (*(uint8_t*)this->buf[1]),
                                      (*(uint8_t*)this->buf[2]), 
                                      (*(uint8_t*)this->buf[3]));*/
    if (this->buf != NULL) {
        allocBytes.deallocate(reinterpret_cast<uint8_t*>(this->buf), 2048);
        this->buf = NULL;
        this->bufLen = 0;
    }
    delete this;
}

// -------------------------------------------------------------------------

//void* k_timer_user_data_get(k_timer* timer) {
//    return reinterpret_cast<void*>(timer);
//}
void* my_k_timer_user_data_get(k_timer* timer) {
    return reinterpret_cast<void*>(timer);
}


// -------------------------------------------------------------------

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

FlashFS::FlashFS(const char* name) { }

FlashFS& FlashFS::inst() {
    static FlashFS myInstance("fs");
    return myInstance;
}

FlashFS::~FlashFS() { }

bool FlashFS::file_exists(const char* path) {
    return gbRetVal;
}
