/******************************************************************************
 *  File Name:
 *    freertos_hooks.cpp
 *
 *  Description:
 *    Implements custom callbacks into common FreeRTOS hooks. It is expected
 *    that the Chimera backend driver will implement the below functions.
 *
 *  2019-2020 | Brandon Braun | brandonbraun653@gmail.com
 *****************************************************************************/

#include <Chimera/common>
#include <Chimera/thread>

#if defined( USING_FREERTOS_THREADS )

#ifdef __cplusplus
extern "C"
{
#endif

  void vApplicationStackOverflowHook( TaskHandle_t xTask, char *pcTaskName )
  {
    Chimera::Thread::FreeRTOS::ApplicationStackOverflowHook( xTask, pcTaskName );
  }

  void vApplicationTickHook()
  {
    Chimera::Thread::FreeRTOS::ApplicationTickHook();
  }

  void vApplicationMallocFailedHook()
  {
    Chimera::Thread::FreeRTOS::ApplicationMallocFailedHook();
  }

  void vApplicationIdleHook()
  {
    Chimera::Thread::FreeRTOS::ApplicationIdleHook();
  }

#if( configSUPPORT_STATIC_ALLOCATION == 1 )
  void vApplicationGetTimerTaskMemory( StaticTask_t **ppxTimerTaskTCBBuffer, StackType_t **ppxTimerTaskStackBuffer, uint32_t *pulTimerTaskStackSize )
  {
    static StaticTask_t timerTCB;
    static StackType_t timerStack[ 64 ];

    *ppxTimerTaskTCBBuffer = &timerTCB;
    *ppxTimerTaskStackBuffer = timerStack;
    *pulTimerTaskStackSize   = ARRAY_COUNT( timerStack );
  }

  void vApplicationGetIdleTaskMemory( StaticTask_t **ppxIdleTaskTCBBuffer, StackType_t **ppxIdleTaskStackBuffer, uint32_t *pulIdleTaskStackSize )
  {
    static StaticTask_t idleTCB;
    static StackType_t idleStack[ 64 ];

    *ppxIdleTaskTCBBuffer = &idleTCB;
    *ppxIdleTaskStackBuffer = idleStack;
    *pulIdleTaskStackSize   = ARRAY_COUNT( idleStack );
  }
#endif

#ifdef __cplusplus
}
#endif

#endif /* CHIMERA_CFG_FREERTOS */
