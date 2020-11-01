/********************************************************************************
 *  File Name:
 *    threading_user.hpp
 *
 *  Description:
 *    High level functions for interacting with the system thread scheduler
 *
 *  2019-2020 | Brandon Braun | brandonbraun653@gmail.com
 ********************************************************************************/

#pragma once
#ifndef CHIMERA_THREADING_HPP
#define CHIMERA_THREADING_HPP

/* STL Includes */
#include <cstddef>

namespace Chimera::Threading
{
  /**
   *  Starts the scheduler. This primarily only has meaning in the
   *  context of embedded systems where the scheduler is actually
   *  controllable by the user.
   *
   *  @return void
   */
  void startScheduler();

  /**
   *  Stops the scheduler execution and breaks the program flow back
   *  to where startScheduler() was called.
   *
   *  @return void
   */
  void stopScheduler();

  /**
   *  Gets the resolution of the scheduler in milliseconds. This defines the
   *  minimum scale for all time based operations.
   */
  size_t schedulerResolution();

  /**
   *  How many threads are able to execute in true parallelism on this device
   *
   *  @return int
   */
  int hardwareConcurrency();

}  // namespace Chimera::Threading

#endif /* CHIMERA_THREADING_HPP */
