/********************************************************************************
 *  File Name:
 *    spi_types.hpp
 *
 *  Description:
 *    Chimera SPI types
 *
 *  Note:
 *    Don't change the order or numbering of the various enum classes as they
 *    are allowed to be used as array indexes. Only append to the end.
 *
 *  2019-2020 | Brandon Braun | brandonbraun653@gmail.com
 ********************************************************************************/

#pragma once
#ifndef CHIMERA_SPI_TYPES_HPP
#define CHIMERA_SPI_TYPES_HPP

/* STL Includes */
#include <cstring>
#include <cstdint>
#include <memory>

/* Chimera Includes */
#include <Chimera/common>
#include <Chimera/gpio>

namespace Chimera::SPI
{
  /*-------------------------------------------------------------------------------
  Forward Declarations
  -------------------------------------------------------------------------------*/
  class ISPI;

  /*-------------------------------------------------------------------------------
  Aliases
  -------------------------------------------------------------------------------*/
  using ClockFreq    = size_t;
  using TransferMode = Chimera::Hardware::PeripheralMode;
  using ISPI_sPtr    = std::shared_ptr<ISPI>;

  /*-------------------------------------------------------------------------------
  Enumerations
  -------------------------------------------------------------------------------*/
  /**
   *  Controls the endianness of the transfers.
   */
  enum class BitOrder : uint8_t
  {
    MSB_FIRST = 0, /**< The most significant bit will be transmitted first */
    LSB_FIRST,     /**< The least significant bit will be transmitted first */

    NUM_OPTIONS
  };

  /**
   *  Controls the clock phase and polarity using common industry mode types
   *  @see https://en.wikipedia.org/wiki/Serial_Peripheral_Interface
   */
  enum class ClockMode : uint8_t
  {
    MODE0 = 0, /**< CPOL=0, CPHA=0 */
    MODE1,     /**< CPOL=0, CPHA=1 */
    MODE2,     /**< CPOL=1, CPHA=0 */
    MODE3,     /**< CPOL=1, CPHA=1 */

    NUM_OPTIONS
  };

  /**
   *  Overarching control scheme for the peripheral. Are we going
   *  to be acting as a Master or Slave device?
   */
  enum class ControlMode : uint8_t
  {
    MASTER = 0, /**< This device starts/stops transfers */
    SLAVE,      /**< This device responds to another master on the bus */

    NUM_OPTIONS
  };

  /**
   *  Controls how many bits wide each transfer will be. Most chips
   *  support anywhere from 8-16 bits.
   */
  enum class DataSize : uint8_t
  {
    SZ_8BIT = 0,
    SZ_9BIT,
    SZ_10BIT,
    SZ_11BIT,
    SZ_12BIT,
    SZ_13BIT,
    SZ_14BIT,
    SZ_15BIT,
    SZ_16BIT,

    NUM_OPTIONS
  };

  /**
   *  A higher level configuration option that defines the behavior of the
   *  chip select line between transfers. This functionality could be implemented
   *  in either software or hardware, so the performance of this could vary from
   *  chip to chip.
   */
  enum class CSMode : uint8_t
  {
    MANUAL = 0,            /**< SW must manually control the state of the chip select line */
    AUTO_BETWEEN_TRANSFER, /**< Twiddle the chip select between transfers, disabling on completion */
    AUTO_AFTER_TRANSFER,   /**< Disable the chip select only after all transfers complete */

    NUM_OPTIONS
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
   *  Number of supported hardware channels. Each channel corresponds to a physical
   *  peripheral, starting with SPI1 up to SPIx
   */
  enum class Channel : uint8_t
  {
    SPI1,
    SPI2,
    SPI3,
    SPI4,
    SPI5,
    SPI6,

    NUM_OPTIONS,
    NOT_SUPPORTED
  };

  /*-------------------------------------------------------------------------------
  Structures
  -------------------------------------------------------------------------------*/
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
    Channel hwChannel       = Channel::SPI1;          /**< Hardware channel to be configured */
    TransferMode txfrMode   = TransferMode::BLOCKING; /**< Transfer controller mode */
    bool validity           = false;                  /**< Structure validity */
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
    bool externalCS;        /**< Indicates if an external chip select is used */
    bool validity;          /**< Defines if the configuration is valid */

    void clear()
    {
      SCKInit.clear();
      MOSIInit.clear();
      MISOInit.clear();
      CSInit.clear();
      externalCS = false;
      validity   = false;
      HWInit     = {};
    }
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
      ISPI_sPtr ( *getDriver )( const Channel channel );
    };
  }  // namespace Backend

  /*-------------------------------------------------------------------------------
  Classes
  -------------------------------------------------------------------------------*/
  class Status : public Status
  {
  public:
    static constexpr Status_t codeOffset = Chimera::Status::Internal::spiErrorCodeStart;

    static constexpr Status_t PACKET_TOO_LARGE_FOR_BUFFER = codeOffset + 1;
    static constexpr Status_t FAILED_CONVERSION           = codeOffset + 2;
    static constexpr Status_t INVALID_HARDWARE_PARAM      = codeOffset + 3;
    static constexpr Status_t FAILED_CHIP_SELECT_WRITE    = codeOffset + 4;
    static constexpr Status_t CLOCK_SET_LT                = codeOffset + 5;
    static constexpr Status_t CLOCK_SET_GT                = codeOffset + 6;
    static constexpr Status_t CLOCK_SET_EQ                = OK;
    static constexpr Status_t TRANSFER_COMPLETE           = codeOffset + 7;
    static constexpr Status_t TRANSFER_IN_PROGRESS        = codeOffset + 8;
    static constexpr Status_t TRANSFER_ERROR              = codeOffset + 9;
  };

}  // namespace Chimera::SPI

#endif /* !CHIMERA_SPI_TYPES_HPP */
