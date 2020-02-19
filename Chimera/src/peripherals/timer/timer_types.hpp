/********************************************************************************
 *  File Name:
 *    timer_types.hpp
 *
 *	 Description:
 *    Types for the Chimera Timer module
 *
 *  2020 | Brandon Braun | brandonbraun653@gmail.com
 *******************************************************************************/

#pragma once
#ifndef CHIMERA_TIMER_TYPES_HPP
#define CHIMERA_TIMER_TYPES_HPP

/* STL Includes */
#include <cstdlib>

/* Chimera Includes */
#include <Chimera/common>

namespace Chimera::Timer
{

  namespace Backend
  {
    using Initialize_FPtr        = Chimera::Status_t ( * )( void );
    using Reset_FPtr             = Chimera::Status_t ( * )( void );
    using Millis_FPtr            = size_t ( * )( void );
    using DelayMilliseconds_FPtr = void ( * )( const size_t );
    using DelayMicroseconds_FPtr = void ( * )( const size_t );

    struct DriverConfig
    {
      bool isSupported;
      Initialize_FPtr initialize;
      Reset_FPtr reset;
      Millis_FPtr millis;
      DelayMilliseconds_FPtr delayMilliseconds;
      DelayMicroseconds_FPtr delayMicroseconds;
    };
  }  // namespace Backend
}

#endif  /* !CHIMERA_TIMER_TYPES_HPP */
