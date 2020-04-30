/********************************************************************************
 *  File Name:
 *    chimera_watchdog.cpp
 *
 *  Description:
 *    Implements watchdog specific functionality
 *
 *  2019-2020 | Brandon Braun | brandonbraun653@gmail.com
 ********************************************************************************/

/* STL Includes */
#include <memory>
#include <cstring>

/* Chimera Includes */
#include <Chimera/thread>
#include <Chimera/watchdog>

namespace Chimera::Watchdog
{
  static Backend::DriverConfig s_backend_driver;

  Chimera::Status_t initialize()
  {
    memset( &s_backend_driver, 0, sizeof( s_backend_driver ) );

    /*------------------------------------------------
    Register the backend interface with Chimera
    ------------------------------------------------*/
    auto result = Backend::registerDriver( s_backend_driver );
    if ( result != Chimera::CommonStatusCodes::OK )
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
      return Chimera::CommonStatusCodes::NOT_SUPPORTED;
    }
  }

  Watchdog_sPtr create_shared_ptr()
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

  Watchdog_uPtr create_unique_ptr()
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

  void invokeTimeout()
  {
#if defined( USING_FREERTOS ) && ( CHIMERA_CFG_FREERTOS == 1 )
    vTaskSuspendAll();
#endif

    while ( 1 )
    {
    }
  }
}  // namespace Chimera::Watchdog
