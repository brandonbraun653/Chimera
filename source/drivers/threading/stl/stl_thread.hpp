/******************************************************************************
 *  File Name:
 *    stl_thread.hpp
 *
 *  Description:
 *    Thread class declaration for the stl variant
 *
 *  2021 | Brandon Braun | brandonbraun653@gmail.com
 *****************************************************************************/

#pragma once
#ifndef CHIMERA_THREAD_STL_HPP
#define CHIMERA_THREAD_STL_HPP

/* Chimera Includes */
#include <Chimera/common>
#include <Chimera/thread>

#if defined( USING_NATIVE_THREADS )
#include <mutex>
#include <condition_variable>

namespace Chimera::Thread
{
  /*---------------------------------------------------------------------------
  Classes
  ---------------------------------------------------------------------------*/
  /**
   *  A mostly C++ STL compatible thread class, but optimized for embedded OS environments
   *  that have more stringent requirements on thread creation due to resource limitations.
   */
  class Task : public ITask
  {
  public:
    Task();
    Task( Task &&other );
    Task( const Task & ) = delete;
    ~Task();

    TaskId start() final override;
    void suspend() final override;
    void resume() final override;
    void join() final override;
    bool joinable() final override;
    detail::native_thread_handle_type native_handle() final override;
    detail::native_thread_id native_id() final override;

  protected:
    friend bool sendTaskMsg( const TaskId, const TaskMsg, const size_t );
    friend bool this_thread::receiveTaskMsg( TaskMsg &, const size_t );
    friend TaskId this_thread::id();
    friend TaskId getIdFromNativeId( const detail::native_thread_id );

    /**
     *  Accepts a new task message, overwriting the old. This will also wake up
     *  any threads pending on receiving a message.
     *
     *  @param[in]  msg       The message to send
     *  @param[in]  timeout   How long to wait for the message to be accepted
     *  @return bool
     */
    bool acceptTaskMessage( const TaskMsg msg, const size_t timeout );

    /**
     *  Blocks the current thread to wait for a task message to arrive.
     *
     *  @param[out] msg       The received message data
     *  @param[in]  timeout   How long to wait for a new message
     *  @return bool
     */
    bool pendTaskMessage( TaskMsg &msg, const size_t timeout );

  private:
    /*-------------------------------------------------------------------------
    Task Message Data: Kept to a simple POD type as the
    signaling mechanism must use the lowest common
    denominator among supported Chimera thread systems,
    which in this case is FreeRTOS.
    -------------------------------------------------------------------------*/
    TaskMsg mTaskMsgData;                       /**< Latest task message data */
    std::mutex *mTaskMsgMutex;                  /**< Exclusive lock to prevent multiple threads sending a message */
    std::condition_variable *mTaskMsgCondition; /**< Allows pending on task messages */
    bool mTaskMsgReady;                         /**< Prevent condition variable spurious wakeup */

    /*-------------------------------------------------------------------------
    Private Helper Functions
    -------------------------------------------------------------------------*/
    void lookup_handle();
  };
}  // namespace Chimera::Thread

#endif /* USING_NATIVE_THREADS */
#endif /* !CHIMERA_THREAD_STL_HPP */
