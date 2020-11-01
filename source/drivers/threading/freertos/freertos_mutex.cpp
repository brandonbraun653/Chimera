/********************************************************************************
 *  File Name:
 *    freertos_mutex.cpp
 *
 *  Description:
 *    Chimera mutex implementation with FreeRTOS
 *
 *  2020 | Brandon Braun | brandonbraun653@gmail.com
 *******************************************************************************/

/* STL Includes */
#include <cstring>

/* Chimera Includes */
#include <Chimera/common>
#include <Chimera/system>
#include <Chimera/thread>

#if defined( USING_FREERTOS ) || defined( USING_FREERTOS_THREADS )

/* FreeRTOS Includes */
#include <FreeRTOS/FreeRTOS.h>
#include <FreeRTOS/task.h>
#include <FreeRTOS/semphr.h>

namespace Chimera::Threading
{
  /*-------------------------------------------------------------------------------
  Standard Mutex
  -------------------------------------------------------------------------------*/
  Mutex::Mutex()
  {
    _mtx = xSemaphoreCreateMutex();
  }


  Mutex::~Mutex()
  {
    vSemaphoreDelete( _mtx );
  }


  void Mutex::lock()
  {
    if ( xTaskGetSchedulerState() == taskSCHEDULER_RUNNING )
    {
      xSemaphoreTake( _mtx, portMAX_DELAY );
    }
  }


  bool Mutex::try_lock()
  {
    if ( xTaskGetSchedulerState() == taskSCHEDULER_RUNNING )
    {
      return ( xSemaphoreTake( _mtx, 0 ) == pdPASS );
    }
    else
    {
      return true;
    }
  }


  void Mutex::unlock()
  {
    if ( xTaskGetSchedulerState() == taskSCHEDULER_RUNNING )
    {
      xSemaphoreGive( _mtx );
    }
  }


  /*-------------------------------------------------------------------------------
  Recursive Mutex
  -------------------------------------------------------------------------------*/
  RecursiveMutex::RecursiveMutex()
  {
    _mtx = xSemaphoreCreateRecursiveMutex();
  }


  RecursiveMutex::~RecursiveMutex()
  {
    vSemaphoreDelete( _mtx );
  }


  void RecursiveMutex::lock()
  {
    if ( xTaskGetSchedulerState() == taskSCHEDULER_RUNNING )
    {
      xSemaphoreTakeRecursive( _mtx, portMAX_DELAY );
    }
  }


  bool RecursiveMutex::try_lock()
  {
    if ( xTaskGetSchedulerState() == taskSCHEDULER_RUNNING )
    {
      return ( xSemaphoreTakeRecursive( _mtx, 0 ) == pdPASS );
    }
    else
    {
      return true;
    }
  }


  void RecursiveMutex::unlock()
  {
    if ( xTaskGetSchedulerState() == taskSCHEDULER_RUNNING )
    {
      xSemaphoreGiveRecursive( _mtx );
    }
  }


  /*-------------------------------------------------------------------------------
  Timed Mutex
  -------------------------------------------------------------------------------*/
  TimedMutex::TimedMutex()
  {
    _mtx = xSemaphoreCreateMutex();
  }


  TimedMutex::~TimedMutex()
  {
    vSemaphoreDelete( _mtx );
  }


  void TimedMutex::lock()
  {
    if ( xTaskGetSchedulerState() == taskSCHEDULER_RUNNING )
    {
      xSemaphoreTake( _mtx, portMAX_DELAY );
    }
  }


  bool TimedMutex::try_lock()
  {
    if ( xTaskGetSchedulerState() == taskSCHEDULER_RUNNING )
    {
      return ( xSemaphoreTake( _mtx, 0 ) == pdPASS );
    }
    else
    {
      return true;
    }
  }


  bool TimedMutex::try_lock_for( const size_t timeout )
  {
    if ( xTaskGetSchedulerState() == taskSCHEDULER_RUNNING )
    {
      return ( xSemaphoreTake( _mtx, pdMS_TO_TICKS( timeout ) ) == pdPASS );
    }
    else
    {
      return true;
    }
  }


  bool TimedMutex::try_lock_until( const size_t timeout )
  {
    if ( xTaskGetSchedulerState() == taskSCHEDULER_RUNNING )
    {
      auto now = Chimera::millis();
      return ( xSemaphoreTake( _mtx, pdMS_TO_TICKS( timeout - now ) ) == pdPASS );
    }
    else
    {
      return true;
    }
  }


  void TimedMutex::unlock()
  {
    if ( xTaskGetSchedulerState() == taskSCHEDULER_RUNNING )
    {
      xSemaphoreGive( _mtx );
    }
  }


  /*-------------------------------------------------------------------------------
  Recursive Timed Mutex
  -------------------------------------------------------------------------------*/
  RecursiveTimedMutex::RecursiveTimedMutex()
  {
    _mtx = xSemaphoreCreateRecursiveMutex();
  }


  RecursiveTimedMutex::~RecursiveTimedMutex()
  {
    vSemaphoreDelete( _mtx );
  }


  void RecursiveTimedMutex::lock()
  {
    if ( xTaskGetSchedulerState() == taskSCHEDULER_RUNNING )
    {
      xSemaphoreTakeRecursive( _mtx, portMAX_DELAY );
    }
  }


  bool RecursiveTimedMutex::try_lock()
  {
    if ( xTaskGetSchedulerState() == taskSCHEDULER_RUNNING )
    {
      return ( xSemaphoreTakeRecursive( _mtx, 0 ) == pdPASS );
    }
    else
    {
      return true;
    }
  }


  bool RecursiveTimedMutex::try_lock_for( const size_t timeout )
  {
    if ( xTaskGetSchedulerState() == taskSCHEDULER_RUNNING )
    {
      return ( xSemaphoreTakeRecursive( _mtx, pdMS_TO_TICKS( timeout ) ) == pdPASS );
    }
    else
    {
      return true;
    }
  }


  bool RecursiveTimedMutex::try_lock_until( const size_t timeout )
  {
    if ( xTaskGetSchedulerState() == taskSCHEDULER_RUNNING )
    {
      auto now = Chimera::millis();
      return ( xSemaphoreTakeRecursive( _mtx, pdMS_TO_TICKS( timeout - now ) ) == pdPASS );
    }
    else
    {
      return true;
    }
  }


  void RecursiveTimedMutex::unlock()
  {
    if ( xTaskGetSchedulerState() == taskSCHEDULER_RUNNING )
    {
      xSemaphoreGiveRecursive( _mtx );
    }
  }
}  // namespace Chimera::Threading

#endif /* FREERTOS */
