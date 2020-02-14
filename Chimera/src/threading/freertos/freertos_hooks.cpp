/********************************************************************************
 *  File Name:
 *    freertos_hooks.cpp
 *
 *  Description:
 *    Implements custom callbacks into common FreeRTOS hooks. It is expected
 *    that the Chimera backend driver will implement the below functions.
 *
 *  2019-2020 | Brandon Braun | brandonbraun653@gmail.com
 ********************************************************************************/

#include "chimeraConfig.hpp"
#include <Chimera/chimera.hpp>
#include <Chimera/threading/freertos/freertos_hooks.hpp>

#if defined( USING_FREERTOS ) && ( CHIMERA_CFG_FREERTOS == 1 )

#ifdef __cplusplus
extern "C"
{
#endif

  void vApplicationStackOverflowHook( TaskHandle_t xTask, char *pcTaskName )
  {
    Chimera::Threading::FreeRTOS::ApplicationStackOverflowHook( xTask, pcTaskName );
  }

  void vApplicationTickHook()
  {
    Chimera::Threading::FreeRTOS::ApplicationTickHook();
  }

  void vApplicationMallocFailedHook()
  {
    Chimera::Threading::FreeRTOS::ApplicationMallocFailedHook();
  }

  void vApplicationIdleHook()
  {
    Chimera::Threading::FreeRTOS::ApplicationIdleHook();
  }

#ifdef __cplusplus
}
#endif

#endif /* CHIMERA_CFG_FREERTOS */
