/********************************************************************************
*   File Name:
*       sim_chimera.hpp
*
*   Description:
*       Simulator variant of the core chimera functionality.
*
*   2019 | Brandon Braun | brandonbraun653@gmail.com
********************************************************************************/

#pragma once
#ifndef SIM_CHIMERA_HPP
#define SIM_CHIMERA_HPP

/* C++ Includes */
#include <cstdint>

namespace ChimeraSim
{

  size_t millis();

  void delayMilliseconds( size_t val );

  void delayMicroseconds( size_t val );

}

#endif /* SIM_CHIMERA_HPP */