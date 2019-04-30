/********************************************************************************
 *  File Name:
 *    serial_integration_fixture.hpp
 *
 *  Description:
 *    Implements the Serial test fixture for the Chimera backend driver
 *
 *  2019 | Brandon Braun | brandonbraun653@gmail.com
 ********************************************************************************/

#pragma once
#ifndef CHIMERA_SERIAL_INTEGRATION_TEST_FIXTURE_HPP
#define CHIMERA_SERIAL_INTEGRATION_TEST_FIXTURE_HPP

/* Testing Includes */
#include <gtest/gtest.h>

/* Chimera Includes */
#include "chimeraPort.hpp"

namespace Chimera
{
  namespace Serial
  {
    class SerialIntegrationTestFixture : public ::testing::Test
    {
    protected:
      virtual void SetUp() override;

      virtual void TearDown() override;

      /**
       *	Passes the Serial initialization sequence
       *	
       *	@return void
       */
       void defaultInit();

       void reset_test();

      CHIMERA_INHERITED_SERIAL *serial = nullptr;
    };
  }  // namespace Serial
}  // namespace Chimera

#endif  // !CHIMERA_SERIAL_INTEGRATION_TEST_FIXTURE_HPP
