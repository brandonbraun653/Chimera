/********************************************************************************
 *  File Name:
 *    chimera_threading_freertos.cpp
 *
 *  Description:
 *    Specializes the threading primitives interface for FreeRTOS
*
*  2019 | Brandon Braun | brandonbraun653@gmail.com
********************************************************************************/

/* Chimera Includes */
#include <Chimera/chimera.hpp>
#include <Chimera/threading.hpp>


#if defined( USING_FREERTOS ) && ( CHIMERA_CFG_FREERTOS == 1 )

/* FreeRTOS Includes */
#ifdef __cplusplus
extern "C"
{
#include "FreeRTOS.h"
#include "task.h"
#include "queue.h"
#include "semphr.h"
}
#endif /* __cplusplus */

namespace Chimera::Threading
{
  void yield()
  {
    taskYIELD();
  }

  /*------------------------------------------------
  Recursive Mutex Specialization
  ------------------------------------------------*/
  template<>
  RecursiveMutex createSyncObject()
  {
    return xSemaphoreCreateRecursiveMutex();
  }

  template<>
  void destroySyncObject( RecursiveMutex mutex )
  {
    vPortFree( mutex );
  }

  template<>
  bool lock( RecursiveMutex mutex )
  {
    return ( xSemaphoreTakeRecursive( mutex, portMAX_DELAY ) == pdPASS );
  }

  template<>
  bool lockFromISR( RecursiveMutex mutex )
  {
    return ( xSemaphoreTakeFromISR( mutex, nullptr ) == pdPASS );
  }

  template<>
  bool unlock( RecursiveMutex mutex )
  {
    return ( xSemaphoreGiveRecursive( mutex ) == pdPASS );
  }

  template<>
  bool unlockFromISR( RecursiveMutex mutex )
  {
    return ( xSemaphoreGiveFromISR( mutex, nullptr) == pdPASS );
  }


  /*------------------------------------------------
  Recursive Timed Mutex Specialization:
  The underlying type of RecursiveTimedMutex == RecursiveMutex for 
  FreeRTOS, so we only need to specialize this one instance.
  ------------------------------------------------*/
  template<>
  bool lock( RecursiveTimedMutex mutex, const size_t timeout_ms )
  {
    return ( xSemaphoreTakeRecursive( mutex, pdMS_TO_TICKS( timeout_ms ) ) == pdPASS );
  }
}

#endif /* CHIMERA_CFG_FREERTOS */
