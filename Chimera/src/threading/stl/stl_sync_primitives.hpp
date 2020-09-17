/********************************************************************************
 *  File Name:
 *    types.hpp
 *
 *  Description:
 *    STL specific thread primitive native types
 *
 *  2020 | Brandon Braun | brandonbraun653@gmail.com
 ********************************************************************************/

#ifndef CHIMERA_THREADING_STL_TYPES_HPP
#define CHIMERA_THREADING_STL_TYPES_HPP

#if defined( USING_NATIVE_THREADS )

/* STL Includes */
#include <mutex>
#include <thread>

namespace Chimera::Threading::detail
{
  using native_mutex                 = std::mutex;
  using native_recursive_mutex       = std::recursive_mutex;
  using native_timed_mutex           = std::timed_mutex;
  using native_recursive_timed_mutex = std::recursive_timed_mutex;

  /* Use mutex until compiler supports the C++20 semaphore */
  using native_binary_semaphore   = std::timed_mutex;
  using native_counting_semaphore = std::timed_mutex;

  using native_thread             = std::thread;
  using native_thread_handle_type = std::thread::native_handle_type;
}  // namespace Chimera::Threading::detail

#endif /* USING_NATIVE_THREADS */
#endif /* !CHIMERA_THREADING_STL_TYPES_HPP */
