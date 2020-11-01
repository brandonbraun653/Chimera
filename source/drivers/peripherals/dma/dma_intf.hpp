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

/* STL Includes */
#include <cstdint>

/* Chimera Includes */
#include <Chimera/event>
#include <Chimera/thread>
#include <Chimera/source/drivers/peripherals/dma/dma_types.hpp>

namespace Chimera::DMA
{
  /*-------------------------------------------------------------------------------
  Public Functions
  -------------------------------------------------------------------------------*/
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

  /*-------------------------------------------------------------------------------
  Class
  -------------------------------------------------------------------------------*/
  /**
   * Defines expected behavior for all embedded systems that hardware control
   */
  class HWInterface
  {
  public:
    virtual ~HWInterface() = default;

    /**
     *  Initializes the low level hardware. Must be called before any
     *  transfers are started.
     *
     *  @return Chimera::Status_t
     *
     *  |  Return Value |                Explanation                |
     *  |:-------------:|:-----------------------------------------:|
     *  |            OK | The transfer was successfully initialized |
     *  |       TIMEOUT | The DMA hardware could not be acquired    |
     */
    virtual Chimera::Status_t init() = 0;

    virtual Chimera::Status_t reset() = 0;

    virtual Chimera::Status_t start() = 0;

    /**
     *  Configures a new transfer
     *
     *  @param[in]  config    Description of how the transfer should execute
     *  @param[in]  transfer  The transfer memory/data details
     *  @param[in]  timeout   How long to wait in mS for the DMA hardware to become free
     *  @param[out] handle    If transfer has started, it will contain a handle to the transfer instance
     *  @return Chimera::Status_t
     *
     *  | Return Value |               Explanation              |
     *  |:------------:|:--------------------------------------:|
     *  |           OK | The transfer was successfully started  |
     *  |         FAIL | The transfer could not start           |
     *  |      TIMEOUT | The DMA hardware could not be acquired |
     */
    virtual Chimera::Status_t configure( const Init &config, const TCB &transfer, const size_t timeout,
                                         TransferHandle_t *const handle ) = 0;

    /**
     *  Kills an ongoing transfer
     *
     *  @param[in]  handle    The handle returned when the transfer started
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
     *  @param[in]  handle    The handle of the transfer to be queried
     *  @param[in]  timeout   How long to wait in mS for the DMA hardware to become free
     *  @return Chimera::Status_t
     */
    virtual Chimera::Status_t status( TransferHandle_t handle, const size_t timeout ) = 0;
  };

  /**
   *  Virtual class to facilitate easy mocking of the driver
   */
  class IDMA : virtual public HWInterface, virtual public Chimera::Threading::LockableInterface
  {
  public:
    virtual ~IDMA() = default;
  };


  /**
   *  Concrete class declaration that promises to implement the virtual one, to
   *  avoid paying the memory penalty of a v-table lookup. Implemented project side
   *  using the Bridge pattern.
   */
  class Driver
  {
  public:
    Driver();
    ~Driver();

    /*-------------------------------------------------
    Interface: Hardware
    -------------------------------------------------*/
    Chimera::Status_t init();
    Chimera::Status_t reset();
    Chimera::Status_t start();
    Chimera::Status_t configure( const Init &config, const TCB &transfer, const size_t timeout,
                                 TransferHandle_t *const handle );
    Chimera::Status_t abort( TransferHandle_t handle, const size_t timeout );
    Chimera::Status_t status( TransferHandle_t handle, const size_t timeout );

    /*-------------------------------------------------
    Interface: Lockable
    -------------------------------------------------*/
    void lock();
    void lockFromISR();
    bool try_lock_for( const size_t timeout );
    void unlock();
    void unlockFromISR();

  private:
    void *mDriver; /**< Opaque pointer to the implementer's driver */
  };
}  // namespace Chimera::DMA

#endif /* !CHIMERA_DMA_INTERFACE_HPP */