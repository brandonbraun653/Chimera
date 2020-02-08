/********************************************************************************
 *  File Name:
 *    sim_chimera.cpp
 *
 *  Description:
 *    Simulator variant of the core chimera functionality.
 *
 *  2019-2020 | Brandon Braun | brandonbraun653@gmail.com
 ********************************************************************************/

#if defined( CHIMERA_SIMULATOR )

/* C++ Includes */
#include <atomic>
#include <cstdint>
#include <chrono>
#include <thread>

/* Chimera Includes */
#include <Chimera/sim/sim_chimera_system.hpp>

namespace ChimeraSim
{
  size_t millis()
  {
    return Chimera::System::getSystemTick();
  }

  void delayMilliseconds( const size_t val )
  {
    /* OS dependent on whether or not this sleep is accurate */
    std::this_thread::sleep_for( std::chrono::milliseconds( val ) );
  }

  void delayMicroseconds( const size_t val )
  {
    /* OS dependent on whether or not this sleep is accurate */
    std::this_thread::sleep_for( std::chrono::microseconds( val ) );
  }
}

#if defined( CHIMERA_STANDALONE )
int main()
{
  // Provide an entry point for the VS project standalone compilation target
  return 0;
}
#endif

#endif /* _WIN32 || _WIN64 */
