#pragma once
#ifndef CHIMERA_SPI_HPP
#define CHIMERA_SPI_HPP

#include <Chimera/chimera.hpp>

namespace Chimera
{
	using namespace Chimera::SPI;

	class SPIClass : public CHIMERA_SPI_INHERITED
	{
	public:
		Status init(uint8_t channel, const Setup& setupStruct)
		{
			return spi->init(channel, setupStruct);
		}

		// How do I want to pass in core things like speed, msb, etc?

		// All the write functionalities

		// All the read functionalities

		// Read write functionalities

		// Mode functionalities: Blocking, Interrupt, DMA

		SPIClass()
		{
			spi = Chimera::make_shared<CHIMERA_SPI_INHERITED>();
		};

		~SPIClass() = default;

	private:
		Chimera::shared_ptr<CHIMERA_SPI_INHERITED> spi;
	};
	
}




#endif 