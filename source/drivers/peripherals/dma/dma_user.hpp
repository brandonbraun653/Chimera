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
#ifndef CHIMERA_DMA_USER_HPP
#define CHIMERA_DMA_USER_HPP

/* STL Includes */
#include <memory>

/* Chimera Includes */
#include <Chimera/source/drivers/peripherals/dma/dma_intf.hpp>
#include <Chimera/source/drivers/peripherals/dma/dma_types.hpp>

namespace Chimera::DMA
{
  Chimera::Status_t initialize();
  Chimera::Status_t reset();
  Driver_rPtr getDriver( const Controller channel );
}  // namespace Chimera::DMA

#endif /* !CHIMERA_DMA_HPP */