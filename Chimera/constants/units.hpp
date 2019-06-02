/********************************************************************************
 *   File Name:
 *    physics.hpp
 *
 *   Description:
 *    Physics constants for Chimera
 *
 *   2019 | Brandon Braun | brandonbraun653@gmail.com
 ********************************************************************************/

#pragma once
#ifndef CHIMERA_UNITS_CONSTANTS_HPP
#define CHIMERA_UNITS_CONSTANTS_HPP

/* C++ Includes */
#include <limits>
#include <cstdint>


namespace Chimera::Units
{
  template<typename T>
  struct MilliToKilo
  {
    static constexpr T val = static_cast<T>( 1000 );
  };
}  // namespace Chimera::Units


#endif /* !CHIMERA_UNITS_CONSTANTS_HPP */