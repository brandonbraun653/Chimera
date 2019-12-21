/********************************************************************************
 *  File Name:
 *    chimera_threading_stl.cpp
 *
 *  Description:
 *    Specializes the threading primitives interface for C++ STL
 *
 *  2019 | Brandon Braun | brandonbraun653@gmail.com
 ********************************************************************************/

#if defined( WIN32 ) || defined( WIN64 )

/* C++ Includes */
#include <chrono>
#include <memory>
#include <mutex>
#include <thread>

/* Chimera Includes */
#include <Chimera/threading.hpp>

namespace Chimera::Threading
{
  void yield()
  {
    std::this_thread::yield();
  }

  /*------------------------------------------------
  Recursive Mutex Specialization
  ------------------------------------------------*/
  template<>
  RecursiveMutex createSyncObject()
  {
    return new std::recursive_mutex();
  }

  template<>
  void destroySyncObject( RecursiveMutex mutex )
  {
    delete mutex;
  }

  template<>
  bool lock( RecursiveMutex mutex )
  {
    return mutex->try_lock();
  }

  template<>
  bool lockFromISR( RecursiveMutex mutex )
  {
    return mutex->try_lock();
  }

  template<>
  bool unlock( RecursiveMutex mutex )
  {
    mutex->unlock();
    return true;
  }

  template<>
  bool unlockFromISR( RecursiveMutex mutex )
  {
    mutex->unlock();
    return true;
  }


  /*------------------------------------------------
  Recursive Timed Mutex Specialization
  ------------------------------------------------*/
  template<>
  RecursiveTimedMutex createSyncObject()
  {
    return new std::recursive_timed_mutex();
  }

  template<>
  void destroySyncObject( RecursiveTimedMutex mutex )
  {
    delete mutex;
  }

  template<>
  bool lock( RecursiveTimedMutex mutex, const size_t timeout_ms )
  {
    return mutex->try_lock_for( std::chrono::milliseconds( timeout_ms ) );
  }

  template<>
  bool lockFromISR( RecursiveTimedMutex mutex )
  {
    return mutex->try_lock();
  }

  template<>
  bool unlock( RecursiveTimedMutex mutex )
  {
    mutex->unlock();
    return true;
  }

  template<>
  bool unlockFromISR( RecursiveTimedMutex mutex )
  {
    mutex->unlock();
    return true;
  }
}

#endif /* WIN32 || WIN64 */