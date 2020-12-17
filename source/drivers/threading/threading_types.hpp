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
  /*-------------------------------------------------------------------------------
  Forward Declarations
  -------------------------------------------------------------------------------*/
  class Thread;


  /*-------------------------------------------------------------------------------
  Aliases
  -------------------------------------------------------------------------------*/
  using ThreadArg      = void *;
  using ThreadFunctPtr = void ( * )( ThreadArg );
  using ThreadId       = size_t;
  using ThreadMsg      = uint32_t;

  /*-------------------------------------------------------------------------------
  Constants
  -------------------------------------------------------------------------------*/
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

  static constexpr size_t MAX_NAME_LEN             = 16;
  static constexpr size_t MAX_REGISTERABLE_THREADS = 5;
  static constexpr ThreadId THREAD_ID_INVALID      = std::numeric_limits<ThreadId>::max();

  /*-------------------------------------------------------------------------------
  Enumerations
  -------------------------------------------------------------------------------*/
  /**
   *  Thread execution priority levels
   */
  enum class Priority
  {
    LEVEL_0, /**< Lowest priority, indicating a task that doesn't care if it runs */
    LEVEL_1,
    LEVEL_2,
    LEVEL_3,
    LEVEL_4,
    LEVEL_5, /**< Highest priority, must "always" run when requested */

    CRITICAL  = LEVEL_5,
    IMPORTANT = LEVEL_3,
    NORMAL    = LEVEL_2,
    LOW       = LEVEL_0,

    MINIMUM = LEVEL_0,
    MAXIMUM = LEVEL_5
  };

  /**
   *  Common inter-thread communication message identifiers.
   *
   *  @note Don't change to enum class type as these values should
   *        directly compare against ThreadMsg types.
   */
  enum ITCMsg : ThreadMsg
  {
    ITC_NOP = 0,     /**< Base message that means do nothing */
    ITC_WAKEUP,      /**< Wake up the thread to do some processing */
    ITC_EXIT,        /**< Instructs the task to exit */
    ITC_ISR_HANDLER, /**< Execute the ISR handler */

    ITC_USR_START = 100 /**< Begin of IDs for user messages */
  };

}  // namespace Chimera::Threading

#endif /* !CHIMERA_THREADING_COMMON_TYPES_HPP */
