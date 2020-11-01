/********************************************************************************
 *  File Name:
 *    can_types.hpp
 *
 *  Description:
 *    Chimera CAN types
 *
 *  2019-2020 | Brandon Braun | brandonbraun653@gmail.com
 ********************************************************************************/

#pragma once
#ifndef CHIMERA_CAN_TYPES_HPP
#define CHIMERA_CAN_TYPES_HPP

/* STL Includes */
#include <cstdint>
#include <cstring>
#include <memory>

/* Chimera Includes */
#include <Chimera/common>
#include <Chimera/gpio>
#include <Chimera/source/drivers/peripherals/peripheral_types.hpp>

namespace Chimera::CAN
{
  /*-------------------------------------------------------------------------------
  Forward Declarations
  -------------------------------------------------------------------------------*/
  class Driver;
  struct BasicFrame;

  /*-------------------------------------------------------------------------------
  Aliases
  -------------------------------------------------------------------------------*/
  using Driver_sPtr = std::shared_ptr<Driver>;

  using Identifier_t = uint32_t;
  using DataLength_t = uint8_t;

  using FrameCallback_t = void ( * )( const BasicFrame *const frame );

  /*-------------------------------------------------------------------------------
  Constants
  -------------------------------------------------------------------------------*/
  static constexpr size_t MAX_PAYLOAD_LENGTH = 8;  // Bytes
  static constexpr size_t ID_MASK_11_BIT     = 0x000007FF;
  static constexpr size_t ID_MASK_29_BIT     = 0x1FFFFFFF;

  /*-------------------------------------------------
  Special message ID used to indicate to the driver
  that a given operation will work on all message
  types. Useful because the max ID is 29 bits and
  this data type contains 32.
  -------------------------------------------------*/
  static constexpr Identifier_t ANY_MESSAGE_ID = std::numeric_limits<Identifier_t>::max();

  /*-------------------------------------------------------------------------------
  Enumerations
  -------------------------------------------------------------------------------*/
  enum class DebugMode : uint8_t
  {
    SILENT,
    LOOPBACK,
    LOOPBACK_AND_SILENT,

    NUM_OPTIONS,
    UNKNOWN
  };

  enum class BufferType : uint8_t
  {
    TX,
    RX,

    NUM_OPTIONS,
    UNKNOWN
  };

  enum class Channel : uint8_t
  {
    CAN0,
    CAN1,
    CAN2,

    NUM_OPTIONS,
    UNKNOWN
  };

  enum class IdType : uint8_t
  {
    STANDARD,
    EXTENDED,

    NUM_OPTIONS,
    UNKNOWN
  };


  enum class FrameType : uint8_t
  {
    DATA,
    REMOTE,

    NUM_OPTIONS,
    UNKNOWN
  };

  enum class ErrorType : uint8_t
  {

    NUM_OPTIONS,
    UNKNOWN
  };

  enum class InterruptType : uint8_t
  {
    /*-------------------------------------------------
    Transmit Interrupt Category
    -------------------------------------------------*/
    TX_ISR                 = 0,
    TRANSMIT_MAILBOX_EMPTY = TX_ISR,

    /*-------------------------------------------------
    Receive Interrupt Category
    -------------------------------------------------*/
    RX_ISR,
    RECEIVE_FIFO_NEW_MESSAGE = RX_ISR,
    RECEIVE_FIFO_FULL,
    RECEIVE_FIFO_OVERRUN,

    /*-------------------------------------------------
    Status Change Interrupt Category
    -------------------------------------------------*/
    STS_ISR,
    SLEEP_EVENT = STS_ISR,
    WAKEUP_EVENT,

    /*-------------------------------------------------
    Error Interrupt Category
    -------------------------------------------------*/
    ERR_ISR,
    ERROR_CODE_EVENT = ERR_ISR,
    ERROR_BUS_OFF_EVENT,
    ERROR_PASSIVE_EVENT,
    ERROR_WARNING_EVENT,

    NUM_OPTIONS,
    UNKNOWN
  };

  /*-------------------------------------------------------------------------------
  Structures
  -------------------------------------------------------------------------------*/
  struct BasicFrame
  {
    Identifier_t id;                    /**< Message identifier, sized according to idMode */
    IdType idMode;                      /**< Specifies either standard or extended ID */
    FrameType frameType;                /**< Is this a data or remote frame */
    DataLength_t dataLength;            /**< How many bytes are in the data field */
    uint8_t filterIndex;                /**< RX only: Which filter this frame matched against */
    uint8_t data[ MAX_PAYLOAD_LENGTH ]; /**< Data payload */

    void clear()
    {
      id          = 0;
      idMode      = IdType::UNKNOWN;
      frameType   = FrameType::UNKNOWN;
      dataLength  = 0;
      filterIndex = 0;
      memset( data, 0, MAX_PAYLOAD_LENGTH );
    }
  };


  struct CANStatus
  {
    // Buffer fill levels
    // Last TX/RX timestamps
    // Listening/not listening
    // Identifiers being listened to
  };


  struct Filter
  {
    uint32_t id;   /**< ID being filtered on */
    uint32_t mask; /**< Mask bits for the ID  */
    bool extended; /**< Is the CAN ID an extended format? Else standard.*/
  };


  struct HardwareInit
  {
    Channel channel;          /**< Channel the config settings are for */
    BasicFrame *txBuffer;     /**< Buffer for queueing TX frames */
    size_t txElements;        /**< Number of frames the TX buffer can hold */
    BasicFrame *rxBuffer;     /**< Buffer for queuing RX frames */
    size_t rxElements;        /**< Number of frame the RX buffer can hold */
    float samplePointPercent; /**< How far into the bit period to sample (CANOpen: 87.5%, ARINC825: 75%)*/
    size_t baudRate;          /**< Bus communication rate in Hz*/
    uint8_t timeQuanta;       /**< Number of intervals each bit is divided into (Recommend 16 or 8) */
    uint8_t resyncJumpWidth;  /**< Number of time quanta allowed to shift for syncing (Recommend 1) */
    float maxBaudError;       /**< Max allowable baud rate error abs(%) */

    void clear()
    {
      channel            = Channel::UNKNOWN;
      txBuffer           = nullptr;
      txElements         = 0;
      rxBuffer           = nullptr;
      rxElements         = 0;
      timeQuanta         = 16;
      resyncJumpWidth    = 1;
      samplePointPercent = 0.875;
      baudRate           = 100000;
    }
  };

  struct DriverConfig
  {
    Chimera::GPIO::PinInit TXInit; /**< The GPIO pin settings used for TX */
    Chimera::GPIO::PinInit RXInit; /**< The GPIO pin settings used for RX */
    HardwareInit HWInit;           /**< Hardware configuration options */
    bool validity;                 /**< Defines if the configuration is valid */

    void clear()
    {
      TXInit.clear();
      RXInit.clear();
      HWInit.clear();
      validity = false;
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
       *  Factory function that creates a shared_ptr instance of the backend
       *  driver, as long as it conforms to the expected interface.
       */
      Driver_sPtr ( *getDriver )( const Channel channel );
    };
  }  // namespace Backend
}  // namespace Chimera::CAN

#endif /* !CHIMERA_CAN_TYPES_HPP */
