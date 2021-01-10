/********************************************************************************
 *  File Name:
 *    can.hpp
 *
 *  Description:
 *    Implements an interface to create a Chimera CAN peripheral
 *
 *  2020 | Brandon Braun | brandonbraun653@gmail.com
 ********************************************************************************/

#pragma once
#ifndef CHIMERA_CAN_HPP
#define CHIMERA_CAN_HPP

/* STL Includes */
#include <memory>

/* Chimera Includes */
#include <Chimera/source/drivers/peripherals/can/can_intf.hpp>
#include <Chimera/source/drivers/peripherals/can/can_types.hpp>

namespace Chimera::CAN
{
  /*-------------------------------------------------------------------------------
  Public Functions
  -------------------------------------------------------------------------------*/
  Chimera::Status_t initialize();
  Chimera::Status_t reset();
  Driver_rPtr getDriver( const Channel channel );


  /*-------------------------------------------------------------------------------
  Classes
  -------------------------------------------------------------------------------*/
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
    Chimera::Status_t open( const DriverConfig &cfg );
    Chimera::Status_t close();
    CANStatus getStatus();
    Chimera::Status_t send( const BasicFrame &frame );
    Chimera::Status_t receive( BasicFrame &frame );
    Chimera::Status_t filter( const Filter *const list, const size_t size );
    Chimera::Status_t flush( BufferType buffer );
    size_t available();

    /*-------------------------------------------------
    Interface: Listener
    -------------------------------------------------*/
    Chimera::Status_t registerListener( Chimera::Event::Actionable &listener, const size_t timeout, size_t &registrationID );
    Chimera::Status_t removeListener( const size_t registrationID, const size_t timeout );

    /*-------------------------------------------------
    Interface: AsyncIO
    -------------------------------------------------*/
    Chimera::Status_t await( const Chimera::Event::Trigger event, const size_t timeout );
    Chimera::Status_t await( const Chimera::Event::Trigger event, Chimera::Threading::BinarySemaphore &notifier,
                             const size_t timeout );

    /*-------------------------------------------------
    Interface: Lockable
    -------------------------------------------------*/
    void lock();
    void lockFromISR();
    bool try_lock_for( const size_t timeout );
    void unlock();
    void unlockFromISR();

  private:
    void * mDriver; /**< Instance of the implementer's CAN driver */
  };
}  // namespace Chimera::CAN

#endif  /* !CHIMERA_CAN_HPP */
