#pragma once
#ifndef CHIMERA_SPI_HPP
#define CHIMERA_SPI_HPP

/* Boost Includes */
#include <boost/shared_ptr.hpp>
#include <boost/make_shared.hpp>

/* Chimera Includes */
#include <Chimera/chimera.hpp>
#include <Chimera/config.hpp>

namespace Chimera
{
	namespace SPI
	{
		class SPIClass : public CHIMERA_INHERITED_SPI
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
				spi = boost::make_shared<CHIMERA_INHERITED_SPI>(channel);
			}
			;

			~SPIClass() = default;

		private:
			int usr_channel;
			boost::shared_ptr<CHIMERA_INHERITED_SPI> spi;
		};
		typedef boost::shared_ptr<SPIClass> SPIClass_sPtr;
	}
}

#endif 