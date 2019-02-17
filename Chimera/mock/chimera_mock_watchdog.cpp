/********************************************************************************
 *   File Name:
 *      chimera_mock_watchdog.cpp
 *
 *   Description:
 *      Simulates the hardware watchdog functionality by using threads.
 *
 *   2019 | Brandon Braun | brandonbraun653@gmail.com
 ********************************************************************************/

#include <limits>
#include <stdexcept>

#include <Chimera/threading.hpp>
#include <Chimera/mock/watchdog.hpp>

namespace Chimera
{
  namespace Mock
  {
    namespace Watchdog
    {
      static void simWatchdogThread( void *arguments )
      {
        SimWatchdog *wd = reinterpret_cast<SimWatchdog *>( arguments );

        if ( !wd )
        {
          throw std::invalid_argument("Expected a SimWatchdog instance but got nullptr instead");
        }

        uint32_t timeout;
        wd->getTimeout( timeout );

        Chimera::Threading::signalThreadSetupComplete();

        for ( ;; )
        {
          if ((millis() - wd->tick) > timeout)
          {
            throw std::runtime_error("Watchdog timeout occurred!");
          }

          vTaskDelay( pdMS_TO_TICKS( 5 ) );
        }

        vTaskDelete( NULL );
      }

      SimWatchdog::SimWatchdog()
      {
        setTimeout = 0u;
      }

      Chimera::Watchdog::Status SimWatchdog::initialize( const uint32_t timeout_mS )
      {
        setTimeout = timeout_mS;
        return Chimera::Watchdog::Status::OK;
      }

      Chimera::Watchdog::Status SimWatchdog::start()
      {
        Chimera::Threading::addThread(simWatchdogThread, "wd", 500, this, configLIBRARY_MAX_SYSCALL_INTERRUPT_PRIORITY, NULL);

        return Chimera::Watchdog::Status::OK;
      }

      Chimera::Watchdog::Status SimWatchdog::stop()
      {
        /*------------------------------------------------
        Once the watchdog starts, it cannot be stopped.
        ------------------------------------------------*/
        return Chimera::Watchdog::Status::FEATURE_NOT_SUPPORTED;
      }

      Chimera::Watchdog::Status SimWatchdog::kick()
      {
        tick = Chimera::millis();
        return Chimera::Watchdog::Status::OK;
      }

      Chimera::Watchdog::Status SimWatchdog::getTimeout( uint32_t &timeout )
      {
        timeout = setTimeout;
        return Chimera::Watchdog::Status::OK;
      }

      Chimera::Watchdog::Status SimWatchdog::pauseOnDebugHalt( const bool enable )
      {
        /*------------------------------------------------
        This is really a hardware only kind of thing. It is meant to stop the
        HW watchdog when the CPU is interrupted on the debug registers. It either
        works or it doesn't. Not much testing can be done for that.
        ------------------------------------------------*/
        return Chimera::Watchdog::Status::FEATURE_NOT_SUPPORTED;
      }

      bool SimWatchdog::isSupported()
      {
        return true;
      }
    }  // namespace Watchdog
  }    // namespace Mock
}  // namespace Chimera
