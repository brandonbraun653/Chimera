/******************************************************************************
 *  File Name:
 *    threading_internal.hpp
 *
 *  Description:
 *    Methods for internal use to the threading library
 *
 *  2020-2021 | Brandon Braun | brandonbraun653@gmail.com
 *****************************************************************************/

#pragma once
#ifndef CHIMERA_THREADING_INTERNAL_HPP
#define CHIMERA_THREADING_INTERNAL_HPP

/* Chimera Includes */
#include <Chimera/common>
#include <Chimera/source/drivers/threading/threading_detail.hpp>
#include <Chimera/source/drivers/threading/threading_types.hpp>

namespace Chimera::Thread
{
  /**
   *  Registers a thread with the backend such that it can be
   *  referenced or manipulated later.
   *
   *  @param[in]  thread      The thread to copy
   *  @return TaskId
   */
  TaskId registerThread( Task &&thread );

  /**
   *  Removes a thread from the registry
   *
   *  @param[in]  id    Which thread to remove
   *  @return Chimera::Status_t
   */
  Chimera::Status_t unregisterThread( const TaskId id );

  /**
   *  Looks up the appropriate thread id given a native handle
   *
   *  @param[in]  handle      The native OS handle to the thread
   *  @return TaskId
   */
  TaskId getIdFromNativeHandle( detail::native_thread_handle_type handle );

  /**
   *  Looks up the appropriate thread id given a native id
   *
   *  @param[in]  handle      The native OS id to the thread
   *  @return TaskId
   */
  TaskId getIdFromNativeId( const detail::native_thread_id id );

}  // namespace Chimera::Thread

#endif /* !CHIMERA_THREADING_INTERNAL_HPP */
