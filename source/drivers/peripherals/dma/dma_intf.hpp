/******************************************************************************
 *   File Name:
 *    dma_intf.hpp
 *
 *   Description:
 *    Models the Chimera DMA interface
 *
 *   2019-2021 | Brandon Braun | brandonbraun653@gmail.com
 *****************************************************************************/

#pragma once
#ifndef CHIMERA_DMA_INTERFACE_HPP
#define CHIMERA_DMA_INTERFACE_HPP

/* Chimera Includes */
#include <Chimera/common>
#include <Chimera/source/drivers/peripherals/dma/dma_types.hpp>

namespace Chimera::DMA
{
  /*---------------------------------------------------------------------------
  Public Functions
  ---------------------------------------------------------------------------*/
  namespace Backend
  {
    /**
     *  Registers the backend driver with Chimera
     *
     *  @param[in]  registry    Chimera's copy of the driver interface
     *  @return Chimera::Status_t
     */
    extern Chimera::Status_t registerDriver( DriverConfig &registry );
  }  // namespace Backend


  namespace Util
  {
    /**
     * @brief Generates a unique request ID
     * @return RequestId
     */
    RequestId genRequestId();

    void initializeQueues();

    bool enqueuePipeTransfer( PipeTransfer &transfer );

    bool nextPipeTransfer( PipeTransfer &transfer );

    bool enqueueMemTransfer( MemTransfer &transfer );

    bool nextMemTransfer( MemTransfer &transfer );
  }
}  // namespace Chimera::DMA

#endif /* !CHIMERA_DMA_INTERFACE_HPP */
