/******************************************************************************
 *  File Name:
 *    types.hpp
 *
 *  Description:
 *    STL specific thread primitive native types
 *
 *  2020-2022 | Brandon Braun | brandonbraun653@gmail.com
 *****************************************************************************/

#ifndef CHIMERA_THREADING_STL_TYPES_HPP
#define CHIMERA_THREADING_STL_TYPES_HPP

#if defined( USING_NATIVE_THREADS )

/*-----------------------------------------------------------------------------
Includes
-----------------------------------------------------------------------------*/
#include <condition_variable>
#include <cstdint>
#include <limits>
#include <mutex>
#include <queue>
#include <thread>

/*-----------------------------------------------------------------------------
Macros
-----------------------------------------------------------------------------*/
#define STACK_BYTES( x ) ( x )

namespace Chimera::Thread::detail
{
  /*---------------------------------------------------------------------------
  Constants
  ---------------------------------------------------------------------------*/
  static constexpr uint32_t THREAD_MAX_PRIORITY = std::numeric_limits<uint32_t>::max();
  static constexpr uint32_t THREAD_MIN_PRIORITY = std::numeric_limits<uint32_t>::min();

  /*---------------------------------------------------------------------------
  Mutex Types
  ---------------------------------------------------------------------------*/
  using native_mutex                 = std::mutex;
  using native_recursive_mutex       = std::recursive_mutex;
  using native_timed_mutex           = std::timed_mutex;
  using native_recursive_timed_mutex = std::recursive_timed_mutex;

  /*---------------------------------------------------------------------------
  Semaphore Types
  ---------------------------------------------------------------------------*/
  using native_binary_semaphore   = std::binary_semaphore;

  /*---------------------------------------------------------------------------
  Thread Types
  ---------------------------------------------------------------------------*/
  using native_thread             = std::thread;
  using native_thread_id          = std::thread::id;
  using native_thread_handle_type = std::thread::native_handle_type;

  /*---------------------------------------------------------------------------
  Queue Types
  ---------------------------------------------------------------------------*/
  /*-------------------------------------------------
  FreeRTOS requires a structure for static allocation
  of a queue, but the STL has no concept of this. Use
  a dummy structure to keep the Queue class happy.
  -------------------------------------------------*/
  struct _DummyQueueStructure
  {
    uint8_t placeholder;
  };

  using native_queue           = void*;
  using native_queue_structure = _DummyQueueStructure;
}  // namespace Chimera::Thread::detail

#endif /* USING_NATIVE_THREADS */
#endif /* !CHIMERA_THREADING_STL_TYPES_HPP */
