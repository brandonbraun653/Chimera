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
  enum class TransferDirection : uint8_t
  {
    PERIPH_TO_MEMORY = 0,
    MEMORY_TO_PERIPH,
    MEMORY_TO_MEMORY,
    PERIPH_TO_PERIPH,

    NUM_TRANSFER_DIRECTIONS
  };

  enum class TransferType : uint8_t
  {
    DIRECT = 0,
    CIRCULAR, 

    NUM_TRANSFER_TYPES
  };


  struct TCB
  {
    TransferDirection direction;

  };

}  // namespace Chimera::DMA

#endif /* !CHIMERA_DMA_TYPES_HPP */