/********************************************************************************
 *   File Name:
 *       test_watchdog.cpp
 *
 *   Description:
 *       Tests the watchdog interface of Chimera for proper functionality
 *
 *   2019 | Brandon Braun | brandonbraun653@gmail.com
 ********************************************************************************/

/* Chimera Includes */
#include <Chimera/interface.hpp>
#include <Chimera/watchdog.hpp>

/* C++ Includes */
#include <limits>
#include <exception>
#include <iostream>

/* Boost Includes */
#include <boost/chrono.hpp>
#include <boost/thread.hpp>

/* Test Framework Includes */
#include "gtest/gtest.h"
#include "gmock/gmock.h"
#include "gtest_init.hpp"


using namespace Chimera::Watchdog;

class WatchdogTest : public FreeRTOSTest
{
public:
};

/*------------------------------------------------
Verify the interface defaults to the correct error code if
a derived class has not overridden its behavior.
------------------------------------------------*/
TEST_F( FreeRTOSTest, NoBackend )
{
  /*------------------------------------------------
  Raw interface
  ------------------------------------------------*/
  Interface wd;
  ASSERT_EQ( wd.initialize( 1000 ), Status::FEATURE_NOT_SUPPORTED );
  ASSERT_EQ( wd.start(), Status::FEATURE_NOT_SUPPORTED );
  ASSERT_EQ( wd.stop(), Status::FEATURE_NOT_SUPPORTED );
  ASSERT_EQ( wd.kick(), Status::FEATURE_NOT_SUPPORTED );
  ASSERT_EQ( wd.pauseOnDebugHalt( true ), Status::FEATURE_NOT_SUPPORTED );
  ASSERT_EQ( wd.pauseOnDebugHalt( false ), Status::FEATURE_NOT_SUPPORTED );
  ASSERT_EQ( wd.isSupported(), false );

  auto testTimeout = std::numeric_limits<uint32_t>::max();
  ASSERT_EQ( wd.getTimeout( testTimeout ), Status::FEATURE_NOT_SUPPORTED );
  ASSERT_EQ( testTimeout, 0u );

  /*------------------------------------------------
  Check derived interface as a double check
  ------------------------------------------------*/
  class Derived : public Interface
  {
  public:
    Derived()  = default;
    ~Derived() = default;
  };

  Derived derived_wd;

  ASSERT_EQ( derived_wd.initialize( 1000 ), Status::FEATURE_NOT_SUPPORTED );
  ASSERT_EQ( derived_wd.start(), Status::FEATURE_NOT_SUPPORTED );
  ASSERT_EQ( derived_wd.stop(), Status::FEATURE_NOT_SUPPORTED );
  ASSERT_EQ( derived_wd.kick(), Status::FEATURE_NOT_SUPPORTED );
  ASSERT_EQ( derived_wd.pauseOnDebugHalt( true ), Status::FEATURE_NOT_SUPPORTED );
  ASSERT_EQ( derived_wd.pauseOnDebugHalt( false ), Status::FEATURE_NOT_SUPPORTED );
  ASSERT_EQ( derived_wd.isSupported(), false );

  testTimeout = std::numeric_limits<uint32_t>::max();
  ASSERT_EQ( derived_wd.getTimeout( testTimeout ), Status::FEATURE_NOT_SUPPORTED );
  ASSERT_EQ( testTimeout, 0u );
}

/*------------------------------------------------
Simulator watchdog initialization
------------------------------------------------*/
TEST_F( FreeRTOSTest, initialization )
{
  WatchdogClass wd;
  const uint32_t wdTimeout_mS = 1000;

  /*------------------------------------------------
  Ensure the WD timer isn't started in the initialization
  function by waiting for a watchdog timeout to occur.
  ------------------------------------------------*/
  try
  {
    wd.initialize(wdTimeout_mS);
    Chimera::delayMilliseconds( wdTimeout_mS * 2 );
  }
  catch ( const std::exception &e )
  {
    std::cout << e.what() << std::endl;
    ADD_FAILURE();
  }

  /*------------------------------------------------
  Make sure the initialization value is reported correctly
  ------------------------------------------------*/
  uint32_t actTimeout = 0;
  wd.getTimeout(actTimeout);
  EXPECT_EQ(wdTimeout_mS, actTimeout);
}

