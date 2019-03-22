/********************************************************************************
 *  File Name:
 *    flash.hpp
 *
 *  Description:
 *    Describes common interface to low level flash memory
 *
 *  2019 | Brandon Braun | brandonbraun653@gmail.com
 ********************************************************************************/

#pragma once
#ifndef CHIMERA_MOD_MEMORY_FLASH_HPP
#define CHIMERA_MOD_MEMORY_FLASH_HPP

/* Chimera Includes */
#include <Chimera/chimera.hpp>

namespace Chimera
{
  namespace Modules
  {
    namespace Memory
    {
      class Status : public Chimera::CommonStatusCodes
      {
      public:
        static constexpr Status_t OUT_OF_MEMORY = status_offset_module_memory_flash + 1; /**< Pretty self-explanatory... */
        static constexpr Status_t OVERRUN       = status_offset_module_memory_flash
                                            + 2; /**< The end of a buffer was hit pre-maturely */
        static constexpr Status_t UNALIGNED_MEM = status_offset_module_memory_flash + 3; /**< Memory was not aligned correctly */
        static constexpr Status_t UNKNOWN_JEDEC = status_offset_module_memory_flash
                                                  + 4; /**< Device reported an invalid JEDEC code */
        static constexpr Status_t HF_INIT_FAIL = status_offset_module_memory_flash
                                                 + 5; /**< High frequency interface failed to initialize */
        static constexpr Status_t NOT_PAGE_ALIGNED = status_offset_module_memory_flash + 6; /**< Memory is not page aligned */
      };

      /**
       * A helper class that generates memory ranging information useful in flash memory driver applications.
       * While named with 'Block', this technically can apply to pages, sectors, etc.
       */
      class MemoryBlockRange
      {
      public:
        /**
         *	Simple constructor to generate a new class from scratch
         *
         *	@param[in]	startAddress    Absolute address to start at
         *	@param[in]	endAddress      Absolute address greater than startAddress to end at
         *	@param[in]	blockSize       The block size in bytes
         */
        MemoryBlockRange( const uint32_t startAddress, const uint32_t endAddress, const uint32_t blockSize );

        /**
         *	Copy constructor to generate a new class from an existing one
         *
         *	@param[in]	cls             The existing class object
         */
        MemoryBlockRange( const MemoryBlockRange &cls );

        ~MemoryBlockRange() = default;

        /**
         *  Calculates the block of the startAddress
         *
         *  In the example below, this function will return the block number
         *  associated with 'a' because this is the block that the startAddress
         *  currently resides.
         *
         *  ================== Example ==================
         *  Data Range: ***   StartAddr: p1   EndAddr: p2
         *  Don't Care: ---   BlockBoundaries: a,b,c,d,e
         *
         *  a    p1    b          c          d   p2     e
         *  |-----*****|**********|**********|****------|
         *
         *	@return uint32_t
         *
         *	|             Return Value             |                             Explanation                             |
         *  |:------------------------------------:|:-------------------------------------------------------------------:|
         *  | std::numeric_limits<uint32_t>::max() | The class was not initialized with valid address range / block size |
         *  |                     All other values | The calculated data                                                 |
         */
        uint32_t startBlock();

        /**
         *  Starting from the beginning of startblock(), calculates the number
         *  of bytes until the startAddress has been reached (point 'p1').
         *
         *  In the example below, it would return the difference between
         *  the address of 'p1' and the address of 'a'. Mathematically this is:
         *
         *  startOffset = p1 - a;
         *
         *  ================== Example ==================
         *  Data Range: ***   StartAddr: p1   EndAddr: p2
         *  Don't Care: ---   blockBoundaries: a,b,c,d,e
         *
         *  a    p1    b          c          d   p2     e
         *  |-----*****|**********|**********|****------|
         *
         *	@return uint32_t
         *
         *	|             Return Value             |                             Explanation                             |
         *  |:------------------------------------:|:-------------------------------------------------------------------:|
         *  | std::numeric_limits<uint32_t>::max() | The class was not initialized with valid address range / block size |
         *  |                     All other values | The calculated data                                                 |
         */
        uint32_t startOffset();

        /**
         *  Starting from startAddress, calculates the number of bytes until the next block
         *  boundary point has been reached.
         *
         *  In the example below, it would return the difference between
         *  the address of 'b' and the address of 'p1'. Mathematically this is:
         *
         *  startBytes = b - p1;
         *
         *  ================== Example ==================
         *  Data Range: ***   StartAddr: p1   EndAddr: p2
         *  Don't Care: ---   blockBoundaries: a,b,c,d,e
         *
         *  a    p1    b          c          d   p2     e
         *  |-----*****|**********|**********|****------|
         *
         *	@return uint32_t
         *
         *	|             Return Value             |                             Explanation                             |
         *  |:------------------------------------:|:-------------------------------------------------------------------:|
         *  | std::numeric_limits<uint32_t>::max() | The class was not initialized with valid address range / block size |
         *  |                     All other values | The calculated data                                                 |
         */
        uint32_t startBytes();

        /**
         *  Calculates the block of the endAddress
         *
         *  In the example below, this function will return the block number
         *  associated with 'd' because this is the block that the endAddress
         *  currently resides.
         *
         *  ================== Example ==================
         *  Data Range: ***   StartAddr: p1   EndAddr: p2
         *  Don't Care: ---   blockBoundaries: a,b,c,d,e
         *
         *  a    p1    b          c          d   p2     e
         *  |-----*****|**********|**********|****------|
         *
         *	@return uint32_t
         *
         *	|             Return Value             |                             Explanation                             |
         *  |:------------------------------------:|:-------------------------------------------------------------------:|
         *  | std::numeric_limits<uint32_t>::max() | The class was not initialized with valid address range / block size |
         *  |                     All other values | The calculated data                                                 |
         */
        uint32_t endBlock();

        /**
         *  Starting from the beginning of endblock(), calculates the number
         *  of bytes until the endAddress has been reached (point 'p2').
         *
         *  In the example below, it would return the difference between
         *  the address of 'p2' and the address of 'd'. Mathematically this is:
         *
         *  endOffset = p2 - d;
         *
         *  ================== Example ==================
         *  Data Range: ***   StartAddr: p1   EndAddr: p2
         *  Don't Care: ---   blockBoundaries: a,b,c,d,e
         *
         *  a    p1    b          c          d   p2     e
         *  |-----*****|**********|**********|****------|
         *
         *	@return uint32_t
         *
         *	|             Return Value             |                             Explanation                             |
         *  |:------------------------------------:|:-------------------------------------------------------------------:|
         *  | std::numeric_limits<uint32_t>::max() | The class was not initialized with valid address range / block size |
         *  |                     All other values | The calculated data                                                 |
         */
        uint32_t endOffset();

        /**
         *  Starting from endAddress, calculates the number of bytes until the next block
         *  boundary point has been reached.
         *
         *  In the example below, it would return the difference between
         *  the address of 'e' and the address of 'p2'. Mathematically this is:
         *
         *  startBytes = e - p2;
         *
         *  ================== Example ==================
         *  Data Range: ***   StartAddr: p1   EndAddr: p2
         *  Don't Care: ---   blockBoundaries: a,b,c,d,e
         *
         *  a    p1    b          c          d   p2     e
         *  |-----*****|**********|**********|****------|
         *
         *	@return uint32_t
         *
         *	|             Return Value             |                             Explanation                             |
         *  |:------------------------------------:|:-------------------------------------------------------------------:|
         *  | std::numeric_limits<uint32_t>::max() | The class was not initialized with valid address range / block size |
         *  |                     All other values | The calculated data                                                 |
         */
        uint32_t endBytes();

        /**
         *	Calculates which Blocks are fully spanned by the ctor address range
         *
         *	@param[out]	start           The start Block number
         *	@param[out]	end             The end Block number
         *	@return void
         *
         *	|            Assigned Value            |                             Explanation                             |
         *  |:------------------------------------:|:-------------------------------------------------------------------:|
         *  | std::numeric_limits<uint32_t>::max() | The class was not initialized with valid address range / block size |
         *  |                     All other values | The calculated data                                                 |
         */
        void getConsecutiveBlocks( uint32_t &start, uint32_t &end );

      protected:
        static constexpr uint32_t U32MAX = std::numeric_limits<uint32_t>::max();

        bool initialized = false;

        uint32_t _blockSize = U32MAX;

        uint32_t _startBlock    = U32MAX;
        uint32_t _startAddress = U32MAX;
        uint32_t _startOffset  = U32MAX;
        uint32_t _startBytes  = U32MAX;

        uint32_t _endBlock    = U32MAX;
        uint32_t _endAddress = U32MAX;
        uint32_t _endOffset  = U32MAX;
        uint32_t _endBytes = U32MAX;
      };



      /**
       *  Models interactions with a Flash memory device from the perspective that it is
       *  one continuous block of memory. Paging, partitioning, and all other device specific
       *  information is left up to the inheriting driver. All the user cares about is that
       *  data can be written, read, and erased.
       */
      class GenericFlashInterface
      {
      public:
        /**
         *	Virtual destructor necessary for GMock as well as inheritors
         */
        virtual ~GenericFlashInterface() = default;

        /**
         *	Writes data into flash memory.
         *
         *  @note   Does not require page alignment, but be aware of the possibility that unaligned
         *          data could possibly take longer to write. This is dependent upon the specific device.
         *
         *	@param[in]	address       The start address to write data into
         *	@param[in]	data          The buffer of data that will be written
         *	@param[in]	length        Number of bytes to be written
         *	@return Chimera::Status_t
         *
         *  |  Return Value |                             Explanation                            |
         *  |:-------------:|:------------------------------------------------------------------:|
         *  |            OK | The write completed successfully                                   |
         *  |          FAIL | The write did not succeed for some reason (device specific)        |
         *  |          BUSY | Flash is doing something at the moment. Try again later.           |
         *  | OUT_OF_MEMORY | Zero or more bytes were written, but not the full amount requested |
         */
        virtual Chimera::Status_t write( const uint32_t address, const uint8_t *const data, const uint32_t length ) = 0;

        /**
         *  Reads data in a contiguous block, starting from the given address. Should *not* be able to
         *  read across the end of the device memory and wrap around to the beginning.
         *
         *	@param[in]	address       The address to start the read from
         *	@param[out]	data          Buffer of data to read into
         *	@param[in]	length        How many bytes to read out
         *	@return Chimera::Status_t
         *
         *  | Return Value |                         Explanation                         |
         *  |:------------:|:-----------------------------------------------------------:|
         *  |           OK | The read completed successfully                             |
         *  |         FAIL | The read did not succeed for some reason (device specific)  |
         *  |         BUSY | Flash is doing something at the moment. Try again later.    |
         *  |      OVERRUN | A boundary was reached and the read halted.                 |
         */
        virtual Chimera::Status_t read( const uint32_t address, uint8_t *const data, const uint32_t length ) = 0;

        /**
         *  Erase a region of memory. Due to common device architecture, it is likely that the given
         *  address range will need to be page, block, or sector aligned.
         *
         *	@param[in]	begin         The address to start erasing at
         *	@param[in]	end           The address to stop erasing at
         *	@return Chimera::Status_t
         *
         *  | Return Value |                         Explanation                         |
         *  |:------------:|:-----------------------------------------------------------:|
         *  |           OK | The erase completed successfully                            |
         *  |         FAIL | The erase did not succeed for some reason (device specific) |
         *  |         BUSY | Flash is doing something at the moment. Try again later.    |
         *  |    UNALIGNED | The range wasn't aligned with the device's erasable regions |
         */
        virtual Chimera::Status_t erase( const uint32_t begin, const uint32_t end ) = 0;

        /**
         *	Register a callback to be executed when the write has been completed. The input parameter
         *  will let the function know how many bytes were actually written.
         *
         *	@param[in]	func          Function pointer to the callback
         *	@return Chimera::Status_t
         *
         *  | Return Value |                    Explanation                   |
         *  |:------------:|:------------------------------------------------:|
         *  |           OK | The callback registration completed successfully |
         *  |         FAIL | The callback registration failed                 |
         */
        virtual Chimera::Status_t writeCompleteCallback( const Chimera::void_func_uint32_t func ) = 0;

        /**
         *	Register a callback to be executed when the read has been completed. The input parameter
         *  will let the function know how many bytes were actually read.
         *
         *	@param[in]	func          Function pointer to the callback
         *	@return Chimera::Status_t
         *
         *  | Return Value |                    Explanation                   |
         *  |:------------:|:------------------------------------------------:|
         *  |           OK | The callback registration completed successfully |
         *  |         FAIL | The callback registration failed                 |
         */
        virtual Chimera::Status_t readCompleteCallback( const Chimera::void_func_uint32_t func ) = 0;

        /**
         *	Register a callback to be executed when the erase has been completed. The input parameter
         *  will let the function know how many bytes were actually erased.
         *
         *	@param[in]	func          Function pointer to the callback
         *	@return Chimera::Status_t
         *
         *  | Return Value |                    Explanation                   |
         *  |:------------:|:------------------------------------------------:|
         *  |           OK | The callback registration completed successfully |
         *  |         FAIL | The callback registration failed                 |
         */
        virtual Chimera::Status_t eraseCompleteCallback( const Chimera::void_func_uint32_t func ) = 0;

        /**
         *	Checks if the device has been initialized properly and is ok to talk with
         *
         *	@return bool
         *
         *  | Return Value |           Explanation           |
         *  |:------------:|:-------------------------------:|
         *  |         true | The device has been initialized |
         *  |        false | The device is not initialized   |
         */
         virtual bool isInitialized() = 0;
      };

    }  // namespace Memory
  }    // namespace Module
}  // namespace Chimera

#endif /* !CHIMERA_MOD_MEMORY_FLASH_HPP */
