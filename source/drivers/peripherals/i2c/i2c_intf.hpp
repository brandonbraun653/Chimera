/******************************************************************************
 *  File Name:
 *    i2c_intf.hpp
 *
 *  Description:
 *    Chimera I2C interface
 *
 *  2021-2022 | Brandon Braun | brandonbraun653@gmail.com
 *****************************************************************************/

#pragma once
#ifndef CHIMERA_I2C_INTERFACE_HPP
#define CHIMERA_I2C_INTERFACE_HPP

/*-----------------------------------------------------------------------------
Includes
-----------------------------------------------------------------------------*/
#include <Chimera/common>
#include <Chimera/event>
#include <Chimera/source/drivers/peripherals/i2c/i2c_types.hpp>
#include <Chimera/thread>
#include <cstddef>
#include <cstdint>


namespace Chimera::I2C
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

  /*---------------------------------------------------------------------------
  Classes
  ---------------------------------------------------------------------------*/
  class HWInterface
  {
    virtual ~HWInterface() = default;

    virtual Chimera::Status_t open( const DriverConfig &cfg )                                              = 0;
    virtual Chimera::Status_t close()                                                                      = 0;
    virtual Chimera::Status_t stop()                                                                       = 0;
    virtual Chimera::Status_t start()                                                                      = 0;
    virtual Chimera::Status_t read( const uint16_t address, void *const data, const size_t length )        = 0;
    virtual Chimera::Status_t write( const uint16_t address, const void *const data, const size_t length ) = 0;
    virtual Chimera::Status_t transfer( const uint16_t address, const void *const tx_data, void *const rx_data,
                                        const size_t length )                                              = 0;
  };


/**
 * Virtual class to facilitate easy mocking of the driver
 */
#if defined( CHIMERA_VIRTUAL )
  class II2C : virtual public HWInterface,
               virtual public Chimera::Event::ListenerInterface,
               virtual public Chimera::Thread::AsyncIOInterface,
               virtual public Chimera::Thread::LockableInterface
  {
  public:
    virtual ~II2C() = default;
  };
#else
  class II2C
  {
  };
#endif /* CHIMERA_VIRTUAL */
}  // namespace Chimera::I2C

#endif /* !CHIMERA_I2C_INTERFACE_HPP */
