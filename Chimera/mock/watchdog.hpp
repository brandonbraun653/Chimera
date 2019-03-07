/********************************************************************************
 *   File Name:
 *     watchdog.hpp
 *
 *   Description:
 *     Provides a mocked watchdog timer interface for Chimera
 *
 *   2019 | Brandon Braun | brandonbraun653@gmail.com
 ********************************************************************************/

#pragma once
#ifndef CHIMERA_MOCK_WATCHDOG_HPP
#define CHIMERA_MOCK_WATCHDOG_HPP

#include <Chimera/interface.hpp>

/* C++ Includes */
#include <atomic>

/* Test Framework Includes */
#include <gmock/gmock.h>

namespace Chimera
{
  namespace Mock
  {
    namespace Watchdog
    {
      class MockWatchdog : public Chimera::Watchdog::Interface
      {
      public:
        MOCK_METHOD1( initialize, Chimera::Watchdog::Status( uint32_t ) );
        MOCK_METHOD0( start, Chimera::Watchdog::Status() );
        MOCK_METHOD0( stop, Chimera::Watchdog::Status() );
        MOCK_METHOD0( kick, Chimera::Watchdog::Status() );
        MOCK_METHOD1( getTimeout, Chimera::Watchdog::Status( uint32_t & ) );
        MOCK_METHOD1( pauseOnDebugHalt, Chimera::Watchdog::Status( const bool ) );
        MOCK_METHOD0( isSupported, bool() );
      };

#if defined( USING_FREERTOS ) 
      class SimWatchdog : public Chimera::Watchdog::Interface
      {
      public:
        SimWatchdog();
        ~SimWatchdog() = default;

        Chimera::Watchdog::Status initialize( const uint32_t timeout_mS ) override;

        Chimera::Watchdog::Status start() override;

        Chimera::Watchdog::Status stop() override;

        Chimera::Watchdog::Status kick() override;

        Chimera::Watchdog::Status getTimeout( uint32_t &timeout ) override;

        Chimera::Watchdog::Status pauseOnDebugHalt( const bool enable ) override;

        bool isSupported() override;

        bool TEST_isTriggered();

        void TEST_reset();

        uint32_t tick;

        /* I don't like making this public, but seeing as I'm the only owner of this code,
        it works for the moment. I know not to override this. */
        std::atomic<bool> stickyTrigger;

      private:
        uint32_t setTimeout;
        TaskHandle_t task;
      };
#endif /* !USING_FREERTOS */

    }  // namespace Watchdog
  }    // namespace Mock
}  // namespace Chimera

#endif /* !CHIMERA_MOCK_WATCHDOG_HPP */
