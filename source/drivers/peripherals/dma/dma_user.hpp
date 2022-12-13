/******************************************************************************
 *  File Name:
 *    dma.hpp
 *
 *  Description:
 *    Implements an interface to create a Chimera DMA peripheral
 *
 *  2019-2021 | Brandon Braun | brandonbraun653@gmail.com
 *****************************************************************************/

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
  /*---------------------------------------------------------------------------
  Public Functions
  ---------------------------------------------------------------------------*/
  /**
   * @brief Initialize the DMA module
   *
   * @return Chimera::Status_t
   */
  Chimera::Status_t initialize();

  /**
   * @brief Reset the DMA module
   *
   * @return Chimera::Status_t
   */
  Chimera::Status_t reset();

  /**
   * @brief Constructs a permanent DMA pipe
   *
   * @param config        Pipe configuration parameters
   * @return RequestId    Unique Id identifying the pipe
   */
  RequestId constructPipe( const PipeConfig &config );

  /**
   * @brief Makes a transfer request between memory
   *
   * Performs an immediate transaction if possible, else queues it
   * for when the hardware is ready to do more work.
   *
   * @param transfer      Parameters describing the request
   * @return RequestId    Unique ID identifying the request
   */
  RequestId transfer( const MemTransfer &transfer );

  /**
   * @brief Makes a transfer request on a DMA pipe
   *
   * @param transfer      Parameters describing the request
   * @return RequestId    Unique ID identifying the request
   */
  RequestId transfer( const PipeTransfer &transfer );

}  // namespace Chimera::DMA

#endif /* !CHIMERA_DMA_HPP */
