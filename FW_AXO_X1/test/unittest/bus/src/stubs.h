#ifndef __STUBS_H__
#define __STUBS_H__

#include <stdio.h>
#include <stdlib.h>

typedef uint8_t gpio_pin_t;
typedef uint16_t gpio_dt_flags_t;
typedef uint16_t spi_operation_t;

struct gpio_dt_spec {
        //const struct device *port;
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
#define DT_NODELABEL(x) 0
#define GPIO_DT_SPEC_GET(x,y) gGpioDtSpec

#define SPI_WORD_SET(x) x


extern bool gbReady;
extern bool gbRetVal;

#define device_is_ready(dev) my_device_is_ready(dev)

int i2c_configure(const struct device *dev, uint32_t dev_config);
bool my_device_is_ready(const struct device *dev);

#endif // __STUBS_H__