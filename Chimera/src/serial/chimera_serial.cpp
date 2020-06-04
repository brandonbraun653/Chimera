/********************************************************************************
 *  File Name:
 *    chimera_serial.cpp
 *
 *	 Description:
 *    Implements the Chimera Serial interface
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


namespace Chimera::Serial
{
  static std::array<Serial_sPtr, static_cast<size_t>( Chimera::Serial::Channel::NUM_OPTIONS )> s_serial_shared_drivers;

  Chimera::Status_t initialize()
  {
    s_serial_shared_drivers.fill( nullptr );
    return Chimera::CommonStatusCodes::OK;
  }

  Serial_sPtr create_shared_ptr( const Chimera::Serial::Channel channel )
  {
    /*-------------------------------------------------
    Boundary check the input
    -------------------------------------------------*/
    auto idx = static_cast<size_t>( channel );
    if ( channel >= Channel::NUM_OPTIONS )
    {
      return nullptr;
    }

    /*-------------------------------------------------
    Create the driver if no one has yet
    -------------------------------------------------*/
    if ( !s_serial_shared_drivers[ idx ] )
    {
      if ( Chimera::USART::isChannelUSART( channel ) )
      {
        auto tmp = Chimera::USART::create_shared_ptr();
        s_serial_shared_drivers[ idx ] = tmp;
      }
      else if ( Chimera::UART::isChannelUART( channel ) )
      {
        auto tmp = Chimera::UART::create_shared_ptr();
        s_serial_shared_drivers[ idx ] = tmp;
      }
    }

    return s_serial_shared_drivers[ idx ];
  }

  Serial_uPtr create_unique_ptr( const Chimera::Serial::Channel channel )
  {
    if ( Chimera::USART::isChannelUSART( channel ) )
    {
      return Chimera::USART::create_unique_ptr();
    }
    else if ( Chimera::UART::isChannelUART( channel ) )
    {
      return Chimera::UART::create_unique_ptr();
    }
    else
    {
      return nullptr;
    }
  }
}