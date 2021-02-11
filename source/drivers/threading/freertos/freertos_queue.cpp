/********************************************************************************
 *  File Name:
 *    freertos_queue_implementation.cpp
 *
 *  Description:
 *    Queue implementation with FreeRTOS
 *
 *  2020 | Brandon Braun | brandonbraun653@gmail.com
 *******************************************************************************/

/* Chimera Includes */
#include <Chimera/common>
#include <Chimera/thread>

#if defined( USING_FREERTOS ) || defined( USING_FREERTOS_THREADS )

/* FreeRTOS Includes */
#include <FreeRTOS/FreeRTOS.h>
#include <FreeRTOS/queue.h>

namespace Chimera::Thread
{
  /*-------------------------------------------------------------------------------
  Constructor/Destructor
  -------------------------------------------------------------------------------*/
  Queue::Queue() : mMaxSize( 0 )
  {
  }

  Queue::~Queue()
  {
    vQueueDelete( mQueueHandle );
  }

  bool Queue::create(const size_t qLen, const size_t itemSize, void *qBuffer )
  {
    mMaxSize = qLen;
    mQueueHandle = xQueueCreateStatic( qLen, itemSize, reinterpret_cast<uint8_t *>( qBuffer ), &mQueueStructure );
    return true;
  }

  /*-------------------------------------------------------------------------------
  Capacity Methods
  -------------------------------------------------------------------------------*/
  bool Queue::empty()
  {
    return ( 0 == uxQueueMessagesWaiting( mQueueHandle ) );
  }


  bool Queue::isrEmpty() const
  {
    return ( pdTRUE == xQueueIsQueueEmptyFromISR( mQueueHandle ) );
  }


  bool Queue::full()
  {
    return ( mMaxSize == uxQueueMessagesWaiting( mQueueHandle ) );
  }


  bool Queue::isrFull() const
  {
    return ( pdTRUE == xQueueIsQueueFullFromISR( mQueueHandle ) );
  }


  size_t Queue::size()
  {
    return static_cast<size_t>( uxQueueMessagesWaiting( mQueueHandle ) );
  }


  size_t Queue::isrSize() const
  {
    return static_cast<size_t>( uxQueueMessagesWaitingFromISR( mQueueHandle ) );
  }


  size_t Queue::available()
  {
    return static_cast<size_t>( uxQueueSpacesAvailable( mQueueHandle ) );
  }


  size_t Queue::max_size() const
  {
    return mMaxSize;
  }

  /*-------------------------------------------------------------------------------
  Modifier Methods
  -------------------------------------------------------------------------------*/
  bool Queue::push( const void *element, const size_t timeout )
  {
    return ( pdPASS == xQueueSendToBack( mQueueHandle, element, pdMS_TO_TICKS( timeout ) ) );
  }


  bool Queue::isrPush( const void *element )
  {
    return ( pdPASS == xQueueSendToBackFromISR( mQueueHandle, element, nullptr) );
  }


  bool Queue::pop( void *element, const size_t timeout )
  {
    return ( pdPASS == xQueueReceive( mQueueHandle, element, pdMS_TO_TICKS( timeout ) ) );
  }


  bool Queue::isrPop( void *element )
  {
    return ( pdPASS == xQueueReceiveFromISR( mQueueHandle, element, nullptr ) );
  }


  bool Queue::peek( void *element, const size_t timeout )
  {
    return ( pdPASS == xQueuePeek( mQueueHandle, element, pdMS_TO_TICKS( timeout ) ) );
  }


  bool Queue::isrPeek( void *element )
  {
    return ( pdPASS == xQueuePeekFromISR( mQueueHandle, element ) );
  }


  bool Queue::clear( const size_t timeout )
  {
    /*-------------------------------------------------
    If we can obtain a lock, the reset functionality
    will always work. See FreeRTOS documenation of this
    function for more details.
    -------------------------------------------------*/
    if( try_lock_for( timeout ) )
    {
      xQueueReset( mQueueHandle );
      unlock();
      return true;
    }

    return false;
  }

  /*-------------------------------------------------------------------------------
  Internal Data
  -------------------------------------------------------------------------------*/
  detail::native_queue& Queue::getHandle()
  {
    return mQueueHandle;
  }

}  // namespace Chimera::Thread

#endif /* USING_FREERTOS || USING_FREERTOS_THREADS */
