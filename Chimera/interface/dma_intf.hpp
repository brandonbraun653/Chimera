/********************************************************************************
 *   File Name:
 *    dma_intf.hpp
 *
 *   Description:
 *    Models the Chimera DMA interface
 *
 *   2019 | Brandon Braun | brandonbraun653@gmail.com
 ********************************************************************************/

#pragma once
#ifndef CHIMERA_DMA_INTERFACE_HPP
#define CHIMERA_DMA_INTERFACE_HPP

/* C++ Includes */
#include <cstdint>

/* Chimera Includes */
#include <Chimera/types/dma_types.hpp>

namespace Chimera::DMA
{
  class Interface
  {
  public:
    virtual ~Interface() = default;

    virtual Chimera::Status_t startTransfer( )
  };

  class DMAUnsupported : public Interface
  {
  public:
    DMAUnsupported() = default;
  };

  using CHIMERA_INHERITED_DMA = DMAUnsupported;

}  // namespace Chimera::DMA

#endif /* !CHIMERA_DMA_INTERFACE_HPP */