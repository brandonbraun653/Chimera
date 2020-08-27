/********************************************************************************
 *  File Name:
 *    chimera_gpio.cpp
 *
 *  Description:
 *    Implements Chimera GPIO
 *
 *  2020 | Brandon Braun | brandonbraun653@gmail.com
 ********************************************************************************/

/* STL Includes */
#include <memory>
#include <cstring>

/* Chimera Includes */
#include <Chimera/gpio>

namespace Chimera::GPIO
{
  static Backend::DriverConfig s_backend_driver;

  Chimera::Status_t initialize()
  {
    memset( &s_backend_driver, 0, sizeof( s_backend_driver ) );

    /*------------------------------------------------
    Register the backend interface with Chimera
    ------------------------------------------------*/
    auto result = Backend::registerDriver( s_backend_driver );
    if ( result != Chimera::Status::OK )
    {
      return result;
    }

    /*------------------------------------------------
    Try and invoke the registered init sequence
    ------------------------------------------------*/
    if ( s_backend_driver.isSupported && s_backend_driver.initialize )
    {
      return s_backend_driver.initialize();
    }

    return result;
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

  IGPIO_sPtr getDriver( const Pin pin )
  {
    if ( s_backend_driver.isSupported && s_backend_driver.getDriver )
    {
      return s_backend_driver.getDriver( pin );
    }
    else
    {
      return nullptr;
    }
  }
}  // namespace Chimera::GPIO