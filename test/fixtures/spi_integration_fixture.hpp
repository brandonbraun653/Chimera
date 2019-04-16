/********************************************************************************
 *  File Name:
 *    spi_integration.hpp
 *
 *  Description:
 *    Implements the SPI test fixture for the Chimera backend driver
 *
 *  2019 | Brandon Braun | brandonbraun653@gmail.com
 ********************************************************************************/

#pragma once
#ifndef CHIMERA_SPI_INTEGRATION_TEST_FIXTURE_HPP
#define CHIMERA_SPI_INTEGARTION_TEST_FIXTURE_HPP

/* Testing Includes */
#include <gtest/gtest.h>

/* Chimera Includes */
#include "chimeraPort.hpp"

namespace Chimera
{
  namespace SPI
  {
    class SPIIntegrationTestFixture : public ::testing::Test
    {
    protected:
      virtual void SetUp() override;

      virtual void TearDown() override;

      /**
       *	Passes the initialization sequence
       *	
       *	@return void
       */
       void defaultInit();

      CHIMERA_INHERITED_SPI *spi = nullptr;
    };
  }  // namespace SPI
}  // namespace Chimera

#endif /* !CHIMERA_SPI_INTEGRATION_TEST_FIXTURE_HPP */