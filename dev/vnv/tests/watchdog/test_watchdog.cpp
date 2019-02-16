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

/* Boost Includes */
#include <boost/chrono.hpp>
#include <boost/thread.hpp>

/* Test Framework Includes */
#include "gtest/gtest.h"
#include "gmock/gmock.h"


using namespace Chimera::Watchdog;

/*------------------------------------------------
Verify the interface defaults to the correct error code if
a derived class has not overridden its behavior.
------------------------------------------------*/
TEST(WatchdogTest, NoBackend)
{
  /*------------------------------------------------
  Raw interface
  ------------------------------------------------*/
  Interface wd;
  ASSERT_EQ(wd.initialize(1000), Status::FEATURE_NOT_SUPPORTED);
  ASSERT_EQ(wd.start(), Status::FEATURE_NOT_SUPPORTED);
  ASSERT_EQ(wd.stop(), Status::FEATURE_NOT_SUPPORTED);
  ASSERT_EQ(wd.kick(), Status::FEATURE_NOT_SUPPORTED);
  ASSERT_EQ(wd.pauseOnDebugHalt(true), Status::FEATURE_NOT_SUPPORTED);
  ASSERT_EQ(wd.pauseOnDebugHalt(false), Status::FEATURE_NOT_SUPPORTED);
  ASSERT_EQ(wd.isSupported(), false);

  auto testTimeout = std::numeric_limits<uint32_t>::max();
  ASSERT_EQ(wd.getTimeout(testTimeout), Status::FEATURE_NOT_SUPPORTED);
  ASSERT_EQ(testTimeout, 0u);

  /*------------------------------------------------
  Check derived interface as a double check
  ------------------------------------------------*/
  class Derived : public Interface
  {
    public:

      Derived() = default;
      ~Derived() = default;
  };

  Derived derived_wd;

  ASSERT_EQ(derived_wd.initialize(1000), Status::FEATURE_NOT_SUPPORTED);
  ASSERT_EQ(derived_wd.start(), Status::FEATURE_NOT_SUPPORTED);
  ASSERT_EQ(derived_wd.stop(), Status::FEATURE_NOT_SUPPORTED);
  ASSERT_EQ(derived_wd.kick(), Status::FEATURE_NOT_SUPPORTED);
  ASSERT_EQ(derived_wd.pauseOnDebugHalt(true), Status::FEATURE_NOT_SUPPORTED);
  ASSERT_EQ(derived_wd.pauseOnDebugHalt(false), Status::FEATURE_NOT_SUPPORTED);
  ASSERT_EQ(derived_wd.isSupported(), false);

  testTimeout = std::numeric_limits<uint32_t>::max();
  ASSERT_EQ(derived_wd.getTimeout(testTimeout), Status::FEATURE_NOT_SUPPORTED);
  ASSERT_EQ(testTimeout, 0u);
}

/*------------------------------------------------
Simulator watchdog initialization
------------------------------------------------*/
TEST(WatchdogSim, initialization)
{
  //Make sure the watchdog doesn't start (FreeRTOS thread)

  //Might need to make a getter to check what the init value was.
  //This could be useful for code later so you can get the value
  //at runtime.

}

TEST(WatchdogSim, intentionalDelay)
{
  std::cout << "Sleeping for 10 seconds" << std::endl;
  boost::chrono::seconds sleepTime(10);
  boost::this_thread::sleep_for(sleepTime);
  std::cout << "Done!" << std::endl;

}