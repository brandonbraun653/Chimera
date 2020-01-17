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

/* Global system tick counter */
static std::atomic<size_t> sSysTick = 0;

/**
 *	Thread that updates the Chimera millisecond timer counter
 *	@return void
 */
static void SimSystemTick();
static std::thread sSysTickThread;

void ChimeraInit()
{
  sSysTick       = 0;
  sSysTickThread = std::thread( SimSystemTick );
}

namespace ChimeraSim
{
  size_t millis()
  {
    /*------------------------------------------------
    Last known value of the system millisecond timer. Intentionally
    using an async thread to update the tick count to reflect how 
    FreeRTOS updates their system tick counter. Most of my systems 
    at the moment are Embedded FreeRTOS based, so it makes some sense.
    ------------------------------------------------*/
    return sSysTick;
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

static void SimSystemTick()
{
  using namespace std::chrono;

  /*------------------------------------------------
  Initialize the timer
  ------------------------------------------------*/
  const size_t tickStart = duration_cast<milliseconds>( system_clock::now().time_since_epoch() ).count();
  size_t currentTick = tickStart;
  sSysTick = 0;

  /*------------------------------------------------
  As long as the program is executing, this thread cannot quit.
  Unfortunately that also means the system tick can't quit either, so debugging
  timing issues while using breakpoints are going to be a pain in the butt.
  ------------------------------------------------*/
  while ( true )
  {
    currentTick = duration_cast<milliseconds>( system_clock::now().time_since_epoch() ).count();
    sSysTick = currentTick - tickStart;

    // I can't really guarantee that this will sleep for this amount of time...
    std::this_thread::sleep_for( std::chrono::microseconds( 1000 ) );
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
