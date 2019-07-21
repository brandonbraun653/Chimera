/********************************************************************************
 *   File Name:
 *    serial_types.hpp
 *
 *   Description:
 *    Chimera Serial types
 *
 *   2019 | Brandon Braun | brandonbraun653@gmail.com
 ********************************************************************************/

#pragma once
#ifndef CHIMERA_SERIAL_TYPES_HPP
#define CHIMERA_SERIAL_TYPES_HPP

/* C++ Includes */
#include <cstdint>
#include <memory>

/* Chimera Includes */
#include <Chimera/types/common_types.hpp>
#include <Chimera/types/gpio_types.hpp>

namespace Chimera::Serial
{
  class Interface;
  using Interface_sPtr = std::shared_ptr<Interface>;
  using Interface_uPtr = std::unique_ptr<Interface>;

  class SerialClass;
  using SerialClass_sPtr = std::shared_ptr<SerialClass>;
  using SerialClass_uPtr = std::unique_ptr<SerialClass>;

  class Status : public CommonStatusCodes
  {
  public:
    static constexpr Status_t codeOffset = Chimera::Status::Internal::serialOffset;

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

  enum class CharWid : uint8_t
  {
    CW_8BIT = 8
  };

  enum class Parity : uint8_t
  {
    PAR_NONE = 0,
    PAR_ODD,
    PAR_EVEN
  };

  enum class StopBits : uint8_t
  {
    SBITS_ONE = 0,
    SBITS_ONE_POINT_FIVE,
    SBITS_TWO
  };

  enum class FlowControl : uint8_t
  {
    FCTRL_NONE,
    FCTRL_SW,
    FCTRL_HW
  };

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

  struct COMConfig
  {
    size_t baud;
    CharWid width;
    Parity parity;
    StopBits stopBits;
    FlowControl flow;
  };

}  // namespace Chimera::Serial

#endif /* !CHIMERA_SERIAL_TYPES_HPP */