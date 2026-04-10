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

#include "stubs.h"

bool gbRetVal;
bool gbReady;
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