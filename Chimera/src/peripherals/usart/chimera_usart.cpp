/********************************************************************************
 *  File Name:
 *    chimera_usart.cpp
 *
 *	 Description:
 *    Implements the Chimera USART driver
 *
 *  2020 | Brandon Braun | brandonbraun653@gmail.com
 *******************************************************************************/

/* STL Includes */
#include <memory>
#include <cstring>

/* Chimera Includes */
#include <Chimera/usart>

namespace Chimera::USART
{
  static Backend::DriverConfig s_backend_driver;

  Chimera::Status_t initialize()
  {
    memset( &s_backend_driver, 0, sizeof( s_backend_driver ) );
    return Backend::registerDriver( s_backend_driver );
  }

  Chimera::Status_t reset()
  {
    if ( s_backend_driver.isSupported && s_backend_driver.reset )
    {
      return s_backend_driver.reset();
    }
    else
    {
      return Chimera::Status::NOT_SUPPORTED;
    }
  }

  bool isChannelUSART( const Chimera::Serial::Channel channel )
  {
    if ( s_backend_driver.isSupported && s_backend_driver.isChannelUSART )
    {
      return s_backend_driver.isChannelUSART( channel );
    }
    else
    {
      return false;
    }
  }

  IUSART_sPtr getDriver( const Chimera::Serial::Channel channel )
  {
    if ( s_backend_driver.isSupported && s_backend_driver.getDriver )
    {
      return s_backend_driver.getDriver( channel );
    }
    else
    {
      return nullptr;
    }
  }
}  // namespace Chimera::USART
