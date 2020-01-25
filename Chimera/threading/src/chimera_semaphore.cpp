/********************************************************************************
 *  File Name:
 *    chimera_semaphore.cpp
 *
 *  Description:
 *    Implements the Chimera variant of a semaphore
 *
 *  2020 | Brandon Braun | brandonbraun653@gmail.com
 ********************************************************************************/

/* C++ Includes */
#include <cstdlib>

/* Chimera Includes */
#include <Chimera/threading/semaphore.hpp>


namespace Chimera::Threading
{
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
    mLock.lock();
    auto newVal = mCount + update;
    mCount = ( newVal <= mMaxCount) ? newVal : mMaxCount;
    mLock.unlock();
  }

  void CountingSemaphore::acquire()
  {
    bool acquired = false;
    do
    {
      mLock.lock();

      if ( mCount > 0 )
      {
        --mCount;
        acquired = true;
      }

      mLock.unlock();
    } while ( !acquired );
  }

  bool CountingSemaphore::try_acquire()
  {
    if ( mLock.try_lock() && ( mCount > 0 ) )
    {
      --mCount;
      return true;
    }

    return false;
  }

  bool CountingSemaphore::try_acquire_for( const size_t timeout )
  {
    if ( mLock.try_lock_for( timeout ) && ( mCount > 0 ) )
    {
      --mCount;
      return true;
    }

    return false;
  }

  bool CountingSemaphore::try_acquire_until( const size_t abs_time )
  {
    if ( mLock.try_lock_until( abs_time ) && ( mCount > 0 ) )
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

}  // namespace Chimera::Threading
