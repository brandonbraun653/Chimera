/********************************************************************************
 *  File Name:
 *    chimera_pwm.cpp
 *
 *  Description:
 *    Driver registration for the PWM module
 *
 *  2020 | Brandon Braun | brandonbraun653@gmail.com
 *******************************************************************************/

/* STL Includes */
#include <cstring>

/* Chimera Includes */
#include <Chimera/pwm>

namespace Chimera::PWM
{
  static Backend::DriverRegistration s_backend_driver;

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

  IPWM_sPtr getDriver( const size_t channel )
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

  size_t numSupportedChannels()
  {
    if ( s_backend_driver.isSupported && s_backend_driver.numSupportedChannels )
    {
      return s_backend_driver.numSupportedChannels();
    }
    else
    {
      return 0;
    }
  }
}
