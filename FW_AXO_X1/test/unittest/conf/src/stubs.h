#ifndef __STUBS_H__
#define __STUBS_H__

#include <stdio.h>
#include <stdlib.h>
    #include <fcntl.h>
    #include <unistd.h>

extern bool gbRetVal;
extern char gBuf[1024];

//const char* getFwVersion();

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

//void* k_timer_user_data_get(k_timer* timer);


#endif // __STUBS_H__