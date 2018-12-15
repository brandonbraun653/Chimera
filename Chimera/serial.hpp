#pragma once
#ifndef CHIMERA_SERIAL_HPP
#define CHIMERA_SERIAL_HPP

/* Chimera Includes */
#include <Chimera/chimera.hpp>
#include <Chimera/config.hpp>

namespace Chimera
{
	namespace Serial
	{
		#if 0
		class SerialClass : public CHIMERA_INHERITED_SERIAL
		{
		public:


			/* Captures the inherited class constructor and creates this first BEFORE creating
			 * the Chimera version. In this way the class is instantiated correctly. The inherited
			 * constructor handles object creation and referencing locally. The user only needs to
			 * reference the chimera pointer. Clever I guess? */
			SerialClass(const int& channel, const void* params = nullptr) : CHIMERA_INHERITED_SERIAL(channel)
			{
				this->serialChannel = channel;
			}

			~SerialClass() = default;

		private:
			int serialChannel = 0;
		};
        #endif
	}
}

#endif
