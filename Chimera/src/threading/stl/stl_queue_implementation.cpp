/********************************************************************************
 *  File Name:
 *    stl_queue_implementation.cpp
 *
 *  Description:
 *    Queue implementation with STL
 *
 *  2020 | Brandon Braun | brandonbraun653@gmail.com
 *******************************************************************************/

/* Chimera Includes */
#include <Chimera/common>
#include <Chimera/thread>

#if defined( USING_WINDOWS ) || defined( USING_LINUX ) || defined( USING_NATIVE_THREADS )

#pragma warning("Queue definition exists but isn't functional")
namespace Chimera::Threading
{
  /*-------------------------------------------------------------------------------
  Constructor/Destructor
  -------------------------------------------------------------------------------*/
  Queue::Queue()
  {
  }

  Queue::~Queue()
  {

  }

  bool create( const size_t qLen, const size_t itemSize, void *qBuffer )
  {

  }

  /*-------------------------------------------------------------------------------
  Capacity Methods
  -------------------------------------------------------------------------------*/
  bool Queue::empty()
  {
    return false;
  }


  bool Queue::isrEmpty() const
  {
    return false;
  }


  bool Queue::full()
  {
    return false;
  }


  bool Queue::isrFull() const
  {
    return false;
  }


  size_t Queue::size()
  {
    return false;
  }


  size_t Queue::isrSize() const
  {
    return false;
  }


  size_t Queue::available()
  {
    return false;
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
    return false;
  }


  bool Queue::isrPush( const void *element )
  {
    return false;
  }


  bool Queue::pop( void *element, const size_t timeout )
  {
    return false;
  }


  bool Queue::isrPop( void *element )
  {
    return false;
  }


  bool Queue::peek( void *element, const size_t timeout )
  {
    return false;
  }


  bool Queue::isrPeek( void *element )
  {
    return false;
  }


  bool Queue::clear( const size_t timeout )
  {
    return false;
  }

  /*-------------------------------------------------------------------------------
  Internal Data
  -------------------------------------------------------------------------------*/
  detail::native_queue& Queue::getHandle()
  {
    return mQueueHandle;
  }

}  // namespace Chimera::Threading

#endif /* USING_FREERTOS || USING_FREERTOS_THREADS */
