/********************************************************************************
 *   File Name:
 *       blockDevice.hpp
 *
 *   Description:
 *       Models a generic memory device that uses block memory. Examples of this
 *       would be nearly every non-volatile form of memory storage on the
 *market.
 *
 *   2019 | Brandon Braun | brandonbraun653@gmail.com
 ********************************************************************************/

#pragma once
#ifndef CHIMERA_MOD_MEMORY_BLOCK_DEVICE_HPP
#define CHIMERA_MOD_MEMORY_BLOCK_DEVICE_HPP

/* C++ Includes */
#include <cstdint>
#include <memory>

namespace Chimera
{
  namespace Modules
  {
    namespace Memory
    {
      /**
       *   Status codes that can be returned from the block device object.
       *   Their values are dual-maintained with the definitions found in
       *   rederrno.h from the Reliance Edge FS.
       */
      enum class BlockStatus : uint8_t
      {
        BLOCK_DEV_OK           = 0,  /**< Everything is ok! */
        BLOCK_DEV_EPERM        = 1,  /**< Operation not permitted. */
        BLOCK_DEV_ENOENT       = 2,  /**< No such file or directory. */
        BLOCK_DEV_EIO          = 5,  /**< I/O error. */
        BLOCK_DEV_EBADF        = 9,  /**< Bad file number. */
        BLOCK_DEV_ENOMEM       = 12, /**< Out of memory */
        BLOCK_DEV_EBUSY        = 16, /**< Device or resource busy. */
        BLOCK_DEV_EEXIST       = 17, /**< File exists. */
        BLOCK_DEV_EXDEV        = 18, /**< Cross-device link. */
        BLOCK_DEV_ENOTDIR      = 20, /**< Not a directory. */
        BLOCK_DEV_EISDIR       = 21, /**< Is a directory. */
        BLOCK_DEV_EINVAL       = 22, /**< Invalid argument. */
        BLOCK_DEV_ENFILE       = 23, /**< File table overflow. */
        BLOCK_DEV_EMFILE       = 24, /**< Too many open files. */
        BLOCK_DEV_EFBIG        = 27, /**< File too large. */
        BLOCK_DEV_ENOSPC       = 28, /**< No space left on device. */
        BLOCK_DEV_EROFS        = 30, /**< Read-only file system. */
        BLOCK_DEV_EMLINK       = 31, /**< Too many links. */
        BLOCK_DEV_ERANGE       = 34, /**< Math result not representable. */
        BLOCK_DEV_ENAMETOOLONG = 36, /**< File name too long. */
        BLOCK_DEV_ENOSYS       = 38, /**< Function not implemented. */
        BLOCK_DEV_ENOTEMPTY    = 39, /**< Directory not empty. */
        BLOCK_DEV_ENODATA      = 61, /**< No data available. */
        BLOCK_DEV_EUSERS       = 87, /**< Too many users. */

        /* Custom Error Codes */
        BLOCK_DEV_ENOINIT   = 200, /**< Failed initialization */
        BLOCK_DEV_ENODEINIT = 201, /**< Failed deinitialization */
        BLOCK_DEV_LOCKED    = 202, /**< Device is locked from accessing resources */
      };

      /**
       *   Defines the open/access modes for the block device. Their definitions are
       *   dual-maintained with BDEVOPENMODE found in redosserv.h from the Reliance
       * Edge FS.
       */
      enum class BlockMode : uint8_t
      {
        OPEN_RDONLY,
        OPEN_WRONLY,
        OPEN_RDWR
      };

      class BlockDevice
      {
      public:
        /**
         *   Initializes a disk
         *
         *   @param[in]  volNum          The volume number of the volume who is being
         * opened
         *   @return BlockStatus
         */
        virtual BlockStatus DiskOpen( const uint8_t volNum, BlockMode openMode ) = 0;

        /**
         *   Uninitializes a disk
         *
         *   @param[in]  volNum          The volume number of the volume who is being
         * closed
         *   @return BlockStatus
         */
        virtual BlockStatus DiskClose( const uint8_t volNum ) = 0;

        /**
         *   Read sectors from a disk
         *
         *   @param[in]  volNum          The volume number of the volume whose block
         * device is being read from
         *   @param[in]  sectorStart     The starting sector number
         *   @param[in]  sectorCount     The number of sectors to read
         *   @param[in]  readBuffer      The buffer into which to read the sector data
         *   @return BlockStatus
         */
        virtual BlockStatus DiskRead( const uint8_t volNum, const uint64_t sectorStart, const uint32_t sectorCount,
                                      void *const readBuffer ) = 0;

        /**
         *   Write sectors to a disk
         *
         *   @param[in]  volNum          The volume number of the volume whose block
         * device is being written to
         *   @param[in]  sectorStart     The starting sector number
         *   @param[in]  sectorCount     The number of sectors to write
         *   @param[in]  writeBuffer     The buffer from which to write the sector
         * data
         *   @return BlockStatus
         */
        virtual BlockStatus DiskWrite( const uint8_t volNum, const uint64_t sectorStart, const uint32_t sectorCount,
                                       const void *const writeBuffer ) = 0;

        /**
         *   Flush any caches beneath the file system
         *
         *   @param[in]  volNum          The volume number of the volume whose block
         * device is being flushed
         *   @return BlockStatus
         */
        virtual BlockStatus DiskFlush( const uint8_t volNum ) = 0;

      protected:
      private:
      };

      typedef std::shared_ptr<BlockDevice> BlockDevice_sPtr;
      typedef std::unique_ptr<BlockDevice> BlockDevice_uPtr;
    }  // namespace Memory
  }    // namespace Modules
}  // namespace Chimera

#endif /* !CHIMERA_MOD_MEMORY_BLOCK_DEVICE_HPP */
