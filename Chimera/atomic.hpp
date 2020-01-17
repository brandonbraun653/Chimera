/********************************************************************************
*  File Name:
*    atomic.hpp
*
*  Description:
*    Includes headers for implementations of system atomic types 
*
*  2020 | Brandon Braun | brandonbraun653@gmail.com
********************************************************************************/

#ifndef CHIMERA_ATOMIC_HPP
#define CHIMERA_ATOMIC_HPP

/* Chimera Include */
#include <Chimera/chimera.hpp>

namespace Chimera
{
#if defined( FULL_CPP_STL_AVAILABLE )
  #include <Chimera/atomic/atomic_stl.hpp>
#elif defined( USING_FREERTOS )
  #include <Chimera/atomic/atomic_freertos.hpp>
#else
  #include <Chimera/atomic/atomic_32bit.hpp>
#endif
}

#endif	/* !CHIMERA_ATOMIC_HPP */
