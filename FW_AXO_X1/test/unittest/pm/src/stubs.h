#ifndef __STUBS_H__
#define __STUBS_H__

#include <stdio.h>
#include <stdlib.h>
    #include <fcntl.h>
    #include <unistd.h>

#define SYS_REBOOT_COLD 8

extern bool gbRetVal;
extern uint32_t gu32Val;
extern int gIntVal;
extern char gBuf[1024];

#define k_timer_user_data_get(x) my_k_timer_user_data_get(x)
#define k_sleep(x)  
#define k_uptime_get() my_k_uptime_get()

void* my_k_timer_user_data_get(k_timer* timer);
void sys_reboot (int type);
int64_t my_k_uptime_get (void);

// ---------------c----------------------------------------------------------------

#define zephyr_user 77

extern struct gpio_dt_spec gGpioDtSpec;

#ifdef DEVICE_DT_GET
    #undef DEVICE_DT_GET
#endif
#define DEVICE_DT_GET(x) x

#ifdef DT_NODELABEL
    #undef DT_NODELABEL
#endif    
#define DT_NODELABEL(x) 0


#ifdef GPIO_DT_SPEC_GET
    #undef GPIO_DT_SPEC_GET
#endif    
#define GPIO_DT_SPEC_GET(x,y) gGpioDtSpec

#define GPIO_OUTPUT_INACTIVE 0
#define GPIO_OUTPUT_ACTIVE 1

#define GPIO_INPUT   BIT(16)
#define GPIO_OUTPUT  BIT(17)

typedef uint8_t gpio_pin_t;
typedef uint16_t gpio_dt_flags_t;
typedef uint16_t spi_operation_t;
typedef uint16_t pwm_flags_t;
typedef uint32_t gpio_flags_t;

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


int gpio_pin_configure_dt (const struct gpio_dt_spec *spec, gpio_flags_t extra_flags);
int gpio_pin_set_dt (const struct gpio_dt_spec *spec, int value);
int gpio_pin_get_raw (const struct device *port, gpio_pin_t pin);

#define device_is_ready(dev) my_device_is_ready(dev)

bool my_device_is_ready(const struct device *dev);


// -----------------------------------------------------------------------------------


struct fs_file_t {
   FILE* pF;
};
//#define FS_O_CREATE | FS_O_RDWR "w"
#define FS_O_CREATE 0x01
#define FS_O_RDWR 0x02
#define FS_O_READ 0x04
#define FS_SEEK_SET 0

//int file_remove(const char* path);

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

// -----------------------------------------------------------------------------------

int mqtt_helper_disconnect(void);


// -----------------------------------------------------------------------------------

int nrf_modem_at_printf(const char* fmt, ...);

// -----------------------------------------------------------------------------------

typedef enum
{
    NRFX_RESET_REASON_RESETPIN_MASK   = 0x00000001,
    NRFX_RESET_REASON_DOG0_MASK       = 0x00000002,
    NRFX_RESET_REASON_DOG_MASK        = 0x00000004, 
    NRFX_RESET_REASON_CTRLAP_MASK     = 0x00000008,
    NRFX_RESET_REASON_SREQ_MASK       = 0x00000010,
    NRFX_RESET_REASON_LOCKUP_MASK     = 0x00000020,
    NRFX_RESET_REASON_OFF_MASK        = 0x00000040,
    NRFX_RESET_REASON_LPCOMP_MASK     = 0x00000080,
    NRFX_RESET_REASON_DIF_MASK        = 0x00000100,
    NRFX_RESET_REASON_LSREQ_MASK      = 0x00000200,
    NRFX_RESET_REASON_LLOCKUP_MASK    = 0x00000400,
    NRFX_RESET_REASON_LDOG_MASK       = 0x00000800,
    NRFX_RESET_REASON_MFORCEOFF_MASK  = 0x00001000,
    NRFX_RESET_REASON_NFC_MASK        = 0x00002000,
    NRFX_RESET_REASON_DOG1_MASK       = 0x00004000,
    NRFX_RESET_REASON_VBUS_MASK       = 0x00008000,
    NRFX_RESET_REASON_LCTRLAP_MASK    = 0x00010000,
    NRFX_RESET_REASON_CTRLAPSOFT_MASK = 0x00020000,
    NRFX_RESET_REASON_CTRLAPHARD_MASK = 0x00040000,
    NRFX_RESET_REASON_CTRLAPPIN_MASK  = 0x00080000,
    NRFX_RESET_REASON_GRTC_MASK       = 0x00100000,
    NRFX_RESET_REASON_SECTAMPER_MASK  = 0x00200000,
} nrfx_reset_reason_mask_t;


uint32_t nrfx_reset_reason_get(void);
void nrfx_reset_reason_clear(uint32_t mask);

// -----------------------------------------------------------------------------------

int popcount32(unsigned int x);

// -----------------------------------------------------------------------------------


enum pm_device_action { 
    PM_DEVICE_ACTION_SUSPEND , 
    PM_DEVICE_ACTION_RESUME , 
    PM_DEVICE_ACTION_TURN_OFF , 
    PM_DEVICE_ACTION_TURN_ON 
};

int pm_device_action_run (const struct device *dev, enum pm_device_action action);


#endif // __STUBS_H__