#define private public
/*#include "rtos/mbox.h"
#include "rtos/mpelem.h"
#include "rtos/mpool.h"
#include "rtos/mqueue.h"
#include "rtos/task.h"
#include "rtos/task.h"

#include "bus/spi/spibus.h"
#include "drv/spi/fram.h"
#include "drv/spi/adxl37x.h"
#include "conf/config.h"
#include "./version.h"
*/

#include "util/certmgr.h"
#include "util/keygen.h"
#include "stubs.h"


// -------------------------------------------------------------------------

bool gbRetVal;
bool gbReady;
int  gIntRetVal;
char gBuf[1024];

/*/ -------------------------------------------------------------------------
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
*/


static struct fs_file_t stFile;

FlashFS::FlashFS(const char* name) { }

FlashFS& FlashFS::inst() {
    static FlashFS myInstance("fs");
    return myInstance;
}

FlashFS::~FlashFS() { }


bool FlashFS::file_exists(const char* path) {
    switch (gIntRetVal) {
        case 1: 
            if (strcmp(path, BROKER_CFG_FILENAME) == 0) {
                return false;
            } 
            break;
        case 2: 
            if (strcmp(path, JOVIA_CERT_FILENAME) == 0) {
                return false;
            } 
            break;
        case 3: 
            if (strcmp(path, THINGSBOARD_CERT_FILENAME) == 0) {
                return false;
            } 
            break;
        case 20: 
            return false;
            break;
        case 21: 
            if (strcmp(path, PRIVATE_KEY_FILENAME) == 0) {
                if (gbRetVal) {
                    return true;
                } else {
                    return false;
                }
            } 
            break;
        case 22: 
            if (strcmp(path, PUBLIC_KEY_FILENAME) == 0) {
                if (gbRetVal) {
                    return true;
                } else {
                    return false;
                }
            } 
            break;
        case 23: 
            if (strcmp(path, KEY_TRANSFERED_FILENAME) == 0) {
                if (gbRetVal) {
                    return true;
                } else {
                    return false;
                }
            } 
            break;
        default:
            return true;
            break;
    }
    return true;
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

void FlashFS::eraseFs() {
    memset(gBuf, 0, sizeof(gBuf));
}

int FlashFS::file_tree(const char *path) {
    return 0; 
} 

int FlashFS::statvfs(const char* path, struct fs_statvfs* buf) {
    return 0; 
}

int FlashFS::unmount()  {
    return 0; 
}

volatile int iret;

int FlashFS::stat(const char* path, struct fs_dirent* st) {
    const int JoviaLen = 1657;
    const int TBLen = 1657;

    switch (gIntRetVal) {
        case 10: 
            if (strcmp(path, JOVIA_CERT_FILENAME) == 0) {
                st->size = JoviaLen;
            } else {
                st->size = 0;
            }
            break;
        case 11: 
            if (strcmp(path, JOVIA_CERT_FILENAME) == 0) {
                st->size = 0;
            }  else {
                st->size = 0;
            }
            break;
        case 12: 
            if (strcmp(path, THINGSBOARD_CERT_FILENAME) == 0) {
                st->size = TBLen;
            }  else {
                st->size = 0;
            }
            break;
        case 13: 
            if (strcmp(path, THINGSBOARD_CERT_FILENAME) == 0) {
                st->size = 0;
            }  else {
                st->size = 0;
            }
            break;
        case 14: 
            st->size = TBLen;
            break;
        default:
            st->size = 0;
            break;
    }
    return 0; 
}


// -----------------------------------------------------------------------------

int modem_key_mgmt_exists(nrf_sec_tag_t sec_tag, enum modem_key_mgmt_cred_type cred_type, bool *exists) {
    *exists = false;
    if (gIntRetVal == -1) {
        return -17;
    } else if (gIntRetVal == 1) {
        *exists = true;
        return 0;
    }
    return 0;
}

int modem_key_mgmt_write(nrf_sec_tag_t sec_tag, enum modem_key_mgmt_cred_type cred_type, const void *buf, size_t len) {
    return 0;
}

int modem_key_mgmt_cmp(nrf_sec_tag_t sec_tag, enum modem_key_mgmt_cred_type cred_type, const void *buf, size_t len) {
    const int JoviaLen = 578;
    const int TBLen = 939;

    if (len == JoviaLen) {
        if (gbRetVal) {
            return 0;
        } else {
            return -1;
        }
    }
    else if (len == TBLen) {
        if (gbRetVal) {
            return -1;
        } else {
            return 0;
        }
    }
    return 0xFF;
}

// -----------------------------------------------------------------------------

/*void mbedtls_sha256_init( mbedtls_sha256_context *ctx ) {

}

void mbedtls_sha256_free( mbedtls_sha256_context *ctx ) {

}


int mbedtls_sha256_starts( mbedtls_sha256_context *ctx, int is224 ) {
    return 0;
}


void mbedtls_sha256_update( mbedtls_sha256_context *ctx, const unsigned char *input, size_t ilen ) {

}


void mbedtls_sha256_finish( mbedtls_sha256_context *ctx, unsigned char output[32] ) {

}*/

void mbedtls_ctr_drbg_init( mbedtls_ctr_drbg_context *ctx ){

}
int mbedtls_ctr_drbg_seed( mbedtls_ctr_drbg_context *ctx,
                   int (*f_entropy)(void *, unsigned char *, size_t),
                   void *p_entropy,
                   const unsigned char *custom,
                   size_t len ) {
    return 0;
}

void mbedtls_ctr_drbg_free( mbedtls_ctr_drbg_context *ctx ) {

}

int mbedtls_ctr_drbg_random( void *p_rng,
                     unsigned char *output, size_t output_len ) {
    return 0;
}

  
int mbedtls_ecp_gen_key( mbedtls_ecp_group_id grp_id, mbedtls_ecp_keypair *key,
                 int (*f_rng)(void *, unsigned char *, size_t), void *p_rng ) {
    return 0;
}


void mbedtls_pk_init( mbedtls_pk_context *ctx ){

}
int mbedtls_pk_setup( mbedtls_pk_context *ctx, const mbedtls_pk_info_t *info ) {
    return 0;
}

void mbedtls_pk_free( mbedtls_pk_context *ctx ){

}

int mbedtls_pk_write_pubkey_pem( const mbedtls_pk_context *ctx, unsigned char *buf, size_t size ) {
    return 0;
}

int mbedtls_pk_write_key_pem( const mbedtls_pk_context *ctx, unsigned char *buf, size_t size ) {
    return 0;
}

const mbedtls_pk_info_t *mbedtls_pk_info_from_type( mbedtls_pk_type_t pk_type ) {
    return NULL;
}

int mbedtls_pk_parse_key( mbedtls_pk_context *ctx,
              const unsigned char *key, size_t keylen,
              const unsigned char *pwd, size_t pwdlen,
              int (*f_rng)(void *, unsigned char *, size_t), void *p_rng ) {
    return 0;
}

int mbedtls_pk_sign( mbedtls_pk_context *ctx, mbedtls_md_type_t md_alg,
              const unsigned char *hash, size_t hash_len,
              unsigned char *sig, size_t sig_size, size_t *sig_len,
              int (*f_rng)(void *, unsigned char *, size_t), void *p_rng )  {
    strcpy((char*)sig, "77777");
    *sig_len = 5;
    return 0;
}


int entropy_get_entropy(const struct device *dev,
                                  uint8_t *buffer,
                                  uint16_t length) {
    return 0;
}

// ------------------------------------------------------
   
bool my_device_is_ready(const struct device *dev) {
    if (gbReady) {
        return true;
    }
    return false;
}

// ------------------------------------------------------
   
void mbedtls_sha1_init( mbedtls_sha1_context *ctx ) { 
    ctx->state = 0;
}

void mbedtls_sha1_free( mbedtls_sha1_context *ctx ) {
    ctx->state += 40;
}

int mbedtls_sha1_starts( mbedtls_sha1_context *ctx ) {
    ctx->state = 17;
    return 0;
}

void mbedtls_sha1_update( mbedtls_sha1_context *ctx, const unsigned char *input, size_t ilen ) {
    ctx->state = 27;
}

void mbedtls_sha1_finish( mbedtls_sha1_context *ctx, unsigned char output[20] ) {
    unsigned char buf[20] ={1,1,1,1,1,1,1,1,1,1,2,2,2,2,2,2,2,2,2,2};
    ctx->state = 37;
    memcpy(output, buf, 20);
}


void mbedtls_sha256_init( mbedtls_sha256_context *ctx ) { 
    ctx->state = 0;
}

void mbedtls_sha256_free( mbedtls_sha256_context *ctx ) {
    ctx->state += 40;
}

int mbedtls_sha256_starts( mbedtls_sha256_context *ctx, int is224) {
    ctx->state = 17;
    return 0;
}

void mbedtls_sha256_update( mbedtls_sha256_context *ctx, const unsigned char *input, size_t ilen ) {
    ctx->state = 27;
}

void mbedtls_sha256_finish( mbedtls_sha256_context *ctx, unsigned char output[20] ) {
    unsigned char buf[20] ={1,1,1,1,1,1,1,1,1,1,2,2,2,2,2,2,2,2,2,2};
    ctx->state = 37;
    memcpy(output, buf, 20);
}


