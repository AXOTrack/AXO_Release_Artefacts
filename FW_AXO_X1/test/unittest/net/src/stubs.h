#ifndef __STUBS_H__
#define __STUBS_H__

#include <stdio.h>
#include <stdlib.h>

#include <string>

extern bool gbRetVal;
extern int gIntVal;
//extern char gBuf[1024];

void* k_timer_user_data_get(k_timer* timer);

// ------------------------------------------------------------------------------

#define SHA_HASH_SIZE 32
#define TLS_SEC_TAG 956
#define MAX_SIGNATURE_LEN 72

/*typedef struct
{
    uint32_t state; //[8];               
    //uint32_t total[2];          
    //unsigned char buffer[64];   
} mbedtls_sha256_context;*/


class Hash {
 public:
    ~Hash();

    static Hash* inst();

    void init();
    void update(uint8_t* pBuf, size_t len);
    void finalize();
    void print();

    uint8_t* getHash();
    uint16_t getHashLen();

 private:
    Hash();
    
    //mbedtls_sha256_context sha_ctx; //!< Context for SHA256 calculation  

    uint8_t hash[SHA_HASH_SIZE];  //!< The calculated SHA1 or SHA256 checksum
};

class ECDSASigner {
 public:
    ~ECDSASigner();

    static ECDSASigner* inst();
    
    void setPrivateKey(const std::string& pemKey);

    std::string signHash(const unsigned char* hash, size_t hashLen);

    void printSignature(const std::string& signature);

 private:
    ECDSASigner();
    
    //mbedtls_pk_context pk; //!< Context varaible
    //mbedtls_ctr_drbg_context ctr_drbg; //!< Context varaible
    std::string privateKeyPEM; //!< The private key in PEM format

    std::string string_to_hex(const std::string& input);
};

// ------------------------------------------------------------------------------

#define socklen_t uint32_t

typedef short unsigned int sa_family_t;

//#include <zephyr/net/net_ip.h>

/*struct sockaddr {
    int sin_family;
    int sin_port;
    uint32_t sin_addr;
};*/

struct addrinfo {
    int              ai_flags;
    int              ai_family;
    int              ai_socktype;
    int              ai_protocol;
    //socklen_t        ai_addrlen;
    struct sockaddr *ai_addr;
    //char            *ai_canonname;
    //struct addrinfo *ai_next;
};

int connect(int socket, const struct sockaddr *address, socklen_t address_len);
ssize_t send(int socket, const void *buffer, size_t length, int flags);
ssize_t recv(int socket, void *buffer, size_t length, int flags);
int setsockopt(int socket, int level, int option_name, const void *option_value, socklen_t option_len);
int getaddrinfo(const char * nodename, const char * servname,
       const struct addrinfo * hints, struct addrinfo ** res);
int socket(int domain, int type, int protocol);
int close(int fildes);

#ifdef htons
  #undef htons
#endif
uint16_t htons(uint16_t hostshort);


// ------------------------------------------------------------------------------

struct fs_file_t {
   FILE* pF;
};

#define FS_O_READ       0x01
#define FS_O_WRITE      0x02
#define FS_O_RDWR       (FS_O_READ | FS_O_WRITE)
#define FS_O_MODE_MASK  0x03
 
#define FS_O_CREATE     0x10
#define FS_O_APPEND     0x20
#define FS_O_TRUNC      0x40
#define FS_O_FLAGS_MASK 0x70
 
#define FS_SEEK_SET  0
#define FS_SEEK_CUR  1
#define FS_SEEK_END  2
 
#define MAX_FILE_NAME 128

//int file_remove(const char* path);

enum fs_dir_entry_type {
        FS_DIR_ENTRY_FILE = 0,
        FS_DIR_ENTRY_DIR
};
 struct fs_dirent {
        enum fs_dir_entry_type type;
        char name[MAX_FILE_NAME + 1];
        size_t size;
};
 
struct fs_statvfs {
        unsigned long f_bsize;
        unsigned long f_frsize;
        unsigned long f_blocks;
        unsigned long f_bfree;
};

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
    static void eraseFs();
    int file_tree(const char *path);
    int statvfs(const char* path, struct fs_statvfs* buf);
    int unmount();
    int stat(const char* path, struct fs_dirent* st);
    unsigned char* GetFsStat(const char *path);
};

class ResetCtrl /*: public Task*/ {
 public:
   /* explicit ResetCtrl(const char* name = "??");
    ResetCtrl() = delete;
    virtual ~ResetCtrl();
    virtual void task(void);
    static void timer_cb(k_timer* timer);
    void timer1_expired();
    void initDevice();
    void trigger();
    Mbox* pInMbox;   //!< Mailbox of the reset control task
    Timer* myTimer;  //!< Timer to control cold start cycle period*/

    static void performSwReset();

 /*private:
    struct gpio_dt_spec trigger_gpio;  //!< Container for GPIO pin information*/
};


enum mqtt_conn_return_code {
	MQTT_CONNECTION_ACCEPTED                = 0x00,
	MQTT_UNACCEPTABLE_PROTOCOL_VERSION      = 0x01,
	MQTT_IDENTIFIER_REJECTED                = 0x02,
	MQTT_SERVER_UNAVAILABLE                 = 0x03,
	MQTT_BAD_USER_NAME_OR_PASSWORD          = 0x04,
	MQTT_NOT_AUTHORIZED                     = 0x05
};

enum mqtt_qos {
   MQTT_QOS_0_AT_MOST_ONCE = 0x00 , 
   MQTT_QOS_1_AT_LEAST_ONCE = 0x01 , 
   MQTT_QOS_2_EXACTLY_ONCE = 0x02 
};
enum mqtt_helper_error {
	/** The received payload is larger than the payload buffer. */
	MQTT_HELPER_ERROR_MSG_SIZE,
};
struct mqtt_helper_buf {
	char *ptr;
	size_t size;
};

struct mqtt_utf8 {
	const uint8_t *utf8;       /**< Pointer to UTF-8 string. */
	uint32_t size;             /**< Size of UTF string, in bytes. */
};
struct mqtt_binstr {
	uint8_t *data;             /**< Pointer to binary stream. */
	uint32_t len;              /**< Length of binary stream. */
};
struct mqtt_topic {
	struct mqtt_utf8 topic;
	uint8_t qos;
};
struct mqtt_publish_message {
	struct mqtt_topic topic;     /**< Topic on which data was published. */
	struct mqtt_binstr payload; /**< Payload on the topic published. */
};
struct mqtt_publish_param {
	struct mqtt_publish_message message;
	uint16_t message_id;
	uint8_t dup_flag : 1;
	uint8_t retain_flag : 1;
};
struct mqtt_subscription_list {
        struct mqtt_topic *list; 
        uint16_t list_count; 
        uint16_t message_id;
};
struct mqtt_helper_conn_params {
	/* The hostname must be null-terminated. */
	struct mqtt_helper_buf hostname;
	struct mqtt_helper_buf device_id;
	struct mqtt_helper_buf user_name;
	struct mqtt_helper_buf password;
};
typedef void (*mqtt_helper_handler_t)(struct mqtt_evt *evt);
typedef void (*mqtt_helper_on_connack_t)(enum mqtt_conn_return_code return_code);
typedef void (*mqtt_helper_on_disconnect_t)(int result);
typedef void (*mqtt_helper_on_publish_t)(struct mqtt_helper_buf topic_buf,
					 struct mqtt_helper_buf payload_buf);
typedef void (*mqtt_helper_on_puback_t)(uint16_t message_id, int result);
typedef void (*mqtt_helper_on_suback_t)(uint16_t message_id, int result);
typedef void (*mqtt_helper_on_pingresp_t)(void);
typedef void (*mqtt_helper_on_error_t)(enum mqtt_helper_error error);

struct mqtt_helper_cfg {
	struct {
		mqtt_helper_on_connack_t on_connack;
		mqtt_helper_on_disconnect_t on_disconnect;
		mqtt_helper_on_publish_t on_publish;
		mqtt_helper_on_puback_t on_puback;
		mqtt_helper_on_suback_t on_suback;
		mqtt_helper_on_pingresp_t on_pingresp;
		mqtt_helper_on_error_t on_error;
	} cb;
};

int mqtt_helper_publish(const struct mqtt_publish_param *param);
int mqtt_helper_subscribe(struct mqtt_subscription_list *sub_list);
int mqtt_helper_connect(struct mqtt_helper_conn_params *conn_params);
int mqtt_helper_disconnect(void);
int mqtt_helper_abort(void) ;
int hw_id_get(char* buf, size_t buf_len);
int mqtt_helper_init(struct mqtt_helper_cfg *cfg);


/*
enum pdn_event {
   PDN_EVENT_CNEC_ESM,
   PDN_EVENT_ACTIVATED,
   PDN_EVENT_DEACTIVATED,
   PDN_EVENT_IPV6_UP,
   PDN_EVENT_IPV6_DOWN,
   PDN_EVENT_NETWORK_DETACH,
   PDN_EVENT_APN_RATE_CONTROL_ON,
   PDN_EVENT_APN_RATE_CONTROL_OFF,
   PDN_EVENT_CTX_DESTROYED,
};

typedef void (*pdn_event_handler_t)(uint8_t cid, enum pdn_event event, int reason);
typedef void (*lte_lc_evt_handler_t)(const struct lte_lc_evt *const evt);

int pdn_default_ctx_cb_reg (pdn_event_handler_t cb);
void lte_lc_register_handler(lte_lc_evt_handler_t handler);
int lte_lc_modem_events_enable(void);
int lte_lc_init(void);
int lte_lc_connect_async(lte_lc_evt_handler_t handler);
int lte_lc_deinit(void);
int lte_lc_offline(void);
int lte_lc_normal(void);
int lte_lc_psm_req(bool enable);
int lte_lc_power_off(void);
*/
#endif // __STUBS_H__