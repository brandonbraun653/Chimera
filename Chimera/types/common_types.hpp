/********************************************************************************
 *   File Name:
 *    common_types.hpp
 *
 *   Description:
 *    Common Chimera system types
 *
 *   2019 | Brandon Braun | brandonbraun653@gmail.com
 ********************************************************************************/

#pragma once
#ifndef CHIMERA_COMMON_TYPES_HPP
#define CHIMERA_COMMON_TYPES_HPP

/* C++ Includes */
#include <cstdint>

namespace Chimera
{
  using Status_t = uint16_t;
  namespace Status
  {
    namespace Internal
    {
      static constexpr Status_t gpioOffset     = 100;
      static constexpr Status_t crcOffset      = 200;
      static constexpr Status_t dmaOffset      = 300;
      static constexpr Status_t serialOffset   = 400;
      static constexpr Status_t spiOffset      = 500;
      static constexpr Status_t watchdogOffset = 600;
    }
  }

  class CommonStatusCodes
  {
  public:
    static constexpr Status_t UNKNOWN_ERROR    = 0;  /**< Don't know what went wrong, but need to report an error */
    static constexpr Status_t OK               = 1;  /**< Everything is just fine. No errors. */
    static constexpr Status_t NOT_INITIALIZED  = 2;  /**< The system has not been initialized yet. */
    static constexpr Status_t LOCKED           = 3;  /**< The system has been locked */
    static constexpr Status_t EXCEPTION        = 4;  /**< An exception was thrown */
    static constexpr Status_t TIMEOUT          = 5;  /**< The system timed-out on an operation */
    static constexpr Status_t NOT_SUPPORTED    = 10; /**< Some system functionality is not enabled/supported */
    static constexpr Status_t NOT_READY        = 11; /**< The system is not ready to go yet */
    static constexpr Status_t TX_IN_PROGRESS   = 12; /**< A transmission is in progress */
    static constexpr Status_t RX_IN_PROGRESS   = 13; /**< A reception is in progress */
    static constexpr Status_t BUSY             = 14; /**< The system can't be bothered to respond right now. It's busy. */
    static constexpr Status_t INVAL_FUNC_PARAM = 15; /**< A parameter passed into a function was invalid */
    static constexpr Status_t EMPTY            = 16; /**< The result of the operation had no data available */
    static constexpr Status_t FULL             = 17; /**< The result of the operation had no data left */
    static constexpr Status_t MEMORY           = 18; /**< There was a generic issue with memory of some kind */

    static constexpr Status_t FAIL           = 30; /**< Generic error to signify a non-explicit failure type */
    static constexpr Status_t FAILED_INIT    = 31; /**< Somehow failed an initialization sequence */
    static constexpr Status_t FAILED_LOCK    = 32; /**< Could not lock a resource */
    static constexpr Status_t FAILED_RELEASE = 33; /**< Could not release a locked resource */
    static constexpr Status_t FAILED_WRITE   = 34; /**< Failed some kind of write operation */
    static constexpr Status_t FAILED_READ    = 35; /**< Failed some kind of read operation */
    static constexpr Status_t FAILED_ERASE   = 36; /**< Failed some kind of erase operation */

    /*------------------------------------------------
    Offsets to identify particular Chimera modules
    ------------------------------------------------*/
    static constexpr Status_t status_offset_module_memory_flash = 1000;
  };

  namespace Function
  {
    typedef void ( *void_func_void )( void );
    typedef void ( *void_func_void_ptr )( void * );
    typedef void ( *void_func_uint32_t )( uint32_t );
  }  // namespace Function

  namespace Hardware
  {
    enum class SubPeripheral : uint8_t
    {
      RX = 0,
      TX,
      TXRX,

      NUM_SUBPERIPH,
      UNKNOWN_SUBPERIPH
    };

    enum class SubPeripheralMode : uint8_t
    {
      BLOCKING = 0,
      INTERRUPT,
      DMA,

      NUM_SUBPERIPH_MODES,
      UNKNOWN_MODE
    };
    
    enum class AccessMode : uint8_t
    {
      BARE_METAL,
      THREADED,
      ATOMIC,
      
      NUM_ACCESS_MODES
    };
  }
}  // namespace Chimera

#endif /* !CHIMERA_COMMON_TYPES_HPP */