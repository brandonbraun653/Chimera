/********************************************************************************
 *  File Name:
 *    chimera_timer.cpp
 *
 *	 Description:
 *    Chimera timer implementation
 *
 *  2020 | Brandon Braun | brandonbraun653@gmail.com
 *******************************************************************************/

/* STL Includes */
#include <cstdlib>
#include <cstring>

/* Chimera Includes */
#include <Chimera/common>
#include <Chimera/timer>

namespace Chimera::Timer
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

  ITimer_sPtr create_shared_ptr()
  {
    if ( s_backend_driver.isSupported && s_backend_driver.create_shared_ptr )
    {
      return s_backend_driver.create_shared_ptr();
    }
    else
    {
      return nullptr;
    }
  }

  ITimer_uPtr create_unique_ptr()
  {
    if ( s_backend_driver.isSupported && s_backend_driver.create_unique_ptr )
    {
      return s_backend_driver.create_unique_ptr();
    }
    else
    {
      return nullptr;
    }
  }

  size_t millis()
  {
    if ( s_backend_driver.isSupported && s_backend_driver.millis )
    {
      return s_backend_driver.millis();
    }
    else
    {
      return 0;
    }
  }

  void delayMilliseconds( const size_t val )
  {
    if ( s_backend_driver.isSupported && s_backend_driver.delayMilliseconds )
    {
      s_backend_driver.delayMilliseconds( val );
    }
  }

  void delayMicroseconds( const size_t val )
  {
    if ( s_backend_driver.isSupported && s_backend_driver.delayMicroseconds )
    {
      s_backend_driver.delayMicroseconds( val );
    }
  }
}