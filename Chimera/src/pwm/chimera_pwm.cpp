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
      return Chimera::CommonStatusCodes::NOT_SUPPORTED;
    }
  }

  PWM_sPtr create_shared_ptr()
  {
    if ( s_backend_driver.isSupported && s_backend_driver.createShared )
    {
      return s_backend_driver.createShared();
    }
    else
    {
      return nullptr;
    }
  }

  PWM_uPtr create_unique_ptr()
  {
    if ( s_backend_driver.isSupported && s_backend_driver.createUnique )
    {
      return s_backend_driver.createUnique();
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
