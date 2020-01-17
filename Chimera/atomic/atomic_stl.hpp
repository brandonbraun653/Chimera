/********************************************************************************
 *  File Name:
 *    atomic_stl.hpp
 *
 *  Description:
 *    Chimera atomic types implemented with the C++ STL
 *
 *  2020 | Brandon Braun | brandonbraun653@gmail.com
 ********************************************************************************/

#ifndef CHIMERA_ATOMIC_WINDOWS_HPP
#define CHIMERA_ATOMIC_WINDOWS_HPP

/* Chimera Includes */
#include <Chimera/chimera.hpp>

#if defined( FULL_CPP_STL_AVAILABLE )
/* C++ Includes */
#include <atomic>

namespace Chimera
{
  template<typename AtomicType>
  using Atomic = std::atomic<AtomicType>;
}

#endif /* FULL_CPP_STL_AVAILABLE */
#endif /* !CHIMERA_ATOMIC_WINDOWS_HPP */
