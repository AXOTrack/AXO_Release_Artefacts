#ifndef __STUBS_H__
#define __STUBS_H__

#include <stdio.h>
#include <stdlib.h>
    #include <fcntl.h>
    #include <unistd.h>

extern bool gbRetVal;
extern int gIntVal;


#define k_timer_user_data_get(x) my_k_timer_user_data_get(x)
#define k_sleep(x)  
#define k_uptime_get() my_k_uptime_get()

#define SYS_REBOOT_COLD 1
#define SYS_REBOOT_WARM 2

void* my_k_timer_user_data_get(k_timer* timer);
void sys_reboot (int type);
int64_t my_k_uptime_get (void);


// ---------------------------------------------------------------------------------

enum fota_download_evt_id {
    FOTA_DOWNLOAD_EVT_PROGRESS,
    FOTA_DOWNLOAD_EVT_FINISHED,
    FOTA_DOWNLOAD_EVT_ERASE_PENDING,
    FOTA_DOWNLOAD_EVT_ERASE_DONE,
    FOTA_DOWNLOAD_EVT_ERROR,
    FOTA_DOWNLOAD_EVT_CANCELLED
};

enum fota_download_error_cause {
    FOTA_DOWNLOAD_ERROR_CAUSE_NO_ERROR,
    FOTA_DOWNLOAD_ERROR_CAUSE_DOWNLOAD_FAILED,
    FOTA_DOWNLOAD_ERROR_CAUSE_INVALID_UPDATE,
    FOTA_DOWNLOAD_ERROR_CAUSE_TYPE_MISMATCH,
    FOTA_DOWNLOAD_ERROR_CAUSE_INTERNAL,
};

struct fota_download_evt {
    enum fota_download_evt_id id;

    union {
        enum fota_download_error_cause cause;
        int progress;
    };
};

typedef void (*fota_download_callback_t)(const struct fota_download_evt *evt);

int fota_download_init(fota_download_callback_t client_callback);
int fota_download_start(const char *host, const char *file, int sec_tag,
            uint8_t pdn_id, size_t fragment_size);
void fota_download_set_expected(int32_t size, uint8_t *sha1);

int boot_write_img_confirmed (void);

// ---------------------------------------------------------------------------------

class FwUpdCtrl {
 public:
    explicit FwUpdCtrl();
    virtual ~FwUpdCtrl();
	void otaStartConfirm();  // publish to /device/{device_id}/command/accepted
	void otaStartReject();   // publish to /device/{device_id}/command/rejected
	void otaFwConfirm();  // publish to /device/{device_id}/ota/update/accepted
	void otaFwReject();   // publish to /device/{device_id}/ota/update/rejected
	void startRebootTimer();  // delay reboot a few seconds on dl ready
	void startRetryTimer();   // generate timeout to next dl trial
	void requestDownload();   // request download on previous failure
	void startTimeout(uint16_t);

	uint16_t retryCnt;   //!< Retry counter: Retry attempt on donwload failure
	uint8_t maxRetries;  //!< Maximum number of retries
};



// ---------------------------------------------------------------------------------



// ---------------------------------------------------------------------------------



#endif // __STUBS_H__