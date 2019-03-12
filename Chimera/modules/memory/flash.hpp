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
        static constexpr Status_t OUT_OF_MEMORY = status_offset_module_memory_flash + 1;
        static constexpr Status_t OVERRUN       = status_offset_module_memory_flash + 2;
        static constexpr Status_t UNALIGNED     = status_offset_module_memory_flash + 3;
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
      };

    }  // namespace Memory
  }    // namespace Module
}  // namespace Chimera

#endif /* !CHIMERA_MOD_MEMORY_FLASH_HPP */