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

namespace Chimera::Threading::detail
{
  using native_mutex = SemaphoreHandle_t;
}

#endif /* !CHIMERA_THREADING_FREERTOS_TYPES_HPP */
