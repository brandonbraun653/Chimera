/********************************************************************************
 *  File Name:
 *    dma.hpp
 *
 *  Description:
 *    Implements an interface to create a Chimera DMA peripheral
 *
 *  2019-2020 | Brandon Braun | brandonbraun653@gmail.com
 ********************************************************************************/

#pragma once
#ifndef CHIMERA_DMA_HPP
#define CHIMERA_DMA_HPP

/* STL Includes */
#include <memory>

/* Chimera Includes */
#include <Chimera/src/peripherals/dma/dma_intf.hpp>

namespace Chimera::DMA
{
  using DMA_sPtr = std::shared_ptr<IDMA>;
  using DMA_uPtr = std::unique_ptr<IDMA>;

  Chimera::Status_t initialize();

  DMA_sPtr create_shared_ptr();

  DMA_uPtr create_unique_ptr();
}  // namespace Chimera::DMA

#endif /* !CHIMERA_DMA_HPP */
