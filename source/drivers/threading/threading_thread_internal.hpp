/********************************************************************************
 *  File Name:
 *    threading_thread_internal.hpp
 *
 *  Description:
 *    Methods for internal use to the threading library
 *
 *  2020 | Brandon Braun | brandonbraun653@gmail.com
 *******************************************************************************/

#pragma once
#ifndef CHIMERA_THREADING_INTERNAL_HPP
#define CHIMERA_THREADING_INTERNAL_HPP

/* Chimera Includes */
#include <Chimera/common>
#include <Chimera/source/drivers/threading/threading_types.hpp>

namespace Chimera::Threading
{
  /**
   *  Registers a thread with the backend such that it can be
   *  referenced or manipulated later.
   *
   *  @param[in]  thread      The thread to copy
   *  @return ThreadId
   */
  ThreadId registerThread( Thread &&thread );

  /**
   *  Removes a thread from the registry
   *
   *  @param[in]  id    Which thread to remove
   *  @return Chimera::Status_t
   */
  Chimera::Status_t unregisterThread( const ThreadId id );

  /**
   *  Looks up the appropriate thread id given a native handle
   *
   *  @param[in]  handle      The native OS handle to the thread
   *  @return ThreadId
   */
  ThreadId getIdFromNativeHandle( detail::native_thread_handle_type handle );

}  // namespace Chimera::Threading

#endif /* !CHIMERA_THREADING_INTERNAL_HPP */
