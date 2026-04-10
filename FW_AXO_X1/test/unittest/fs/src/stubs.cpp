#define private public
#include "rtos/mbox.h"
#include "rtos/mpelem.h"
#include "rtos/mpool.h"
#include "rtos/mqueue.h"
#include "rtos/task.h"
#include "rtos/task.h"

//#include "bus/spi/spibus.h"
//#include "drv/spi/fram.h"
//#include "drv/spi/adxl37x.h"
#include "pm/reset_ctrl.h"
//#include "./version.h"


#include "stubs.h"


// -------------------------------------------------------------------------

bool gbRetVal;
int gIntVal;
char gBuf[1024];

int32_t mysleep(int32_t y) { return 0; }


int fs_open(struct fs_file_t *filp, const char *fs_path, fs_mode_t flags) {
    return gIntVal;
}
ssize_t fs_read(struct fs_file_t *filp, void *dest, size_t nbytes) {
    uint8_t* des = (uint8_t*)dest;
    des[0] = 1;
    des[1] = 2;
    des[2] = 1;
    des[3] = 3;
    des[4] = 1;
    return nbytes;
}
ssize_t fs_write(struct fs_file_t *filp, const void *src, size_t nbytes) {
    uint8_t* p8 = (uint8_t*)src;
    gBuf[0] = p8[0];
    gBuf[1] = p8[1];
    gBuf[2] = p8[2];
    return nbytes;
}
int fs_seek(struct fs_file_t *filp, off_t off, int whence) {
    return 0;
}
off_t fs_tell(struct fs_file_t *filp) {
    return 0;
}
int fs_truncate(struct fs_file_t *filp, off_t length) {
    return 0;
}
int fs_sync(struct fs_file_t *filp) {
    return 0;
}
int fs_close(struct fs_file_t *filp) {
    return 0;
}
int fs_opendir(struct fs_dir_t *dirp, const char *fs_path) {
    if (gIntVal == -7) {
        return gIntVal;
    }
    return 0;
}
int fs_readdir(struct fs_dir_t *dirp, struct fs_dirent *entry) {
    static int iPassed = 0;
    if (gbRetVal) {
        if (gIntVal == -7) {
            entry->name[0] = 0;
            return gIntVal;
        } else if (gIntVal == -6) {
            entry->name[0] = 'a';
            return gIntVal;
        } else if (gIntVal == 1) {
            entry->name[0] = 0;
            return gIntVal;
        } else if (gIntVal == 2) {
            entry->name[0] = 'a';
            entry->type = FS_DIR_ENTRY_DIR;
            //printk("fs_readdir: gIntVal = %d,  iPassed = %d, entry.type = %d\n\n", gIntVal, iPassed, entry->type);
            if (iPassed == 1) {
                entry->type = FS_DIR_ENTRY_FILE;
                //printk("fs_readdir: gIntVal = %d,  iPassed = %d, entry.type = %d\n\n", gIntVal, iPassed, entry->type);
                gIntVal = -7;
                iPassed = 0;
                return 0;
            } /*else if (iPassed == 2) {
                iPassed = 0;
                entry->name[0] = 0;
                //printk("fs_readdir: gIntVal = %d,  iPassed = %d, entry.type = %d\n\n", gIntVal, iPassed, entry->type);
                return 2;
            }*/
            iPassed++;
        } else if (gIntVal == 3) {
            entry->name[0] = 'a';
            entry->type = FS_DIR_ENTRY_DIR;
            if (iPassed == 2) {
                iPassed = 0;
                entry->name[0] = 0;
                gIntVal = 0;
                return 0;
            }
            iPassed++;
        } else if (gIntVal == 4) {
            entry->name[0] = 0;
            gIntVal = -7;
            return 0;
        } else if (gIntVal == 5) {
            entry->name[0] = 'a';
            entry->type = FS_DIR_ENTRY_DIR;
            //printk("fs_readdir: gIntVal = %d,  iPassed = %d, entry.type = %d\n\n", gIntVal, iPassed, entry->type);
            if (iPassed == 1) {
                iPassed = 0;
                entry->name[0] = 0;
                //printk("fs_readdir: gIntVal = %d,  iPassed = %d, entry.type = %d\n\n", gIntVal, iPassed, entry->type);
                return 2;
            }
            iPassed++;
        }
    } else {
        if (gIntVal == -17) {
            entry->name[0] = 0;
            return gIntVal;
        } else if (gIntVal == -16) {
            entry->name[0] = 'a';
            return gIntVal;
        } else if (gIntVal == 11) {
            entry->name[0] = 0;
            return 0;
        } else if (gIntVal == 12) {
            entry->name[0] = 'a';
            entry->type = FS_DIR_ENTRY_DIR;
            printk("fs_readdir: gIntVal = %d,  iPassed = %d, entry.type = %d\n\n", gIntVal, iPassed, entry->type);
            if (iPassed == 1) {
                entry->type = FS_DIR_ENTRY_FILE;
                printk("fs_readdir: gIntVal = %d,  iPassed = %d, entry.type = %d\n\n", gIntVal, iPassed, entry->type);
            } else if (iPassed == 2) {
                iPassed = 0;
                entry->name[0] = 0;
                printk("fs_readdir: gIntVal = %d,  iPassed = %d, entry.type = %d\n\n", gIntVal, iPassed, entry->type);
                return gIntVal;
            }
            iPassed++;
        } else if (gIntVal == 13) {
            entry->name[0] = 'a';
            entry->type = FS_DIR_ENTRY_FILE;
            gIntVal = 11;
        } else if (gIntVal == 14) {
            entry->name[0] = 'a';
            entry->type = FS_DIR_ENTRY_DIR;
            gbRetVal = true;
            gIntVal = 4;
        }
    }
    return 0;
}
int fs_closedir(struct fs_dir_t *dirp) {
    if (gIntVal == -7) {
        return 0;
    }
    return gIntVal;
}
int fs_mount(struct fs_mount_t *mountp) {
    return gIntVal;
}
int fs_unmount(struct fs_mount_t *mountp) {
    return gIntVal;
}
int fs_unlink(const char *name) {
    return 0;
}
int fs_rename(const char *from, const char *to) {
    return 0;
}
int fs_mkdir(const char *name) {
    return 0;
}
int fs_stat(const char *path, struct fs_dirent *entry) {
    return gIntVal;
}
int fs_statvfs(const char *path,struct fs_statvfs *stat) {
    stat->f_bfree = 0;
    stat->f_frsize = 0;
    if (gIntVal) {
        stat->f_bfree = 7;
        stat->f_frsize = 700;
    }
    return gIntVal;
}

void fs_file_t_init(struct fs_file_t *zfp) { }
void fs_dir_t_init(struct fs_dir_t *zdp) { }

int lfs_dir_rewind(lfs_t *lfs, lfs_dir_t *dir) {
    return 0;
}

lfs_soff_t lfs_file_size(lfs_t *lfs, lfs_file_t *file) {
    return gIntVal;
}

int lfs_dir_seek(lfs_t *lfs, lfs_dir_t *dir, lfs_off_t off) {
    return 0;
}
lfs_soff_t lfs_dir_tell(lfs_t *lfs, lfs_dir_t *dir) {
    return 0;
}


int flash_area_open(uint8_t id, const struct flash_area **fa) {
    *fa = NULL;
    return 0;
}

void flash_area_close(const struct flash_area *fa) { 
    return;
}

int flash_area_erase(const struct flash_area *fa, uint32_t off, uint32_t len) {
    return 0;
}

void ResetCtrl::performSwReset() {}

