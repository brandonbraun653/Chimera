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
#if defined( WIN32 ) || defined( WIN64 )
#include <mutex>
#elif defined( USING_FREERTOS )
#include "FreeRTOS.h"
#include "semphr.h"
#endif

namespace Chimera::Threading
{
#if defined( WIN32 ) || defined( WIN64 )
  using RecursiveMutex = ::std::recursive_mutex*;
  using RecursiveTimedMutex = ::std::recursive_timed_mutex*;

#elif defined( USING_FREERTOS )

  using RecursiveMutex = SemaphoreHandle_t;
  using RecursiveTimedMutex = SemaphoreHandle_t;
#endif
}  // namespace Chimera

#endif /* !CHIMERA_THREADING_TYPES_HPP */