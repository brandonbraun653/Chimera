/********************************************************************************
 *  File Name:
 *    mutex.hpp
 *
 *  Description:
 *    Mutex implementation with standard STL interface that works cross platform
 *    on both embedded systems and PCs.
 *
 *  2020 | Brandon Braun | brandonbraun653@gmail.com
 ********************************************************************************/

#ifndef CHIMERA_THREADING_MUTEX_HPP
#define CHIMERA_THREADING_MUTEX_HPP

/* Chimera Includes */
#include <Chimera/threading/internal_types.hpp>

namespace Chimera::Threading
{
  class Mutex
  {
  public:
    Mutex();
    ~Mutex();

    void lock();
    bool try_lock();
    void unlock();

    void operator=( const Mutex & ) = delete;

  private:
    detail::native_mutex _mtx;
  };


  class RecursiveMutex
  {
  public:
    RecursiveMutex();
    ~RecursiveMutex();

    void lock();
    bool try_lock();
    void unlock();

    void operator=( const RecursiveMutex & ) = delete;

  private:
    detail::native_recursive_mutex _mtx;
  };


  class TimedMutex
  {
  public:
    TimedMutex();
    ~TimedMutex();

    void lock();
    bool try_lock();
    bool try_lock_for( const size_t timeout );
    bool try_lock_until( const size_t timeout );
    void unlock();

    void operator=( const TimedMutex & ) = delete;

  private:
    detail::native_timed_mutex _mtx;
  };


  class RecursiveTimedMutex
  {
  public:
    RecursiveTimedMutex() ;
    ~RecursiveTimedMutex();

    void lock();
    bool try_lock();
    bool try_lock_for( const size_t timeout );
    bool try_lock_until( const size_t timeout );
    void unlock();

    void operator=( const RecursiveTimedMutex & ) = delete;

  private:
    detail::native_recursive_timed_mutex _mtx;
  };

}  // namespace Chimera::Threading

#endif /* !CHIMERA_THREADING_MUTEX_HPP */
