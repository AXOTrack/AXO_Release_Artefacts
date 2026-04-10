#include "string.h"

#define private public
#include "rtos/mbox.h"
#include "rtos/mpelem.h"
#include "rtos/mpool.h"
#include "rtos/mqueue.h"
#include "rtos/task.h"
#include "rtos/timer.h"

#include "bus/i2c/i2cbus.h"
#include "bus/spi/spibus.h"
#include "drv/spi/adxl37x.h"
#include "conf/config.h"
/*#include "drv/spi/fram.h"
#include "drv/spi/adxl37x.h"
#include "./version.h"
*/

#include "stubs.h"

// -------------------------------------------------------------------------

bool gbRetVal;
bool gbReady;
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

/*void* k_timer_user_data_get(k_timer* timer) {
    return reinterpret_cast<void*>(timer);
}*/

void* my_k_timer_user_data_get(k_timer* timer) {
    return reinterpret_cast<void*>(timer);
}

// -------------------------------------------------------------------------


struct gpio_dt_spec gGpioDtSpec;

int i2c_configure(const struct device *dev, uint32_t dev_config) {
    if (gbRetVal) {
        return 0;
    }
    return 1;
}

bool my_device_is_ready(const struct device *dev) {
    if (gbReady) {
        return true;
    }
    return false;
}

const struct device* device_get_binding(const char *name) {
    static struct device d;
    d.name = name;
    return (const struct device*)&d;
}

int pwm_set(const struct device *dev, uint32_t channel, uint32_t period, uint32_t pulse, pwm_flags_t flags) {
    return gbRetVal;
}

int sensor_sample_fetch(const struct device *dev) {
    if (gbReady) {
        return 1;
    }
    return 0;
}

int sensor_channel_get(const struct device *dev, enum sensor_channel chan, struct sensor_value *val){
    return 0;
}

int sensor_value_from_double(struct sensor_value *val, double inp)
{
        if (inp < INT32_MIN || inp > INT32_MAX) {
                return -ERANGE;
        }
 
        double val2 = (inp - (int32_t)inp) * 1000000.0;
 
        if (val2 < INT32_MIN || val2 > INT32_MAX) {
                return -ERANGE;
        }
 
        val->val1 = (int32_t)inp;
        val->val2 = (int32_t)val2;
 
        return 0;
}

double sensor_value_to_double(const struct sensor_value *val)
{
        return (double)val->val1 + (double)val->val2 / 1000000;
}

int sensor_trigger_set(const struct device *dev,
                        const struct sensor_trigger *trig,
                        sensor_trigger_handler_t handler) {
    return gbRetVal;
}

int sensor_attr_set(const struct device *dev,
                    enum sensor_channel chan,
                    enum sensor_attribute attr,
                    const struct sensor_value *val) {
    return 0;
}

// --------------------------------------------------------------------

I2cBus::I2cBus() { }

I2cBus::~I2cBus() { }

I2cBus* I2cBus::inst() {
    static I2cBus myInstance;
    return &myInstance;
}

void I2cBus::init() {
    init_done = true;
}

// --------------------------------------------------------------------

spibus::spibus() { }

spibus::~spibus() { }

spibus* spibus::inst() {
    static spibus myInstance;
    return &myInstance;
}

void spibus::init() {
    init_done = true;
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
    static stCfgSensor ret = {"aa","bb","200","20"};
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

// ----------------------------------------------------------------------------------

int adxl372_set_op_mode(const struct device *dev, enum adxl372_op_mode op_mode) {
    gIntVal = (int)op_mode;
    return gbRetVal;
}

int adxl372_get_fifo_xyz_data(const struct device* dev,
                              struct adxl372_xyz_accel_data* samples,
                              uint16_t* cnt) {
    *cnt = 6;
    return gbRetVal;
}

int adxl372_configure_fifo(const struct device* dev,
                           enum adxl372_fifo_mode mode,
                           enum adxl372_fifo_format format,
                           uint16_t fifo_samples) {
    return gbRetVal;
} 

int32_t adxl372_get_status(struct adxl372_dev *dev,
               uint8_t *status1,
               uint8_t *status2,
               uint16_t *fifo_entries) {
    return gIntVal;
}

int adxl372_set_ext_sync(const struct device* dev, int state) {
    return gbRetVal;
}

int adxl372_get_status(const struct device* dev, uint8_t* status1,
                       uint8_t* status2, uint16_t* fifo_entries) {
    return gbRetVal;
}

bool getSelftestSuccess(void) {
    return gbRetVal;
}

// ----------------------------------------------------------------------------------

int spi_transceive(const struct device *dev,
                const struct spi_config *config,
                const struct spi_buf_set *tx_bufs,
                const struct spi_buf_set *rx_bufs) {
    return gIntVal;
}

int spi_write(const struct device *dev,
                const struct spi_config *config,
                const struct spi_buf_set *tx_bufs) {
    int cmd = *((int*)(tx_bufs->buffers->buf));
    if (gIntVal == 27) {
        switch (cmd) {
            case 6:   return 0;    break;    // MB85RS4MTY_WRITE_ENABLE_CMD  6
            case 2:   return 27;   break;    // MB85RS4MTY_WRITE_CMD   2
            default:               break;
        }
    }
    //printk("tx_bufs %i, %i\n", tx_bufs->count, tx_bufs->buffers->len);
    //printk("tx_bufs %i\n\n", *((int*)(tx_bufs->buffers->buf)));
    //printk("cmd %i\n\n", cmd);
    return gIntVal;  
} 

int gpio_pin_set (const struct device *port, gpio_pin_t pin, int value) {
    if (gIntVal == 27) {
        switch (value) {
            case 0:   return 0;    break;    
            case 1:   return 27;   break;    
            default:               break;
        }
    }    
    return gIntVal;
}

int gpio_pin_configure_dt (const struct gpio_dt_spec *spec, gpio_flags_t extra_flags) {
    return gbRetVal;
}

// ----------------------------------------------------------------------------------

int adc_channel_setup_dt(const struct adc_dt_spec *spec) {
    return 0;
}

int adc_raw_to_millivolts_dt(const struct adc_dt_spec *spec, int32_t *valp) {
    return 0;
}

int adc_read(const struct device *dev, const struct adc_sequence *sequence) {
    return 0;
}

int adc_sequence_init_dt(const struct adc_dt_spec *spec, struct adc_sequence *seq) {
    return 0;
}

// ----------------------------------------------------------------------------------

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

