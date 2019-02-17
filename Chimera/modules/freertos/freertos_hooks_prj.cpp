/********************************************************************************
 *   File Name:
 *       freertos_hooks_prj.cpp
 *
 *   Description:
 *       Implements custom callbacks into common FreeRTOS hooks
 *
 *   2019 | Brandon Braun | brandonbraun653@gmail.com
 ********************************************************************************/

#include <Chimera/chimera.hpp>
#include <Chimera/mock/delay.hpp>
#include <Chimera/modules/freertos/freertos_hooks_prj.hpp>

#ifdef SIM
#include <boost/thread.hpp>
#endif

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
    Chimera::Mock::SystemTickCallback();

    #ifdef SIM
    /*------------------------------------------------
    Allow our simulation environment to interrupt the primary
    FreeRTOS thread. This will throw a boost::thread_interrupted
    exception.
    ------------------------------------------------*/
    boost::this_thread::interruption_point();
    #endif
  }

  void vApplicationMallocFailedHook()
  {
    // TODO: Add the callback redirection
  }

  void vApplicationIdleHook()
  {
    #ifdef SIM
    /*------------------------------------------------
    Allow our simulation environment to interrupt the primary
    FreeRTOS thread. This will throw a boost::thread_interrupted
    exception.
    ------------------------------------------------*/
    boost::this_thread::interruption_point();
    #endif
  }


#ifdef __cplusplus
}
#endif
