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
			Status begin(const Setup& setupStruct)
			{
				return spi->init(usr_channel, setupStruct);
			}

			Status write(uint8_t* data_in, size_t length, const bool& ssDisableAfterTX = true)
			{
				return CHIMERA_INHERITED_SPI::cwrite(data_in, length, ssDisableAfterTX);
			}
			
			Status write(uint8_t* data_in, uint8_t* data_out, size_t length, const bool& ssDisableAfterTX = true)
			{
				return CHIMERA_INHERITED_SPI::cwrite(data_in, data_out, length, ssDisableAfterTX);
			}

			Status setMode(TXRXMode mode);

			SPIClass(const int& channel)
			{
				usr_channel = channel;
				spi = boost::make_shared<CHIMERA_INHERITED_SPI>(channel);
			};

			~SPIClass() = default;

		private:
			int usr_channel;
			boost::shared_ptr<CHIMERA_INHERITED_SPI> spi;
		};
		typedef boost::shared_ptr<SPIClass> SPIClass_sPtr;
		
		
		CLASS_METHOD_CHECKER(has_cwrite1, CHIMERA_INHERITED_SPI, cwrite, Status, uint8_t*, size_t, const bool&);
		CLASS_METHOD_CHECKER(has_cwrite2, CHIMERA_INHERITED_SPI, cwrite, Status, uint8_t*, uint8_t*, size_t, const bool&);
	}
}

#endif 