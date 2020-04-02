/********************************************************************************
 *  File Name:
 *    chimera.cpp
 *
 *  Description:
 *      Implements common Chimera functions
 *
 *  2019-2020 | Brandon Braun | brandonbraun653@gmail.com
 ********************************************************************************/

/* Chimera Includes */
#include <Chimera/common>
#include <Chimera/gpio>
#include <Chimera/spi>
#include <Chimera/system>
#include <Chimera/thread>
#include <Chimera/timer>
#include <Chimera/watchdog>

void ChimeraInit()
{
  /*------------------------------------------------
  Initialize the back end driver first so that all the necessary
  resources can be initialized properly
  ------------------------------------------------*/
  Chimera::System::initialize();
  Chimera::System::systemStartup();

  /*------------------------------------------------
  Now initialize the Chimera drivers, which hook into the backend
  ------------------------------------------------*/
  Chimera::GPIO::initialize();
  Chimera::SPI::initialize();
  Chimera::Timer::initialize();
  Chimera::Watchdog::initialize();
}

namespace Chimera
{
  size_t millis()
  {
    return Timer::millis();
  }

  void delayMilliseconds( const size_t val )
  {
    Timer::delayMilliseconds( val );
  }

  void delayMicroseconds( const size_t val )
  {
    Timer::delayMicroseconds( val );
  }
}  // namespace Chimera
