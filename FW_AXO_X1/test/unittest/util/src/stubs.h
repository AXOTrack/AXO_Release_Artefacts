#ifndef __STUBS_H__
#define __STUBS_H__

#include <stdlib.h>
#include <stdio.h>
#include <string.h>

extern bool gbRetVal;
extern bool gbReady;
extern int gIntRetVal;
extern char gBuf[1024];

//const char* getFwVersion();

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
};

//void* k_timer_user_data_get(k_timer* timer);

typedef uint32_t nrf_sec_tag_t;

enum modem_key_mgmt_cred_type {
	MODEM_KEY_MGMT_CRED_TYPE_CA_CHAIN,
	MODEM_KEY_MGMT_CRED_TYPE_PUBLIC_CERT,
	MODEM_KEY_MGMT_CRED_TYPE_PRIVATE_CERT,
	MODEM_KEY_MGMT_CRED_TYPE_PSK,
	MODEM_KEY_MGMT_CRED_TYPE_IDENTITY
};

int modem_key_mgmt_exists(nrf_sec_tag_t sec_tag,
			  enum modem_key_mgmt_cred_type cred_type,
			  bool *exists);
int modem_key_mgmt_write(nrf_sec_tag_t sec_tag,
			 enum modem_key_mgmt_cred_type cred_type,
			 const void *buf, size_t len);
int modem_key_mgmt_cmp(nrf_sec_tag_t sec_tag,
		       enum modem_key_mgmt_cred_type cred_type,
		       const void *buf, size_t len);

// ----------------------------------------------------------------------

typedef struct
{
    uint32_t state; //[8];               
    //uint32_t total[2];          
    //unsigned char buffer[64];   
} mbedtls_sha1_context;

typedef struct
{
    uint32_t state; //[8];               
    //uint32_t total[2];          
    //unsigned char buffer[64];   
} mbedtls_sha256_context;

void mbedtls_sha1_init( mbedtls_sha1_context *ctx );
void mbedtls_sha1_free( mbedtls_sha1_context *ctx );
int mbedtls_sha1_starts( mbedtls_sha1_context *ctx );
void mbedtls_sha1_update( mbedtls_sha1_context *ctx, const unsigned char *input, size_t ilen );
void mbedtls_sha1_finish( mbedtls_sha1_context *ctx, unsigned char output[20] );

void mbedtls_sha256_init( mbedtls_sha256_context *ctx );
void mbedtls_sha256_free( mbedtls_sha256_context *ctx );
int mbedtls_sha256_starts( mbedtls_sha256_context *ctx, int is224 );
void mbedtls_sha256_update( mbedtls_sha256_context *ctx, const unsigned char *input, size_t ilen );
void mbedtls_sha256_finish( mbedtls_sha256_context *ctx, unsigned char output[20] );

// ----------------------------------------------------------------------


#ifdef DT_CHOSEN
    #undef DT_CHOSEN
#endif
#define DT_CHOSEN(y) y

#ifdef DEVICE_DT_GET
    #undef DEVICE_DT_GET
#endif
#define DEVICE_DT_GET(x) x

#ifdef DEVICE_NAME_GET
    #undef DEVICE_NAME_GET
#endif
#define DEVICE_NAME_GET(id) "name"

#define zephyr_entropy 0

#define device_is_ready(dev) my_device_is_ready(dev)

bool my_device_is_ready(const struct device *dev);

// ----------------------------------------------------------------------

#define MBEDTLS_ECDSA_MAX_LEN 124

/*typedef struct
{
        uint32_t state[8];            
        uint32_t total[2];            
        unsigned char buffer[64];     
        int is224;                    
} mbedtls_sha256_context;

void mbedtls_sha256_init( mbedtls_sha256_context *ctx );
void mbedtls_sha256_free( mbedtls_sha256_context *ctx );
int mbedtls_sha256_starts( mbedtls_sha256_context *ctx, int is224 );
void mbedtls_sha256_update( mbedtls_sha256_context *ctx, const unsigned char *input, size_t ilen );
void mbedtls_sha256_finish( mbedtls_sha256_context *ctx, unsigned char output[32] );*/

typedef struct
{
    unsigned int                    keybits;   
    uint32_t                        key[8];    
} mbedtls_aes_context;

typedef struct
{
    unsigned char counter[16];  
    int reseed_counter;         
    int prediction_resistance;  
    size_t entropy_len;         
    int reseed_interval;        
    mbedtls_aes_context aes_ctx;        
    int (*f_entropy)(void *, unsigned char *, size_t);
    void *p_entropy;            
} mbedtls_ctr_drbg_context;

typedef enum {
    MBEDTLS_PK_NONE=0,
    MBEDTLS_PK_ECKEY,
    MBEDTLS_PK_ECKEY_DH,
    MBEDTLS_PK_ECDSA,
    MBEDTLS_PK_RSA_ALT,
    MBEDTLS_PK_RSASSA_PSS,
} mbedtls_pk_type_t;

 typedef enum {
    MBEDTLS_MD_NONE=0,
    MBEDTLS_MD_MD2,
     MBEDTLS_MD_MD4,
    MBEDTLS_MD_MD5,
   MBEDTLS_MD_SHA1,
    MBEDTLS_MD_SHA224,
     MBEDTLS_MD_SHA256,
     MBEDTLS_MD_SHA384,
    MBEDTLS_MD_SHA512,
     MBEDTLS_MD_RIPEMD160,
} mbedtls_md_type_t;

 typedef enum
{
    MBEDTLS_ECP_DP_NONE = 0,
    MBEDTLS_ECP_DP_SECP192R1,      
    MBEDTLS_ECP_DP_SECP224R1,      
    MBEDTLS_ECP_DP_SECP256R1,      
    MBEDTLS_ECP_DP_SECP384R1,      
    MBEDTLS_ECP_DP_SECP521R1,      
    MBEDTLS_ECP_DP_BP256R1,        
    MBEDTLS_ECP_DP_BP384R1,        
    MBEDTLS_ECP_DP_BP512R1,        
    MBEDTLS_ECP_DP_CURVE25519,           
     MBEDTLS_ECP_DP_SECP192K1,      
    MBEDTLS_ECP_DP_SECP224K1,      
   MBEDTLS_ECP_DP_SECP256K1,      
} mbedtls_ecp_group_id;

 typedef enum
{
    MBEDTLS_PK_DEBUG_NONE = 0,
    MBEDTLS_PK_DEBUG_MPI,
     MBEDTLS_PK_DEBUG_ECP,
} mbedtls_pk_debug_type;

typedef struct
{
    mbedtls_pk_debug_type type;
     const char *name;
     void *value;
} mbedtls_pk_debug_item;

struct mbedtls_pk_info_t
{
        mbedtls_pk_type_t type;
        const char *name;
        size_t (*get_bitlen)( const void * );
        int (*can_do)( mbedtls_pk_type_t type );
        int (*verify_func)( void *ctx, mbedtls_md_type_t md_alg,
                                const unsigned char *hash, size_t hash_len,
                                const unsigned char *sig, size_t sig_len );
        int (*sign_func)( void *ctx, mbedtls_md_type_t md_alg,
                        const unsigned char *hash, size_t hash_len,
                        unsigned char *sig, size_t *sig_len,
                        int (*f_rng)(void *, unsigned char *, size_t),
                        void *p_rng );
        int (*decrypt_func)( void *ctx, const unsigned char *input, size_t ilen,
                                unsigned char *output, size_t *olen, size_t osize,
                                int (*f_rng)(void *, unsigned char *, size_t),
                                void *p_rng );
        int (*encrypt_func)( void *ctx, const unsigned char *input, size_t ilen,
                                unsigned char *output, size_t *olen, size_t osize,
                                int (*f_rng)(void *, unsigned char *, size_t),
                                void *p_rng );
        int (*check_pair_func)( const void *pub, const void *prv );
        void * (*ctx_alloc_func)( void );
        void (*ctx_free_func)( void *ctx );
        void (*debug_func)( const void *ctx, mbedtls_pk_debug_item *items );
};

typedef uint32_t mbedtls_mpi_uint;

typedef struct
{
    int s;              
    size_t n;           
    mbedtls_mpi_uint *p;          
} mbedtls_mpi;

typedef struct
{
    mbedtls_mpi X;          
    mbedtls_mpi Y;          
    mbedtls_mpi Z;          
} mbedtls_ecp_point;

typedef struct
{
    mbedtls_ecp_group_id id;    
   mbedtls_mpi P;              
    mbedtls_mpi A;              
    mbedtls_mpi B;              
    mbedtls_ecp_point G;        
    mbedtls_mpi N;              
    size_t pbits;       
     size_t nbits;       
     unsigned int h;     
     int (*modp)(mbedtls_mpi *); 
     int (*t_pre)(mbedtls_ecp_point *, void *);  
     int (*t_post)(mbedtls_ecp_point *, void *); 
    void *t_data;                       
     mbedtls_ecp_point *T;       
    size_t T_size;      
} mbedtls_ecp_group;

typedef struct
{
    mbedtls_ecp_group grp;      
    mbedtls_mpi d;              
    mbedtls_ecp_point Q;        
} mbedtls_ecp_keypair;



typedef struct
{
    const mbedtls_pk_info_t *   pk_info; 
    void *                      pk_ctx;  
} mbedtls_pk_context;

void mbedtls_ctr_drbg_init( mbedtls_ctr_drbg_context *ctx );
int mbedtls_ctr_drbg_seed( mbedtls_ctr_drbg_context *ctx,
                   int (*f_entropy)(void *, unsigned char *, size_t),
                   void *p_entropy,
                   const unsigned char *custom,
                   size_t len );
void mbedtls_ctr_drbg_free( mbedtls_ctr_drbg_context *ctx );

int mbedtls_ctr_drbg_random( void *p_rng,
                     unsigned char *output, size_t output_len );
  
int mbedtls_ecp_gen_key( mbedtls_ecp_group_id grp_id, mbedtls_ecp_keypair *key,
                 int (*f_rng)(void *, unsigned char *, size_t), void *p_rng );

   void mbedtls_pk_init( mbedtls_pk_context *ctx );
   int mbedtls_pk_setup( mbedtls_pk_context *ctx, const mbedtls_pk_info_t *info );
   void mbedtls_pk_free( mbedtls_pk_context *ctx );

int mbedtls_pk_write_pubkey_pem( const mbedtls_pk_context *ctx, unsigned char *buf, size_t size );
int mbedtls_pk_write_key_pem( const mbedtls_pk_context *ctx, unsigned char *buf, size_t size );  
const mbedtls_pk_info_t *mbedtls_pk_info_from_type( mbedtls_pk_type_t pk_type ); 
static inline mbedtls_ecp_keypair *mbedtls_pk_ec( const mbedtls_pk_context pk )
{
    return( (mbedtls_ecp_keypair *) (pk).pk_ctx );
}
int mbedtls_pk_parse_key( mbedtls_pk_context *ctx,
              const unsigned char *key, size_t keylen,
              const unsigned char *pwd, size_t pwdlen,
              int (*f_rng)(void *, unsigned char *, size_t), void *p_rng );
int mbedtls_pk_sign( mbedtls_pk_context *ctx, mbedtls_md_type_t md_alg,
              const unsigned char *hash, size_t hash_len,
              unsigned char *sig, size_t sig_size, size_t *sig_len,
              int (*f_rng)(void *, unsigned char *, size_t), void *p_rng );              

int entropy_get_entropy(const struct device *dev,
                                  uint8_t *buffer,
                                  uint16_t length);

#endif // __STUBS_H__