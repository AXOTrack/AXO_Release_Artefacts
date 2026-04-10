#include <modem/lte_lc.h>
#include <modem/pdn.h>

#include <zephyr/net/socket.h>

#define private public
#include "rtos/mbox.h"
#include "rtos/mpelem.h"
#include "rtos/mpool.h"
#include "rtos/mqueue.h"
#include "rtos/task.h"


#include "util/keygen.h"
#include "conf/config.h"
/*#include "drv/spi/fram.h"
#include "drv/spi/adxl37x.h"
#include "./version.h"*/


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

void* k_timer_user_data_get(k_timer* timer) {
    return reinterpret_cast<void*>(timer);
}

// -------------------------------------------------------------------------


Hash::Hash() { }
Hash::~Hash() { }

Hash* Hash::inst() { 
    static Hash myInstance;
    return &myInstance;
}

void Hash::init() { }
void Hash::update(uint8_t* pBuf, size_t len) { }
void Hash::finalize() { }
void Hash::print() { }
uint8_t* Hash::getHash() {
    static uint8_t buf[10] = {'1','2','1','2','1','2','1','2','1','2'};
    return buf;
}
uint16_t Hash::getHashLen() {
    return 10;
}

// -------------------------------------------------------------------------

ECDSASigner::ECDSASigner() { }
ECDSASigner::~ECDSASigner() { }

ECDSASigner* ECDSASigner::inst() { 
    static ECDSASigner myInstance;
    return &myInstance;
}
    
void ECDSASigner::setPrivateKey(const std::string& pemKey) { }

std::string ECDSASigner::signHash(const unsigned char* hash, size_t hashLen) { 
    std:: string s = "1234567890";
    return s;
}

void ECDSASigner::printSignature(const std::string& signature) { }

// -------------------------------------------------------------------------

KeyGen::KeyGen() { }
KeyGen::~KeyGen() { }

KeyGen* KeyGen::inst() { 
    static KeyGen myInstance;
    return &myInstance;
}

std::string KeyGen::getPrivateKey() {
    std:: string s;
    if (gbRetVal) {
        s = "0987654321";;
    } else {
        s = "-----INVALID KEY-----";
    }
    return s;
}

bool KeyGen::bIsKeyTransfered() {
    return gbRetVal;
}

void KeyGen::setKeyTransfered() { }

uint16_t KeyGen::getPublicKey(unsigned char* pBuf, size_t len) {
    return 0;
}
 
void KeyGen::deleteKeyFiles() { }

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
    static stCfgMetrics ret = {"2"};
    return &ret;
}

stCfgDatalog* Config::getCfgDatalog() {
    // std::string dispmodeoff; 
    // std::string rawtransferon; 
    // std::string disptransferon;  
    // std::string rms_thres;   
    // std::string max_raw_per_day;
    static stCfgDatalog ret_true = {"true","true","true","25","5"};
    static stCfgDatalog ret_false = {"false","false","false","27","7"};
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

unsigned char* Config::getConfigJson() { 
    static char out[32] = "qwertz";
    return (unsigned char*)out;
}

int Config::saveToFile(std::string config) {
    return 0;
}
int Config::saveToFile() {
    return 0;
}
bool Config::isValid(const char* buf) {
    return gbRetVal;
}
void Config::Autocorrection() { }
void Config::deleteConfigFile() { }




// -------------------------------------------------------------------------

const char* getFwVersion() { return "9.77"; }

// -------------------------------------------------------------------------


char gBuf[1024];

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
}

unsigned char* FlashFS::GetFsStat(const char *path) {
    static unsigned char buf[3] = {'1', '2', '3'};
    return buf;
}

int FlashFS::unmount() {
    return 0; 
}

void FlashFS::eraseFs() { }


// -------------------------------------------------------------------------


int connect(int socket, const struct sockaddr *address, socklen_t address_len) {
    switch (gIntVal) {
        case 1:   errno = 0;  return 0;    break;
        case 2:   errno = 0;  return -17;  break;
        case 3:   errno = 11; return -17;  break;
        default:  return 0;
    }
    return 0;
}

ssize_t send(int socket, const void *buffer, size_t length, int flags) {
    switch (gIntVal) {
        case 1:   errno = 0;  return 0;    break;
        case 2:   errno = 0;  return -17;  break;
        case 3:   errno = 11; return -17;  break;
        default:  return 0;
    }
    return 0;
}

ssize_t recv(int socket, void *buffer, size_t length, int flags) {
    switch (gIntVal) {
        case 1:   errno = 0;  return 0;    break;
        case 2:   errno = 0;  return -17;  break;
        case 3:   errno = 11; return -17;  break;
        default:  return 0;
    }
    return 0;
}

//#define socklen_t uit16_t
#define 	TLS_SEC_TAG_LIST   1
#define 	TLS_HOSTNAME   2
#define 	TLS_PEER_VERIFY   5

int setsockopt(int socket, int level, int option_name, const void *option_value, socklen_t option_len) {
    if ((option_name == TLS_PEER_VERIFY) && (gIntVal == 1)) { 
        return -17; 
    }
    if ((option_name == TLS_SEC_TAG_LIST) && (gIntVal == 2)) { 
        return -17; 
    }
    if ((option_name == TLS_HOSTNAME) && (gIntVal == 3)) { 
        return -17; 
    }
    return 0;
}

int getaddrinfo(const char * nodename, const char * servname,
       const struct addrinfo * hints, struct addrinfo ** res) {
    // Speicher für addrinfo und sockaddr_in allokieren
    struct addrinfo *ai = (struct addrinfo*)malloc(sizeof(struct addrinfo));
    if (!ai) return -1;

    struct sockaddr_in *addr = (struct sockaddr_in*)malloc(sizeof(struct sockaddr_in));
    if (!addr) {
        free(ai);
        return -1;
    }

    // sockaddr_in füllen
    addr->sin_family = AF_INET;
    addr->sin_port = 0x5000; //htons(80); // z. B. Port 80
    addr->sin_addr.s_addr = 0x5db8d822;
    //inet_pton(AF_INET, "127.0.0.1", &(addr->sin_addr));

    // addrinfo füllen
    memset(ai, 0, sizeof(struct addrinfo));
    ai->ai_family = AF_INET;
    ai->ai_socktype = SOCK_STREAM;
    if (gIntVal == 0) {
        ai->ai_protocol = IPPROTO_TCP;
    } else {
        ai->ai_protocol = IPPROTO_TLS_1_2;
    }
    //ai->ai_addrlen = sizeof(struct sockaddr_in);
    ai->ai_addr = (struct sockaddr *)addr;
    //ai->ai_next = NULL;

    *res = ai;

    return gIntVal; 
}

int socket(int domain, int type, int protocol) {
    return 0;
}

int close(int fildes) {
    return 0;
}

uint16_t htons(uint16_t hostshort) {
    return 0x8000; //hostshort;
}


// -------------------------------------------------------------------------

void ResetCtrl::performSwReset() { }

// -------------------------------------------------------------------------

int mqtt_helper_publish(const struct mqtt_publish_param *param) {
    if (!gbRetVal) {
        return 17;
    }
    return 0;
}

int mqtt_helper_subscribe(struct mqtt_subscription_list *sub_list) {
    return 0;
}

int mqtt_helper_connect(struct mqtt_helper_conn_params *conn_params) {
    if (!gbRetVal) {
        return 17;
    }
    return 0;
}

int mqtt_helper_disconnect(void) {
    return 0;
}

int mqtt_helper_abort(void) {
    return 0;
}

int hw_id_get(char* buf, size_t buf_len) {
    strncpy(buf, "123456789012345", buf_len);
    return 0;
}

int mqtt_helper_init(struct mqtt_helper_cfg *cfg) {
    return 0;
}

// -------------------------------------------------------------------------

int pdn_default_ctx_cb_reg (pdn_event_handler_t cb) {
    return 0;
}
void lte_lc_register_handler(lte_lc_evt_handler_t handler) { }

int lte_lc_modem_events_enable(void) {
    return 0;
}
int lte_lc_init(void) {
    return 0;
}
int lte_lc_connect_async(lte_lc_evt_handler_t handler) {
    return 0;
}
int lte_lc_deinit(void) {
    return 0;
}
int lte_lc_offline(void) {
    return 0;
}
int lte_lc_normal(void) {
    return 0;
}
int lte_lc_psm_req(bool enable) {
    return 0;
}
int lte_lc_power_off(void) {
    return 0;
}
int lte_lc_func_mode_set(enum lte_lc_func_mode mode) {
    return 0;
}

// ----------------------------------------------------------

