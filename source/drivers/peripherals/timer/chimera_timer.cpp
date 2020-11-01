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
  /*-------------------------------------------------------------------------------
  Static Data
  -------------------------------------------------------------------------------*/
  static Backend::DriverConfig s_backend_driver;

  /*-------------------------------------------------------------------------------
  Public Functions
  -------------------------------------------------------------------------------*/
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
    else
    {
      return Chimera::Status::NOT_SUPPORTED;
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


  Driver_sPtr getDriver( const Chimera::Timer::Peripheral peripheral )
  {
    if ( s_backend_driver.isSupported && s_backend_driver.getDriver )
    {
      return s_backend_driver.getDriver( peripheral );
    }
    else
    {
      return nullptr;
    }
  }

}  // namespace Chimera::Timer
