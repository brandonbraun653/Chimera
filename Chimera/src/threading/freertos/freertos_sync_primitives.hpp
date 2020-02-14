/********************************************************************************
 *  File Name:
 *    types.hpp
 *
 *  Description:
 *    FreeRTOS thread primitive native types
 *
 *  2020 | Brandon Braun | brandonbraun653@gmail.com
 ********************************************************************************/

#ifndef CHIMERA_THREADING_FREERTOS_TYPES_HPP
#define CHIMERA_THREADING_FREERTOS_TYPES_HPP

/* FreeRTOS Includes */
#include "FreeRTOS.h"
#include "semphr.h"
#include "task.h"

namespace Chimera::Threading::detail
{
  using native_mutex                 = SemaphoreHandle_t;
  using native_recursive_mutex       = SemaphoreHandle_t;
  using native_timed_mutex           = SemaphoreHandle_t;
  using native_recursive_timed_mutex = SemaphoreHandle_t;

  using native_binary_semaphore   = SemaphoreHandle_t;
  using native_counting_semaphore = SemaphoreHandle_t;

  using native_thread             = TaskHandle_t;
  using native_thread_handle_type = TaskHandle_t;

}  // namespace Chimera::Threading::detail

#endif /* !CHIMERA_THREADING_FREERTOS_TYPES_HPP */
