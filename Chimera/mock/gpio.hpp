/********************************************************************************
 * File Name:
 *	gpio.hpp
 *
 * Description:
 *	Defines the mock class for the Chimera GPIO
 *
 * 2019 | Brandon Braun | brandonbraun653@gmail.com
 ********************************************************************************/

#pragma once
#ifndef CHIMERA_GPIO_MOCK_HPP
#define CHIMERA_GPIO_MOCK_HPP

/* Chimera Includes */
#include <Chimera/interface.hpp>

/* Test Framework Includes */
#include <gmock/gmock.h>

namespace Chimera
{
  namespace Mock
  {
    class GPIOMock : public Chimera::GPIO::Interface
    {
    public:
      MOCK_METHOD2(init, Chimera::Status_t(const Chimera::GPIO::Port, const uint8_t));
      MOCK_METHOD2(setMode, Chimera::Status_t(const Chimera::GPIO::Drive, const bool));
      MOCK_METHOD1(setState, Chimera::Status_t(const Chimera::GPIO::State));
      MOCK_METHOD1(getState, Chimera::Status_t(const Chimera::GPIO::State &));
      MOCK_METHOD0(toggle, Chimera::Status_t());

      void dummyFunc()
      {
      }
    };

    using GPIOMock_sPtr = std::shared_ptr<Chimera::Mock::GPIOMock>;
    using GPIOMock_uPtr = std::unique_ptr<Chimera::Mock::GPIOMock>;
  }
}

#endif /* !CHIMERA_GPIO_MOCK_HPP */
