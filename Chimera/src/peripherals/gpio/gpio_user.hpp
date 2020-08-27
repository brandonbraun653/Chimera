/********************************************************************************
 *  File Name:
 *    gpio.hpp
 *
 *  Description:
 *    Implements an interface to create a Chimera GPIO peripheral
 *
 *  2019-2020 | Brandon Braun | brandonbraun653@gmail.com
 ********************************************************************************/

#pragma once
#ifndef CHIMERA_GPIO_HPP
#define CHIMERA_GPIO_HPP

/* STL Includes */
#include <memory>

/* Chimera Includes */
#include <Chimera/src/peripherals/gpio/gpio_intf.hpp>

namespace Chimera::GPIO
{
  Chimera::Status_t initialize();
  Chimera::Status_t reset();
  IGPIO_sPtr getDriver( const Pin pin );
}  // namespace Chimera::GPIO

#endif  /* !CHIMERA_GPIO_HPP */
