/********************************************************************************
 *  File Name:
 *    chimera_serial.cpp
 *
 *	 Description:
 *    Implements the serial driver. Note that because virtual inheritance is
 *    frowned upon for memory consumption reasons, the interface is forced to
 *    choose which driver it executes at runtime.
 *
 *  2020 | Brandon Braun | brandonbraun653@gmail.com
 *******************************************************************************/

/* STL Includes */
#include <array>
#include <memory>

/* Chimera Includes */
#include <Chimera/common>
#include <Chimera/serial>
#include <Chimera/uart>
#include <Chimera/usart>

/*-------------------------------------------------------------------------------
Constants
-------------------------------------------------------------------------------*/
static constexpr size_t NUM_DRIVERS = static_cast<size_t>( Chimera::Serial::Channel::NUM_OPTIONS );

/*-------------------------------------------------------------------------------
Variables
-------------------------------------------------------------------------------*/
static Chimera::Serial::Driver s_raw_driver[ NUM_DRIVERS ];
static Chimera::Serial::Driver_rPtr s_shared_driver[ NUM_DRIVERS ];
static Chimera::UART::Driver_rPtr s_uart_driver[ NUM_DRIVERS ];
static Chimera::USART::Driver_rPtr s_usart_driver[ NUM_DRIVERS ];


namespace Chimera::Serial
{
  /*-------------------------------------------------------------------------------
  Public Functions
  -------------------------------------------------------------------------------*/
  Chimera::Status_t initialize()
  {
    for ( auto x = 0; x < NUM_DRIVERS; x++ )
    {
      if ( !s_shared_driver[ x ] )
      {
        s_shared_driver[ x ] = Driver_rPtr( &s_raw_driver[ x ] );
      }
    }

    return Chimera::Status::OK;
  }

  Chimera::Status_t reset()
  {
    return Chimera::Status::OK;
  }

  Driver_rPtr getDriver( const Channel channel )
  {
    /*-------------------------------------------------
    Boundary check the input
    -------------------------------------------------*/
    auto idx = static_cast<size_t>( channel );
    if ( channel >= Channel::NUM_OPTIONS )
    {
      return nullptr;
    }

    return s_shared_driver[ idx ];
  }

  /*-------------------------------------------------------------------------------
  Driver Implementation
  -------------------------------------------------------------------------------*/
  Driver::Driver() : mUsart( false ), mChannel( Chimera::Serial::Channel::NOT_SUPPORTED )
  {
  }

  Driver::~Driver()
  {
  }

  /*-------------------------------------------------
  Interface: Hardware
  -------------------------------------------------*/
  Chimera::Status_t Driver::assignHW( const Chimera::Serial::Channel channel, const Chimera::Serial::IOPins &pins )
  {
    mChannel = channel;
    auto idx = static_cast<size_t>( channel );

    if ( Chimera::USART::isChannelUSART( channel ) )
    {
      /*-------------------------------------------------
      Register the driver if no one has yet
      -------------------------------------------------*/
      mUsart = true;
      if ( !s_usart_driver[ idx ] )
      {
        s_usart_driver[ idx ] = Chimera::USART::getDriver( channel );
      }

      /*-------------------------------------------------
      Invoke the expected method
      -------------------------------------------------*/
      return s_usart_driver[ idx ]->assignHW( channel, pins );
    }
    else if ( Chimera::UART::isChannelUART( channel ) )
    {
      /*-------------------------------------------------
      Register the driver if no one has yet
      -------------------------------------------------*/
      mUsart = false;
      if ( !s_uart_driver[ idx ] )
      {
        s_uart_driver[ idx ] = Chimera::UART::getDriver( channel );
      }

      /*-------------------------------------------------
      Invoke the expected method
      -------------------------------------------------*/
      return s_uart_driver[ idx ]->assignHW( channel, pins );
    }
    else
    {
      return Chimera::Status::FAIL;
    }
  }


  Chimera::Status_t Driver::begin( const Chimera::Hardware::PeripheralMode txMode,
                                   const Chimera::Hardware::PeripheralMode rxMode )
  {
    if ( mUsart )
    {
      return s_usart_driver[ static_cast<size_t>( mChannel ) ]->begin( txMode, rxMode );
    }
    else
    {
      return s_uart_driver[ static_cast<size_t>( mChannel ) ]->begin( txMode, rxMode );
    }
  }


  Chimera::Status_t Driver::end()
  {
    if ( mUsart )
    {
      return s_usart_driver[ static_cast<size_t>( mChannel ) ]->end();
    }
    else
    {
      return s_uart_driver[ static_cast<size_t>( mChannel ) ]->end();
    }
  }


  Chimera::Status_t Driver::configure( const Chimera::Serial::Config &config )
  {
    if ( mUsart )
    {
      return s_usart_driver[ static_cast<size_t>( mChannel ) ]->configure( config );
    }
    else
    {
      return s_uart_driver[ static_cast<size_t>( mChannel ) ]->configure( config );
    }
  }


  Chimera::Status_t Driver::setBaud( const uint32_t baud )
  {
    if ( mUsart )
    {
      return s_usart_driver[ static_cast<size_t>( mChannel ) ]->setBaud( baud );
    }
    else
    {
      return s_uart_driver[ static_cast<size_t>( mChannel ) ]->setBaud( baud );
    }
  }


  Chimera::Status_t Driver::setMode( const Chimera::Hardware::SubPeripheral periph,
                                     const Chimera::Hardware::PeripheralMode mode )
  {
    if ( mUsart )
    {
      return s_usart_driver[ static_cast<size_t>( mChannel ) ]->setMode( periph, mode );
    }
    else
    {
      return s_uart_driver[ static_cast<size_t>( mChannel ) ]->setMode( periph, mode );
    }
  }


  Chimera::Status_t Driver::write( const void *const buffer, const size_t length )
  {
    if ( mUsart )
    {
      return s_usart_driver[ static_cast<size_t>( mChannel ) ]->write( buffer, length );
    }
    else
    {
      return s_uart_driver[ static_cast<size_t>( mChannel ) ]->write( buffer, length );
    }
  }


  Chimera::Status_t Driver::read( void *const buffer, const size_t length )
  {
    if ( mUsart )
    {
      return s_usart_driver[ static_cast<size_t>( mChannel ) ]->read( buffer, length );
    }
    else
    {
      return s_uart_driver[ static_cast<size_t>( mChannel ) ]->read( buffer, length );
    }
  }


  Chimera::Status_t Driver::flush( const Chimera::Hardware::SubPeripheral periph )
  {
    if ( mUsart )
    {
      return s_usart_driver[ static_cast<size_t>( mChannel ) ]->flush( periph );
    }
    else
    {
      return s_uart_driver[ static_cast<size_t>( mChannel ) ]->flush( periph );
    }
  }


  Chimera::Status_t Driver::toggleAsyncListening( const bool state )
  {
    if ( mUsart )
    {
      return s_usart_driver[ static_cast<size_t>( mChannel ) ]->toggleAsyncListening( state );
    }
    else
    {
      return s_uart_driver[ static_cast<size_t>( mChannel ) ]->toggleAsyncListening( state );
    }
  }


  Chimera::Status_t Driver::readAsync( uint8_t *const buffer, const size_t len )
  {
    if ( mUsart )
    {
      return s_usart_driver[ static_cast<size_t>( mChannel ) ]->readAsync( buffer, len );
    }
    else
    {
      return s_uart_driver[ static_cast<size_t>( mChannel ) ]->readAsync( buffer, len );
    }
  }


  Chimera::Status_t Driver::enableBuffering( const Chimera::Hardware::SubPeripheral periph,
                                             boost::circular_buffer<uint8_t> *const userBuffer, uint8_t *const hwBuffer,
                                             const size_t hwBufferSize )
  {
    if ( mUsart )
    {
      return s_usart_driver[ static_cast<size_t>( mChannel ) ]->enableBuffering( periph, userBuffer, hwBuffer, hwBufferSize );
    }
    else
    {
      return s_uart_driver[ static_cast<size_t>( mChannel ) ]->enableBuffering( periph, userBuffer, hwBuffer, hwBufferSize );
    }
  }


  Chimera::Status_t Driver::disableBuffering( const Chimera::Hardware::SubPeripheral periph )
  {
    if ( mUsart )
    {
      return s_usart_driver[ static_cast<size_t>( mChannel ) ]->disableBuffering( periph );
    }
    else
    {
      return s_uart_driver[ static_cast<size_t>( mChannel ) ]->disableBuffering( periph );
    }
  }


  bool Driver::available( size_t *const bytes )
  {
    if ( mUsart )
    {
      return s_usart_driver[ static_cast<size_t>( mChannel ) ]->available( bytes );
    }
    else
    {
      return s_uart_driver[ static_cast<size_t>( mChannel ) ]->available( bytes );
    }
  }


  void Driver::postISRProcessing()
  {
    if ( mUsart )
    {
      return s_usart_driver[ static_cast<size_t>( mChannel ) ]->postISRProcessing();
    }
    else
    {
      return s_uart_driver[ static_cast<size_t>( mChannel ) ]->postISRProcessing();
    }
  }


  /*-------------------------------------------------
  Interface: AsyncIO
  -------------------------------------------------*/
  Chimera::Status_t Driver::await( const Chimera::Event::Trigger event, const size_t timeout )
  {
    if ( mUsart )
    {
      return s_usart_driver[ static_cast<size_t>( mChannel ) ]->await( event, timeout );
    }
    else
    {
      return s_uart_driver[ static_cast<size_t>( mChannel ) ]->await( event, timeout );
    }
  }


  Chimera::Status_t Driver::await( const Chimera::Event::Trigger event, Chimera::Thread::BinarySemaphore &notifier,
                                   const size_t timeout )
  {
    if ( mUsart )
    {
      return s_usart_driver[ static_cast<size_t>( mChannel ) ]->await( event, notifier, timeout );
    }
    else
    {
      return s_uart_driver[ static_cast<size_t>( mChannel ) ]->await( event, notifier, timeout );
    }
  }

  /*-------------------------------------------------
  Interface: Lockable
  -------------------------------------------------*/
  void Driver::lock()
  {
    if ( mUsart )
    {
      return s_usart_driver[ static_cast<size_t>( mChannel ) ]->lock();
    }
    else
    {
      return s_uart_driver[ static_cast<size_t>( mChannel ) ]->lock();
    }
  }

  void Driver::lockFromISR()
  {
    if ( mUsart )
    {
      return s_usart_driver[ static_cast<size_t>( mChannel ) ]->lockFromISR();
    }
    else
    {
      return s_uart_driver[ static_cast<size_t>( mChannel ) ]->lockFromISR();
    }
  }

  bool Driver::try_lock_for( const size_t timeout )
  {
    if ( mUsart )
    {
      return s_usart_driver[ static_cast<size_t>( mChannel ) ]->try_lock_for( timeout );
    }
    else
    {
      return s_uart_driver[ static_cast<size_t>( mChannel ) ]->try_lock_for( timeout );
    }
  }

  void Driver::unlock()
  {
    if ( mUsart )
    {
      return s_usart_driver[ static_cast<size_t>( mChannel ) ]->unlock();
    }
    else
    {
      return s_uart_driver[ static_cast<size_t>( mChannel ) ]->unlock();
    }
  }

  void Driver::unlockFromISR()
  {
    if ( mUsart )
    {
      return s_usart_driver[ static_cast<size_t>( mChannel ) ]->unlockFromISR();
    }
    else
    {
      return s_uart_driver[ static_cast<size_t>( mChannel ) ]->unlockFromISR();
    }
  }
}  // namespace Chimera::Serial
