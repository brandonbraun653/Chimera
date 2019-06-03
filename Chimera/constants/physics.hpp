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
#ifndef CHIMERA_PHYSICS_CONSTANTS_HPP
#define CHIMERA_PHYSICS_CONSTANTS_HPP

/* C++ Includes */
#include <limits>
#include <cstdint>


namespace Chimera::Physics
{
  static constexpr float GRAVITY = -9.80665f;  /**< Average gravity in m/s2 felt at sea level (signed) */
  static constexpr float GRAVITY_ABS = std::abs( GRAVITY );
}


#endif /* !CHIMERA_PHYSICS_CONSTANTS_HPP */