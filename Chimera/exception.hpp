/********************************************************************************
 *   File Name:
 *    exception.hpp
 *
 *   Description:
 *    Lightweight exception definitions
 *
 *   2019 | Brandon Braun | brandonbraun653@gmail.com
 ********************************************************************************/

#pragma once
#ifndef CHIMERA_EXCEPTION_HPP
#define CHIMERA_EXCEPTION_HPP

/* C++ Includes */
#include <cstdint>

namespace Chimera
{
  namespace Exception
  {
    /**
     *  Initializes the exception handling framework
     *
     *  @return void
     */
    void initialize();

    /**
     *  Checks if an out of range exception has been thrown.
     *
     *  @return bool
     */
    bool out_of_range_thrown();

    /**
     *  Clears the out of range exception flag
     *
     *  @return void
     */
    void out_of_range_clear();
  }  // namespace Exception
}  // namespace Chimera

#endif /* !CHIMERA_EXCEPTION_HPP */