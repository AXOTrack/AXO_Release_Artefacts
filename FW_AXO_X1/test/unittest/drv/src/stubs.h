#ifndef __STUBS_H__
#define __STUBS_H__

#include <stdio.h>
#include <stdlib.h>
#include <cstdint>

extern bool gbReady;
extern bool gbRetVal;
extern int gIntVal;


#define 	ERANGE   34
#define 	SENSOR_G   9806650LL

typedef uint8_t gpio_pin_t;
typedef uint16_t gpio_dt_flags_t;
typedef uint16_t spi_operation_t;
typedef uint16_t pwm_flags_t;

struct device {
        const char *name;
        const void *config;
        const void *api;
        //struct device_state *state;
        void *data;
        //struct device_ops ops;
        //device_flags_t flags;
};
struct gpio_dt_spec {
        const struct device *port;
        gpio_pin_t pin;
        gpio_dt_flags_t dt_flags;
};
 
struct spi_cs_control {
        struct gpio_dt_spec gpio;
        uint32_t delay;
};
struct spi_config {
        uint32_t frequency;
        spi_operation_t operation;
        uint16_t slave;
        struct spi_cs_control* cs;
};
struct spi_buf {
        void *buf;
        size_t len;
};
struct spi_buf_set {
        const struct spi_buf *buffers;
        size_t count;
};

extern struct gpio_dt_spec gGpioDtSpec;

#define I2C_SPEED_SET(x) x
    
#define I2C_SPEED_STANDARD 0
#define I2C_MODE_CONTROLLER 0

#ifdef DEVICE_DT_GET
    #undef DEVICE_DT_GET
#endif
#define DEVICE_DT_GET(x) x
#define I2C_DEV_NODE NULL

#ifdef DT_NODELABEL
    #undef DT_NODELABEL
#endif    
#define DT_NODELABEL(x) x

#ifdef GPIO_DT_SPEC_GET
    #undef GPIO_DT_SPEC_GET
#endif    
#define GPIO_DT_SPEC_GET(x,y) gGpioDtSpec

#ifdef SPI_WORD_SET
    #undef SPI_WORD_SET
#endif    
#define SPI_WORD_SET(x) x

typedef uint32_t gpio_flags_t;

#define GPIO_OUTPUT_INACTIVE 0
#define GPIO_OUTPUT_ACTIVE 1



#define device_is_ready(dev) my_device_is_ready(dev)
#define k_timer_user_data_get(x) my_k_timer_user_data_get(x)
#define k_sleep(x)

int i2c_configure(const struct device *dev, uint32_t dev_config);
bool my_device_is_ready(const struct device *dev);
const struct device* device_get_binding(const char *name);

void* my_k_timer_user_data_get(k_timer* timer);

int pwm_set(const struct device *dev, uint32_t channel, uint32_t period, uint32_t pulse, pwm_flags_t flags);

int spi_transceive(const struct device *dev,
                const struct spi_config *config,
                const struct spi_buf_set *tx_bufs,
                const struct spi_buf_set *rx_bufs);

int spi_write(const struct device *dev,
                const struct spi_config *config,
                const struct spi_buf_set *tx_bufs);                                        

int gpio_pin_set (const struct device *port, gpio_pin_t pin, int value);

int gpio_pin_configure_dt (const struct gpio_dt_spec *spec, gpio_flags_t extra_flags);

// ------------------------------------------------------------------------------------

struct sensor_value {
        int32_t val1;
        int32_t val2;
};

enum sensor_channel {
        SENSOR_CHAN_ACCEL_X,
        SENSOR_CHAN_ACCEL_Y,
        SENSOR_CHAN_ACCEL_Z,
        SENSOR_CHAN_ACCEL_XYZ,
        SENSOR_CHAN_GYRO_X,
        SENSOR_CHAN_GYRO_Y,
        SENSOR_CHAN_GYRO_Z,
        SENSOR_CHAN_GYRO_XYZ,
        SENSOR_CHAN_MAGN_X,
        SENSOR_CHAN_MAGN_Y,
        SENSOR_CHAN_MAGN_Z,
        SENSOR_CHAN_MAGN_XYZ,
        SENSOR_CHAN_DIE_TEMP,
        SENSOR_CHAN_AMBIENT_TEMP,
        SENSOR_CHAN_PRESS,
        SENSOR_CHAN_PROX,
        SENSOR_CHAN_HUMIDITY,
        SENSOR_CHAN_LIGHT,
        SENSOR_CHAN_IR,
        SENSOR_CHAN_RED,
        SENSOR_CHAN_GREEN,
        SENSOR_CHAN_BLUE,
        SENSOR_CHAN_ALTITUDE,
 
        SENSOR_CHAN_PM_1_0,
        SENSOR_CHAN_PM_2_5,
        SENSOR_CHAN_PM_10,
        SENSOR_CHAN_DISTANCE,
 
        SENSOR_CHAN_CO2,
        SENSOR_CHAN_O2,
        SENSOR_CHAN_VOC,
        SENSOR_CHAN_GAS_RES,
 
        SENSOR_CHAN_VOLTAGE,
 
        SENSOR_CHAN_VSHUNT,
 
        SENSOR_CHAN_CURRENT,
        SENSOR_CHAN_POWER,
 
        SENSOR_CHAN_RESISTANCE,
 
        SENSOR_CHAN_ROTATION,
 
        SENSOR_CHAN_POS_DX,
        SENSOR_CHAN_POS_DY,
        SENSOR_CHAN_POS_DZ,
        SENSOR_CHAN_POS_DXYZ,
 
        SENSOR_CHAN_RPM,
 
        SENSOR_CHAN_FREQUENCY,
 
        SENSOR_CHAN_GAUGE_VOLTAGE,
        SENSOR_CHAN_GAUGE_AVG_CURRENT,
        SENSOR_CHAN_GAUGE_STDBY_CURRENT,
        SENSOR_CHAN_GAUGE_MAX_LOAD_CURRENT,
        SENSOR_CHAN_GAUGE_TEMP,
        SENSOR_CHAN_GAUGE_STATE_OF_CHARGE,
        SENSOR_CHAN_GAUGE_FULL_CHARGE_CAPACITY,
        SENSOR_CHAN_GAUGE_REMAINING_CHARGE_CAPACITY,
        SENSOR_CHAN_GAUGE_NOM_AVAIL_CAPACITY,
        SENSOR_CHAN_GAUGE_FULL_AVAIL_CAPACITY,
        SENSOR_CHAN_GAUGE_AVG_POWER,
        SENSOR_CHAN_GAUGE_STATE_OF_HEALTH,
        SENSOR_CHAN_GAUGE_TIME_TO_EMPTY,
        SENSOR_CHAN_GAUGE_TIME_TO_FULL,
        SENSOR_CHAN_GAUGE_CYCLE_COUNT,
        SENSOR_CHAN_GAUGE_DESIGN_VOLTAGE,
        SENSOR_CHAN_GAUGE_DESIRED_VOLTAGE,
        SENSOR_CHAN_GAUGE_DESIRED_CHARGING_CURRENT,
        SENSOR_CHAN_GAME_ROTATION_VECTOR,
        SENSOR_CHAN_GRAVITY_VECTOR,
        SENSOR_CHAN_GBIAS_XYZ,
 
        SENSOR_CHAN_ALL,
 
        SENSOR_CHAN_COMMON_COUNT,
 
        SENSOR_CHAN_PRIV_START = SENSOR_CHAN_COMMON_COUNT,
 
        SENSOR_CHAN_MAX = INT16_MAX,
};
 
enum sensor_trigger_type {
        SENSOR_TRIG_TIMER,
        SENSOR_TRIG_DATA_READY,
        SENSOR_TRIG_DELTA,
        SENSOR_TRIG_NEAR_FAR,
        SENSOR_TRIG_THRESHOLD,
 
        SENSOR_TRIG_TAP,
 
        SENSOR_TRIG_DOUBLE_TAP,
 
        SENSOR_TRIG_FREEFALL,
 
        SENSOR_TRIG_MOTION,
 
        SENSOR_TRIG_STATIONARY,
 
        SENSOR_TRIG_FIFO_WATERMARK,
 
        SENSOR_TRIG_FIFO_FULL,
 
        SENSOR_TRIG_TILT,
 
        SENSOR_TRIG_COMMON_COUNT,

        SENSOR_TRIG_FIFO,
 
        SENSOR_TRIG_PRIV_START = SENSOR_TRIG_COMMON_COUNT,
 
        SENSOR_TRIG_MAX = INT16_MAX,
};

struct sensor_trigger {
        enum sensor_trigger_type type;
        enum sensor_channel chan;
};

 
enum sensor_attribute {
        SENSOR_ATTR_SAMPLING_FREQUENCY,
        SENSOR_ATTR_LOWER_THRESH,
        SENSOR_ATTR_UPPER_THRESH,
        SENSOR_ATTR_SLOPE_TH,
        SENSOR_ATTR_SLOPE_DUR,
        /* Hysteresis for trigger thresholds. */
        SENSOR_ATTR_HYSTERESIS,
        SENSOR_ATTR_OVERSAMPLING,
        SENSOR_ATTR_FULL_SCALE,
        SENSOR_ATTR_OFFSET,
        SENSOR_ATTR_CALIB_TARGET,
        SENSOR_ATTR_CONFIGURATION,
        SENSOR_ATTR_CALIBRATION,
        SENSOR_ATTR_FEATURE_MASK,
        SENSOR_ATTR_ALERT,
        SENSOR_ATTR_FF_DUR,
 
        SENSOR_ATTR_BATCH_DURATION,
        /* Configure the gain of a sensor. */
        SENSOR_ATTR_GAIN,
        /* Configure the resolution of a sensor. */
        SENSOR_ATTR_RESOLUTION,
        SENSOR_ATTR_COMMON_COUNT,
 
        SENSOR_ATTR_PRIV_START = SENSOR_ATTR_COMMON_COUNT,
 
        SENSOR_ATTR_MAX = INT16_MAX,
};

typedef void (*sensor_trigger_handler_t)(const struct device *dev,
                                         const struct sensor_trigger *trigger);
 

int sensor_sample_fetch(const struct device *dev);

int sensor_channel_get(const struct device *dev, enum sensor_channel chan, struct sensor_value *val);

int sensor_value_from_double(struct sensor_value *val, double inp);

double sensor_value_to_double(const struct sensor_value *val);

int sensor_trigger_set(const struct device *dev,
                        const struct sensor_trigger *trig,
                        sensor_trigger_handler_t handler);

int sensor_attr_set(const struct device *dev,
                    enum sensor_channel chan,
                    enum sensor_attribute attr,
                    const struct sensor_value *val);

// ------------------------------------------------------------------------

enum adc_gain {
        ADC_GAIN_1_6, 
        ADC_GAIN_1_5, 
        ADC_GAIN_1_4, 
        ADC_GAIN_2_7, 
        ADC_GAIN_1_3, 
        ADC_GAIN_2_5, 
        ADC_GAIN_1_2, 
        ADC_GAIN_2_3, 
        ADC_GAIN_4_5, 
        ADC_GAIN_1,   
        ADC_GAIN_2,   
        ADC_GAIN_3,   
        ADC_GAIN_4,   
        ADC_GAIN_6,   
        ADC_GAIN_8,   
        ADC_GAIN_12,  
        ADC_GAIN_16,  
        ADC_GAIN_24,  
        ADC_GAIN_32,  
        ADC_GAIN_64,  
        ADC_GAIN_128, 
};

enum adc_reference {
        ADC_REF_VDD_1,     
        ADC_REF_VDD_1_2,   
        ADC_REF_VDD_1_3,   
        ADC_REF_VDD_1_4,   
        ADC_REF_INTERNAL,  
        ADC_REF_EXTERNAL0, 
        ADC_REF_EXTERNAL1, 
};

struct adc_channel_cfg {
        enum adc_gain gain;
        enum adc_reference reference;
        uint16_t acquisition_time;
        uint8_t channel_id   : 5;
        uint8_t differential : 1;
/*#ifdef CONFIG_ADC_CONFIGURABLE_INPUTS
        uint8_t input_positive;
 
        uint8_t input_negative;
#endif 
 
#ifdef CONFIG_ADC_CONFIGURABLE_EXCITATION_CURRENT_SOURCE_PIN
        uint8_t current_source_pin_set : 1;
        uint8_t current_source_pin[2];
#endif 
 
#ifdef CONFIG_ADC_CONFIGURABLE_VBIAS_PIN
        uint32_t vbias_pins;
#endif */
};

enum adc_action {
        ADC_ACTION_CONTINUE = 0,
 
        ADC_ACTION_REPEAT,
 
        ADC_ACTION_FINISH,
};
 
typedef enum adc_action (*adc_sequence_callback)(const struct device *dev,
                                                 const struct adc_sequence *sequence,
                                                 uint16_t sampling_index);
 
struct adc_sequence_options {
        uint32_t interval_us;
 
        adc_sequence_callback callback;
 
        void *user_data;
 
        uint16_t extra_samplings;
};

struct adc_sequence {
        const struct adc_sequence_options *options;
        uint32_t channels;
        void *buffer;
        size_t buffer_size;
        uint8_t resolution;
        uint8_t oversampling; 
        bool calibrate;
};
 
struct adc_dt_spec {
        const struct device *dev;
        uint8_t channel_id;
        bool channel_cfg_dt_node_exists;
        struct adc_channel_cfg channel_cfg;
        uint16_t vref_mv;
        uint8_t resolution;
        uint8_t oversampling;
};

int adc_channel_setup_dt(const struct adc_dt_spec *spec);
int adc_raw_to_millivolts_dt(const struct adc_dt_spec *spec, int32_t *valp);
int adc_read(const struct device *dev, const struct adc_sequence *sequence);
int adc_sequence_init_dt(const struct adc_dt_spec *spec, struct adc_sequence *seq);

// ------------------------------------------------------------------------

struct fs_file_t {
   FILE* pF;
};
//#define FS_O_CREATE | FS_O_RDWR "w"
#define FS_O_CREATE 0x01
#define FS_O_RDWR 0x02
#define FS_O_READ 0x04
#define FS_SEEK_SET 0

class FlashFS {
 public:
    explicit FlashFS(const char* name = "??");

    static FlashFS& inst();
    FlashFS() = delete;
    virtual ~FlashFS();
    bool file_exists(const char* path);
    int file_sync(fs_file_t* file);
    off_t file_seek(fs_file_t* file, off_t offset, int whence);    
    fs_file_t* file_open(const char* path, int flags);
    int file_close(fs_file_t* file);
    ssize_t file_read(fs_file_t* file, void* buffer, size_t size);
    ssize_t file_write(fs_file_t* file, const void* buffer, size_t size);
    int remove(const char* path);
};


#endif // __STUBS_H__