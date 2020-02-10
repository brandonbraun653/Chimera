/********************************************************************************
 *   File Name:
 *    dma_base.hpp
 *
 *   Description:
 *    Abstract base class interfaces
 *
 *   2019 | Brandon Braun | brandonbraun653@gmail.com
 ********************************************************************************/

#pragma once
#ifndef CHIMERA_DMA_ABSTRACT_BASE_HPP
#define CHIMERA_DMA_ABSTRACT_BASE_HPP

/* STL Includes */
#include <cstdint>

/* Chimera Includes*/
#include <Chimera/interface/dma_intf.hpp>

namespace Chimera::DMA
{
  class HWInterfaceUnsupported : public HWInterface
  {
  public:
    HWInterfaceUnsupported()  = default;
    ~HWInterfaceUnsupported() = default;

    Chimera::Status_t init( const Init &config, const size_t timeout, TransferHandle_t handle )
    {
      return Chimera::CommonStatusCodes::NOT_SUPPORTED;
    }

    Chimera::Status_t start( TransferHandle_t handle, const TCB &transfer, const size_t timeout )
    {
      return Chimera::CommonStatusCodes::NOT_SUPPORTED;
    }

    Chimera::Status_t abort( TransferHandle_t handle )
    {
      return Chimera::CommonStatusCodes::NOT_SUPPORTED;
    }

    Chimera::Status_t status( TransferHandle_t handle )
    {
      return Chimera::CommonStatusCodes::NOT_SUPPORTED;
    }
  };
}  // namespace Chimera::DMA

#endif /* !CHIMERA_CRC_ABSTRACT_BASE_HPP */