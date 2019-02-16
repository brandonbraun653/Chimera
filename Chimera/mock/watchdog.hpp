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

#include <Chimera/watchdog.hpp>

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

    }  // namespace Watchdog
  }    // namespace Mock
}  // namespace Chimera

#endif /* !CHIMERA_MOCK_WATCHDOG_HPP */