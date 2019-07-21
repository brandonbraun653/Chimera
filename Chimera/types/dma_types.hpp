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

/* C++ Includes */
#include <cstdint>
#include <memory>

/* Chimera Includes */
#include <Chimera/types/common_types.hpp>

namespace Chimera::DMA
{
  using TransferHandle_t = void *;

  /**
   *  Options for the memory transfer direction
   */
  enum class TransferDirection : uint8_t
  {
    PERIPH_TO_MEMORY = 0,
    MEMORY_TO_PERIPH,
    MEMORY_TO_MEMORY,
    PERIPH_TO_PERIPH,

    NUM_TRANSFER_DIRECTIONS
  };

  /**
   *  Options for the DMA hardware operational mode
   */
  enum class Mode : uint8_t
  {
    NORMAL,
    CIRCULAR,
    PERIPH_CONTROL
  };

  /**
   *  Options for which channel to use on the DMA transfer
   */
  enum class Channel : uint8_t
  {
    CHANNEL0 = 0,
    CHANNEL1,
    CHANNEL2,
    CHANNEL3,
    CHANNEL4,
    CHANNEL5,
    CHANNEL6,
    CHANNEL7,
    CHANNEL8,
    CHANNEL9,
    CHANNEL10,
    CHANNEL11,
    CHANNEL12,
    CHANNEL13,
    CHANNEL14,
    CHANNEL15,

    CHANNEL_UNSUPPORTED,
    MAX_NUM_CHANNELS
  };

  /**
   *  Selects whether or not to automatically increment the 
   *  selected peripheral address when the transfer completes.
   */
  enum class PeripheralIncrement : uint8_t
  {
    ENABLED = 0,
    DISABLED,

    NUM_PERIPH_INCREMENT_OPTIONS
  };

  /**
   *  Selects whether or not to automatically increment the 
   *  selected memory address when the transfer completes. 
   */
  enum class MemoryIncrement : uint8_t
  {
    ENABLED = 0,
    DISABLED,

    NUM_MEMORY_INCREMENT_OPTIONS
  };

  /**
   *  Selects the alignment for memory to memory transfers
   */
  enum class MemoryAlignment : uint8_t
  {
    ALIGN_BYTE,
    ALIGN_HALF_WORD,
    ALIGN_WORD,

    NUM_ALIGNMENT_OPTIONS
  };

  /**
   * Selects the alignment for peripheral to memory transfers
   */
  enum class PeriphMemoryAlignment : uint8_t
  {
    ALIGN_BYTE,
    ALIGN_HALF_WORD,
    ALIGN_WORD,

    NUM_ALIGNMENT_OPTIONS
  };

  enum class Priority : uint8_t
  {
    LOW,
    MEDIUM,
    HIGH,
    VERY_HIGH,

    NUM_PRIORITY_OPTIONS
  };

  struct Init
  {
    Channel channel;                /**< Which hardware channel should be used for the transfer */
    Mode mode;
    TransferDirection direction;    /**< What direction the transfer will be occuring */

  };

  struct TCB
  {
    uint32_t source;
    uint32_t sink;
    uint32_t size;
  };

}  // namespace Chimera::DMA

#endif /* !CHIMERA_DMA_TYPES_HPP */