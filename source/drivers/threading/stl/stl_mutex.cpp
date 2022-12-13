/******************************************************************************
 *  File Name:
 *    stl_mutex.cpp
 *
 *  Description:
 *    Mutex wrapper around std::mutex
 *
 *  2021 | Brandon Braun | brandonbraun653@gmail.com
 *****************************************************************************/

/* Chimera Includes */
#include <Chimera/common>
#include <Chimera/thread>

#if defined( USING_NATIVE_THREADS )

namespace Chimera::Thread
{
  /*---------------------------------------------------------------------------
  Mutex Implementation
  ---------------------------------------------------------------------------*/
  Mutex::Mutex()
  {
  }

  Mutex::~Mutex()
  {
  }

  void Mutex::lock()
  {
    _mtx.lock();
  }

  bool Mutex::try_lock()
  {
    return _mtx.try_lock();
  }

  void Mutex::unlock()
  {
    _mtx.unlock();
  }


  /*---------------------------------------------------------------------------
  Recursive Mutex Implementation
  ---------------------------------------------------------------------------*/
  RecursiveMutex::RecursiveMutex()
  {
  }

  RecursiveMutex::~RecursiveMutex()
  {
  }

  void RecursiveMutex::lock()
  {
    _mtx.lock();
  }

  bool RecursiveMutex::try_lock()
  {
    return _mtx.try_lock();
  }

  void RecursiveMutex::unlock()
  {
    _mtx.unlock();
  }


  /*---------------------------------------------------------------------------
  Timed Mutex Implementation
  ---------------------------------------------------------------------------*/
  TimedMutex::TimedMutex()
  {
  }

  TimedMutex::~TimedMutex()
  {
  }

  void TimedMutex::lock()
  {
    _mtx.lock();
  }

  bool TimedMutex::try_lock()
  {
    return _mtx.try_lock();
  }

  bool TimedMutex::try_lock_for( const size_t timeout )
  {
    return _mtx.try_lock_for( std::chrono::milliseconds( timeout ) );
  }

  bool TimedMutex::try_lock_until( const size_t timeout )
  {
    auto now = std::chrono::steady_clock::now();
    return _mtx.try_lock_until( now + std::chrono::milliseconds( timeout ) );
  }

  void TimedMutex::unlock()
  {
    _mtx.unlock();
  }


  /*---------------------------------------------------------------------------
  Recursive Timed Mutex Implementation
  ---------------------------------------------------------------------------*/
  RecursiveTimedMutex::RecursiveTimedMutex()
  {
  }

  RecursiveTimedMutex::~RecursiveTimedMutex()
  {
  }

  void RecursiveTimedMutex::lock()
  {
    _mtx.lock();
  }

  bool RecursiveTimedMutex::try_lock()
  {
    return _mtx.try_lock();
  }

  bool RecursiveTimedMutex::try_lock_for( const size_t timeout )
  {
    return _mtx.try_lock_for( std::chrono::milliseconds( timeout ) );
  }

  bool RecursiveTimedMutex::try_lock_until( const size_t timeout )
  {
    auto now = std::chrono::steady_clock::now();
    return _mtx.try_lock_until( now + std::chrono::milliseconds( timeout ) );
  }

  void RecursiveTimedMutex::unlock()
  {
    _mtx.unlock();
  }

}  // namespace Chimera::Thread

#endif  /* USING_NATIVE_THREADS */
