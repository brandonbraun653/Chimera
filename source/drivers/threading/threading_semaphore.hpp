/********************************************************************************
 *  File Name:
 *    semaphore.hpp
 *
 *  Description:
 *    Semaphore implementation for Chimera
 *
 *  2020-2022 | Brandon Braun | brandonbraun653@gmail.com
 ********************************************************************************/

#pragma once
#ifndef CHIMERA_THREADING_SEMAPHORE_HPP
#define CHIMERA_THREADING_SEMAPHORE_HPP

/*-----------------------------------------------------------------------------
Includes
-----------------------------------------------------------------------------*/
#include <cstdlib>
#include <Chimera/source/drivers/threading/threading_detail.hpp>

namespace Chimera::Thread
{
  /*---------------------------------------------------------------------------
  Counting Semaphore API
  ---------------------------------------------------------------------------*/
  /**
   * @brief Typical counting semaphore API, adapted for embedded systems
   *
   * When compiling with native threads, the semaphore is implemented using
   * condition variables as opposed to the std::counting_semaphore type to
   * avoid requiring templates.
   */
  class CountingSemaphore
  {
  public:
    CountingSemaphore();
    CountingSemaphore( const size_t maxCounts );
    ~CountingSemaphore();

    void   release( const size_t update = 1 );
    void   acquire();
    bool   try_acquire();
    bool   try_acquire_for( const size_t timeout );
    bool   try_acquire_until( const size_t abs_time );
    size_t max() const;
    void   acquireFromISR();
    void   releaseFromISR();

  private:
    void operator=( const CountingSemaphore & ) = delete;

    const size_t mMaxCount;
    size_t       mCount;

#if defined( USING_NATIVE_THREADS )
    std::mutex              mMutex;
    std::condition_variable mCV;
#elif defined( USING_FREERTOS_THREADS )
    detail::native_counting_semaphore mSemphr;
#endif
  };

  /*---------------------------------------------------------------------------
  Binary Semaphore API
  ---------------------------------------------------------------------------*/
  /**
   * @brief Typical binary semaphore API, adapted for embedded systems
   */
  class BinarySemaphore
  {
  public:
    BinarySemaphore();
    ~BinarySemaphore();

    void   release( const size_t update = 1 );
    void   acquire();
    bool   try_acquire();
    bool   try_acquire_for( const size_t timeout );
    bool   try_acquire_until( const size_t abs_time );
    size_t max() const;
    void   acquireFromISR();
    void   releaseFromISR();

  private:
    void operator=( const BinarySemaphore & ) = delete;

    detail::native_binary_semaphore mSemphr;
  };

}  // namespace Chimera::Thread

#endif /* !CHIMERA_THREADING_SEMAPHORE_HPP */
