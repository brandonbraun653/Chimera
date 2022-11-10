/********************************************************************************
 *  File Name:
 *    stl_semaphore.cpp
 *
 *  Description:
 *    Wrapper around an STL semaphore, which for now is really just a mutex
 *
 *  2021-2022 | Brandon Braun | brandonbraun653@gmail.com
 *******************************************************************************/

/*-----------------------------------------------------------------------------
Includes
-----------------------------------------------------------------------------*/
#include <Chimera/common>
#include <Chimera/thread>

#if defined( USING_NATIVE_THREADS )

namespace Chimera::Thread
{
  /*-------------------------------------------------------------------------------
  Counting Semaphore Implementation
  -------------------------------------------------------------------------------*/
  CountingSemaphore::CountingSemaphore() : mMaxCount( 1 ), mCount( 1 )
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
    RT_HARD_ASSERT( false );
  }

  void CountingSemaphore::acquire()
  {
    RT_HARD_ASSERT( false );
  }

  bool CountingSemaphore::try_acquire()
  {
    RT_HARD_ASSERT( false );
    return false;
  }

  bool CountingSemaphore::try_acquire_for( const size_t timeout )
  {
    RT_HARD_ASSERT( false );
    return false;
  }

  bool CountingSemaphore::try_acquire_until( const size_t abs_time )
  {
    RT_HARD_ASSERT( false );
    return false;
  }

  size_t CountingSemaphore::max() const
  {
    RT_HARD_ASSERT( false );
    return mMaxCount;
  }

  void CountingSemaphore::acquireFromISR()
  {
    RT_HARD_ASSERT( false );
    Chimera::insert_debug_breakpoint();
  }

  void CountingSemaphore::releaseFromISR()
  {
    RT_HARD_ASSERT( false );
    Chimera::insert_debug_breakpoint();
  }


  /*-------------------------------------------------------------------------------
  Counting Semaphore Implementation
  -------------------------------------------------------------------------------*/
  BinarySemaphore::BinarySemaphore() : mSemphr( 1 )
  {
  }

  BinarySemaphore::~BinarySemaphore()
  {
  }

  void BinarySemaphore::release( const size_t update )
  {
    mSemphr.release( update );
  }

  void BinarySemaphore::acquire()
  {
    mSemphr.acquire();
  }

  bool BinarySemaphore::try_acquire()
  {
    return mSemphr.try_acquire();
  }

  bool BinarySemaphore::try_acquire_for( const size_t timeout )
  {
    return mSemphr.try_acquire_for( std::chrono::milliseconds( timeout ) );
  }

  bool BinarySemaphore::try_acquire_until( const size_t abs_time )
  {
    auto now = std::chrono::steady_clock::now();
    return mSemphr.try_acquire_until( now + std::chrono::milliseconds( abs_time ) );
  }

  size_t BinarySemaphore::max() const
  {
    return 1;
  }

  void BinarySemaphore::acquireFromISR()
  {
    Chimera::insert_debug_breakpoint();
  }

  void BinarySemaphore::releaseFromISR()
  {
    Chimera::insert_debug_breakpoint();
  }

}  // namespace Chimera::Thread

#endif /* USING_NATIVE_THREADS */
