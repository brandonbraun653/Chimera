/********************************************************************************
 *  File Name:
 *    dma_base.hpp
 *
 *  Description:
 *    Abstract base class interfaces
 *
 *  2019-2020 | Brandon Braun | brandonbraun653@gmail.com
 ********************************************************************************/

#pragma once
#ifndef CHIMERA_DMA_ABSTRACT_BASE_HPP
#define CHIMERA_DMA_ABSTRACT_BASE_HPP

/* STL Includes */
#include <cstdint>

/* Chimera Includes*/
#include <Chimera/thread>
#include <Chimera/src/peripherals/dma/dma_intf.hpp>

namespace Chimera::DMA
{
  class DMAUnsupported : virtual public IDMA, public Chimera::Threading::Lockable
  {
  public:
    DMAUnsupported()  = default;
    ~DMAUnsupported() = default;

    Chimera::Status_t init() final override
    {
      return Chimera::CommonStatusCodes::NOT_SUPPORTED;
    }

    Chimera::Status_t reset() final override
    {
      return Chimera::CommonStatusCodes::NOT_SUPPORTED;
    }

    Chimera::Status_t start() final override
    {
      return Chimera::CommonStatusCodes::NOT_SUPPORTED;
    }

    Chimera::Status_t configure( const Init &config, const TCB &transfer, const size_t timeout,
                                 TransferHandle_t *const handle ) final override
    {
      return Chimera::CommonStatusCodes::NOT_SUPPORTED;
    }

    Chimera::Status_t abort( TransferHandle_t handle, const size_t timeout ) final override
    {
      return Chimera::CommonStatusCodes::NOT_SUPPORTED;
    }

    Chimera::Status_t status( TransferHandle_t handle, const size_t timeout ) final override
    {
      return Chimera::CommonStatusCodes::NOT_SUPPORTED;
    }
  };
}  // namespace Chimera::DMA

#endif /* !CHIMERA_CRC_ABSTRACT_BASE_HPP */