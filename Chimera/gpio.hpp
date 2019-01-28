#pragma once
#ifndef CHIMERA_GPIO_HPP
#define CHIMERA_GPIO_HPP

/* C/C++ Includes */
#include <stdint.h>
#include <memory>

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
		class GPIOClass : public CHIMERA_INHERITED_GPIO
		{
		public:
            GPIOClass() = default;
            ~GPIOClass() = default;
		};

		typedef std::shared_ptr<Chimera::GPIO::GPIOClass> GPIOClass_sPtr;
		typedef std::unique_ptr<Chimera::GPIO::GPIOClass> GPIOClass_uPtr;

        static_assert(std::is_base_of<Interface, GPIOClass>::value, "CHIMERA: Base class does not implement the correct interface");
	}
}

#endif
