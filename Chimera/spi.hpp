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
				return CHIMERA_INHERITED_SPI::begin(setupStruct);
			}

			SPI::Status write(uint8_t* data_in, size_t length, const bool& ssDisableAfterTX = true)
			{
				return CHIMERA_INHERITED_SPI::write(data_in, length, ssDisableAfterTX);
			}
			
			SPI::Status write(uint8_t* data_in, uint8_t* data_out, size_t length, const bool& ssDisableAfterTX = true)
			{
				return CHIMERA_INHERITED_SPI::write(data_in, data_out, length, ssDisableAfterTX);
			}

			SPI::Status setMode(SubPeripheral periph, SubPeripheralMode mode)
			{
				return CHIMERA_INHERITED_SPI::setMode(periph, mode);
			}
			
			SPI::Status updateClockFrequency(uint32_t freq)
			{
				return CHIMERA_INHERITED_SPI::updateClockFrequency(freq);
			}

			uint32_t getClockFrequency()
			{
				return CHIMERA_INHERITED_SPI::getClockFrequency();
			}

			void writeSS(Chimera::GPIO::State value)
			{
				CHIMERA_INHERITED_SPI::writeSS(value);
			}
			
			SPIClass(const int& channel) : CHIMERA_INHERITED_SPI( channel )
			{

			};

			~SPIClass() = default;

		private:

		};
		typedef std::shared_ptr<SPIClass> SPIClass_sPtr;
		typedef std::unique_ptr<SPIClass> SPIClass_uPtr;
	}
}

#endif 