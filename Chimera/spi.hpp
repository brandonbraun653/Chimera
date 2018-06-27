#pragma once
#ifndef CHIMERA_SPI_HPP
#define CHIMERA_SPI_HPP

/* C/C++ Includes */
#include <memory>

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

			uint32_t getClockFrequency()
			{
				return spi->cgetClockFrequency();
			}

			void writeSS(Chimera::GPIO::State value)
			{
				spi->cwriteSS(value);
			}
			
			SPIClass(const int& channel)
			{
				#if defined(USING_BOOST)
				if (CHIMERA_INHERITED_SPI::usesBoost)
				{
					auto boost_spi = CHIMERA_INHERITED_SPI::create(channel);
					this->spi = Chimera::convert_shared_ptr(boost_spi);
				}
				#else
				this->spi = CHIMERA_INHERITED_SPI::create(channel);
				#endif
				
			};

			~SPIClass() = default;

		private:
			std::shared_ptr<CHIMERA_INHERITED_SPI> spi;
		};
		typedef std::shared_ptr<SPIClass> SPIClass_sPtr;
		typedef std::unique_ptr<SPIClass> SPIClass_uPtr;
		
		CLASS_METHOD_CHECKER(has_cbegin, CHIMERA_INHERITED_SPI, cbegin, Status, const Setup&);
		CLASS_METHOD_CHECKER(has_cwrite1, CHIMERA_INHERITED_SPI, cwrite, Status, uint8_t*, size_t, const bool&);
		CLASS_METHOD_CHECKER(has_cwrite2, CHIMERA_INHERITED_SPI, cwrite, Status, uint8_t*, uint8_t*, size_t, const bool&);
		CLASS_METHOD_CHECKER(has_csetMode, CHIMERA_INHERITED_SPI, csetMode, Status, SubPeripheral, SubPeripheralMode);
		CLASS_METHOD_CHECKER(has_cupdateClockFrequency, CHIMERA_INHERITED_SPI, cupdateClockFrequency, Status, uint32_t);
		CLASS_METHOD_CHECKER(has_cwritess, CHIMERA_INHERITED_SPI, cwriteSS, void, Chimera::GPIO::State);
		CLASS_METHOD_CHECKER(has_cgetClockFrequency, CHIMERA_INHERITED_SPI, cgetClockFrequency, uint32_t, void);
	}
}

#endif 