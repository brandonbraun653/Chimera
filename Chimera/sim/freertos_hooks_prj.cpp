/********************************************************************************
 *  File Name:
 *    freertos_hooks_prj.cpp
 *
 *  Description:
 *    Implements Chimera simulator hooks into common FreeRTOS functions
 *
 *  2019 | Brandon Braun | brandonbraun653@gmail.com
 ********************************************************************************/

/* C++ Includes */
#include <stdexcept>
#include <string>
#include <cstring>

/* Chimera Includes */
#include <Chimera/threading.hpp>
#include <Chimera/modules/freertos/freertos_hooks.hpp>

namespace Chimera::Modules::FreeRTOS
{
  void ApplicationStackOverflowHook( TaskHandle_t xTask, char *pcTaskName )
  {
    static constexpr size_t bufferSize = 100;

    char strBuff[ bufferSize ];
    memset( strBuff, 0, bufferSize );
    snprintf( strBuff, bufferSize, "Task [%s]: stack overflow occurred\r\n", pcTaskName );

    throw std::overflow_error( strBuff );
  }

  void ApplicationTickHook()
  {
  }

  void ApplicationMallocFailedHook()
  {
    throw std::bad_alloc();
  }

  void ApplicationIdleHook()
  {
  }

}  // namespace Chimera::Modules::FreeRTOS