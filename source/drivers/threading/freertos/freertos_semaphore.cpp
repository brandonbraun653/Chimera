/********************************************************************************
 *  File Name:
 *    freertos_semaphore.cpp
 *
 *  Description:
 *    Chimera semaphore implementation with FreeRTOS
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
  Counting Semaphore
  -------------------------------------------------------------------------------*/
  CountingSemaphore::CountingSemaphore() : mMaxCount( 1 )
  {
    mSemphr = xSemaphoreCreateCounting( mMaxCount, mMaxCount );
  }


  CountingSemaphore::CountingSemaphore( const size_t maxCounts ) : mMaxCount( maxCounts )
  {
    mSemphr = xSemaphoreCreateCounting( mMaxCount, mMaxCount );
  }


  CountingSemaphore::~CountingSemaphore()
  {
    vSemaphoreDelete( mSemphr );
  }


  void CountingSemaphore::release( const size_t update )
  {
    if ( xTaskGetSchedulerState() == taskSCHEDULER_RUNNING )
    {
      for ( size_t x = 0; x < update; x++ )
      {
        if ( xSemaphoreGive( mSemphr ) != pdPASS )
        {
          break;
        }
      }
    }
  }


  void CountingSemaphore::acquire()
  {
    if ( xTaskGetSchedulerState() == taskSCHEDULER_RUNNING )
    {
      xSemaphoreTake( mSemphr, portMAX_DELAY );
    }
  }


  bool CountingSemaphore::try_acquire()
  {
    if ( xTaskGetSchedulerState() == taskSCHEDULER_RUNNING )
    {
      return ( xSemaphoreTake( mSemphr, 0 ) == pdPASS );
    }
    else
    {
      return true;
    }
  }


  bool CountingSemaphore::try_acquire_for( const size_t timeout )
  {
    if ( xTaskGetSchedulerState() == taskSCHEDULER_RUNNING )
    {
      size_t _t = pdMS_TO_TICKS( timeout );
      if ( timeout == Chimera::Thread::TIMEOUT_BLOCK )
      {
        _t = portMAX_DELAY;
      }

      return ( xSemaphoreTake( mSemphr, _t ) == pdPASS );
    }
    else
    {
      return true;
    }
  }


  bool CountingSemaphore::try_acquire_until( const size_t abs_time )
  {
    if ( xTaskGetSchedulerState() == taskSCHEDULER_RUNNING )
    {
      auto const currentTime = Chimera::millis();
      return ( xSemaphoreTake( mSemphr, pdMS_TO_TICKS( abs_time + currentTime ) ) == pdPASS );
    }
    else
    {
      return true;
    }
  }


  size_t CountingSemaphore::max()
  {
    return mMaxCount;
  }


  void CountingSemaphore::acquireFromISR()
  {
    if ( xTaskGetSchedulerState() != taskSCHEDULER_NOT_STARTED )
    {
      BaseType_t xHigherPriorityTaskWoken = pdFALSE;
      xSemaphoreTakeFromISR( mSemphr, &xHigherPriorityTaskWoken );
      portYIELD_FROM_ISR( xHigherPriorityTaskWoken );
    }
    else
    {
      /*-------------------------------------------------
      This shouldn't be happening. If it is, then you are
      likely running ISRs that expect the scheduler to be
      running. Investigate why you hit this point.
      -------------------------------------------------*/
      Chimera::insert_debug_breakpoint();
    }
  }


  void CountingSemaphore::releaseFromISR()
  {
    if ( xTaskGetSchedulerState() != taskSCHEDULER_NOT_STARTED )
    {
      BaseType_t xHigherPriorityTaskWoken = pdFALSE;
      xSemaphoreGiveFromISR( mSemphr, &xHigherPriorityTaskWoken );
      portYIELD_FROM_ISR( xHigherPriorityTaskWoken );
    }
    else
    {
      /*-------------------------------------------------
      This shouldn't be happening. If it is, then you are
      likely running ISRs that expect the scheduler to be
      running. Investigate why you hit this point.
      -------------------------------------------------*/
      Chimera::insert_debug_breakpoint();
    }
  }
}  // namespace Chimera::Thread

#endif /* FREERTOS */
