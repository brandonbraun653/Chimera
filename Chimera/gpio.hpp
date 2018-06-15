#pragma once
#ifndef CHIMERA_GPIO_HPP
#define CHIMERA_GPIO_HPP

/* C/C++ Includes */
#include <stdint.h>

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
			Status mode(Chimera::GPIO::Mode mode, bool pullup = false) 
			{ 
				return gpioPin.cmode(mode, pullup); 
			}

			Status write(Chimera::GPIO::State state) 
			{ 
				return gpioPin.cwrite(state); 
			}

			bool read()
			{
				return gpioPin.cread();
			}

			void toggle()
			{
				gpioPin.ctoggle();
			}

			GPIOClass(const Port port, const uint8_t pin)
			{
				gpioPin.cinit(port, pin);
			}

			~GPIOClass() = default;

		private:
			CHIMERA_INHERITED_GPIO gpioPin;
		};
		typedef boost::shared_ptr<Chimera::GPIO::GPIOClass> GPIOClass_sPtr;
		
		CLASS_METHOD_CHECKER(has_init, CHIMERA_INHERITED_GPIO, cinit, Chimera::GPIO::Status, Chimera::GPIO::Port, uint8_t);
		CLASS_METHOD_CHECKER(has_mode, CHIMERA_INHERITED_GPIO, cmode, Chimera::GPIO::Status, Chimera::GPIO::Mode, bool);
		CLASS_METHOD_CHECKER(has_write, CHIMERA_INHERITED_GPIO, cwrite, Chimera::GPIO::Status, Chimera::GPIO::State);
		CLASS_METHOD_CHECKER(has_toggle, CHIMERA_INHERITED_GPIO, ctoggle, void, void);
		CLASS_METHOD_CHECKER(has_read, CHIMERA_INHERITED_GPIO, cread, bool, void);
	}
}

#endif