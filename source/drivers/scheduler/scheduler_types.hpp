/********************************************************************************
 *  File Name:
 *    scheduler_types.hpp
 *
 *  Description:
 *    Chimera Scheduler Types
 *
 *  2020 | Brandon Braun | brandonbraun653@gmail.com
 *******************************************************************************/

#pragma once
#ifndef CHIMERA_SCHEDULER_TYPES_HPP
#define CHIMERA_SCHEDULER_TYPES_HPP

/* STL Includes */
#include <cstdint>

/* Chimera Includes */
#include <Chimera/function>

namespace Chimera::Scheduler
{
  /*-------------------------------------------------------------------------------
  Enumerations
  -------------------------------------------------------------------------------*/
  enum class CallType : uint8_t
  {
    ONE_SHOT,
    PERIODIC,
    PERIODIC_LIMITED,

    NUM_OPTIONS,
    UNKNOWN
  };

  enum class TimingType : uint8_t
  {
    ABSOLUTE,
    RELATIVE,

    NUM_OPTIONS,
    UNKNOWN
  };


  /*-------------------------------------------------------------------------------
  Structures
  -------------------------------------------------------------------------------*/
  struct SoftwareTimerEntry
  {
    Chimera::Function::Opaque *func; /**< Function to be invoked */
    size_t callRate;                 /**< For periodic, rate at which function should be invoked */
    size_t nextCallTime;             /**< Absolute system time when the function should be invoked next */
    size_t numCalls;                 /**< Tracks how many times the function has been called */
    size_t maxCalls;                 /**< For periodic limited, the max number of calls before expiring */
    CallType callType;               /**< What kind of timer this is */

    void clear()
    {
      func         = nullptr;
      callRate     = 0;
      nextCallTime = 0;
      numCalls     = 0;
      maxCalls     = 0;
      callType     = CallType::UNKNOWN;
    }
  };

}  // namespace Chimera::Scheduler

#endif /* !CHIMERA_SCHEDULER_TYPES_HPP */
