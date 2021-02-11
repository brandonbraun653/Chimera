/********************************************************************************
 *  File Name:
 *    freertos_mutex.cpp
 *
 *  Description:
 *    Chimera mutex implementation with FreeRTOS. Note that v10.0 of the API
 *    states that recursive calls cannot happen inside ISRs (critical sections)
 *    and must be avoided. This limits the use of the Mutex class of objects to
 *    thread mode only and care must be taken to prevent a system architecture
 *    that requires access in an ISR.
 *
 *    Given that from a conceptual perspective mutexes shouldn't be used in ISRs
 *    anyways (they are thread synchronizers, not interrupt synchronizers), this
 *    shouldn't be too much to ask.
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

namespace Chimera::Thread
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
    if ( !Chimera::System::inISR() && ( xTaskGetSchedulerState() == taskSCHEDULER_RUNNING ) )
    {
      xSemaphoreTake( _mtx, portMAX_DELAY );
    }
  }


  bool Mutex::try_lock()
  {
    if ( !Chimera::System::inISR() && ( xTaskGetSchedulerState() == taskSCHEDULER_RUNNING ) )
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
    if ( !Chimera::System::inISR() && ( xTaskGetSchedulerState() == taskSCHEDULER_RUNNING ) )
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
    if ( !Chimera::System::inISR() && ( xTaskGetSchedulerState() == taskSCHEDULER_RUNNING ) )
    {
      xSemaphoreTakeRecursive( _mtx, portMAX_DELAY );
    }
  }


  bool RecursiveMutex::try_lock()
  {
    if ( !Chimera::System::inISR() && ( xTaskGetSchedulerState() == taskSCHEDULER_RUNNING ) )
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
    if ( !Chimera::System::inISR() && ( xTaskGetSchedulerState() == taskSCHEDULER_RUNNING ) )
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
    if ( !Chimera::System::inISR() && ( xTaskGetSchedulerState() == taskSCHEDULER_RUNNING ) )
    {
      xSemaphoreTake( _mtx, portMAX_DELAY );
    }
  }


  bool TimedMutex::try_lock()
  {
    if ( !Chimera::System::inISR() && ( xTaskGetSchedulerState() == taskSCHEDULER_RUNNING ) )
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
    if ( !Chimera::System::inISR() && ( xTaskGetSchedulerState() == taskSCHEDULER_RUNNING ) )
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
    if ( !Chimera::System::inISR() && ( xTaskGetSchedulerState() == taskSCHEDULER_RUNNING ) )
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
    if ( !Chimera::System::inISR() && ( xTaskGetSchedulerState() == taskSCHEDULER_RUNNING ) )
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
    if ( !Chimera::System::inISR() && ( xTaskGetSchedulerState() == taskSCHEDULER_RUNNING ) )
    {
      xSemaphoreTakeRecursive( _mtx, portMAX_DELAY );
    }
  }


  bool RecursiveTimedMutex::try_lock()
  {
    if ( !Chimera::System::inISR() && ( xTaskGetSchedulerState() == taskSCHEDULER_RUNNING ) )
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
    if ( !Chimera::System::inISR() && ( xTaskGetSchedulerState() == taskSCHEDULER_RUNNING ) )
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
    if ( !Chimera::System::inISR() && ( xTaskGetSchedulerState() == taskSCHEDULER_RUNNING ) )
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
    if ( !Chimera::System::inISR() && ( xTaskGetSchedulerState() == taskSCHEDULER_RUNNING ) )
    {
      xSemaphoreGiveRecursive( _mtx );
    }
  }
}  // namespace Chimera::Thread

#endif /* FREERTOS */
