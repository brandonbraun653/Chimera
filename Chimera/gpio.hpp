#pragma once
#ifndef CHIMERA_GPIO_HPP
#define CHIMERA_GPIO_HPP

/* C/C++ Includes */
#include <stdint.h>
#include <type_traits>

/* Boost Includes */
#include <boost/shared_ptr.hpp>
#include <boost/make_shared.hpp>

/* Chimera Includes */
#include <Chimera/chimera.hpp>
#include <Chimera/config.hpp>

/** @namespace Chimera */
namespace Chimera
{
	/** @namespace GPIO */
	namespace GPIO
	{
		class GPIOClass : public CHIMERA_INHERITED_GPIO
		{
		public:
			Status mode(Mode mode, bool pullup = false) { return CHIMERA_INHERITED_GPIO::mode(mode, pullup); }
			Status write(State state) { return CHIMERA_INHERITED_GPIO::write(state); }

			GPIOClass(const Port& port, const uint8_t& pin)
			{
				_port = port;
				_pin = pin;

				CHIMERA_INHERITED_GPIO::init(port, pin);
			}

			~GPIOClass() = default;

		private:
			Port _port;
			uint8_t _pin;
		};
		typedef boost::shared_ptr<GPIOClass> GPIOClass_sPtr;
		
		CLASS_METHOD_CHECKER(has_init, CHIMERA_INHERITED_GPIO, init, Chimera::GPIO::Status, Chimera::GPIO::Port, uint8_t);
		CLASS_METHOD_CHECKER(has_mode, CHIMERA_INHERITED_GPIO, mode, Chimera::GPIO::Status, Chimera::GPIO::Mode, bool);
		CLASS_METHOD_CHECKER(has_write, CHIMERA_INHERITED_GPIO, write, Chimera::GPIO::Status, Chimera::GPIO::State);	
	}
}

#endif