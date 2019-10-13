/********************************************************************************
 *   File Name:
 *    spi_types.hpp
 *
 *   Description:
 *    Chimera SPI types
 *
 *   2019 | Brandon Braun | brandonbraun653@gmail.com
 ********************************************************************************/

#pragma once
#ifndef CHIMERA_SPI_TYPES_HPP
#define CHIMERA_SPI_TYPES_HPP

/* C++ Includes */
#include <cstdint>
#include <memory>

/* Chimera Includes */
#include <Chimera/types/common_types.hpp>
#include <Chimera/types/gpio_types.hpp>

namespace Chimera::SPI
{
  class SPIClass;
  using SPIClass_sPtr = std::shared_ptr<SPIClass>;
  using SPIClass_uPtr = std::unique_ptr<SPIClass>;

  using ClockFreq    = size_t;
  using Channel      = uint8_t;
  using TransferMode = Chimera::Hardware::PeripheralMode;

  class Status : public CommonStatusCodes
  {
  public:
    static constexpr Status_t codeOffset = Chimera::Status::Internal::spiOffset;

    static constexpr Status_t PACKET_TOO_LARGE_FOR_BUFFER = codeOffset + 1;
    static constexpr Status_t FAILED_CONVERSION           = codeOffset + 2;
    static constexpr Status_t INVALID_HARDWARE_PARAM      = codeOffset + 3;
    static constexpr Status_t FAILED_CHIP_SELECT_WRITE    = codeOffset + 4;
    static constexpr Status_t CLOCK_SET_LT                = codeOffset + 5;
    static constexpr Status_t CLOCK_SET_GT                = codeOffset + 6;
    static constexpr Status_t CLOCK_SET_EQ                = OK;
  };

  /**
   *  Controls the endianness of the transfers.
   */
  enum class BitOrder : uint8_t
  {
    MSB_FIRST,  /**< The most significant bit will be transmitted first */
    LSB_FIRST   /**< The least significant bit will be transmitted first */
  };

  /**
   *  Controls the clock phase and polarity using common industry mode types
   *  @see https://en.wikipedia.org/wiki/Serial_Peripheral_Interface
   */
  enum class ClockMode : uint8_t
  {
    MODE0, /**< CPOL=0, CPHA=0 */
    MODE1, /**< CPOL=0, CPHA=1 */
    MODE2, /**< CPOL=1, CPHA=0 */
    MODE3  /**< CPOL=1, CPHA=1 */
  };

  /**
   *  Overarching control scheme for the peripheral. Are we going 
   *  to be acting as a Master or Slave device?
   */
  enum class ControlMode : uint8_t
  {
    MASTER, /**< This device starts/stops transfers */
    SLAVE   /**< This device responds to another master on the bus */
  };

  /**
   *  Controls how many bits wide each transfer will be. Most chips 
   *  support anywhere from 8-16 bits.
   */
  enum class DataSize : uint8_t
  {
    SZ_8BIT,
    SZ_9BIT,
    SZ_10BIT,
    SZ_11BIT,
    SZ_12BIT,
    SZ_13BIT,
    SZ_14BIT,
    SZ_15BIT,
    SZ_16BIT,
  };

  /**
   *  A higher level configuration option that defines the behavior of the 
   *  chip select line between transfers. This functionality could be implemented
   *  in either software or hardware, so the performance of this could vary from
   *  chip to chip.
   */
  enum class CSMode : uint8_t
  {
    MANUAL,                /**< SW must manually control the state of the chip select line */
    AUTO_BETWEEN_TRANSFER, /**< Twiddle the chip select between transfers, disabling on completion */
    AUTO_AFTER_TRANSFER    /**< Disable the chip select only after all transfers complete */
  };

  /**
   *  Possible events that can occur within the driver
   */
  enum class Event : uint8_t
  {
    RX_COMPLETE,           /**< Receive complete */
    QUEUED_RX_COMPLETE,    /**< Queued receive completed */
    TX_COMPLETE,           /**< Transmit complete */
    QUEUED_TX_COMPLETE,    /**< Queued transmit completed */
    TXRX_COMPLETE,         /**< Transmit receive operation complete */
    QUEUED_TX_RX_COMPLETE, /**< Queued transfer completed */
    SLAVE_RX_BYTE,         /**< Slave receive buffer got a byte */
    SLAVE_RX_HALF,         /**< Slave receive buffer half full */
    SLAVE_RX_FULL,         /**< Slave receive buffer full */

    NUM_OPTIONS
  };

  /**
   *  Low level hardware configuration options that define the physical
   *  layer behavior common to all SPI peripherals.
   */
  struct HardwareInit
  {
    BitOrder bitOrder       = BitOrder::MSB_FIRST;    /**< Sets LSB or MSB ordering of the transfers */
    ControlMode controlMode = ControlMode::MASTER;    /**< The primary arbitration method for the peripheral */
    ClockFreq clockFreq     = 1000000;                /**< The desired approximate clock frequency */
    ClockMode clockMode     = ClockMode::MODE0;       /**< Sets the clock phase and polarity options */
    CSMode csMode           = CSMode::MANUAL;         /**< Chip select control mode */
    DataSize dataSize       = DataSize::SZ_8BIT;      /**< How wide each transfer should minimally be */
    Channel hwChannel       = 0;                      /**< Hardware channel to be configured */
    TransferMode txfrMode   = TransferMode::BLOCKING; /**< Transfer controller mode */
  };

  /**
   *  Configuration struct that fully defines all the necessary information to 
   *  initialize an SPI peripheral 
   */
  struct DriverConfig
  {
    GPIO::PinInit SCKInit;  /**< The GPIO pin settings used for SCK */
    GPIO::PinInit MOSIInit; /**< The GPIO pin settings used for MOSI */
    GPIO::PinInit MISOInit; /**< The GPIO pin settings used for MISO */
    GPIO::PinInit CSInit;   /**< The GPIO pin settings used for CS */
    HardwareInit HWInit;    /**< Hardware driver configuration options */
  };


}  // namespace Chimera::SPI

#endif /* !CHIMERA_SPI_TYPES_HPP */