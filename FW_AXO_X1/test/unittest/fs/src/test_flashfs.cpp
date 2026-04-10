#include <zephyr/ztest.h>
#include <vector>
#include <algorithm>

#define private public
#include "fs/flashfs.h"

#include "stubs.h"

ZTEST_SUITE(tests_fs_flashfs, NULL, NULL, NULL, NULL, NULL);

// test constructor and destructor
ZTEST(tests_fs_flashfs, test_fs_flashfs_constructor)
{
	FlashFS* pFs = new FlashFS("lfs");
	zassert_not_null(pFs, "memory alloc failed");
	delete pFs;
}

// MISSING for 100% branch coverage static instantiation does not deliver NULL
// here code line 
// static FlashFS myInstance("fs", &lfs_storage_mnt);
// delete f; in function file_open
// delete d; in function dir_open
// are not covered completly.



// test constructor and destructor
ZTEST(tests_fs_flashfs, test_fs_flashfs_constructor2)
{
	struct fs_mount_t m;

	FlashFS* pFs = new FlashFS("lfs", &m);
	zassert_not_null(pFs, "memory alloc failed");
	delete pFs;
}

// test function lfs_toerror
ZTEST(tests_fs_flashfs, test_fs_flashfs_lfs_toerror)
{
	int iRet;

	iRet = FlashFS::inst().lfs_toerror(LFS_ERR_OK);
	zassert_equal(iRet, 0, "iRet != 0");
	iRet = FlashFS::inst().lfs_toerror(LFS_ERR_IO);
	zassert_equal(iRet, -EIO, "iRet != -EIO");
	iRet = FlashFS::inst().lfs_toerror(LFS_ERR_NOENT);
	zassert_equal(iRet, -ENOENT, "iRet != -ENOENT");
	iRet = FlashFS::inst().lfs_toerror(LFS_ERR_EXIST);
	zassert_equal(iRet, -EEXIST, "iRet != -EEXIST");
	iRet = FlashFS::inst().lfs_toerror(LFS_ERR_NOTDIR);
	zassert_equal(iRet, -ENOTDIR, "iRet != -ENOTDIR");
	iRet = FlashFS::inst().lfs_toerror(LFS_ERR_ISDIR);
	zassert_equal(iRet, -EISDIR, "iRet != -EISDIR");
	iRet = FlashFS::inst().lfs_toerror(LFS_ERR_INVAL);
	zassert_equal(iRet, -EINVAL, "iRet != -EINVAL");
	iRet = FlashFS::inst().lfs_toerror(LFS_ERR_NOSPC);
	zassert_equal(iRet, -ENOSPC, "iRet != -ENOSPC");
	iRet = FlashFS::inst().lfs_toerror(LFS_ERR_NOMEM);
	zassert_equal(iRet, -ENOMEM, "iRet != -ENOMEM");
	iRet = FlashFS::inst().lfs_toerror(LFS_ERR_CORRUPT);
	zassert_equal(iRet, -EILSEQ, "iRet != -EILSEQ");
	iRet = FlashFS::inst().lfs_toerror(0xFF);
	zassert_equal(iRet, 0xFF, "iRet != 0xFF");
}

// test function erase
ZTEST(tests_fs_flashfs, test_fs_flashfs_erase)
{
	int iRet;
	struct fs_mount_t m;

	FlashFS* pFs = new FlashFS("lfs", &m);
	zassert_not_null(pFs, "memory alloc failed");
	iRet = pFs->erase(&m);
	zassert_equal(iRet, 0, "iRet != 0");

	delete pFs;
}

static struct fs_mount_t lfs_storage_mnt;

// test function eraseFs
ZTEST(tests_fs_flashfs, test_fs_flashfs_eraseFs)
{
	int iRet=0;

	FlashFS* pFs = new FlashFS("lfs", &lfs_storage_mnt);
	zassert_not_null(pFs, "memory alloc failed");
	pFs->eraseFs();
	zassert_equal(iRet, 0, "iRet != 0");

	delete pFs;
}

// test function mount
ZTEST(tests_fs_flashfs, test_fs_flashfs_mount)
{
	int iRet;
	struct fs_mount_t m;

	gIntVal = 0;
	iRet = FlashFS::inst().mount(&m);
	zassert_equal(iRet, 0, "iRet != 0");

	gIntVal = -17;
	iRet = FlashFS::inst().mount(&m);
	zassert_equal(iRet, -17, "iRet != -17");
}

// test function unmount
ZTEST(tests_fs_flashfs, test_fs_flashfs_unmount)
{
	int iRet;
	struct fs_mount_t m;

	gIntVal = 0;
	iRet = FlashFS::inst().unmount();
	zassert_equal(iRet, 0, "iRet != 0");

	gIntVal = -17;
	iRet = FlashFS::inst().unmount();
	zassert_equal(iRet, -17, "iRet != -17");
}

// test function format
ZTEST(tests_fs_flashfs, test_fs_flashfs_format)
{
	int iRet;
	struct fs_mount_t m;

	iRet = FlashFS::inst().format(&m);
	zassert_equal(iRet, 0, "iRet != 0");
}

// test function remove
ZTEST(tests_fs_flashfs, test_fs_flashfs_remove)
{
	int iRet;
	iRet = FlashFS::inst().remove("filename");
	zassert_equal(iRet, 0, "iRet != 0");
}

// test function rename
ZTEST(tests_fs_flashfs, test_fs_flashfs_rename)
{
	int iRet;
	iRet = FlashFS::inst().rename("oldname", "newname");
	zassert_equal(iRet, 0, "iRet != 0");
}

// test function mkdir
ZTEST(tests_fs_flashfs, test_fs_flashfs_mkdir)
{
	int iRet;
	iRet = FlashFS::inst().mkdir("dirname", 0);
	zassert_equal(iRet, 0, "iRet != 0");
}

// test function stat
ZTEST(tests_fs_flashfs, test_fs_flashfs_stat)
{
	int iRet;
	struct fs_dirent d;

	gIntVal = 0;
	iRet = FlashFS::inst().stat("dirname", &d);
	zassert_equal(iRet, 0, "iRet != 0");
}

// test function statvfs
ZTEST(tests_fs_flashfs, test_fs_flashfs_statvfs)
{
	int iRet;
	struct fs_statvfs st;

    gIntVal = 0;
	iRet = FlashFS::inst().statvfs("dirname", &st);
	zassert_equal(iRet, 0, "iRet != 0");

    gIntVal = 7;
	iRet = FlashFS::inst().statvfs("dirname", &st);
	zassert_equal(iRet, 7, "iRet != 7");
}

// test function file_open
ZTEST(tests_fs_flashfs, test_fs_flashfs_file_open)
{
	fs_file_t* pf;
	struct fs_dirent d;

    gIntVal = 0;
	pf = FlashFS::inst().file_open("filename", 0);
	zassert_not_null(pf, "memory alloc failed");

    gIntVal = 7;
	pf = FlashFS::inst().file_open("filename", 0);
	zassert_is_null(pf, "pf != NULL");
}

// test function file_close
ZTEST(tests_fs_flashfs, test_fs_flashfs_file_close)
{
	int iRet;
	fs_file_t* f = new fs_file_t;

	iRet = FlashFS::inst().file_close(f);
	zassert_equal(iRet, 0, "iRet != 0");
	
	f = NULL;
	iRet = FlashFS::inst().file_close(f);
	zassert_equal(iRet, 0, "iRet != 0");
}

// test function file_read
ZTEST(tests_fs_flashfs, test_fs_flashfs_file_read)
{
	ssize_t iRet;
	uint8_t buf[5];
	fs_file_t f;

	iRet = FlashFS::inst().file_read(&f, buf, 5);
	zassert_equal(iRet, 5, "iRet != 5");
	zassert_equal(buf[0], 1, "buf wrong");
	zassert_equal(buf[1], 2, "buf wrong");
	zassert_equal(buf[2], 1, "buf wrong");
	zassert_equal(buf[3], 3, "buf wrong");
	zassert_equal(buf[4], 1, "buf wrong");
}

// test function file_write
ZTEST(tests_fs_flashfs, test_fs_flashfs_file_write)
{
	ssize_t iRet;
	uint8_t buf[3] = {7,17,27};
	fs_file_t f;

	iRet = FlashFS::inst().file_write(&f, buf, 3);
	zassert_equal(iRet, 3, "iRet != 3");
	zassert_equal(gBuf[0], 7, "buf wrong");
	zassert_equal(gBuf[1], 17, "buf wrong");
	zassert_equal(gBuf[2], 27, "buf wrong");
}

// test function file_sync
ZTEST(tests_fs_flashfs, test_fs_flashfs_file_sync)
{
	ssize_t iRet;
	fs_file_t f;

	iRet = FlashFS::inst().file_sync(&f);
	zassert_equal(iRet, 0, "iRet != 0");
}

// test function file_seek
ZTEST(tests_fs_flashfs, test_fs_flashfs_file_seek)
{
	ssize_t iRet;
	fs_file_t f;

	iRet = FlashFS::inst().file_seek(&f, 10, 0);
	zassert_equal(iRet, 0, "iRet != 0");
}

// test function file_tell
ZTEST(tests_fs_flashfs, test_fs_flashfs_file_tell)
{
	ssize_t iRet;
	fs_file_t f;

	iRet = FlashFS::inst().file_tell(&f);
	zassert_equal(iRet, 0, "iRet != 0");
}

// test function file_size
ZTEST(tests_fs_flashfs, test_fs_flashfs_file_size)
{
	ssize_t iRet;
	fs_file_t f;

	gIntVal = 0;
	iRet = FlashFS::inst().file_size(&f);
	zassert_equal(iRet, 0, "iRet != 0");

	gIntVal = -7;
	iRet = FlashFS::inst().file_size(&f);
	zassert_equal(iRet, -7, "iRet != -7");
}

// test function file_truncate
ZTEST(tests_fs_flashfs, test_fs_flashfs_file_truncate)
{
	ssize_t iRet;
	fs_file_t f;

	iRet = FlashFS::inst().file_truncate(&f, 5);
	zassert_equal(iRet, 0, "iRet != 0");
}

// test function file_exists
ZTEST(tests_fs_flashfs, test_fs_flashfs_file_exists)
{
	bool bRet;

	gIntVal = 0;
	bRet = FlashFS::inst().file_exists("path");
	zassert_true(bRet, "bRet != true");

	gIntVal = -7;
	bRet = FlashFS::inst().file_exists("path");
	zassert_false(bRet, "bRet != false");
}

// test function dir_open
ZTEST(tests_fs_flashfs, test_fs_flashfs_dir_open)
{
	fs_dir_t* pf;

	gIntVal = 0;
	pf = FlashFS::inst().dir_open("path");
	zassert_not_null(pf, "memory alloc failed");

	gIntVal = -7;
	pf = FlashFS::inst().dir_open("path");
	zassert_is_null(pf, "pf != NULL");
}

// test function dir_close
ZTEST(tests_fs_flashfs, test_fs_flashfs_dir_close)
{
	int iRet;
	fs_dir_t* d = new fs_dir_t;

	iRet = FlashFS::inst().dir_close(d);
	zassert_equal(iRet, 0, "iRet != 0");

	d = NULL;
	iRet = FlashFS::inst().dir_close(d);
	zassert_equal(iRet, 0, "iRet != 0");
}

// test function dir_read
ZTEST(tests_fs_flashfs, test_fs_flashfs_dir_read)
{
	int iRet;
	fs_dir_t d;
	struct fs_dirent e;

	iRet = FlashFS::inst().dir_read(&d, &e);
	//printk("XXXXX iRet = %d\n\n", iRet);
	zassert_equal(iRet, -7, "iRet != -7");
}

// test function dir_seek
ZTEST(tests_fs_flashfs, test_fs_flashfs_dir_seek)
{
	fs_dir_t d;

	FlashFS::inst().dir_seek(&d, 6);
}

// test function dir_tell
ZTEST(tests_fs_flashfs, test_fs_flashfs_dir_tell)
{
	int iRet;
	fs_dir_t d;

	iRet = FlashFS::inst().dir_tell(&d);
	zassert_equal(iRet, 0, "iRet != 0");
}

// test function dir_rewind
ZTEST(tests_fs_flashfs, test_fs_flashfs_dir_rewind)
{
	int iRet;
	fs_dir_t d;

	iRet = FlashFS::inst().dir_rewind(&d);
	zassert_equal(iRet, 0, "iRet != 0");
}

// test function dir_list
ZTEST(tests_fs_flashfs, test_fs_flashfs_dir_list)
{
	int iRet;

	gbRetVal = true;

	gIntVal = 0;
	iRet = FlashFS::inst().dir_list("path");
	zassert_equal(iRet, 0, "iRet != 0");

	gIntVal = 7;
	iRet = FlashFS::inst().dir_list("path");
	zassert_equal(iRet, 7, "iRet != 7");

	gIntVal = 6;
	iRet = FlashFS::inst().dir_list("path");
	zassert_equal(iRet, 6, "iRet != 6");

	gIntVal = -7;
	iRet = FlashFS::inst().dir_list("path");
	zassert_equal(iRet, -7, "iRet != -7");

	gIntVal = -6;
	iRet = FlashFS::inst().dir_list("path");
	zassert_equal(iRet, -6, "iRet != -6");

	gIntVal = 1;
	iRet = FlashFS::inst().dir_list("path");
	zassert_equal(iRet, 1, "iRet != 1");

	gIntVal = 2;
	iRet = FlashFS::inst().dir_list("path");
	//printk("XXXXX iRet = %d\n\n", iRet);
	zassert_equal(iRet, 2, "iRet != 2");
}

// test function file_tree
ZTEST(tests_fs_flashfs, test_fs_flashfs_file_tree)
{
	int iRet;

	gbRetVal = true;

	gIntVal = 0;
	iRet = FlashFS::inst().file_tree("path");
	zassert_equal(iRet, 0, "iRet != 0");

	gIntVal = 7;
	iRet = FlashFS::inst().file_tree("path");
	zassert_equal(iRet, 7, "iRet != 7");

	gIntVal = 6;
	iRet = FlashFS::inst().file_tree("path");
	zassert_equal(iRet, 6, "iRet != 6");

	gIntVal = -7;
	iRet = FlashFS::inst().file_tree("path");
	zassert_equal(iRet, -7, "iRet != -7");

	gIntVal = -6;
	iRet = FlashFS::inst().file_tree("path");
	zassert_equal(iRet, -6, "iRet != -6");

	gIntVal = 1;
	iRet = FlashFS::inst().file_tree("path");
	zassert_equal(iRet, 1, "iRet != 1");

	gIntVal = 2;
	iRet = FlashFS::inst().file_tree("path");
	//printk("XXXXX iRet = %d\n\n", iRet);
	zassert_equal(iRet, 0, "iRet != 0");

	gIntVal = 3;
	iRet = FlashFS::inst().file_tree("path");
	//printk("XXXXX iRet = %d\n\n", iRet);
	zassert_equal(iRet, 0, "iRet != 0");

	gIntVal = 5;
	iRet = FlashFS::inst().file_tree("path");
	//printk("XXXXX iRet = %d\n\n", iRet);
	zassert_equal(iRet, 5, "iRet != 5");

}

// test function CheckAvailableSpace
ZTEST(tests_fs_flashfs, test_fs_flashfs_CheckAvailableSpace)
{
	gIntVal = 0;
	FlashFS::inst().CheckAvailableSpace();
	gIntVal = 7;
	FlashFS::inst().CheckAvailableSpace();
}

// test function GetFsStat
ZTEST(tests_fs_flashfs, test_fs_flashfs_GetFsStat)
{
	unsigned char* pOut;

	gIntVal = 0;
	pOut = FlashFS::inst().GetFsStat("path");

	gIntVal = 7;
	pOut = FlashFS::inst().GetFsStat("path");

	gIntVal = 6;
	FlashFS::inst().GetFsStat("path");

	gIntVal = -7;
	FlashFS::inst().GetFsStat("path");

	gbRetVal = false;
	gIntVal = -17;
	FlashFS::inst().GetFsStat("path");

	gbRetVal = false;
	gIntVal = -16;
	FlashFS::inst().GetFsStat("path");

	gbRetVal = false;
	gIntVal = 11;
	FlashFS::inst().GetFsStat("path");

	gbRetVal = false;
	gIntVal = 12;
	FlashFS::inst().GetFsStat("path");

	gbRetVal = false;
	gIntVal = 13;
	FlashFS::inst().GetFsStat("path");

	gbRetVal = false;
	gIntVal = 14;
	FlashFS::inst().GetFsStat("path");
}

// test function shell_file_tree
ZTEST(tests_fs_flashfs, test_fs_flashfs_shell_file_tree)
{
	int iRet;

	iRet = FlashFS::inst().shell_file_tree(NULL, 0, NULL);
	zassert_equal(iRet, 0, "iRet != 0");
}

// test function shell_fs_stat
ZTEST(tests_fs_flashfs, test_fs_flashfs_shell_fs_stat)
{
	int iRet;

	iRet = FlashFS::inst().shell_fs_stat(NULL, 0, NULL);
	zassert_equal(iRet, 0, "iRet != 0");
}

// test function clear_fs
ZTEST(tests_fs_flashfs, test_fs_flashfs_clear_fs)
{
	int iRet=0;

	FlashFS* pFs = new FlashFS("lfs", &lfs_storage_mnt);
	zassert_not_null(pFs, "memory alloc failed");
	pFs->clear_fs(NULL, 0, NULL);
	printk("clear_fs passed\n");
	zassert_equal(iRet, 0, "iRet != 0");

	delete pFs;
}



