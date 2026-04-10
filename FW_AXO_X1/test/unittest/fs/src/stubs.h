#ifndef __STUBS_H__
#define __STUBS_H__

#include <stdio.h>
#include <stdlib.h>
    #include <fcntl.h>
    #include <unistd.h>

extern bool gbRetVal;
extern int gIntVal;
extern char gBuf[1024];

// --------------------------------------------------------------------------------------

#define k_sleep(x)  mysleep(x)
#ifdef LOG_PRINTK
    #undef LOG_PRINTK
#endif    
#define LOG_PRINTK(x,...) 

int32_t mysleep(int32_t y);
// --------------------------------------------------------------------------------------

#define MAX_FILE_NAME 255

#define FS_LITTLEFS_DECLARE_DEFAULT_CONFIG(x)  

enum lfs_error {
    LFS_ERR_OK          = 0,    // No error
    LFS_ERR_IO          = -5,   // Error during device operation
    LFS_ERR_CORRUPT     = -84,  // Corrupted
    LFS_ERR_NOENT       = -2,   // No directory entry
    LFS_ERR_EXIST       = -17,  // Entry already exists
    LFS_ERR_NOTDIR      = -20,  // Entry is not a dir
    LFS_ERR_ISDIR       = -21,  // Entry is a dir
    LFS_ERR_NOTEMPTY    = -39,  // Dir is not empty
    LFS_ERR_BADF        = -9,   // Bad file number
    LFS_ERR_FBIG        = -27,  // File too large
    LFS_ERR_INVAL       = -22,  // Invalid parameter
    LFS_ERR_NOSPC       = -28,  // No space left on device
    LFS_ERR_NOMEM       = -12,  // No more memory available
    LFS_ERR_NOATTR      = -61,  // No data/attr available
    LFS_ERR_NAMETOOLONG = -36,  // File name too long
};

enum {
	FS_FATFS = 0,
	FS_LITTLEFS,
	FS_TYPE_EXTERNAL_BASE,
};

/*struct _dnode {
	union {
		struct _dnode *head; 
		struct _dnode *next; 
	};
	union {
		struct _dnode *tail; 
		struct _dnode *prev; 
	};
};

typedef struct _dnode sys_dlist_t;
typedef struct _dnode sys_dnode_t;*/

typedef uint8_t fs_mode_t;

typedef uint32_t lfs_size_t;
typedef uint32_t lfs_off_t;

typedef int32_t  lfs_ssize_t;
typedef int32_t  lfs_soff_t;

typedef uint32_t lfs_block_t;

struct fs_file_system_t;

struct fs_mount_t {
	//sys_dnode_t node;
	int type;
	const char *mnt_point;
	void *fs_data;
	void *storage_dev;
	/* fields filled by file system core */
	size_t mountp_len;
	const struct fs_file_system_t *fs;
	uint8_t flags;
};

struct fs_file_t {
	void *filep;
	const struct fs_mount_t *mp;
	fs_mode_t flags;
};

struct fs_dir_t {
	void *dirp;
	const struct fs_mount_t *mp;
};

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

// Configuration provided during initialization of the littlefs
struct lfs_config {
    // Opaque user provided context that can be used to pass
    // information to the block device operations
    void *context;

    // Read a region in a block. Negative error codes are propagated
    // to the user.
    int (*read)(const struct lfs_config *c, lfs_block_t block,
            lfs_off_t off, void *buffer, lfs_size_t size);

    // Program a region in a block. The block must have previously
    // been erased. Negative error codes are propagated to the user.
    // May return LFS_ERR_CORRUPT if the block should be considered bad.
    int (*prog)(const struct lfs_config *c, lfs_block_t block,
            lfs_off_t off, const void *buffer, lfs_size_t size);

    // Erase a block. A block must be erased before being programmed.
    // The state of an erased block is undefined. Negative error codes
    // are propagated to the user.
    // May return LFS_ERR_CORRUPT if the block should be considered bad.
    int (*erase)(const struct lfs_config *c, lfs_block_t block);

    // Sync the state of the underlying block device. Negative error codes
    // are propagated to the user.
    int (*sync)(const struct lfs_config *c);

#ifdef LFS_THREADSAFE
    // Lock the underlying block device. Negative error codes
    // are propagated to the user.
    int (*lock)(const struct lfs_config *c);

    // Unlock the underlying block device. Negative error codes
    // are propagated to the user.
    int (*unlock)(const struct lfs_config *c);
#endif

    // Minimum size of a block read in bytes. All read operations will be a
    // multiple of this value.
    lfs_size_t read_size;

    // Minimum size of a block program in bytes. All program operations will be
    // a multiple of this value.
    lfs_size_t prog_size;

    // Size of an erasable block in bytes. This does not impact ram consumption
    // and may be larger than the physical erase size. However, non-inlined
    // files take up at minimum one block. Must be a multiple of the read and
    // program sizes.
    lfs_size_t block_size;

    // Number of erasable blocks on the device.
    lfs_size_t block_count;

    // Number of erase cycles before littlefs evicts metadata logs and moves
    // the metadata to another block. Suggested values are in the
    // range 100-1000, with large values having better performance at the cost
    // of less consistent wear distribution.
    //
    // Set to -1 to disable block-level wear-leveling.
    int32_t block_cycles;

    // Size of block caches in bytes. Each cache buffers a portion of a block in
    // RAM. The littlefs needs a read cache, a program cache, and one additional
    // cache per file. Larger caches can improve performance by storing more
    // data and reducing the number of disk accesses. Must be a multiple of the
    // read and program sizes, and a factor of the block size.
    lfs_size_t cache_size;

    // Size of the lookahead buffer in bytes. A larger lookahead buffer
    // increases the number of blocks found during an allocation pass. The
    // lookahead buffer is stored as a compact bitmap, so each byte of RAM
    // can track 8 blocks. Must be a multiple of 8.
    lfs_size_t lookahead_size;

    // Optional statically allocated read buffer. Must be cache_size.
    // By default lfs_malloc is used to allocate this buffer.
    void *read_buffer;

    // Optional statically allocated program buffer. Must be cache_size.
    // By default lfs_malloc is used to allocate this buffer.
    void *prog_buffer;

    // Optional statically allocated lookahead buffer. Must be lookahead_size
    // and aligned to a 32-bit boundary. By default lfs_malloc is used to
    // allocate this buffer.
    void *lookahead_buffer;

    // Optional upper limit on length of file names in bytes. No downside for
    // larger names except the size of the info struct which is controlled by
    // the LFS_NAME_MAX define. Defaults to LFS_NAME_MAX when zero. Stored in
    // superblock and must be respected by other littlefs drivers.
    lfs_size_t name_max;

    // Optional upper limit on files in bytes. No downside for larger files
    // but must be <= LFS_FILE_MAX. Defaults to LFS_FILE_MAX when zero. Stored
    // in superblock and must be respected by other littlefs drivers.
    lfs_size_t file_max;

    // Optional upper limit on custom attributes in bytes. No downside for
    // larger attributes size but must be <= LFS_ATTR_MAX. Defaults to
    // LFS_ATTR_MAX when zero.
    lfs_size_t attr_max;

    // Optional upper limit on total space given to metadata pairs in bytes. On
    // devices with large blocks (e.g. 128kB) setting this to a low size (2-8kB)
    // can help bound the metadata compaction time. Must be <= block_size.
    // Defaults to block_size when zero.
    lfs_size_t metadata_max;
};

typedef struct lfs_mdir {
    lfs_block_t pair[2];
    uint32_t rev;
    lfs_off_t off;
    uint32_t etag;
    uint16_t count;
    bool erased;
    bool split;
    lfs_block_t tail[2];
} lfs_mdir_t;

// littlefs directory type
typedef struct lfs_dir {
    struct lfs_dir *next;
    uint16_t id;
    uint8_t type;
    lfs_mdir_t m;

    lfs_off_t pos;
    lfs_block_t head[2];
} lfs_dir_t;

typedef struct lfs_gstate {
    uint32_t tag;
    lfs_block_t pair[2];
} lfs_gstate_t;

typedef struct lfs_cache {
    lfs_block_t block;
    lfs_off_t off;
    lfs_size_t size;
    uint8_t *buffer;
} lfs_cache_t;


// The littlefs filesystem type
typedef struct lfs {
    lfs_cache_t rcache;
    lfs_cache_t pcache;

    lfs_block_t root[2];
    struct lfs_mlist {
        struct lfs_mlist *next;
        uint16_t id;
        uint8_t type;
        lfs_mdir_t m;
    } *mlist;
    uint32_t seed;

    lfs_gstate_t gstate;
    lfs_gstate_t gdisk;
    lfs_gstate_t gdelta;

    struct lfs_free {
        lfs_block_t off;
        lfs_block_t size;
        lfs_block_t i;
        lfs_block_t ack;
        uint32_t *buffer;
    } free;

    const struct lfs_config *cfg;
    lfs_size_t name_max;
    lfs_size_t file_max;
    lfs_size_t attr_max;

} lfs_t;


/// internal littlefs data structures ///
/*typedef struct lfs_cache {
    lfs_block_t block;
    lfs_off_t off;
    lfs_size_t size;
    uint8_t *buffer;
} lfs_cache_t;

typedef struct lfs_mdir {
    lfs_block_t pair[2];
    uint32_t rev;
    lfs_off_t off;
    uint32_t etag;
    uint16_t count;
    bool erased;
    bool split;
    lfs_block_t tail[2];
} lfs_mdir_t;

// littlefs directory type
typedef struct lfs_dir {
    struct lfs_dir *next;
    uint16_t id;
    uint8_t type;
    lfs_mdir_t m;

    lfs_off_t pos;
    lfs_block_t head[2];
} lfs_dir_t;*/

// littlefs file type
typedef struct lfs_file {
    struct lfs_file *next;
    uint16_t id;
    uint8_t type;
    lfs_mdir_t m;

    struct lfs_ctz {
        lfs_block_t head;
        lfs_size_t size;
    } ctz;

    uint32_t flags;
    lfs_off_t pos;
    lfs_block_t block;
    lfs_off_t off;
    lfs_cache_t cache;

    const struct lfs_file_config *cfg;
} lfs_file_t;




#define CONFIG_FS_LITTLEFS_LOOKAHEAD_SIZE 1024
struct fs_littlefs {
	/* Defaulted in driver, customizable before mount. */
	struct lfs_config cfg;

	/* Must be cfg.cache_size */
	uint8_t *read_buffer;

	/* Must be cfg.cache_size */
	uint8_t *prog_buffer;

	/* Must be cfg.lookahead_size/4 elements, and
	 * cfg.lookahead_size must be a multiple of 8.
	 */
	uint32_t *lookahead_buffer[CONFIG_FS_LITTLEFS_LOOKAHEAD_SIZE / sizeof(uint32_t)];

	/* These structures are filled automatically at mount. */
	struct lfs lfs;
	void *backend;
	///struct k_mutex mutex;
};


int open(struct fs_file_t *filp, const char *fs_path, fs_mode_t flags);
ssize_t read(struct fs_file_t *filp, void *dest, size_t nbytes);
ssize_t write(struct fs_file_t *filp, const void *src, size_t nbytes);
int lseek(struct fs_file_t *filp, off_t off, int whence);
off_t tell(struct fs_file_t *filp);
int truncate(struct fs_file_t *filp, off_t length);
int sync(struct fs_file_t *filp);
int close(struct fs_file_t *filp);
int opendir(struct fs_dir_t *dirp, const char *fs_path);
int readdir(struct fs_dir_t *dirp, struct fs_dirent *entry);
int closedir(struct fs_dir_t *dirp);
int mount(struct fs_mount_t *mountp);
int unmount(struct fs_mount_t *mountp);
int unlink(struct fs_mount_t *mountp, const char *name);
int rename(struct fs_mount_t *mountp, const char *from, const char *to);
int mkdir(struct fs_mount_t *mountp, const char *name);
int stat(struct fs_mount_t *mountp, const char *path, struct fs_dirent *entry);
int statvfs(struct fs_mount_t *mountp, const char *path,struct fs_statvfs *stat);

int fs_open(struct fs_file_t *filp, const char *fs_path, fs_mode_t flags);
ssize_t fs_read(struct fs_file_t *filp, void *dest, size_t nbytes);
ssize_t fs_write(struct fs_file_t *filp, const void *src, size_t nbytes);
int fs_seek(struct fs_file_t *filp, off_t off, int whence);
off_t fs_tell(struct fs_file_t *filp);
int fs_truncate(struct fs_file_t *filp, off_t length);
int fs_sync(struct fs_file_t *filp);
int fs_close(struct fs_file_t *filp);
int fs_opendir(struct fs_dir_t *dirp, const char *fs_path);
int fs_readdir(struct fs_dir_t *dirp, struct fs_dirent *entry);
int fs_closedir(struct fs_dir_t *dirp);
int fs_mount(struct fs_mount_t *mountp);
int fs_unmount(struct fs_mount_t *mountp);
int fs_unlink(const char *name);
int fs_rename(const char *from, const char *to);
int fs_mkdir(const char *name);
int fs_stat(const char *path, struct fs_dirent *entry);
int fs_statvfs(const char *path,struct fs_statvfs *stat);

void fs_file_t_init(struct fs_file_t *zfp);
void fs_dir_t_init(struct fs_dir_t *zdp);
int lfs_dir_rewind(lfs_t *lfs, lfs_dir_t *dir);

struct fs_file_system_t {
	/* File operations */
	int (*open)(struct fs_file_t *filp, const char *fs_path,
		    fs_mode_t flags);
	ssize_t (*read)(struct fs_file_t *filp, void *dest, size_t nbytes);
	ssize_t (*write)(struct fs_file_t *filp,
					const void *src, size_t nbytes);
	int (*lseek)(struct fs_file_t *filp, off_t off, int whence);
	off_t (*tell)(struct fs_file_t *filp);
	int (*truncate)(struct fs_file_t *filp, off_t length);
	int (*sync)(struct fs_file_t *filp);
	int (*close)(struct fs_file_t *filp);
	/* Directory operations */
	int (*opendir)(struct fs_dir_t *dirp, const char *fs_path);
	int (*readdir)(struct fs_dir_t *dirp, struct fs_dirent *entry);
	int (*closedir)(struct fs_dir_t *dirp);
	/* File system level operations */
	int (*mount)(struct fs_mount_t *mountp);
	int (*unmount)(struct fs_mount_t *mountp);
	int (*unlink)(struct fs_mount_t *mountp, const char *name);
	int (*rename)(struct fs_mount_t *mountp, const char *from,
					const char *to);
	int (*mkdir)(struct fs_mount_t *mountp, const char *name);
	int (*stat)(struct fs_mount_t *mountp, const char *path,
					struct fs_dirent *entry);
	int (*statvfs)(struct fs_mount_t *mountp, const char *path,
					struct fs_statvfs *stat);
/*#if defined(CONFIG_FILE_SYSTEM_MKFS)
	int (*mkfs)(uintptr_t dev_id, void *cfg, int flags);
#endif*/
};

lfs_soff_t lfs_file_size(lfs_t *lfs, lfs_file_t *file);
int lfs_dir_seek(lfs_t *lfs, lfs_dir_t *dir, lfs_off_t off);
lfs_soff_t lfs_dir_tell(lfs_t *lfs, lfs_dir_t *dir);

// -------------------------------------------------------------------------

struct flash_area {
  uint8_t  fa_id;         /** The slot/scratch identification */
  uint16_t pad16;
  uint32_t fa_off;        /** The flash offset from the beginning */
  uint32_t fa_size;       /** The size of this sector */
  const struct device *fa_dev;
};

int flash_area_open(uint8_t id, const struct flash_area **fa);
void flash_area_close(const struct flash_area *fa);
int flash_area_erase(const struct flash_area *fa, uint32_t off, uint32_t len);



#endif // __STUBS_H__