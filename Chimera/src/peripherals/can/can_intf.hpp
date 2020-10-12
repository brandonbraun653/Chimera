/********************************************************************************
 *  File Name:
 *    can_intf.hpp
 *
 *  Description:
 *    Models the Chimera CAN interface
 *
 *  2020 | Brandon Braun | brandonbraun653@gmail.com
 ********************************************************************************/

#pragma once
#ifndef CHIMERA_CAN_INTERFACE_HPP
#define CHIMERA_CAN_INTERFACE_HPP

/* Chimera Includes */
#include <Chimera/common>
#include <Chimera/thread>
#include <Chimera/event>
#include <Chimera/src/peripherals/can/can_types.hpp>

namespace Chimera::CAN
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
  Classes
  -------------------------------------------------------------------------------*/
  class HWInterface
  {
  public:
    virtual ~HWInterface() = default;

    /**
     *  Opens a CAN port using the given channel. Initializes the TX/RX
     *  FIFOs using the default sizing with dynamic memory allocation.
     *
     *  @param[in]  cfg         The CAN bus configuration settings
     *  @return Chimera::Status_t
     */
    virtual Chimera::Status_t open( const DriverConfig &cfg ) = 0;

    /**
     *  Closes the current port if already open. This will also
     *  de-allocate any memory possibly allocated on open().
     *
     *  @return Chimera::Status_t
     */
    virtual Chimera::Status_t close() = 0;

    /**
     *  Acquires status data related to the bus operation
     *
     *  @return CANStatus
     */
    virtual CANStatus getStatus() = 0;

    /**
     *  Enqueues a frame on the TX FIFO to be sent out on the bus
     *
     *  @param[in]  frame       The frame to be transmitted
     *  @return Chimera::Status_t
     */
    virtual Chimera::Status_t send( const BasicFrame &frame ) = 0;

    /**
     *  Attempts to read a frame off the RX FIFO. If none is available, will
     *  wait the specified timeout period before returning.
     *
     *  @note The RX FIFO only contains messages that are not subscribed to
     *
     *  @param[out] frame       The frame to place the received message into
     *  @param[in]  timeout     How long to wait for a new message to arrive
     *  @return Chimera::Status_t
     */
    virtual Chimera::Status_t receive( BasicFrame &frame, const size_t timeout ) = 0;

    /**
     *  Uses the given filter list to selectively decide which messages
     *  will make it into the RX FIFO. This is a whitelisting approach.
     *
     *  @param[in]  list        Identifier whitelist
     *  @param[in]  size        Number of filter elements in the list
     *  @return Chimera::Status_t
     */
    virtual Chimera::Status_t filter( const Filter *const list, const size_t size ) = 0;

    /**
     *  Flushes the requested buffers. If possible, this also extends
     *  down to the hardware layer and flushes any FIFO queues there too.
     *
     *  @param[in]  buffer      The buffer to be flushed
     *  @return Chimera::Status_t
     */
    virtual Chimera::Status_t flush( BufferType buffer ) = 0;
  };


  /**
   *  Virtual class to facilitate easy mocking of the driver
   */
  class ICAN : virtual public HWInterface,
               virtual public Chimera::Event::ListenerInterface,
               virtual public Chimera::Threading::AsyncIOInterface,
               virtual public Chimera::Threading::LockableInterface
  {
  public:
    virtual ~ICAN() = default;
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
    Chimera::Status_t open( const DriverConfig &cfg );
    Chimera::Status_t close();
    CANStatus getStatus();
    Chimera::Status_t send( const BasicFrame &frame );
    Chimera::Status_t receive( BasicFrame &frame, const size_t timeout );
    Chimera::Status_t filter( const Filter *const list, const size_t size );
    Chimera::Status_t flush( BufferType buffer );

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

#endif /* !CHIMERA_CAN_INTERFACE_HPP */
