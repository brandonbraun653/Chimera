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
#include <memory>

/* Chimera Includes */
#include <Chimera/common>
#include <Chimera/src/peripherals/peripheral_types.hpp>

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
  using Driver_rPtr = Driver *;
  using Driver_sPtr = std::shared_ptr<Driver>;

  using Identifier_t = uint32_t;
  using DataLength_t = uint8_t;

  using FrameCallback_t = void ( * )( const BasicFrame *const frame );

  /*-------------------------------------------------------------------------------
  Constants
  -------------------------------------------------------------------------------*/
  static constexpr size_t MAX_PAYLOAD_LENGTH = 8; // Bytes

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

  enum class IdMode : uint8_t
  {
    STANDARD,
    EXTENDED,

    NUM_OPTIONS,
    UNKNOWN
  };

  enum class FilterMode : uint8_t
  {
    MASK,
    ID_LIST,

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

  /*-------------------------------------------------------------------------------
  Structures
  -------------------------------------------------------------------------------*/
  struct BasicFrame
  {
    Identifier_t id;
    IdMode idMode;
    FrameType frameType;
    DataLength_t dataLength;
    uint8_t data[ MAX_PAYLOAD_LENGTH ];
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
    Identifier_t id;  /**< ID being filtered on */
    FilterMode type;  /**< How to interpret the ID filter */
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
