#ifndef __STUBS_H__
#define __STUBS_H__

#include "rtos/mbox.h"
#include "rtos/task.h"

//! Definition of FRAM raw data storage time
//! when diplacement calculation is enabled
#define FRAM_5_SEC_RAW_DATA_SPACE     5

#define k_sleep(x) 

typedef uint8_t gpio_pin_t;
typedef uint16_t gpio_dt_flags_t;

struct gpio_dt_spec {
        //const struct device *port;
        gpio_pin_t pin;
        gpio_dt_flags_t dt_flags;
};


class PowerMgrX1 {
 public:
    Mbox* pInMbox;
    PowerMgrX1(const char* name = "??") {};
    void create(k_thread_stack_t*, size_t, int) {};
    void configure() {};
    void start() {};
};

class BoostConverter {
 public:
    BoostConverter(const char* name = "??") {};
    void create(k_thread_stack_t*, size_t, int) {};
    void configure() {};
    void start() {};
};

class LteConnector {
 public:
    LteConnector(const char* name = "??") {};
    void create(k_thread_stack_t*, size_t, int) {};
    void configure() {};
    void start() {};
    void registerListener(Mbox* listener);
};

class LteMonitor {
 public:
    LteMonitor(const char* name = "??") {};
    void create(k_thread_stack_t*, size_t, int) {};
    void configure() {};
    void start() {};
    void register_link_quality_notifications() {};
    void resume_link_quality_notifications() {};
    void register_network_notifications() {};
    void resume_network_notifications() {};
};

class Shtcx {
 public:
    Shtcx(const char* name = "??") {};
    void create(k_thread_stack_t*, size_t, int) {};
    void configure() {};
    void start() {};
};

class MqttConnector {
 public:
    Mbox* pInMbox;
    MqttConnector(const char* name = "??") {};
    void create(k_thread_stack_t*, size_t, int) {};
    void configure() {};
    void start() {};
};

class HttpConnector {
 public:
    Mbox* pInMbox;
    HttpConnector(const char* name = "??") {};
    void create(k_thread_stack_t*, size_t, int) {};
    void configure() {};
    void start() {};
};

class DataManager {
 public:
    DataManager(const char* name = "??") {};
    void create(k_thread_stack_t*, size_t, int) {};
    void configure() {};
    void start() {};
};

class ADConverter {
 public:
    ADConverter(const char* name = "??") {};
    void create(k_thread_stack_t*, size_t, int) {};
    void configure() {};
    void start() {};
    void registerListener(uint8_t chmask, Mbox* listener);
};

class BatteryMgr {
 public:
    Mbox* pInMbox;
    BatteryMgr(const char* name = "??") {};
    void create(k_thread_stack_t*, size_t, int) {};
    void configure() {};
    void start() {};
    uint8_t getAdcChannels() { return 1; }
};

class HwMetrics {
 public:
    HwMetrics(const char* name = "??") {};
    void create(k_thread_stack_t*, size_t, int) {};
    void configure() {};
    void start() {};
};

class Adxl37x {
 public:
    Adxl37x(const char* name = "??") {};
    enum { odr = 4400 };         //!< Output data rate max with external sync
    void create(k_thread_stack_t*, size_t, int) {};
    void configure() {};
    void start() {};
};

class AdxlDataStore {
 public:
    AdxlDataStore(const char* name, uint8_t num_seconds,
                   uint16_t samples_per_sec, bool have_http_connector=false) {};
    void create(k_thread_stack_t*, size_t, int) {};
    void configure() {};
    void start() {};
};

class RmsStore {
 public:
    RmsStore(const char* name, uint8_t num_seconds) {};
    void create(k_thread_stack_t*, size_t, int) {};
    void configure() {};
    void start() {};
};

class RmsTask {
 public:
    RmsTask(const char* name, uint8_t num_seconds,
                 uint16_t samples_per_sec) {};
    void create(k_thread_stack_t*, size_t, int) {};
    void configure() {};
    void start() {};
};

class DispTask {
 public:
    DispTask(const char* name = "??") {};
    void create(k_thread_stack_t*, size_t, int) {};
    void configure() {};
    void start() {};
};

class FwUpd {
 public:
    FwUpd(const char* name = "??") {};
    void create(k_thread_stack_t*, size_t, int) {};
    void configure() {};
    void start() {};
};

class FwUpdCtrl {
 public:
    FwUpdCtrl(const char* name = "??") {};
    void create(k_thread_stack_t*, size_t, int) {};
    void configure() {};
    void start() {};
};

class ResetCtrl {
 public:
    ResetCtrl(const char* name = "??") {};
    void create(k_thread_stack_t*, size_t, int) {};
    void configure() {};
    void start() {};
};

class Config {
 public:
    static Config* inst();
    void init();
    virtual ~Config();
 private:
    Config();
};

class KeyGen {
 public:
    static KeyGen* inst();
    void init();
    virtual ~KeyGen();
 private:
    KeyGen();
};

class WakeupCnt {
 public:
    static WakeupCnt* inst();
    void init();
    virtual ~WakeupCnt();
    uint32_t getSleepExitCount();
    uint32_t getPublishMeasCount();
 private:
    WakeupCnt();
};

class FlashFS {
 public:
    explicit FlashFS(const char* name = "??");

    static FlashFS& inst();
    FlashFS() = delete;
    virtual ~FlashFS();
    bool file_exists(const char* path);
    void CheckAvailableSpace();
};

#endif // __STUBS_H__