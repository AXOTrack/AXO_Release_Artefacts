#ifndef __STUBS_H__
#define __STUBS_H__

//#include "rtos/mbox.h"
//#include "rtos/mpelem.h"
//#include "rtos/mpool.h"

extern bool gbRetVal;

#define k_timer_user_data_get(x) my_k_timer_user_data_get(x)

void* my_k_timer_user_data_get(k_timer* timer);


/*#define FRAM_5_SEC_RAW_DATA_SPACE     5
#define FRAM_15_SEC_RAW_DATA_SPACE   15

typedef enum _enCfgSensorId {
	eSensor_adxl372, //!< Acceleration sensor ADXL372
	eSensor_MAX      //!< Number of sensors mentioned in dynamic configuration
} enCfgSensorId;

typedef struct _stCfgGeneral {
	std::string name;    //!< Name of the device class
	std::string type;    //!< Type of the device
	std::string version; //!< Version of the configuration
} stCfgGeneral;

typedef struct _stCfgSensor {
	std::string name;       //!< Name of the sensor
	std::string type;       //!< Type of the sensor
	std::string actthres;   //!< Activation threshold
	std::string deactthres; //!< Deactivation threshold
} stCfgSensor;

typedef struct _stCfgMetrics {
	std::string interval;  //!< Metrics transmission interval
} stCfgMetrics;

typedef struct _stCfgDisplacement {
	std::string b1;  //!< Beautification factor 1
} stCfgDisplacement;

typedef struct _stCfgDatalog {	
   std::string dispmodeoff; 
   std::string rawtransferon; 
   std::string disptransferon;  
   std::string rms_thres;   
   std::string max_raw_per_day; 
} stCfgDatalog;
*/

/*
class Config {
 public:
    static Config* inst();
    void init();
    virtual ~Config();

    stCfgGeneral* getCfgGeneral();
    stCfgMetrics* getCfgMetrics();
    stCfgDatalog* getCfgDatalog();
    //stCfgDisplacement* getCfgDisplacement();
    stCfgSensor*  getCfgSensor(enCfgSensorId id);

    //unsigned char* getConfigJson();
 private:
    Config();

};*/


const char* getFwVersion();

class FlashFS {
 public:
    explicit FlashFS(const char* name = "??");

    static FlashFS& inst();
    FlashFS() = delete;
    virtual ~FlashFS();
    bool file_exists(const char* path);
};
/*
class MetricsBuilder {
 public:
    MetricsBuilder();
    ~MetricsBuilder();

    std::string payload;
    std::string tmp;
    std::string pubkey; 

    void add(PoolElem* pElem);
    void setConfigVersion(std::string& s);
    void setActivationThres(std::string& s);
    void setDeactivationThres(std::string& s);
    void setDispModeOffFlag(std::string& s);
    void setRawTransferOnFlag(std::string& s);
    void setDispTransferOnFlag(std::string& s);
    void setPublicKey(std::string& s);
    void setMetricsInterval(std::string& s);
    void set_Z_RmsThres(std::string& s);

    int scaleBattIn(int32_t ad_in);  // consider voltage divider at A/D in

    void finalize(void);
    void reset();

    bool isComplete();
    void print(void);
    std::string& getPayload(void);   
};

class MeasurementBuilder {
 public:
    explicit MeasurementBuilder(bool have_http_connector = true);
    ~MeasurementBuilder();

    bool have_http_connector; 

    std::string payload;

    void add(PoolElem* pElem);

    void finalize(void);
    void reset();

    bool isComplete(bool disp_mode_off, double rmsZThres);
    std::string& getPayload(void);
};

class KeyGen {
 public:
    ~KeyGen();

    static KeyGen* inst();

    uint16_t getPublicKey(unsigned char* pBuf, size_t len);
    std::string getPrivateKey();
    
 private:
    KeyGen();
};
*/

//void* k_timer_user_data_get(k_timer* timer);

#endif // __STUBS_H__