/********************************************************************************
 *  File Name:
 *    semaphore.hpp
 *
 *  Description:
 *    Semaphore implementation for Chimera
 *
 *  2020 | Brandon Braun | brandonbraun653@gmail.com
 ********************************************************************************/

#pragma once
#ifndef CHIMERA_THREADING_SEMAPHORE_HPP
#define CHIMERA_THREADING_SEMAPHORE_HPP

/* STL Includes */
#include <cstdlib>

/* Chimera Includes */
#include <Chimera/source/drivers/threading/threading_detail.hpp>

namespace Chimera::Thread
{
  class CountingSemaphore
  {
  public:
    CountingSemaphore();
    CountingSemaphore( const size_t maxCounts );
    ~CountingSemaphore();

    void release( const size_t update = 1 );

    void acquire();

    bool try_acquire();

    bool try_acquire_for( const size_t timeout );

    bool try_acquire_until( const size_t abs_time );

    size_t max();

    void acquireFromISR();

    void releaseFromISR();

  private:
    void operator=( const CountingSemaphore & ) = delete;

    const size_t mMaxCount;
    size_t mCount;

    detail::native_counting_semaphore mSemphr;
  };


  using BinarySemaphore = CountingSemaphore;

}  // namespace Chimera::Thread

#endif /* !CHIMERA_THREADING_SEMAPHORE_HPP */
