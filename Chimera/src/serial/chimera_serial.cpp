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
#include <memory>

/* Chimera Includes */
#include <Chimera/common>
#include <Chimera/serial>
#include <Chimera/uart>
#include <Chimera/usart>


namespace Chimera::Serial
{

  Chimera::Status_t initialize()
  {
    return Chimera::CommonStatusCodes::OK;
  }

  Serial_sPtr create_shared_ptr( const Chimera::Serial::Channel channel )
  {
    if ( Chimera::USART::isChannelUSART( channel ) )
    {
      return Chimera::USART::create_shared_ptr();
    }
    else if ( Chimera::UART::isChannelUART( channel ) )
    {
      return Chimera::UART::create_shared_ptr();
    }
    else
    {
      return nullptr;
    }
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