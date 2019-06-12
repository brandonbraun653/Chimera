/********************************************************************************
 *   File Name:
 *    common.hpp
 *
 *   Description:
 *    Common constants utilized across the Chimera software library as well as any
 *    subsystems that inherit from it.
 *
 *   2019 | Brandon Braun | brandonbraun653@gmail.com
 ********************************************************************************/

 #pragma once
 #ifndef CHIMERA_CONSTANTS_COMMON_HPP
 #define CHIMERA_CONSTANTS_COMMON_HPP

 /* C++ Includes */
 #include <cstdlib>

namespace Chimera
{
  namespace Threading
  {
    static constexpr size_t DEFAULT_LOCK_TIMEOUT_MS = 500;
  }
}

#endif /* !CHIMERA_CONSTANTS_COMMON_HPP */