#pragma once
#ifndef CHIMERA_SERIAL_HPP
#define CHIMERA_SERIAL_HPP

/* C++ Includes */
#include <cstdlib>
#include <cstdint>
#include <memory>

/* Chimera Includes */
#include <Chimera/chimera.hpp>
#include <Chimera/config.hpp>

namespace Chimera
{
	namespace Serial
	{
    	class SerialClass;
    	typedef std::shared_ptr<SerialClass> SerialClass_sPtr;
    	typedef std::unique_ptr<SerialClass> SerialClass_uPtr;


		class SerialClass : public CHIMERA_INHERITED_SERIAL
		{
		public:

			SerialClass(const uint8_t channel) : CHIMERA_INHERITED_SERIAL(channel)
			{
				serialChannel = channel;
			}

			~SerialClass() = default;

		private:
			uint8_t serialChannel;
		};
	}
}

#endif
