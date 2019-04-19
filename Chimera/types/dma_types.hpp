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

namespace Chimera
{
  namespace AlgCRC
  {
    class DMAClass;
    using DMAClass_sPtr = std::shared_ptr<DMAClass>;
    using DMAClass_uPtr = std::unique_ptr<DMAClass>;
  }
}  // namespace Chimera

#endif /* !CHIMERA_DMA_TYPES_HPP */