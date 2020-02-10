/********************************************************************************
 *  File Name:
 *    gpio_base.hpp
 *
 *  Description:
 *    Abstract base class interfaces
 *
 *  2019-2020 | Brandon Braun | brandonbraun653@gmail.com
 ********************************************************************************/

#pragma once
#ifndef CHIMERA_GPIO_ABSTRACT_BASE_HPP
#define CHIMERA_GPIO_ABSTRACT_BASE_HPP

/* STL Includes */
#include <cstdint>

/* Chimera Includes*/
#include <Chimera/common>
#include <Chimera/src/peripherals/gpio/gpio_intf.hpp>
#include <Chimera/src/peripherals/gpio/gpio_types.hpp>

namespace Chimera::GPIO
{
  class GPIOUnsupported : public HWInterface
  {
  public:
    GPIOUnsupported()  = default;
    ~GPIOUnsupported() = default;

    Chimera::Status_t init( const Chimera::GPIO::Port port, const uint8_t pin, const size_t timeout = 0 ) final override
    {
      return Chimera::GPIO::Status::FAIL;
    }

    Chimera::Status_t init( const Chimera::GPIO::PinInit &pinInit, const size_t timeout = 0 ) final override
    {
      return Chimera::GPIO::Status::FAIL;
    }

    Chimera::Status_t setMode( const Chimera::GPIO::Drive drive, const Chimera::GPIO::Pull pull,
                               const size_t timeout = 0 ) final override
    {
      return Chimera::GPIO::Status::NOT_INITIALIZED;
    }

    Chimera::Status_t setState( const Chimera::GPIO::State state, const size_t timeout = 0 ) final override
    {
      return Chimera::GPIO::Status::NOT_INITIALIZED;
    }

    Chimera::Status_t getState( Chimera::GPIO::State &state, const size_t timeout = 0 ) final override
    {
      return Chimera::GPIO::Status::NOT_INITIALIZED;
    }

    Chimera::Status_t toggle( const size_t timeout = 0 ) final override
    {
      return Chimera::GPIO::Status::NOT_INITIALIZED;
    }
  };
}  // namespace Chimera::GPIO

#endif /* !CHIMERA_GPIO_ABSTRACT_BASE_HPP */