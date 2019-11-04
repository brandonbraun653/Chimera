/********************************************************************************
 *   File Name:
 *    chimera_watchdog.cpp
 *
 *   Description:
 *    Implements watchdog specific functionality
 *
 *   2019 | Brandon Braun | brandonbraun653@gmail.com
 ********************************************************************************/

/* Chimera Includes */
#include <Chimera/threading.hpp>
#include <Chimera/watchdog.hpp>

namespace Chimera::Watchdog
{
  void invokeTimeout()
  {
#if defined( USING_FREERTOS )
    vTaskSuspendAll();
#endif

    while ( 1 )
    {
    }
  }
}  // namespace Chimera::Watchdog
