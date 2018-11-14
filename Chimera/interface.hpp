#ifndef CHIMERA_INTERFACE_HPP
#define CHIMERA_INTERFACE_HPP

#include <Chimera/types.hpp>
#include <Chimera/preprocessor.hpp>

namespace Chimera
{
    namespace SPI
    {
        class Interface
        {
        public:
            virtual SPI::Status begin(const Setup& setupStruct) = 0;

            virtual SPI::Status write(uint8_t* data_in, size_t length, const bool& ssDisableAfterTX = true) = 0;

            virtual SPI::Status write(uint8_t* data_in, uint8_t* data_out, size_t length, const bool& ssDisableAfterTX = true) = 0;

            virtual SPI::Status setMode(SubPeripheral periph, SubPeripheralMode mode) = 0;

            virtual SPI::Status updateClockFrequency(uint32_t freq) = 0;

            virtual uint32_t getClockFrequency() = 0;

            virtual void writeSS(Chimera::GPIO::State value) = 0;

            //TODO: Implement freertos stub functions (attachThreadTrigger)
        };
    }
}

#endif /* !CHIMERA_INTERFACE_HPP */