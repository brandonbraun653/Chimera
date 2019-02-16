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

FreeRTOSEnvironment FreeRTOSEnv;

void simHeartBeat(void *arguments)
{
  const TickType_t heartBeatPeriod_mS = 1000;
  
  auto previousTick = xTaskGetTickCount();
  for (;;)
  {
    std::cout << "Holy crap this actually worked" << std::endl;

    vTaskDelayUntil(&previousTick, heartBeatPeriod_mS);
  }
}

void FreeRTOSEnvironment::SetUp()
{
  startTaskScheduler();



}

void FreeRTOSEnvironment::TearDown()
{
  killTaskScheduler();
}

void FreeRTOSEnvironment::startTaskScheduler()
{
  xTaskCreate(simHeartBeat, "SimHeartBeat", 1000, NULL, 1, NULL);

  vTaskStartScheduler();
}

void FreeRTOSEnvironment::killTaskScheduler()
{

}