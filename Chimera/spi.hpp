#pragma once
#ifndef CHIMERA_SPI_HPP
#define CHIMERA_SPI_HPP

#include <Chimera/chimera.hpp>
#include <Chimera/config.hpp>

#include <boost/shared_ptr.hpp>
#include <boost/make_shared.hpp>

namespace Chimera
{
	using namespace Chimera::SPI;

	class SPIClass : public CH_INHRT_SPI
	{
	public:
		Status init(const Setup& setupStruct)
		{
			return spi->init(usr_channel, setupStruct);
		}
		
		Status write(uint8_t* in, size_t length);
		Status write(uint8_t* in, uint8_t* out, size_t length);

		Status setTxMode(TXRXMode mode);
		Status setRxMode(TXRXMode mode);

		SPIClass(const int& channel)
		{
			usr_channel = channel;
			spi = boost::make_shared<CH_INHRT_SPI>(channel);
		};

		~SPIClass() = default;

	private:
		int usr_channel;
		boost::shared_ptr<CH_INHRT_SPI> spi;
	};
	//typedef Chimera::shared_ptr<SPIClass> SPIClass_sPtr;
}




#endif 