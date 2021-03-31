/********************************************************************************
 *  File Name:
 *    freertos_hooks_prj.hpp
 *
 *  Description:
 *    Allows the user to register various functions as callbacks into common
 *    FreeRTOS hooks. It is expected that the backend driver will provide the
 *    definition of these functions.
 *
 *  2019-2020 | Brandon Braun | brandonbraun653@gmail.com
 ********************************************************************************/
#pragma once
#ifndef CHIMERA_FREERTOS_HOOKS_PRJ_HPP
#define CHIMERA_FREERTOS_HOOKS_PRJ_HPP

/* Chimera Includes */
#include <Chimera/common>

#if defined( USING_FREERTOS ) || defined( USING_FREERTOS_THREADS )

/* FreeRTOS Includes */
#include "FreeRTOS.h"
#include "task.h"

namespace Chimera::Thread::FreeRTOS
{
  /**
   *  Custom callback that executes when FreeRTOS detects that a stack
   *  overflow condition has occurred.
   *
   *  @param[in]  xTask         Handle to the task which had the overflow
   *  @param[in]  pcTaskName    The user friendly name of the task
   *  @return void
   */
  extern void ApplicationStackOverflowHook( TaskHandle_t xTask, char *pcTaskName );

  /**
   *  Custom callback that executes when the FreeRTOS task scheduler
   *  periodic interrupt fires.
   *
   *  @note Due to the likely high frequency of this call, keep it short
   *
   *  @return void
   */
  extern void ApplicationTickHook();

  /**
   *  Custom callback that executes when the FreeRTOS memory allocator could
   *  not allocate a requested amount of memory.
   *
   *  @return void
   */
  extern void ApplicationMallocFailedHook();

  /**
   *  Custom callback that executes when the FreeRTOS task scheduler places
   *  the idle task into the running state.
   *
   *  @return void
   */
  extern void ApplicationIdleHook();

}  // namespace Chimera::Thread::FreeRTOS
#endif /* CHIMERA_CFG_FREERTOS */

#endif /* !CHIMERA_FREERTOS_HOOKS_PRJ_HPP */
