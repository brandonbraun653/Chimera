/********************************************************************************
 *  File Name:
 *    freertos_thread.hpp
 *
 *  Description:
 *    Thread class declaration for the FreeRTOS variant
 *
 *  2021 | Brandon Braun | brandonbraun653@gmail.com
 *******************************************************************************/

#pragma once
#ifndef CHIMERA_THREAD_FREERTOS_HPP
#define CHIMERA_THREAD_FREERTOS_HPP

/* Chimera Includes */
#include <Chimera/common>
#include <Chimera/thread>

namespace Chimera::Thread
{
  /*-------------------------------------------------------------------------------
  Classes
  -------------------------------------------------------------------------------*/
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

    [[deprecated]] void initialize( TaskFuncPtr func, TaskArg arg, const Priority priority, const size_t stackWords,
                                    const std::string_view name ) final override;
    [[deprecated]] void initialize( TaskDelegate func, TaskArg arg, const Priority priority, const size_t stackWords,
                                    const std::string_view name ) final override;
    TaskId start() final override;
    void suspend() final override;
    void resume() final override;
    void join() final override;
    bool joinable() final override;
    detail::native_thread_handle_type native_handle() final override;
    detail::native_thread_id native_id() final override;

  private:
    void lookup_handle();
    int startStatic();
    int startDynamic();
    int startRestricted();
  };
}  // namespace Chimera::Thread

#endif /* !CHIMERA_THREAD_FREERTOS_HPP */
