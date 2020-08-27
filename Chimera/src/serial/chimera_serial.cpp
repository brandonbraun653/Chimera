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
  Chimera::Status_t initialize()
  {
    return Chimera::Status::OK;
  }

  Chimera::Status_t reset()
  {
    return Chimera::Status::OK;
  }

  ISerial_sPtr getDriver( const Channel channel )
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
    if ( Chimera::USART::isChannelUSART( channel ) )
    {
      return Chimera::USART::getDriver( channel );
    }
    else if ( Chimera::UART::isChannelUART( channel ) )
    {
      return Chimera::UART::getDriver( channel );
    }
    else
    {
      return nullptr;
    }
  }
}  // namespace Chimera::Serial
