/********************************************************************************
 *  File Name:
 *    scheduler_user.hpp
 *
 *  Description:
 *    Provides access to resources that allow scheduling of function calls at
 *    some point in the future.
 *
 *  2020 | Brandon Braun | brandonbraun653@gmail.com
 *******************************************************************************/

#pragma once
#ifndef CHIMERA_SCHEDULER_USER_HPP
#define CHIMERA_SCHEDULER_USER_HPP

/* Chimera Includes */
#include <Chimera/common>
#include <Chimera/source/drivers/function/function_types.hpp>
#include <Chimera/source/drivers/scheduler/scheduler_types.hpp>


namespace Chimera::Scheduler
{
  /*-------------------------------------------------------------------------------
  Low Resolution Scheduler (Millisecond timing)

  This module provides an interface for the user to schedule function calls
  with a software timer. The minimum scheduler period is going to be system
  dependent, so it's only recommended to use this module for relatively low
  frequency calls.

  The actual method invocations are performed inside a system thread, so they
  don't need to be exceedingly short and they are able to take advantage of
  other system resources without issue.

  Note that depending on how many callbacks registered and their execution time,
  the timing of each callback may not execute exactly as configured.
  -------------------------------------------------------------------------------*/
  namespace LoRes
  {
    /**
     *  Initializes the scheduler in the background.
     *
     *  @return Chimera::Status_t
     */
    Chimera::Status_t open();

    /**
     *  Terminates the scheduler
     *
     *  @return Chimera::Status_t
     */
    Chimera::Status_t close();

    /**
     *  Gets the resolution of the scheduler in milliseconds. This is
     *  set by the system and not the user.
     *
     *  @return size_t
     */
    size_t resolution();

    /**
     *  Schedules a function to execute once at some point in the future
     *
     *  @param[in]  method      The function to be executed
     *  @param[in]  when        Absolute time to run the function, in milliseconds
     *  @param[in]  relation    Whether to use absolute or relative timing
     *  @return Chimera::Status_t
     */
    Chimera::Status_t oneShot( Chimera::Function::Opaque *const method, const size_t when, const TimingType relation );

    /**
     *  Schedules a function to execute periodically
     *
     *  @param[in]  methdod     The function to be executed
     *  @param[in]  rate        How often to run the function, in milliseconds
     */
    Chimera::Status_t periodic( Chimera::Function::Opaque *const method, const size_t rate );

    /**
     *  Schedules a function to execute periodically, but only a number of times
     *  before it expires.
     *
     *  @param[in]  method      The function to be executed
     *  @param[in]  rate        How often to run the function, in milliseconds
     *  @param[in]  numTimes    Number of times to run the function before expiring
     *  @return Chimera::Status_t
     */
    Chimera::Status_t periodic( Chimera::Function::Opaque *const method, const size_t rate, const size_t numTimes );

    /**
     *  Stops a function from executing, assuming it's pending
     *
     *  @param[in]  method      The method to cancel
     *  @return Chimera::Status_t
     */
    Chimera::Status_t cancel( Chimera::Function::Opaque *const method );
  }  // namespace LoRes


  /*-------------------------------------------------------------------------------
  High Resolution Scheduler ([Nano, Micro, Milli]-Second timing)
  -------------------------------------------------------------------------------*/
  namespace HiRes
  {
    // TODO: This will use hardware timers to provide exact timing. Much more complicated.
  }
}  // namespace Chimera::Scheduler

#endif /* !CHIMERA_SCHEDULER_USER_HPP */
