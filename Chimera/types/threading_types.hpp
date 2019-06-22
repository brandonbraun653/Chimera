/********************************************************************************
 *   File Name:
 *    threading_types.hpp
 *
 *   Description:
 *    Chimera Threading types
 *
 *   2019 | Brandon Braun | brandonbraun653@gmail.com
 ********************************************************************************/

#pragma once
#ifndef CHIMERA_THREADING_TYPES_HPP
#define CHIMERA_THREADING_TYPES_HPP

/* C++ Includes */
#include <cstdint>
#include <memory>

/* Chimera Includes */
#include <Chimera/types/common_types.hpp>

/* FreeRTOS Includes */
#ifdef USING_FREERTOS
#ifdef __cplusplus
extern "C"
{
#include "semphr.h"
}
#endif /* __cplusplus */
#endif /* USING_FREERTOS */

namespace Chimera::Threading
{
  using RecursiveMutex_t = SemaphoreHandle_t;

}  // namespace Chimera

#endif /* !CHIMERA_WATCHDOG_TYPES_HPP */