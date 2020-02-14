/********************************************************************************
 *  File Name:
 *    threading.hpp
 *
 *  Description:
 *    Includes all the necessary headers for implementing a thread based system
 *
 *  2019-2020 | Brandon Braun | brandonbraun653@gmail.com
 ********************************************************************************/

#pragma once
#ifndef CHIMERA_THREADING_HPP
#define CHIMERA_THREADING_HPP

/* Chimera Includes */
#include <Chimera/thread>

namespace Chimera::Threading
{
  void startScheduler();

  void stopScheduler();

}  // namespace Chimera::Threading

#endif /* CHIMERA_THREADING_HPP */
