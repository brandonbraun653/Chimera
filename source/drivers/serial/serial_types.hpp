/********************************************************************************
 *  File Name:
 *    serial_types.hpp
 *
 *  Description:
 *    Chimera Serial types
 *
 *  2019-2022 | Brandon Braun | brandonbraun653@gmail.com
 ********************************************************************************/

#pragma once
#ifndef CHIMERA_SERIAL_TYPES_HPP
#define CHIMERA_SERIAL_TYPES_HPP

/*-----------------------------------------------------------------------------
Includes
-----------------------------------------------------------------------------*/
#include <Chimera/common>
#include <Chimera/gpio>
#include <Chimera/source/drivers/peripherals/interrupt/interrupt_types.hpp>
#include <cstddef>
#include <cstdint>
#include <etl/bip_buffer_spsc_atomic.h>
#include <type_traits>

namespace Chimera::Serial
{
  /*-------------------------------------------------------------------------------
  Forward Declarations
  -------------------------------------------------------------------------------*/
  class Driver;

  /*-------------------------------------------------------------------------------
  Aliases
  -------------------------------------------------------------------------------*/
  using Driver_rPtr = Driver *;
  using BipBuffer   = etl::ibip_buffer_spsc_atomic<uint8_t>;
  using TxfrMode    = Hardware::PeripheralMode;

  /*-------------------------------------------------------------------------------
  Constants
  -------------------------------------------------------------------------------*/
  class Status
  {
  public:
    static constexpr Status_t codeOffset = Chimera::Internal::serialErrorCodeStart;

    static constexpr Status_t RX_ABORTED                  = codeOffset + 2;
    static constexpr Status_t RX_READY                    = codeOffset + 3;
    static constexpr Status_t RX_IN_PROGRESS              = codeOffset + 4;
    static constexpr Status_t RX_COMPLETE                 = codeOffset + 5;
    static constexpr Status_t NOT_READY                   = codeOffset + 6;
    static constexpr Status_t PACKET_TOO_LARGE_FOR_BUFFER = codeOffset + 7;
    static constexpr Status_t INVALID_BUFFER              = codeOffset + 8;
    static constexpr Status_t FAILED_WRITE                = codeOffset + 11;
    static constexpr Status_t FAILED_READ                 = codeOffset + 12;
    static constexpr Status_t FAILED_OPEN                 = codeOffset + 13;
    static constexpr Status_t FAILED_CLOSE                = codeOffset + 14;
    static constexpr Status_t FAILED_CONFIGURE            = codeOffset + 15;
    static constexpr Status_t TX_READY                    = codeOffset + 16;
    static constexpr Status_t TX_IN_PROGRESS              = codeOffset + 17;
    static constexpr Status_t TX_COMPLETE                 = codeOffset + 18;
    static constexpr Status_t TX_ABORTED                  = codeOffset + 19;
  };

  /*-------------------------------------------------------------------------------
  Enumerations
  -------------------------------------------------------------------------------*/
  /**
   * @brief Options for selecting a baud rate.
   *
   * These are intended to be converted by the backend driver into the appropriate
   * register values for the internal clock generation.
   */
  enum class BaudRate : size_t
  {
    SERIAL_BAUD_110    = 110u,
    SERIAL_BAUD_150    = 150u,
    SERIAL_BAUD_300    = 300u,
    SERIAL_BAUD_1200   = 1200u,
    SERIAL_BAUD_2400   = 2400u,
    SERIAL_BAUD_4800   = 4800u,
    SERIAL_BAUD_9600   = 9600u,
    SERIAL_BAUD_19200  = 19200u,
    SERIAL_BAUD_38400  = 38400u,
    SERIAL_BAUD_57600  = 57600u,
    SERIAL_BAUD_115200 = 115200u,
    SERIAL_BAUD_230400 = 230400u,
    SERIAL_BAUD_460800 = 460800u,
    SERIAL_BAUD_921600 = 921600u
  };

  /**
   * @brief Number of bits that can be sent in one transfer cycle
   */
  enum class CharWid : uint8_t
  {
    CW_8BIT = 0,
    CW_9BIT,

    NUM_OPTIONS
  };

  /**
   * @brief Parity options
   */
  enum class Parity : uint8_t
  {
    PAR_NONE = 0,
    PAR_ODD,
    PAR_EVEN,

    NUM_OPTIONS
  };

  /**
   * @brief Stop bit options
   */
  enum class StopBits : uint8_t
  {
    SBITS_ONE = 0,
    SBITS_ONE_POINT_FIVE,
    SBITS_TWO,

    NUM_OPTIONS
  };

  /**
   * @brief Flow control options
   */
  enum class FlowControl : uint8_t
  {
    FCTRL_NONE = 0,
    FCTRL_SW,
    FCTRL_HW,

    NUM_OPTIONS
  };

  enum class Channel : uint8_t
  {
    SERIAL1,
    SERIAL2,
    SERIAL3,
    SERIAL4,
    SERIAL5,
    SERIAL6,
    SERIAL7,
    SERIAL8,

    NUM_OPTIONS,
    NOT_SUPPORTED
  };

  enum ISREvent : Interrupt::Signal_t
  {
    SIG_TX_COMPLETE,
    SIG_RX_COMPLETE,
    SIG_ERROR,

    SIG_NUM_OPTIONS
  };

  /*-------------------------------------------------------------------------------
  Structures
  -------------------------------------------------------------------------------*/
  struct Config
  {
    Channel     channel;  /**< Serial channel to use */
    size_t      baud;     /**< Desired baud rate (bps) */
    CharWid     width;    /**< Transfer width per-word */
    Parity      parity;   /**< Parity to use */
    StopBits    stopBits; /**< Number of stop bits */
    FlowControl flow;     /**< Any flow control? */
    TxfrMode    txfrMode; /**< Hardware transfer mode in TX/RX */
    BipBuffer  *rxBuffer; /**< IO ring buffer for reception */
    BipBuffer  *txBuffer; /**< IO ring buffer for transmission */
  };


  namespace Backend
  {
    struct DriverConfig
    {
      bool isSupported; /**< A simple flag to let Chimera know if the driver is supported */

      /**
       *  Function pointer that initializes the backend driver's
       *  memory. Should really only call once for initial set up.
       */
      Chimera::Status_t ( *initialize )();

      /**
       *  Resets the backend driver hardware to default configuration
       *  settings, but does not wipe out any memory.
       */
      Chimera::Status_t ( *reset )();

      /**
       *  Gets the driver instance associated with the requested channel
       */
      Driver_rPtr ( *getDriver )( const Channel channel );
    };
  }  // namespace Backend
}  // namespace Chimera::Serial

#endif /* !CHIMERA_SERIAL_TYPES_HPP */