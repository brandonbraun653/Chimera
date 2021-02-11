/********************************************************************************
 *  File Name:
 *    usart_intf.hpp
 *
 *	 Description:
 *    Interface to the USART peripheral
 *
 *  2020-2021 | Brandon Braun | brandonbraun653@gmail.com
 *******************************************************************************/

#pragma once
#ifndef CHIMERA_USART_INTERFACE_HPP
#define CHIMERA_USART_INTERFACE_HPP

/* Chimera Includes */
#include <Chimera/common>
#include <Chimera/source/drivers/peripherals/usart/usart_types.hpp>
#include <Chimera/source/drivers/serial/serial_intf.hpp>

namespace Chimera::USART
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
  /**
   *  Virtual class to facilitate easy mocking of the driver
   */
  class IUSART : virtual public Chimera::Serial::ISerial
  {
  public:
    virtual ~IUSART() = default;
  };


  /**
   *  Concrete class declaration that promises to implement the virtual one, to
   *  avoid paying the memory penalty of a v-table lookup. Implemented project side.
   */
  class Driver
  {
  public:
    Driver();
    ~Driver();

    /*-------------------------------------------------
    Interface: Hardware
    -------------------------------------------------*/
    Chimera::Status_t assignHW( const Chimera::Serial::Channel channel, const Chimera::Serial::IOPins &pins );
    Chimera::Status_t begin( const Chimera::Hardware::PeripheralMode txMode, const Chimera::Hardware::PeripheralMode rxMode );
    Chimera::Status_t end();
    Chimera::Status_t configure( const Chimera::Serial::Config &config );
    Chimera::Status_t setBaud( const uint32_t baud );
    Chimera::Status_t setMode( const Chimera::Hardware::SubPeripheral periph, const Chimera::Hardware::PeripheralMode mode );
    Chimera::Status_t write( const void *const buffer, const size_t length );
    Chimera::Status_t read( void *const buffer, const size_t length );
    Chimera::Status_t flush( const Chimera::Hardware::SubPeripheral periph );
    Chimera::Status_t toggleAsyncListening( const bool state );
    Chimera::Status_t readAsync( uint8_t *const buffer, const size_t len );
    Chimera::Status_t enableBuffering( const Chimera::Hardware::SubPeripheral periph,
                                       boost::circular_buffer<uint8_t> *const userBuffer, uint8_t *const hwBuffer,
                                       const size_t hwBufferSize );
    Chimera::Status_t disableBuffering( const Chimera::Hardware::SubPeripheral periph );
    bool available( size_t *const bytes = nullptr );
    void postISRProcessing();

    /*-------------------------------------------------
    Interface: AsyncIO
    -------------------------------------------------*/
    Chimera::Status_t await( const Chimera::Event::Trigger event, const size_t timeout );
    Chimera::Status_t await( const Chimera::Event::Trigger event, Chimera::Thread::BinarySemaphore &notifier,
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
    Chimera::Serial::Channel mChannel;
  };
}

#endif  /* !CHIMERA_USART_INTERFACE_HPP */
