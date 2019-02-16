/********************************************************************************
 *   File Name:
 *       freertos_hooks_prj.cpp
 *
 *   Description:
 *       Implements custom callbacks into common FreeRTOS hooks
 *
 *   2019 | Brandon Braun | brandonbraun653@gmail.com
 ********************************************************************************/

#include <Chimera/modules/freertos/freertos_hooks_prj.hpp>

#ifdef __cplusplus
extern "C"
{
#endif
/* FreeRTOS Includes */
#include "FreeRTOS.h"
#include "task.h"

  void vApplicationStackOverflowHook( TaskHandle_t xTask, char *pcTaskName )
  {
    // TODO: Add the callback redirection
  }

  void vApplicationTickHook()
  {
    // TODO: Add the callback redirection
  }

  void vApplicationMallocFailedHook()
  {
    // TODO: Add the callback redirection
  }

#ifdef __cplusplus
}
#endif