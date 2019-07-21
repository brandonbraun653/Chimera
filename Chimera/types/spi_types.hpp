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

  enum class Mode : uint8_t
  {
    MASTER,
    SLAVE
  };

  enum class BitOrder : uint8_t
  {
    MSB_FIRST,
    LSB_FIRST
  };

  enum class ClockMode : uint8_t
  {
    MODE0, /**< CPOL=0, CPHA=0 */
    MODE1, /**< CPOL=0, CPHA=1 */
    MODE2, /**< CPOL=1, CPHA=0 */
    MODE3  /**< CPOL=1, CPHA=1 */
  };

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

  enum class ChipSelectMode : uint8_t
  {
    MANUAL,                /**< Manually control the state of the chip select line */
    AUTO_BETWEEN_TRANSFER, /**< Twiddle the chip select between transfers, disabling on completion */
    AUTO_AFTER_TRANSFER    /**< Disable the chip select only after all transfers complete */
  };

  struct Setup
  {
    GPIO::PinInit SCK;  /**< The GPIO pin settings used for SCK */
    GPIO::PinInit MOSI; /**< The GPIO pin settings used for MOSI */
    GPIO::PinInit MISO; /**< The GPIO pin settings used for MISO */
    GPIO::PinInit CS;   /**< The GPIO pin settings used for CS */

    uint8_t channel         = 0;
    Mode mode               = Mode::MASTER;        /**< The primary control method for the peripheral */
    DataSize dataSize       = DataSize::SZ_8BIT;   /**< How wide each transfer should minimally be */
    BitOrder bitOrder       = BitOrder::MSB_FIRST; /**< Sets LSB or MSB ordering */
    ClockMode clockMode     = ClockMode::MODE0;    /**< Sets the clock phase and polarity options */
    uint32_t clockFrequency = 1000000;             /**< The desired approximate clock frequency */
    Chimera::Hardware::SubPeripheralMode transferMode = Chimera::Hardware::SubPeripheralMode::DMA;
  };

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
  };
}  // namespace Chimera::SPI

#endif /* !CHIMERA_SPI_TYPES_HPP */