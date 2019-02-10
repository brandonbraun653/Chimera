/********************************************************************************
*   File Name:
*       red_driver.hpp
*
*   Description:
*       Provides a high level C++ API wrapper around the Reliance-Edge FS
*
*   2019 | Brandon Braun | brandonbraun653@gmail.com
********************************************************************************/

#pragma once
#ifndef CHIMERA_RED_DRIVER_HPP
#define CHIMERA_RED_DRIVER_HPP

/* Reliance Edge FS Includes */
#ifdef __cplusplus
extern "C" {
#include "redfs.h"
#include "redstat.h"
#include "redposix.h"
#include "redvolume.h"
}
#endif

/* C++ Includes */
#include <cstdint>
#include <memory>
#include <type_traits>
#include <string>

/* Chimera Includes */
#include <Chimera/modules/memory/blockDevice.hpp>

namespace Red
{
    static constexpr uint8_t MAX_MEMORY_DEVICES = 5;

    /**
    *   RedFSPosix is a thinly veiled wrapper around the Reliance Edge Posix C file system driver. The purpose
    *   is to allow the system to be reused without the backend function calls being tied to a specific
    *   hardware driver. This transforms the interaction model into more of an object oriented interface.
    *   Under the hood, the class is doing varying amounts of redirection to figure out which device is
    *   actually using the FS.
    *
    *   @note   All documentation (with minor modification) is courtesy of Reliance Edge.
    *
    *
    */
    class RedFSPosix
    {
    public:
        /**
        *   Initialize the Reliance Edge file system driver.
        *
        *   Prepares the Reliance Edge file system driver to be used.  Must be the first
        *   Reliance Edge function to be invoked: no volumes can be mounted or formatted
        *   until the driver has been initialized.
        *
        *   If this function is called when the Reliance Edge driver is already
        *   initialized, it does nothing and returns success.
        *
        *   This function is not thread safe: attempting to initialize from multiple
        *   threads could leave things in a bad state.
        *
        *   @note   Returning -1 could also be due to an internal RedFS error.
        *           Check bdevErr for more info.
        *
        *   @param[in]  device      An object that implements the BlockDevice class. This is the
        *                           actual hardware device where data will be stored.
        *   @param[in]  volumeName  The volume string associated with the device. (Available volumes listed in redconf.c)
        *   @return On success, zero is returned. On error, -1 is returned and red_errno is set appropriately.
        */
        REDSTATUS init(Chimera::Modules::Memory::BlockDevice_sPtr &device, std::string volumeName);

        /**
        *   Uninitialize the Reliance Edge file system driver.
        *
        *   Tears down the Reliance Edge file system driver.  Cannot be used until all
        *   Reliance Edge volumes are unmounted.  A subsequent call to init() will
        *   initialize the driver again.
        *
        *   If this function is called when the Reliance Edge driver is already
        *   uninitialized, it does nothing and returns success.
        *
        *   This function is not thread safe: attempting to uninitialize from multiple
        *   threads could leave things in a bad state.
        *
        *   @return On success, zero is returned. On error, -1 is returned and red_errno is set appropriately.
        */
        static REDSTATUS deInitFileSystem();

        /**
        *   Unregisters a block device with the Reliance Edge file system driver.
        *
        *   Remove's the object that was registered in init(), but does not tear down the actual
        *   file system driver.
        */
        bool deInitBlockDevice();


        /**
        *   Mount the file system volume.
        *
        *   Prepares the file system volume to be accessed.  Mount will fail if the
        *   volume has never been formatted, or if the on-disk format is inconsistent
        *   with the compile-time configuration.
        *
        *   An error is returned if the volume is already mounted.
        *
        *   @return On success, zero is returned. On error, -1 is returned and red_errno is set appropriately.
        */
        REDSTATUS mount();

        /**
        *   Unmount the file system volume.
        *
        *   This function discards the in-memory state for the file system and marks it
        *   as unmounted.  Subsequent attempts to access the volume will fail until the
        *   volume is mounted again.
        *
        *   If unmount automatic transaction points are enabled, this function will
        *   commit a transaction point prior to unmounting.  If unmount automatic
        *   transaction points are disabled, this function will unmount without
        *   transacting, effectively discarding the working state.
        *
        *   Before unmounting, this function will wait for any active file system
        *   thread to complete by acquiring the FS mutex.  The volume will be marked as
        *   unmounted before the FS mutex is released, so subsequent FS threads will
        *   possibly block and then see an error when attempting to access a volume
        *   which is unmounting or unmounted.  If the volume has open handles, the
        *   unmount will fail.
        *
        *   An error is returned if the volume is already unmounted.
        *
        *   @return On success, zero is returned. On error, -1 is returned and red_errno is set appropriately.
        */
        REDSTATUS unmount();


        #if (REDCONF_READ_ONLY == 0) && (REDCONF_API_POSIX_FORMAT == 1)
        int32_t red_format(const char *pszVolume);
        #endif


        #if REDCONF_READ_ONLY == 0
        int32_t red_transact(const char *pszVolume);
        #endif


        #if REDCONF_READ_ONLY == 0
        int32_t red_settransmask(const char *pszVolume, uint32_t ulEventMask);
        #endif


        int32_t red_gettransmask(const char *pszVolume, uint32_t *pulEventMask);


        int32_t red_statvfs(const char *pszVolume, REDSTATFS *pStatvfs);


        #if REDCONF_READ_ONLY == 0
        int32_t red_sync(void);
        #endif


        int32_t red_open(const char *pszPath, uint32_t ulOpenMode);


        #if (REDCONF_READ_ONLY == 0) && (REDCONF_API_POSIX_UNLINK == 1)
        int32_t red_unlink(const char *pszPath);
        #endif


        #if (REDCONF_READ_ONLY == 0) && (REDCONF_API_POSIX_MKDIR == 1)
        int32_t red_mkdir(const char *pszPath);
        #endif


        #if (REDCONF_READ_ONLY == 0) && (REDCONF_API_POSIX_RMDIR == 1)
        int32_t red_rmdir(const char *pszPath);
        #endif


        #if (REDCONF_READ_ONLY == 0) && (REDCONF_API_POSIX_RENAME == 1)
        int32_t red_rename(const char *pszOldPath, const char *pszNewPath);
        #endif


        #if (REDCONF_READ_ONLY == 0) && (REDCONF_API_POSIX_LINK == 1)
        int32_t red_link(const char *pszPath, const char *pszHardLink);
        #endif


        int32_t red_close(int32_t iFildes);


        int32_t red_read(int32_t iFildes, void *pBuffer, uint32_t ulLength);


        #if REDCONF_READ_ONLY == 0
        int32_t red_write(int32_t iFildes, const void *pBuffer, uint32_t ulLength);
        #endif


        #if REDCONF_READ_ONLY == 0
        int32_t red_fsync(int32_t iFildes);
        #endif


        int64_t red_lseek(int32_t iFildes, int64_t llOffset, REDWHENCE whence);


        #if (REDCONF_READ_ONLY == 0) && (REDCONF_API_POSIX_FTRUNCATE == 1)
        int32_t red_ftruncate(int32_t iFildes, uint64_t ullSize);
        #endif


        int32_t red_fstat(int32_t iFildes, REDSTAT *pStat);


        #if REDCONF_API_POSIX_READDIR == 1

        REDDIR *red_opendir(const char *pszPath);


        REDDIRENT *red_readdir(REDDIR *pDirStream);


        void red_rewinddir(REDDIR *pDirStream);


        int32_t red_closedir(REDDIR *pDirStream);
        #endif

        #if REDCONF_API_POSIX_CWD == 1

        int32_t red_chdir(const char *pszPath);


        char *red_getcwd(char *pszBuffer, uint32_t ulBufferSize);
        #endif


        REDSTATUS *red_errnoptr(void);


        Chimera::Modules::Memory::BlockStatus blockDeviceErr;


      private:
        std::string registeredVolume;

        uint8_t registeredDeviceIndex = 0;
        uint32_t registeredDeviceAddress = 0;
    };
}

#endif /* !CHIMERA_RED_DRIVER_HPP */
