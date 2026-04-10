#ifndef __STUBS_H__
#define __STUBS_H__

#include <stdio.h>
#include <stdlib.h>

extern uint8_t gbU8;

#define MODEM_INFO_FWVER_SIZE 41
//#define RSRP_IDX_TO_DBM(rsrp) ((rsrp) < 0 ?  (rsrp) - 140 : (rsrp) - 141)

enum  	modem_info {
  MODEM_INFO_RSRP , MODEM_INFO_CUR_BAND , MODEM_INFO_SUP_BAND , MODEM_INFO_AREA_CODE ,
  MODEM_INFO_UE_MODE , MODEM_INFO_OPERATOR , MODEM_INFO_MCC , MODEM_INFO_MNC ,
  MODEM_INFO_CELLID , MODEM_INFO_IP_ADDRESS , MODEM_INFO_UICC , MODEM_INFO_BATTERY ,
  MODEM_INFO_TEMP , MODEM_INFO_FW_VERSION , MODEM_INFO_ICCID , MODEM_INFO_LTE_MODE ,
  MODEM_INFO_NBIOT_MODE , MODEM_INFO_GPS_MODE , MODEM_INFO_IMSI , MODEM_INFO_IMEI ,
  MODEM_INFO_DATE_TIME , MODEM_INFO_APN , MODEM_INFO_COUNT
};

#define k_timer_user_data_get(x) my_k_timer_user_data_get(x)

void* my_k_timer_user_data_get(k_timer* timer);


typedef void (*at_monitor_handler_t)(const char *notif);

struct at_monitor_entry {
    const char *filter;
    const at_monitor_handler_t handler;
    struct {
        uint8_t paused : 1; /* Monitor is paused. */
        uint8_t direct : 1; /* Dispatch in ISR. */
    } flags;
};


int modem_info_init(void);
int	modem_info_get_batt_voltage(int *val);
int	modem_info_get_temperature(int *val);
int modem_info_get_fw_version (char *buf, size_t buf_size);
int modem_info_string_get(enum modem_info info, char *buf, const size_t buf_size);

int nrf_modem_at_scanf(const char* cmd, const char* fmt, ...);
int nrf_modem_at_printf(const char* fmt, ...);

void at_monitor_resume(struct at_monitor_entry* mon);

int lte_lc_psm_get(int *tau, int *active_time);

int RSRP_IDX_TO_DBM(int inout);




#endif // __STUBS_H__