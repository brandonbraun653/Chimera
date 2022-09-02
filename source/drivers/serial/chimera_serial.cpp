/******************************************************************************
 *  File Name:
 *    chimera_serial.cpp
 *
 *  Description:
 *    Wrapper around UART/USART peripherals to provide a seamless serial driver
 *
 *  2022 | Brandon Braun | brandonbraun653@protonmail.com
 *****************************************************************************/

/*-----------------------------------------------------------------------------
Includes
-----------------------------------------------------------------------------*/
#include <Chimera/serial>
#include <Chimera/common>
#include <Chimera/usart>
#include <Chimera/uart>

namespace Chimera::Serial
{
  struct Impl
  {
    Chimera::UART::Driver_rPtr uartDriver;
    Chimera::USART::Driver_rPtr usartDriver;
  };

  static DeviceManager<Impl, Channel, EnumValue( Channel::NUM_OPTIONS )> s_impl_drivers;
  static DeviceManager<Driver, Channel, EnumValue( Channel::NUM_OPTIONS )> s_raw_drivers;


    // sink->lock();
    // hwResult |= sink->write( message, length );
    // hwResult |= sink->await( Chimera::Event::Trigger::TRIGGER_WRITE_COMPLETE, TIMEOUT_BLOCK );
    // sink->unlock();

  /*---------------------------------------------------------------------------
  Public Functions
  ---------------------------------------------------------------------------*/
  Chimera::Status_t initialize()
  {
    return Chimera::Status::OK;
  }


  Chimera::Status_t attach( const Chimera::Peripheral::Type type, const Channel channel )
  {
    auto impl = s_impl_drivers.getOrCreate( channel );
    auto raw  = s_raw_drivers.getOrCreate( channel );
    RT_DBG_ASSERT( impl && raw );

    if ( type == Peripheral::Type::PERIPH_USART )
    {
      impl->usartDriver = USART::getDriver( channel );
      RT_DBG_ASSERT( impl->usartDriver );
    }
    else
    {
      impl->uartDriver = UART::getDriver( channel );
      RT_DBG_ASSERT( impl->uartDriver );
    }

    return Chimera::Status::OK;
  }


  Driver_rPtr getDriver( const Channel channel )
  {
    /*-------------------------------------------------------------------------
    Driver must be attached first before this will succeed
    -------------------------------------------------------------------------*/
    auto impl = s_impl_drivers.get( channel );
    if ( !impl )
    {
      return nullptr;
    }

    /*-------------------------------------------------------------------------
    Return the driver
    -------------------------------------------------------------------------*/
    auto raw = s_raw_drivers.getOrCreate( channel );
    raw->mImpl = reinterpret_cast<void*>( impl );

    return raw;
  }


  /*---------------------------------------------------------------------------
  Classes
  ---------------------------------------------------------------------------*/
  Driver::Driver()
  {

  }


  Driver::~Driver()
  {

  }


  Chimera::Status_t Driver::open( const Config &config )
  {
    /*-------------------------------------------------------------------------
    Grab the implementation
    -------------------------------------------------------------------------*/
    RT_DBG_ASSERT( mImpl );
    auto impl = reinterpret_cast<Impl*>( mImpl );
    auto result = Chimera::Status::OK;

    /*-------------------------------------------------------------------------
    Invoke the correct driver
    -------------------------------------------------------------------------*/
    if( impl->uartDriver )
    {
      result |= impl->uartDriver->configure( config );
    }
    else
    {
      result |= impl->usartDriver->configure( config );
    }
  }


  Chimera::Status_t Driver::close()
  {
    /*-------------------------------------------------------------------------
    Grab the implementation
    -------------------------------------------------------------------------*/
    RT_DBG_ASSERT( mImpl );
    auto impl = reinterpret_cast<Impl*>( mImpl );
    auto result = Chimera::Status::OK;

    /*-------------------------------------------------------------------------
    Invoke the correct driver
    -------------------------------------------------------------------------*/
    if( impl->uartDriver )
    {
      result |= impl->uartDriver->end();
    }
    else
    {
      result |= impl->usartDriver->end();
    }

  }


  Chimera::Status_t Driver::flush( const Chimera::Hardware::SubPeripheral periph )
  {
    /*-------------------------------------------------------------------------
    Grab the implementation
    -------------------------------------------------------------------------*/
    RT_DBG_ASSERT( mImpl );
    auto impl = reinterpret_cast<Impl*>( mImpl );
    auto result = Chimera::Status::OK;

    /*-------------------------------------------------------------------------
    Invoke the correct driver
    -------------------------------------------------------------------------*/
    if( impl->uartDriver )
    {
      result |= impl->uartDriver->flush( periph );
    }
    else
    {
      result |= impl->usartDriver->flush( periph );
    }
  }


  int Driver::write( const void *const buffer, const size_t length )
  {
    /*-------------------------------------------------------------------------
    Grab the implementation
    -------------------------------------------------------------------------*/
    RT_DBG_ASSERT( mImpl );
    auto impl = reinterpret_cast<Impl*>( mImpl );
    auto result = Chimera::Status::OK;

    /*-------------------------------------------------------------------------
    Invoke the correct driver
    -------------------------------------------------------------------------*/
    if( impl->uartDriver )
    {
      result |= impl->uartDriver->write( buffer, length );
      result |= impl->uartDriver->await( Chimera::Event::Trigger::TRIGGER_WRITE_COMPLETE, Chimera::Thread::TIMEOUT_BLOCK );
    }
    else
    {
      result |= impl->usartDriver->write( buffer, length );
      result |= impl->usartDriver->await( Chimera::Event::Trigger::TRIGGER_WRITE_COMPLETE, Chimera::Thread::TIMEOUT_BLOCK );
    }

    // TODO BMB: Need to return number of bytes actually read
    return 0;
  }


  int Driver::read( void *const buffer, const size_t length )
  {
    /*-------------------------------------------------------------------------
    Grab the implementation
    -------------------------------------------------------------------------*/
    RT_DBG_ASSERT( mImpl );
    auto impl = reinterpret_cast<Impl*>( mImpl );
    auto result = Chimera::Status::OK;

    /*-------------------------------------------------------------------------
    Invoke the correct driver
    -------------------------------------------------------------------------*/
    if( impl->uartDriver )
    {
      result |= impl->uartDriver->read( buffer, length );
      result |= impl->uartDriver->await( Chimera::Event::Trigger::TRIGGER_READ_COMPLETE, Chimera::Thread::TIMEOUT_BLOCK );
    }
    else
    {
      result |= impl->usartDriver->read( buffer, length );
      result |= impl->usartDriver->await( Chimera::Event::Trigger::TRIGGER_READ_COMPLETE, Chimera::Thread::TIMEOUT_BLOCK );
    }

    // TODO BMB: Need to return number of bytes actually read
    return 0;
  }

}  // namespace
