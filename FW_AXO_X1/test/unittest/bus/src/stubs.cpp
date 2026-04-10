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

extern "C" void qemu_exit(int status) {
    /* ADP_Stopped_ApplicationExit */
    static volatile unsigned int exit_block[2];

    exit_block[0] = 0x20026;                       /* reason */
    exit_block[1] = status;                        /* exit code */

    register unsigned int r0 __asm__("r0") = 0x20; /* SYS_EXIT_EXTENDED */
    register unsigned int r1 __asm__("r1") = (unsigned int)exit_block;

    __asm__ volatile("bkpt 0xAB" : : "r"(r0), "r"(r1) : "memory");

    /* Should never return */
    for (;;)
        ;
}

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