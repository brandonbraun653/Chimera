#pragma once
#ifndef CHIMERA_GPIO_HPP
#define CHIMERA_GPIO_HPP

/* Boost Includes */
#include <boost/shared_ptr.hpp>
#include <boost/make_shared.hpp>

/* Chimera Includes */
#include <Chimera/chimera.hpp>
#include <Chimera/config.hpp>

namespace Chimera
{
	namespace GPIO
	{
		class GPIOClass : public CHIMERA_INHERITED_GPIO
		{
		public:
			
			#error WRITE THE GPIO LIBRARY!
			
			GPIOClass() = default;
			~GPIOClass() = default;
			
		private:
			
		};
		typedef boost::shared_ptr<GPIOClass> GPIOClass_sPtr;
	}
}

#endif