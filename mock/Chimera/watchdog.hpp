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

/* C++ Includes */
#include <atomic>

/* Test Framework Includes */
#include <gmock/gmock.h>

/* Chimera Includes */
#include <Chimera/interface/watchdog_intf.hpp>


namespace Chimera::Mock::Watchdog
{
  class WatchdogMock : public Chimera::Watchdog::Interface
  {
  public:
    MOCK_METHOD2( initialize, Chimera::Status_t( const uint32_t, const uint8_t ) );
    MOCK_METHOD0( start, Chimera::Status_t() );
    MOCK_METHOD0( stop, Chimera::Status_t() );
    MOCK_METHOD0( kick, Chimera::Status_t() );
    MOCK_METHOD1( getTimeout, Chimera::Status_t( uint32_t & ) );
    MOCK_METHOD1( pauseOnDebugHalt, Chimera::Status_t( const bool ) );
    MOCK_METHOD0( isSupported, bool() );
  };

#if defined( USING_FREERTOS )
  class SimWatchdog : public Chimera::Watchdog::Interface
  {
  public:
    SimWatchdog();
    ~SimWatchdog() = default;

    Chimera::Status_t initialize( const uint32_t timeout_mS, const uint8_t windowPercent ) override;

    Chimera::Status_t start() override;

    Chimera::Status_t stop() override;

    Chimera::Status_t kick() override;

    Chimera::Status_t getTimeout( uint32_t &timeout ) override;

    Chimera::Status_t pauseOnDebugHalt( const bool enable ) override;

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

}  // namespace Chimera::Mock::Watchdog

#endif /* !CHIMERA_MOCK_WATCHDOG_HPP */
