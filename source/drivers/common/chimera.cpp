/******************************************************************************
 *  File Name:
 *    chimera.cpp
 *
 *  Description:
 *      Implements common Chimera functions
 *
 *  2019-2023 | Brandon Braun | brandonbraun653@gmail.com
 *****************************************************************************/

/*-----------------------------------------------------------------------------
Includes
-----------------------------------------------------------------------------*/
#include <Chimera/adc>
#include <Chimera/can>
#include <Chimera/clock>
#include <Chimera/common>
#include <Chimera/dma>
#include <Chimera/exti>
#include <Chimera/gpio>
#include <Chimera/i2c>
#include <Chimera/interrupt>
#include <Chimera/serial>
#include <Chimera/sdio>
#include <Chimera/spi>
#include <Chimera/system>
#include <Chimera/thread>
#include <Chimera/timer>
#include <Chimera/uart>
#include <Chimera/usart>
#include <Chimera/watchdog>


void ChimeraInit()
{
  /*---------------------------------------------------------------------------
  Boot system drivers first. They support everything.
  ---------------------------------------------------------------------------*/
  Chimera::System::initialize();
  Chimera::System::systemStartup();

  /*---------------------------------------------------------------------------
  Init interrupts early on so drivers may register handlers with the system.
  ---------------------------------------------------------------------------*/
  Chimera::Interrupt::initialize();

  /*---------------------------------------------------------------------------
  Individual driver initialization
  ---------------------------------------------------------------------------*/
  Chimera::ADC::initialize();
  Chimera::CAN::initialize();
  Chimera::Clock::initialize();
  Chimera::DMA::initialize();
  Chimera::EXTI::open();
  Chimera::GPIO::initialize();
  Chimera::I2C::initialize();
  Chimera::SDIO::initialize();
  Chimera::SPI::initialize();
  Chimera::Timer::initialize();
  Chimera::UART::initialize();
  Chimera::USART::initialize();
  Chimera::Watchdog::initialize();

  /*---------------------------------------------------------------------------
  Composite driver initialization
  ---------------------------------------------------------------------------*/
  Chimera::Serial::initialize();  // Consumes UART and USART
}

namespace Chimera
{
  size_t millis()
  {
    return Timer::millis();
  }


  size_t micros()
  {
    return Timer::micros();
  }


  void delayUntil( const size_t val )
  {
    const size_t currentTick = millis();
    if ( currentTick < val )
    {
      delayMilliseconds( val - currentTick );
    }
  }


  void delayMilliseconds( const size_t val )
  {
    Timer::delayMilliseconds( val );
  }


  void delayMilliseconds( const size_t lastTimeWoken, const size_t val )
  {
    delayUntil( lastTimeWoken + val );
  }


  void delayMicroseconds( const size_t val )
  {
    Timer::delayMicroseconds( val );
  }


  void blockDelayMilliseconds( const size_t val )
  {
    Timer::blockDelayMilliseconds( val );
  }


  void blockDelayMicroseconds( const size_t val )
  {
    Timer::blockDelayMicroseconds( val );
  }
}  // namespace Chimera
