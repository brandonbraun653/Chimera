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
#include <Chimera/chimera.hpp>
#include <Chimera/config.hpp>

#include "chimeraPort.hpp"

/** @namespace Chimera */
namespace Chimera
{
  /** @namespace GPIO */
  namespace GPIO
  {
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

    using GPIOClass_sPtr = std::shared_ptr<Chimera::GPIO::GPIOClass>;
    using GPIOClass_uPtr = std::unique_ptr<Chimera::GPIO::GPIOClass>;

    static_assert( std::is_base_of<Interface, GPIOClass>::value, "Base class implements the wrong interface" );
  }  // namespace GPIO
}  // namespace Chimera

#endif
