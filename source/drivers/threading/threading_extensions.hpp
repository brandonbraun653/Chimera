/********************************************************************************
*  File Name:
*    extensions.hpp
*
*  Description:
*    Threading extensions that implement some kind of helper objects
*
*  2020-2021 | Brandon Braun | brandonbraun653@gmail.com
********************************************************************************/

#pragma once
#ifndef CHIMERA_THREADING_EXTENSIONS_HPP
#define CHIMERA_THREADING_EXTENSIONS_HPP

/* Chimera Includes */
#include <Chimera/source/drivers/threading/threading_mutex.hpp>
#include <Chimera/source/drivers/threading/threading_abstract.hpp>

namespace Chimera::Thread
{
  template<class mutex_type>
  class LockGuard
  {
  public:
    explicit LockGuard( mutex_type &mutex ) :
        mtx( mutex )
    {
      mtx.lock();
    }

    ~LockGuard()
    {
      mtx.unlock();
    }

    LockGuard( const LockGuard & ) = delete;

  private:
    mutex_type &mtx;
  };

  template<class mutex_type>
  class TimedLockGuard
  {
  public:
    explicit TimedLockGuard( mutex_type &mutex ) : mtx( mutex ), is_locked( false )
    {
    }

    ~TimedLockGuard()
    {
      if ( is_locked )
      {
        mtx.unlock();
      }
    }

    bool try_lock_for( const size_t timeout = 100 )
    {
      is_locked = mtx.try_lock_for( timeout );
      return is_locked;
    }

  private:
    mutex_type &mtx;
    bool is_locked;
  };

  /**
   *  Variant of the Lockable interface that doesn't depend on inheritance.
   *  This helps embedded systems to consume less memory for drivers.
   */
  template<class T>
  class Lockable
  {
  public:
    void lock()
    {
      static_cast<T *>( this )->mClsMutex.lock();
    }

    void lockFromISR()
    {
      static_cast<T *>( this )->mClsMutex.lock();
    }

    bool try_lock_for( const size_t timeout )
    {
      return static_cast<T *>( this )->mClsMutex.try_lock_for( timeout );
    }

    void unlock()
    {
      static_cast<T *>( this )->mClsMutex.unlock();
    }

    void unlockFromISR()
    {
      static_cast<T *>( this )->mClsMutex.unlock();
    }

  protected:
    RecursiveTimedMutex mClsMutex;
  };
}

#endif	/* !CHIMERA_THREADING_EXTENSIONS_HPP */
