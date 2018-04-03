#pragma once
#ifndef CHIMERA_SPI_HPP
#define CHIMERA_SPI_HPP

#include <Chimera/preprocessor.hpp>
#include <Chimera/types.hpp>
#include <Chimera/chimera_config.hpp>

#include <stdarg.h>

namespace Chimera
{
	using namespace Chimera::Types;

	class SPI : public CHIMERA_SPI_INHERITED
	{
	public:
		int init(uint8_t channel, const ParamVec* additionalParams)
		{
			// Nevermind...make a single struct with common features and leave details
			// up to the actual embedded system
			return CHIMERA_SPI_INHERITED::init(channel, additionalParams);
		}

		// How do I want to pass in core things like speed, msb, etc?

		// All the write functionalities

		// All the read functionalities

		// Read write functionalities

		// Mode functionalities: Blocking, Interrupt, DMA

		SPI() = default;
		~SPI() = default;

	private:

	};
}




#endif 