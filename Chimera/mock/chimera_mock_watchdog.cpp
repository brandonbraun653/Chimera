/********************************************************************************
 *   File Name:
 *      chimera_mock_watchdog.cpp
 *
 *   Description:
 *      Simulates the hardware watchdog functionality by using threads.
 *
 *   2019 | Brandon Braun | brandonbraun653@gmail.com
 ********************************************************************************/

/* C++ Includes */
#include <limits>
#include <stdexcept>

/* Chimera Includes */
#include <Chimera/chimera.hpp>
#include <Chimera/threading.hpp>
#include <Chimera/mock/watchdog.hpp>

namespace Chimera::Mock
{
#if defined( USING_FREERTOS )
  static void simWatchdogThread( void *arguments )
  {
    SimWatchdog *wd = reinterpret_cast<SimWatchdog *>( arguments );

    if ( !wd )
    {
      throw std::invalid_argument( "Expected a SimWatchdog instance but got nullptr instead" );
    }

    uint32_t timeout;
    wd->getTimeout( timeout );

    // Chimera::Threading::signalThreadSetupComplete();

    for ( ;; )
    {
      auto current_tick = millis();
      auto time_elapsed = current_tick - wd->tick;
      if ( time_elapsed > timeout )
      {
        wd->stickyTrigger = true;
      }

      vTaskDelay( pdMS_TO_TICKS( 5 ) );
    }

    vTaskDelete( NULL );
  }

  SimWatchdog::SimWatchdog()
  {
    tick          = 0u;
    task          = NULL;
    setTimeout    = 0u;
    stickyTrigger = false;
  }

  Chimera::Status_t SimWatchdog::initialize( const uint32_t timeout_mS, const uint8_t windowPercent )
  {
    setTimeout = timeout_mS;
    return Chimera::CommonStatusCodes::OK;
  }

  Chimera::Status_t SimWatchdog::start()
  {
    if ( task )
    {
      vTaskResume( task );
    }
    else
    {
      xTaskCreate( simWatchdogThread, "wd", 500, this, configLIBRARY_MAX_SYSCALL_INTERRUPT_PRIORITY, &task );
    }

    return Chimera::CommonStatusCodes::OK;
  }

  Chimera::Status_t SimWatchdog::stop()
  {
    if ( task )
    {
      vTaskSuspend( task );
      kick();
    }

    return Chimera::CommonStatusCodes::OK;
  }

  Chimera::Status_t SimWatchdog::kick()
  {
    tick = Chimera::millis();
    return Chimera::CommonStatusCodes::OK;
  }

  Chimera::Status_t SimWatchdog::getTimeout( uint32_t &timeout )
  {
    timeout = setTimeout;
    return Chimera::CommonStatusCodes::OK;
  }

  Chimera::Status_t SimWatchdog::pauseOnDebugHalt( const bool enable )
  {
    /*------------------------------------------------
    This is really a hardware only kind of thing. It is meant to stop the
    HW watchdog when the CPU is interrupted on the debug registers. It either
    works or it doesn't. Not much testing can be done for that.
    ------------------------------------------------*/
    return Chimera::CommonStatusCodes::NOT_SUPPORTED;
  }

  bool SimWatchdog::TEST_isTriggered()
  {
    return stickyTrigger;
  }

  void SimWatchdog::TEST_reset()
  {
    stickyTrigger = false;
    kick();
  }
#endif /* !USING_FREERTOS */

}  // namespace Chimera::Mock::Watchdog
