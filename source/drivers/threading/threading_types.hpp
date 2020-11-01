/********************************************************************************
*  File Name:
*    threading_types.hpp
*
*  Description:
*    Types used in Chimera Threading
*
*  2020 | Brandon Braun | brandonbraun653@gmail.com
********************************************************************************/

#pragma once
#ifndef CHIMERA_THREADING_COMMON_TYPES_HPP
#define CHIMERA_THREADING_COMMON_TYPES_HPP

/* STL Includes */
#include <cstddef>
#include <limits>

namespace Chimera::Threading
{
  /*------------------------------------------------
  Type Aliases
  ------------------------------------------------*/
  using ThreadArg = void *;
  using ThreadFunctPtr = void ( * )( ThreadArg );

  /*------------------------------------------------
  Enumerations
  ------------------------------------------------*/
  enum class Priority
  {
    LEVEL_0, /**< Lowest priority, indicating a task that doesn't care if it runs */
    LEVEL_1,
    LEVEL_2,
    LEVEL_3,
    LEVEL_4,
    LEVEL_5, /**< Highest priority, must always run when requested */

    CRITICAL  = LEVEL_5,
    IMPORTANT = LEVEL_3,
    NORMAL    = LEVEL_2,
    LOW       = LEVEL_0,

    MINIMUM = LEVEL_0,
    MAXIMUM = LEVEL_5
  };

  /*------------------------------------------------
  Common Timeouts
  ------------------------------------------------*/
  static constexpr size_t TIMEOUT_BLOCK     = std::numeric_limits<size_t>::max();
  static constexpr size_t TIMEOUT_DONT_WAIT = 0;
  static constexpr size_t TIMEOUT_1MS       = 1;
  static constexpr size_t TIMEOUT_5MS       = 5;
  static constexpr size_t TIMEOUT_10MS      = 10;
  static constexpr size_t TIMEOUT_25MS      = 25;
  static constexpr size_t TIMEOUT_50MS      = 50;
  static constexpr size_t TIMEOUT_100MS     = 100;
  static constexpr size_t TIMEOUT_500MS     = 500;
  static constexpr size_t TIMEOUT_1S        = 1000;
  static constexpr size_t TIMEOUT_1MIN      = 60 * TIMEOUT_1S;
  static constexpr size_t TIMEOUT_1HR       = 60 * TIMEOUT_1MIN;
}

#endif	/* !CHIMERA_THREADING_COMMON_TYPES_HPP */
