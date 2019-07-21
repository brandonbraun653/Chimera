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
#include <Chimera/interface/callback_intf.hpp>
#include <Chimera/types/dma_types.hpp>

namespace Chimera::DMA
{
  class Interface
  {
  public:
    virtual ~Interface() = default;

    /**
     *  Prepares the low level hardware for a new transfer
     *
     *  @param[in]  config    Config information for how the DMA should behave
     *  @param[in]  timeout   How long to wait in mS for the DMA hardware to become free
     *  @param[out] handle    If configuration is successful, will contain a handle to the transfer instance
     *  @return Chimera::Status_t
     *
     *  |  Return Value |                Explanation                |
     *  |:-------------:|:-----------------------------------------:|
     *  |            OK | The transfer was successfully initialized |
     *  |          FAIL | The transfer could not initialize         |
     *  |       TIMEOUT | The DMA hardware could not be acquired    |
     *  | NOT_SUPPORTED | Some transfer settings were not allowed   |
     */
    virtual Chimera::Status_t init( const Init &config, const size_t timeout, TransferHandle_t handle) = 0;

    /**
     *  Starts a new transfer
     *
     *  @param[in]  handle    The handle returned from the init() function
     *  @param[in]  transfer  The transfer memory/data details
     *  @param[in]  timeout   How long to wait in mS for the DMA hardware to become free
     *  @return Chimera::Status_t
     *
     *  | Return Value |               Explanation              |
     *  |:------------:|:--------------------------------------:|
     *  |           OK | The transfer was successfully started  |
     *  |         FAIL | The transfer could not start           |
     *  |      TIMEOUT | The DMA hardware could not be acquired |
     */
    virtual Chimera::Status_t start( TransferHandle_t handle, const TCB &transfer, const size_t timeout ) = 0;

    /**
     *  Kills an ongoing transfer
     *
     *  @param[in]  handle    The handle returned from the init() function
     *  @param[in]  timeout   How long to wait in mS for the DMA hardware to become free
     *
     *  | Return Value |                  Explanation                 |
     *  |:------------:|:--------------------------------------------:|
     *  |           OK | The transfer was aborted or already complete |
     *  |         FAIL | The transfer could not abort                 |
     *  |      TIMEOUT | The DMA hardware could not be acquired       |
     */
    virtual Chimera::Status_t abort( TransferHandle_t handle, const size_t timeout ) = 0;

    /**
     *  Gets the status of the current transfer
     *
     *  @param[in] handle     The handle of the transfer to be queried
     *  @param[in]  timeout   How long to wait in mS for the DMA hardware to become free
     *  @return Chimera::Status_t
     */
    virtual Chimera::Status_t status( TransferHandle_t handle, const size_t timeout ) = 0;
  };

  class DMAUnsupported : public Interface
  {
  public:
    DMAUnsupported() = default;
    ~DMAUnsupported() = default;

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

  using CHIMERA_INHERITED_DMA = DMAUnsupported;

}  // namespace Chimera::DMA

#endif /* !CHIMERA_DMA_INTERFACE_HPP */