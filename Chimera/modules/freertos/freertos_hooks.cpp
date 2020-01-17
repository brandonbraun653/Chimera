/********************************************************************************
 *   File Name:
 *       freertos_hooks.cpp
 *
 *   Description:
 *       Implements custom callbacks into common FreeRTOS hooks
 *
 *   2019 | Brandon Braun | brandonbraun653@gmail.com
 ********************************************************************************/

#include "chimeraConfig.hpp"
#include <Chimera/chimera.hpp>
#include <Chimera/modules/freertos/freertos_hooks.hpp>

#if defined( USING_FREERTOS ) && (CHIMERA_CFG_FREERTOS == 1 )

#ifdef __cplusplus
extern "C"
{
#endif

  void vApplicationStackOverflowHook( TaskHandle_t xTask, char *pcTaskName )
  {
    Chimera::Modules::FreeRTOS::ApplicationStackOverflowHook( xTask, pcTaskName );
  }

  void vApplicationTickHook()
  {
    Chimera::Modules::FreeRTOS::ApplicationTickHook();
  }

  void vApplicationMallocFailedHook()
  {
    Chimera::Modules::FreeRTOS::ApplicationMallocFailedHook();
  }

  void vApplicationIdleHook()
  {
    Chimera::Modules::FreeRTOS::ApplicationIdleHook();
  }

#ifdef __cplusplus
}
#endif

#endif /* CHIMERA_CFG_FREERTOS */