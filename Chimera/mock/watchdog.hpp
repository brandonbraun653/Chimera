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

namespace Chimera::Mock
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
}  // namespace Chimera::Mock

#endif /* !CHIMERA_MOCK_WATCHDOG_HPP */
