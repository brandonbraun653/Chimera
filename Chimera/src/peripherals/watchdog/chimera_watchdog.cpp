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

/* Chimera Includes */
#include "chimeraPort.hpp"
#include <Chimera/thread>
#include <Chimera/watchdog>

namespace Chimera::Watchdog
{
#if !defined( CHIMERA_INHERITED_WATCHDOG )
  using CHIMERA_INHERITED_WATCHDOG = WatchdogUnsupported;
  #warning Watchdog driver is unsupported
#endif

  static_assert( std::is_base_of<IWatchdog, CHIMERA_INHERITED_WATCHDOG>::value, "Invalid interface" );

  Chimera::Status_t initialize()
  {
    return Chimera::CommonStatusCodes::OK;
  }

  Watchdog_sPtr create_shared_ptr()
  {
    return std::make_shared<CHIMERA_INHERITED_WATCHDOG>();
  }

  Watchdog_uPtr create_unique_ptr()
  {
    return std::make_unique<CHIMERA_INHERITED_WATCHDOG>();
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
