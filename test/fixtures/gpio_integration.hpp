/********************************************************************************
 *  File Name:
 *    gpio_integration.hpp
 *
 *  Description:
 *    Implements the GPIO test fixture for the Chimera backend driver
 *
 *  2019 | Brandon Braun | brandonbraun653@gmail.com
 ********************************************************************************/

#pragma once
#ifndef CHIMERA_GPIO_INTEGRATION_TEST_FIXTURE_HPP
#define CHIMERA_GPIO_INTEGRATION_TEST_FIXTURE_HPP

/* Testing Includes */
#include <gtest/gtest.h>

/* Chimera Includes */
#include "chimeraPort.hpp"

namespace Chimera
{
  namespace GPIO
  {
    class GPIOIntegrationTestFixture : public ::testing::Test
    {
    protected:
      virtual void SetUp() override;

      virtual void TearDown() override;

      /**
       *	Passes the GPIO initialization sequence
       *	
       *	@return void
       */
       void defaultInit();

      CHIMERA_INHERITED_GPIO *gpio = nullptr;
    };
  }  // namespace GPIO
}  // namespace Chimera


#endif /* !CHIMERA_GPIO_INTEGRATION_TEST_FIXTURE_HPP */