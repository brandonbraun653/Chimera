/********************************************************************************
 *   File Name:
 *    dma_types.hpp
 *
 *   Description:
 *    Chimera DMA types
 *
 *   2019 | Brandon Braun | brandonbraun653@gmail.com
 ********************************************************************************/

#pragma once
#ifndef CHIMERA_DMA_TYPES_HPP
#define CHIMERA_DMA_TYPES_HPP

/* STL Includes */
#include <cstdint>
#include <cstring>
#include <memory>

/* Chimera Includes */
#include <Chimera/common>

namespace Chimera::DMA
{
  using TransferHandle_t = void *;

  /**
   *  Options for which channel to use on the DMA transfer. Usually
   *  these are called streams.
   *
   *  @note These values are used to index arrays! Do not change their order or value!
   */
  using RequestID = uint32_t;

  /**
   *  Options for the memory transfer direction
   *
   *  @note These values are used to index arrays! Do not change their order or value!
   */
  enum class TransferDirection : uint8_t
  {
    PERIPH_TO_MEMORY = 0,
    MEMORY_TO_PERIPH,
    MEMORY_TO_MEMORY,
    PERIPH_TO_PERIPH,

    NUM_OPTIONS
  };

  /**
   *  Options for the DMA hardware operational mode
   *
   *  @note These values are used to index arrays! Do not change their order or value!
   */
  enum class Mode : uint8_t
  {
    NORMAL = 0,
    CIRCULAR,
    PERIPH_CONTROL,

    NUM_OPTIONS
  };

  /**
   *  Selects whether or not to automatically increment the selected peripheral
   *  address when the transfer completes.
   *
   *  @note These values are used to index arrays! Do not change their order or value!
   */
  enum class PeripheralIncrement : uint8_t
  {
    ENABLED = 0,
    DISABLED,

    NUM_OPTIONS
  };

  /**
   *  Selects whether or not to automatically increment the selected memory
   *  address when the transfer completes.
   *
   *  @note These values are used to index arrays! Do not change their order or value!
   */
  enum class MemoryIncrement : uint8_t
  {
    ENABLED = 0,
    DISABLED,

    NUM_OPTIONS
  };

  /**
   *  Selects the alignment for memory to memory transfers
   *
   *  @note These values are used to index arrays! Do not change their order or value!
   */
  enum class MemoryAlignment : uint8_t
  {
    ALIGN_BYTE = 0,
    ALIGN_HALF_WORD,
    ALIGN_WORD,

    NUM_OPTIONS
  };

  /**
   *  Selects the alignment for peripheral to memory transfers
   *
   *  @note These values are used to index arrays! Do not change their order or value!
   */
  enum class PeripheralAlignment : uint8_t
  {
    ALIGN_BYTE = 0,
    ALIGN_HALF_WORD,
    ALIGN_WORD,

    NUM_OPTIONS
  };

  /**
   *  Selects the transfer priority
   *
   *  @note These values are used to index arrays! Do not change their order or value!
   */
  enum class Priority : uint8_t
  {
    LOW = 0,
    MEDIUM,
    HIGH,
    VERY_HIGH,

    NUM_OPTIONS
  };

  struct Init
  {
    TransferDirection direction; /**< What direction the transfer will be occuring */
    Mode mode;                   /**< The style of memory transfer */
    RequestID request;           /**< MCU specific identifier of who generated the request */
    PeripheralIncrement pInc;    /**< Should the peripheral address auto-increment? */
    PeripheralAlignment pAlign;  /**< Byte alignment of the peripheral transfer */
    MemoryIncrement mInc;        /**< Should the memory address auto-increment? */
    MemoryAlignment mAlign;      /**< Byte alignment of the memory transfer */
    Priority priority;           /**< Priority level of the transfer */

    Init()
    {
      memset( this, 0, sizeof( Init ) );
    }
  };

  struct TCB
  {
    uint32_t srcAddress;  /**< Location where data will be copied from */
    uint32_t dstAddress;  /**< Location where data will be copied into */
    size_t transferSize;  /**< How many bytes to transfer */

    TCB()
    {
      memset( this, 0, sizeof( TCB ) );
    }
  };

}  // namespace Chimera::DMA

#endif /* !CHIMERA_DMA_TYPES_HPP */