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
#include <limits>

/* Chimera Includes */
#include <Chimera/cfg>
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

  size_t millis()
  {
    if ( s_backend_driver.isSupported && s_backend_driver.millis )
    {
      return s_backend_driver.millis();
    }
    else
    {
      // Preemptively prevent accidental div/0 errors by returning an unlikely positive value
      return std::numeric_limits<size_t>::max();
    }
  }

  size_t micros()
  {
    if ( s_backend_driver.isSupported && s_backend_driver.millis )
    {
      return s_backend_driver.millis();
    }
    else
    {
      // Preemptively prevent accidental div/0 errors by returning an unlikely positive value
      return std::numeric_limits<size_t>::max();
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

  ITimer_sPtr createSharedInstance( const Chimera::Timer::Peripheral peripheral )
  {
    if ( s_backend_driver.isSupported && s_backend_driver.createSharedInstance )
    {
      return s_backend_driver.createSharedInstance( peripheral );
    }
    else
    {
      return nullptr;
    }
  }

  ITimer_uPtr createUniqueInstance( const Chimera::Timer::Peripheral peripheral )
  {
    if constexpr( Chimera::Config::DriverInfiniteLifetime )
    {
      /*-------------------------------------------------
      Creating a unique pointer doesn't make sense in this case
      because the back end will forever own the driver instance.
      -------------------------------------------------*/
      return nullptr;
    }
    else
    {
      if ( s_backend_driver.isSupported && s_backend_driver.createUniqueInstance )
      {
        return s_backend_driver.createUniqueInstance( peripheral );
      }
      else
      {
        return nullptr;
      }
    }
  }

  ITimer_rPtr createUnsafeInstance( const Chimera::Timer::Peripheral peripheral )
  {
    if constexpr ( Chimera::Config::DriverInfiniteLifetime )
    {
      if ( s_backend_driver.isSupported && s_backend_driver.createUnsafeInstance )
      {
        return s_backend_driver.createUnsafeInstance( peripheral );
      }
      else
      {
        return nullptr;
      }
    }
    else
    {
      return nullptr;
    }
  }
}  // namespace Chimera::Timer
