/********************************************************************************
 *  File Name:
 *    chimera_watchdog.cpp
 *
 *  Description:
 *    Implements watchdog specific functionality
 *
 *  2019-2021 | Brandon Braun | brandonbraun653@gmail.com
 ********************************************************************************/

/* STL Includes */
#include <memory>
#include <cstring>

/* Chimera Includes */
#include <Chimera/thread>
#include <Chimera/watchdog>

namespace Chimera::Watchdog
{
  /*-------------------------------------------------------------------------------
  Static Data
  -------------------------------------------------------------------------------*/
  static Backend::DriverConfig s_backend_driver;


  /*-------------------------------------------------------------------------------
  Public Functions
  -------------------------------------------------------------------------------*/
  namespace Backend
  {
    Chimera::Status_t __attribute__( ( weak ) ) registerDriver( Chimera::Watchdog::Backend::DriverConfig &registry )
    {
      registry.isSupported = false;
      return Chimera::Status::NOT_SUPPORTED;
    }
  }  // namespace Backend

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


  Independent_rPtr getDriver( const IChannel channel )
  {
    if ( s_backend_driver.isSupported && s_backend_driver.getIndependentDriver )
    {
      return s_backend_driver.getIndependentDriver( channel );
    }
    else
    {
      return nullptr;
    }
  }


  Window_rPtr getDriver( const WChannel channel )
  {
    if ( s_backend_driver.isSupported && s_backend_driver.getWindowDriver )
    {
      return s_backend_driver.getWindowDriver( channel );
    }
    else
    {
      return nullptr;
    }
  }


  void invokeTimeout()
  {
    // TODO: This method should have no knowledge of FreeRTOS. Redirect to the threading module.
#if defined( USING_FREERTOS_THREADS )
    vTaskSuspendAll();
#endif

    while ( 1 )
    {
#if defined( CHIMERA_TEST )
      break;
#endif
    }
  }
}  // namespace Chimera::Watchdog
