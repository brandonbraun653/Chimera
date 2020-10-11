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

#pragma error("Missing STL queue implementation")

namespace Chimera::Threading
{
  /*-------------------------------------------------------------------------------
  Constructor/Destructor
  -------------------------------------------------------------------------------*/
  Queue::Queue( const size_t qLen, const size_t itemSize, void *qBuffer )
  {
    // Will need to figure out how to use the templates or perhaps a
    // custom allocator to allow for size definition based on the variables
    // here.
  }

  Queue::~Queue()
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
  bool Queue::push( void *element, const size_t timeout )
  {
    return false;
  }


  bool Queue::isrPush( void *element )
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
