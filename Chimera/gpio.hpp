/********************************************************************************
 * File Name:
 *    gpio.hpp
 *
 * Description:
 *    Implements a common wrapper interface to the Chimera GPIO peripheral.
 *
 * 2019 | Brandon Braun | brandonbraun653@gmail.com
 ********************************************************************************/

#pragma once
#ifndef CHIMERA_GPIO_HPP
#define CHIMERA_GPIO_HPP

/* C/C++ Includes */
#include <memory>
#include <stdint.h>

/* Chimera Includes */
#include <Chimera/base/gpio_base.hpp>
#include <Chimera/interface/gpio_intf.hpp>
#include "chimeraPort.hpp"

namespace Chimera::GPIO
{
#if !defined( CHIMERA_INHERITED_GPIO )
  using CHIMERA_INHERITED_GPIO = GPIOUnsupported;
#endif

  /**
   *  A simple wrapper to provide a common GPIO class type for programs built
   *  with Chimera. The runtime behavior of this class is defined by the user
   *  provided class type CHIMERA_INHERITED_GPIO in chimeraPort.hpp.
   *
   *  If no user class is provided, a default disabled behavior version will be
   *  substituted in its place.
   */
  class GPIOClass : public CHIMERA_INHERITED_GPIO
  {
  public:
    GPIOClass() : CHIMERA_INHERITED_GPIO(){};
    ~GPIOClass() = default;
  };

  static_assert( std::is_base_of<HWInterface, GPIOClass>::value, "Base class implements the wrong interface" );

}  // namespace Chimera::GPIO

#endif
