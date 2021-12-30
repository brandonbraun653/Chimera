/******************************************************************************
 *  File Name:
 *    i2c_user.hpp
 *
 *  Description:
 *    Chimera I2C User Interface
 *
 *  2021 | Brandon Braun | brandonbraun653@gmail.com
 *****************************************************************************/

#pragma once
#ifndef CHIMERA_I2C_USER_INTERFACE_HPP
#define CHIMERA_I2C_USER_INTERFACE_HPP

/*-----------------------------------------------------------------------------
Includes
-----------------------------------------------------------------------------*/
#include <Chimera/source/drivers/peripherals/i2c/i2c_intf.hpp>
#include <Chimera/source/drivers/peripherals/i2c/i2c_types.hpp>


namespace Chimera::I2C
{
  /*---------------------------------------------------------------------------
  Public Functions
  ---------------------------------------------------------------------------*/
  Chimera::Status_t initialize();
  Chimera::Status_t reset();
  Driver_rPtr getDriver( const Channel channel );

  /*---------------------------------------------------------------------------
  Classes
  ---------------------------------------------------------------------------*/
  /**
   * Concrete class declaration that promises to implement the virtual one, to
   * avoid paying the memory penalty of a v-table lookup. Implemented project
   * side using the Bridge pattern.
   */
  class Driver
  {
  public:
    Driver();
    ~Driver();

    /*-------------------------------------------------------------------------
    Interface: Hardware
    -------------------------------------------------------------------------*/
    Chimera::Status_t open( const DriverConfig &cfg );
    Chimera::Status_t close();
    Chimera::Status_t read( const uint16_t address, void *const data, const size_t length );
    Chimera::Status_t write( const uint16_t address, const void *const data, const size_t length );
    Chimera::Status_t transfer( const uint16_t address, const void *const tx_data, void *const rx_data, const size_t length );
    Chimera::Status_t stop();
    Chimera::Status_t start();

    /*-------------------------------------------------------------------------
    Interface: Listener
    -------------------------------------------------------------------------*/
    Chimera::Status_t registerListener( Chimera::Event::Actionable &listener, const size_t timeout, size_t &registrationID );
    Chimera::Status_t removeListener( const size_t registrationID, const size_t timeout );

    /*-------------------------------------------------------------------------
    Interace: AsyncIO
    -------------------------------------------------------------------------*/
    Chimera::Status_t await( const Chimera::Event::Trigger event, const size_t timeout );
    Chimera::Status_t await( const Chimera::Event::Trigger event, Chimera::Thread::BinarySemaphore &notifier,
                             const size_t timeout );

    /*-------------------------------------------------------------------------
    Interface: Lockable
    -------------------------------------------------------------------------*/
    void lock();
    void lockFromISR();
    bool try_lock_for( const size_t timeout );
    void unlock();
    void unlockFromISR();

  private:
    void * mDriver; /**< Instance of the implementer's CAN driver */
  };

}  // namespace Chimera::I2C

#endif  /* !CHIMERA_I2C_USER_INTERFACE_HPP */
