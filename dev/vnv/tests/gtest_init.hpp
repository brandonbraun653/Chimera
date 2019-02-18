/********************************************************************************
 *   File Name:
 *       gtest_init.hpp
 *
 *   Description:
 *       Initialize various resources or environments for the gtest suite
 *
 *   2019 | Brandon Braun | brandonbraun653@gmail.com
 ********************************************************************************/
#pragma once
#ifndef CHIMERA_GTEST_INIT_HPP
#define CHIMERA_GTEST_INIT_HPP

#include <gtest/gtest.h>

/* Boost Includes */
#include <boost/thread.hpp>

/* Chimera Includes */
#include <Chimera/threading.hpp>

class FreeRTOSTest : public ::testing::Test
{
protected:
  virtual void SetUp() override
  {
    /*------------------------------------------------
    After a test has been torn down, the FreeRTOS internal state has all
    interrupts disabled. Even restarting the scheduler does not re-enable
    them, thus it must be performed here.
    ------------------------------------------------*/
    portENABLE_INTERRUPTS();
    startTaskScheduler();
  }

  virtual void TearDown() override
  {
    rtos.interrupt();
    rtos.join();

    vTaskEndScheduler();
  }

private:
  void startTaskScheduler();

  boost::thread rtos;
};


#endif /* !CHIMERA_GTEST_INIT_HPP */
