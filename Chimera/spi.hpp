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
			SPI::Status begin(const Setup& setupStruct)
			{
				return spi->cbegin(setupStruct);
			}

			SPI::Status write(uint8_t* data_in, size_t length, const bool& ssDisableAfterTX = true)
			{
				return spi->cwrite(data_in, length, ssDisableAfterTX);
			}
			
			SPI::Status write(uint8_t* data_in, uint8_t* data_out, size_t length, const bool& ssDisableAfterTX = true)
			{
				return spi->cwrite(data_in, data_out, length, ssDisableAfterTX);
			}

			SPI::Status setMode(SubPeripheral periph, SubPeripheralMode mode)
			{
				return spi->csetMode(periph, mode);
			}
			
			SPI::Status updateClockFrequency(uint32_t freq)
			{
				return spi->cupdateClockFrequency(freq);
			}
			
			SPIClass(const int& channel)
			{
				spi = CHIMERA_INHERITED_SPI::create(channel);
			};

			~SPIClass() = default;

		private:
			boost::shared_ptr<CHIMERA_INHERITED_SPI> spi;
		};
		typedef boost::shared_ptr<SPIClass> SPIClass_sPtr;
		
		CLASS_METHOD_CHECKER(has_cbegin, CHIMERA_INHERITED_SPI, cbegin, Status, const Setup&);
		CLASS_METHOD_CHECKER(has_cwrite1, CHIMERA_INHERITED_SPI, cwrite, Status, uint8_t*, size_t, const bool&);
		CLASS_METHOD_CHECKER(has_cwrite2, CHIMERA_INHERITED_SPI, cwrite, Status, uint8_t*, uint8_t*, size_t, const bool&);
		CLASS_METHOD_CHECKER(has_csetMode, CHIMERA_INHERITED_SPI, csetMode, Status, SubPeripheral, SubPeripheralMode);
		CLASS_METHOD_CHECKER(has_cupdateClockFrequency, CHIMERA_INHERITED_SPI, cupdateClockFrequency, Status, uint32_t);
	}
}

#endif 