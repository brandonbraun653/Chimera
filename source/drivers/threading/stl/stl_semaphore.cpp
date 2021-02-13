/********************************************************************************
 *  File Name:
 *    stl_semaphore.cpp
 *
 *  Description:
 *    Wrapper around an STL semaphore, which for now is really just a mutex
 *
 *  2021 | Brandon Braun | brandonbraun653@gmail.com
 *******************************************************************************/

/* Chimera Includes */
#include <Chimera/common>
#include <Chimera/thread>

#if defined( USING_NATIVE_THREADS )

namespace Chimera::Thread
{
  /*-------------------------------------------------------------------------------
  Counting Semaphore Implementation
  -------------------------------------------------------------------------------*/
  CountingSemaphore::CountingSemaphore() : mMaxCount( 1 ), mCount( 1 ), mSemphr()
  {
  }

  CountingSemaphore::CountingSemaphore( const size_t maxCounts ) : mMaxCount( maxCounts ), mCount( maxCounts )
  {
  }

  CountingSemaphore::~CountingSemaphore()
  {
  }

  void CountingSemaphore::release( const size_t update )
  {
    mSemphr.lock();
    auto newVal = mCount + update;
    mCount      = ( newVal <= mMaxCount ) ? newVal : mMaxCount;
    mSemphr.unlock();
  }

  void CountingSemaphore::acquire()
  {
    mSemphr.lock();

    if ( mCount > 0 )
    {
      --mCount;
    }
  }

  bool CountingSemaphore::try_acquire()
  {
    if ( mSemphr.try_lock() && ( mCount > 0 ) )
    {
      --mCount;
      return true;
    }

    return false;
  }

  bool CountingSemaphore::try_acquire_for( const size_t timeout )
  {
    if ( mSemphr.try_lock_for( std::chrono::milliseconds( timeout ) ) && ( mCount > 0 ) )
    {
      --mCount;
      return true;
    }

    return false;
  }

  bool CountingSemaphore::try_acquire_until( const size_t abs_time )
  {
    auto now = std::chrono::steady_clock::now();
    if ( mSemphr.try_lock_until( now + std::chrono::milliseconds( abs_time ) ) && ( mCount > 0 ) )
    {
      --mCount;
      return true;
    }

    return false;
  }

  size_t CountingSemaphore::max()
  {
    return mMaxCount;
  }

  void CountingSemaphore::acquireFromISR()
  {
    Chimera::insert_debug_breakpoint();
  }

  void CountingSemaphore::releaseFromISR()
  {
    Chimera::insert_debug_breakpoint();
  }
}  // namespace Chimera::Thread

#endif /* USING_NATIVE_THREADS */
