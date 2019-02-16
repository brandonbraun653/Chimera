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


class FreeRTOSEnvironment : public ::testing::Environment
{
public:
  void SetUp() override;

  void TearDown() override;

  void startTaskScheduler();

  void killTaskScheduler();

};

/* There should only ever be a single FreeRTOS environment instance */
extern FreeRTOSEnvironment FreeRTOSEnv;

#endif  /* !CHIMERA_GTEST_INIT_HPP */