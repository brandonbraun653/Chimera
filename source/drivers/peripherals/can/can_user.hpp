/********************************************************************************
 *  File Name:
 *    can.hpp
 *
 *  Description:
 *    Implements an interface to create a Chimera CAN peripheral
 *
 *  2020-2022 | Brandon Braun | brandonbraun653@gmail.com
 ********************************************************************************/

#pragma once
#ifndef CHIMERA_CAN_HPP
#define CHIMERA_CAN_HPP

/*-----------------------------------------------------------------------------
Includes
-----------------------------------------------------------------------------*/
#include <Chimera/source/drivers/peripherals/can/can_intf.hpp>
#include <Chimera/source/drivers/peripherals/can/can_types.hpp>
#include <Chimera/source/drivers/threading/threading_extensions.hpp>

namespace Chimera::CAN
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
  class Driver : public Thread::Lockable<Driver>, public Thread::AsyncIO<Driver>, public ICAN
  {
  public:
    using Chimera::Thread::AsyncIO<Driver>::AsyncIO;

    Driver();
    ~Driver();

    /**
     *  Opens a CAN port using the given channel. Initializes the TX/RX
     *  FIFOs using the default sizing with dynamic memory allocation.
     *
     *  @param[in]  cfg         The CAN bus configuration settings
     *  @return Chimera::Status_t
     */
    Chimera::Status_t open( const DriverConfig &cfg );

    /**
     *  Closes the current port if already open. This will also
     *  de-allocate any memory possibly allocated on open().
     *
     *  @return Chimera::Status_t
     */
    Chimera::Status_t close();

    /**
     *  Acquires status data related to the bus operation
     *
     *  @return CANStatus
     */
    CANStatus getStatus();

    /**
     *  Enqueues a frame on the TX FIFO to be sent out on the bus
     *
     *  @param[in]  frame       The frame to be transmitted
     *  @return Chimera::Status_t
     */
    Chimera::Status_t send( const BasicFrame &frame );

    /**
     *  Attempts to read a frame off the RX FIFO. If none is available, will
     *  wait the specified timeout period before returning.
     *
     *  @note The RX FIFO only contains messages that are not subscribed to
     *
     *  @param[out] frame       The frame to place the received message into
     *  @return Chimera::Status_t
     */
    Chimera::Status_t receive( BasicFrame &frame );

    /**
     *  Uses the given filter list to selectively decide which messages
     *  will make it into the RX FIFO. This is a whitelisting approach.
     *
     *  @param[in]  list        Identifier whitelist
     *  @param[in]  size        Number of filter elements in the list
     *  @return Chimera::Status_t
     */
    Chimera::Status_t filter( const Filter *const list, const size_t size );

    /**
     *  Flushes the requested buffers. If possible, this also extends
     *  down to the hardware layer and flushes any FIFO queues there too.
     *
     *  @param[in]  buffer      The buffer to be flushed
     *  @return Chimera::Status_t
     */
    Chimera::Status_t flush( BufferType buffer );

    /**
     *  Checks how many frames are available for reception
     *
     *  @return size_t
     */
    size_t available();

  private:
    friend Chimera::Thread::Lockable<Driver>;
    friend Chimera::Thread::AsyncIO<Driver>;

    void * mImpl; /**< Instance of the implementer's CAN driver */
  };
}  // namespace Chimera::CAN

#endif  /* !CHIMERA_CAN_HPP */
