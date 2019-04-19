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

/* Chimera Includes */
#include <Chimera/threading.hpp>

namespace Chimera
{
  namespace DMA
  {
    class Interface : public Threading::Lockable
    {
    public:
    };

    class DMAUnsupported : public Interface
    {
    public:
      DMAUnsupported() = default;
    };

    using CHIMERA_INHERITED_DMA = DMAUnsupported;
    
  }  // namespace DMA
}  // namespace Chimera

#endif /* !CHIMERA_DMA_INTERFACE_HPP */