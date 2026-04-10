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

//class FwFsm {
// public:
    /*/! Enumeration defining the state ID's
    enum StateId {
        eNoChange = -1,  //!< Indicating that state has not been changed
        eIdle = 0,       //!< ID of Idle state
        eTriggered,      //!< OTA cmd received, awaiting firmware info
        eDownload,       //!< Download started
        eReady,       //!< Download complete, ready to apply update on restart
        kNumFwStates  //!< Last entry indicationg the number of states
    };
    FwFsm(void* context, FwFsm::StateId initialState);
    FwFsm() = delete;
    virtual ~FwFsm();

    void setState(FwFsm::StateId newStateId);
    int getStateId(void);
    void handle_OtaStart();
    void handle_FwInfo(PoolElem** pElem);
    void handle_DlComplete(PoolElem** pElem);
    void handle_Restart();
    */
    //void handle_DlFailed(PoolElem** pElem);
    //void handle_timer_exp();

    //static uint16_t getTriggerTimeout() { return triggerTimeout; }
    //static uint16_t getDownloadTimeout() { return downloadTimeout; }

 //protected:
    /*/! Pointer to the class instance of the current state
    class FwState* myState;    
    int myStateId;  //!< ID of the current state
    fwStateCreator stateTbl[kNumFwStates];  //!< Array of state creator
                                            //!< functions, ordered by state ID
    void* pd;  //!< Pointer to data object, normally the this pointer of the
               //!< owner class instance

 private:
    // Those files are defined here because its in testing phase Later
    // if needed can be shifted in constructor
    static const uint16_t triggerTimeout =
        300;  //!< The trigger timeout value in s
    static const uint16_t downloadTimeout =
        300;  //!< The download timeout value in s

    void ReportOtaUpdateActive(void);*/
//};

// ---------------------------------------------------------------------------------



// ---------------------------------------------------------------------------------



#endif // __STUBS_H__