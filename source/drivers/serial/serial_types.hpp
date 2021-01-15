/********************************************************************************
 *  File Name:
 *    serial_types.hpp
 *
 *  Description:
 *    Chimera Serial types
 *
 *  2019-2020 | Brandon Braun | brandonbraun653@gmail.com
 ********************************************************************************/

#pragma once
#ifndef CHIMERA_SERIAL_TYPES_HPP
#define CHIMERA_SERIAL_TYPES_HPP

/* STL Includes */
#include <cstdint>
#include <memory>
#include <type_traits>

/* Chimera Includes */
#include <Chimera/common>
#include <Chimera/gpio>
#include <Chimera/source/drivers/peripherals/interrupt/interrupt_types.hpp>

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
   *  Options for selecting a baud rate. These are intended
   *  to be converted by the backend driver into the appropriate
   *  register values for the internal clock generation.
   */
  enum class BaudRate : size_t
  {
    SERIAL_BAUD_110    = 100u,
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
   *  Number of bits that can be sent in one transfer cycle
   *
   *  @warning  Do not change the numbering of the enumerations.
   *            They are used to index arrays.
   */
  enum class CharWid : uint8_t
  {
    CW_8BIT = 0,
    CW_9BIT,

    NUM_OPTIONS
  };

  /**
   *  Parity options
   *
   *  @warning  Do not change the numbering of the enumerations.
   *            They are used to index arrays.
   */
  enum class Parity : uint8_t
  {
    PAR_NONE = 0,
    PAR_ODD,
    PAR_EVEN,

    NUM_OPTIONS
  };

  /**
   *  Stop bit options
   *
   *  @warning  Do not change the numbering of the enumerations.
   *            They are used to index arrays.
   */
  enum class StopBits : uint8_t
  {
    SBITS_ONE = 0,
    SBITS_ONE_POINT_FIVE,
    SBITS_TWO,

    NUM_OPTIONS
  };

  /**
   *  Flow control options
   *
   *  @warning  Do not change the numbering of the enumerations.
   *            They are used to index arrays.
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
  typedef struct
  {
    bool overrun;
    bool error;
    bool asyncReady;
  } HardwareStatus;

  struct IOPins
  {
    GPIO::PinInit rx;
    GPIO::PinInit tx;
  };

  struct Config
  {
    size_t baud;
    CharWid width;
    Parity parity;
    StopBits stopBits;
    FlowControl flow;
  };

}  // namespace Chimera::Serial

#endif /* !CHIMERA_SERIAL_TYPES_HPP */