#pragma once
#ifndef CHIMERA_SERIAL_HPP
#define CHIMERA_SERIAL_HPP

/* Boost Includes */
#include <boost/shared_ptr.hpp>
#include <boost/make_shared.hpp>

/* Chimera Includes */
#include <Chimera/chimera.hpp>
#include <Chimera/config.hpp>

namespace Chimera
{
	namespace Serial
	{
		class SerialClass : public CHIMERA_INHERITED_UART
		{
		public:
			//Don't do the printf type stuff here...only low level interface
			
			//Um...how to choose between UART and USART?...
		private:
			
		};
	}
}

#endif 