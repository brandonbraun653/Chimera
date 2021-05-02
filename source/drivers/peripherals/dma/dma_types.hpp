/********************************************************************************
 *  File Name:
 *    dma_types.hpp
 *
 *  Description:
 *    Chimera DMA types
 *
 *  2019-2021 | Brandon Braun | brandonbraun653@gmail.com
 ********************************************************************************/

#pragma once
#ifndef CHIMERA_DMA_TYPES_HPP
#define CHIMERA_DMA_TYPES_HPP

/* STL Includes */
#include <cstdint>
#include <cstring>
#include <limits>

/* ETL Includes */
#include <etl/delegate.h>

/* Chimera Includes */
#include <Chimera/common>
#include <Chimera/peripheral>

namespace Chimera::DMA
{
  /*-------------------------------------------------------------------------------
  Forward Declarations
  -------------------------------------------------------------------------------*/
  struct TransferStats;


  /*-------------------------------------------------------------------------------
  Aliases
  -------------------------------------------------------------------------------*/
  /**
   * @brief Unique identifier for an immediate or queued transfer
   *
   * Only relevant to non-permanent requests and does not correlate with any
   * internal DMA channels or streams. Mainly used for reporting and tracking.
   */
  using RequestId = uint32_t;

  /**
   * @brief Generic callback type for DMA events
   *
   * @param TransferStats   Information about the transfer
   */
  using TransferCallback = etl::delegate<void( const TransferStats & )>;

  /*-------------------------------------------------------------------------------
  Constants
  -------------------------------------------------------------------------------*/
  static constexpr RequestId INVALID_REQUEST = std::numeric_limits<RequestId>::max();

  /*-------------------------------------------------------------------------------
  Enumerations
  -------------------------------------------------------------------------------*/
  /**
   * @brief Supported memory transfer directions
   */
  enum class Direction : uint8_t
  {
    PERIPH_TO_MEMORY,
    MEMORY_TO_PERIPH,
    MEMORY_TO_MEMORY,
    PERIPH_TO_PERIPH,

    NUM_OPTIONS
  };

  /**
   * @brief Options for the DMA transfer mode
   *
   * Note that depending on the transfer direction and who the flow controller
   * is, a particular Mode may not be available. Check your device datasheet
   * for details.
   */
  enum class Mode : uint8_t
  {
    DIRECT,        /**< Memory is written directly */
    CIRCULAR,      /**< Uses circular buffering */
    DOUBLE_BUFFER, /**< Uses double buffering */

    NUM_OPTIONS
  };

  /**
   *  Selects the alignment for peripheral to memory transfers
   */
  enum class Alignment : uint8_t
  {
    BYTE = 0,
    HALF_WORD,
    WORD,

    NUM_OPTIONS
  };

  /**
   *  Selects the transfer priority
   */
  enum class Priority : uint8_t
  {
    LOW = 0,
    MEDIUM,
    HIGH,
    VERY_HIGH,

    NUM_OPTIONS
  };

  enum class Error : uint8_t
  {
    ERR_OK,

    NUM_OPTIONS
  };


  /*-------------------------------------------------------------------------------
  Structures
  -------------------------------------------------------------------------------*/
  /**
   * @brief Periph <-> Memory pipe configuration descriptor
   *
   * Used when the user wants to transfer data between a peripheral and
   * physical memory. Can be configured to go either direction. This assumes
   * all memory that isn't a peripheral is contiguous and that the peripheral
   * has been properly configured to accept/control DMA transactions.
   *
   * --- Configuration Notes ---
   * Direction: Periph -> Memory
   *    - Assumes contiguous regions in destination memory
   *    - Memory address must exist permanently (statically allocated).
   *
   * Direction: Memory -> Periph
   *    - Memory address must exist while the DMA transaction is in progress
   *    - Take care that the peripheral can accept the data alignment
   *
   * Direction: Memory -> Memory
   *    - Assumes contiguous regions
   *    - Assumes regions are not modified by user/ISR while DMA in progress
   */
  struct PipeConfig
  {
    std::uintptr_t memAddr;    /**< Memory src/dst address */
    size_t memSize;            /**< Number of bytes in the memory backed address */
    std::uintptr_t periphAddr; /**< Peripheral src/dst address */
    Priority priority;         /**< Priority level of the transfer */
    Alignment alignment;       /**< Transfer data alignment */
    Direction direction;       /**< What direction is the data flowing? */
    Mode mode;                 /**< What mode is the transfer using */
    Peripheral::Type periph;   /**< Which peripheral to set up the transfer with */
    TransferCallback callback; /**< Optional callback to be invoked on completion or error */
  };


  /**
   * @brief Manually transfers memory on a pre-constructed DMA pipe
   *
   * Realistically, this is for transfers from Memory -> Peripheral. Going the
   * other way from Peripheral -> Memory would be handled by the peripheral and
   * notify the user via a registered callback.
   */
  struct PipeTransfer
  {
    RequestId pipe;            /**< Which pipe this is destined for */
    std::uintptr_t src;        /**< Source memory address */
    size_t size;               /**< Number of bytes to transfer */
    TransferCallback callback; /**< Optional callback to be invoked on completion or error */
  };


  /**
   * @brief Memory <-> Memory transfer request descriptor
   *
   * Used when the user wants to perform a DMA transfer between two contiguous
   * locations in physical memory. This defaults to a DIRECT transfer mode.
   */
  struct MemTransfer
  {
    RequestId id;              /**< UUID for the transfer */
    std::uintptr_t src;        /**< Source address */
    std::uintptr_t dst;        /**< Destination address */
    size_t size;               /**< Number of bytes */
    Priority priority;         /**< Priority level of the transfer */
    Alignment alignment;       /**< Transfer data alignment */
    TransferCallback callback; /**< Optional callback to be invoked on completion or error */
  };


  /**
   * @brief Transfer statistics for reporting to callbacks
   */
  struct TransferStats
  {
    Error errorCode;     /**< Error status of the transfer */
    RequestId requestId; /**< Which request this occurred on */
    size_t size;         /**< Number of bytes transferred */
  };


  /*-------------------------------------------------------------------------------
  Backend Namespace
  -------------------------------------------------------------------------------*/
  namespace Backend
  {
    /**
     * @brief Driver registration structure
     */
    struct DriverConfig
    {
      bool isSupported;
      Chimera::Status_t ( *initialize )( void );
      Chimera::Status_t ( *reset )( void );
      RequestId ( *constructPipe )( const PipeConfig & );
      RequestId ( *memTransfer )( const MemTransfer & );
      RequestId ( *pipeTransfer )( const PipeTransfer & );
    };
  }  // namespace Backend

}  // namespace Chimera::DMA

#endif /* !CHIMERA_DMA_TYPES_HPP */
