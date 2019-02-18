/********************************************************************************
 *   File Name:
 *       gtest_init.cpp
 *
 *   Description:
 *       Provides initialization environment implementations for the gtest suite
 *
 *   2019 | Brandon Braun | brandonbraun653@gmail.com
 ********************************************************************************/

/* Test Framework Includes */
#include "gtest_init.hpp"

/* Chimera Includes */
#include <Chimera/mock/watchdog.hpp>
#include <Chimera/threading.hpp>

/* C++ Includes */
#include <string>
#include <iostream>
#include <mutex>

/* Boost Includes */
#include <boost/chrono.hpp>

/**
 *  Starts FreeRTOS.
 *
 *  Since this function never returns, it is started inside a thread which
 *  is capable of being interrupted and subsequently killed.
 */
static void startScheduler()
{
  vTaskStartScheduler();
}

void FreeRTOSTest::startTaskScheduler()
{
  rtos = boost::thread( startScheduler );

  while ( xTaskGetSchedulerState() != taskSCHEDULER_RUNNING )
  {
    boost::chrono::milliseconds time( 100 );
    boost::this_thread::sleep_for( time );
  }
}
