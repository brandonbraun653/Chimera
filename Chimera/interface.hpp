#ifndef CHIMERA_INTERFACE_HPP
#define CHIMERA_INTERFACE_HPP

#include <stdint.h>
#include <stdlib.h>

#include <Chimera/types.hpp>
#include <Chimera/preprocessor.hpp>

namespace Chimera
{
    namespace SPI
    {
        class Interface
        {
        public:
            /**
             * @brief Reserves the SPI hardware to allow unobstructed use
             * 
             * @return true if the reservation succeeded, false if not
             */
            virtual bool reserve() = 0;

            /**
             * @brief Releases a previous reservation
             * 
             * @return true if the release succeeded, false if not
             */
            virtual bool release() = 0;

            /**
             * @brief Initializes the SPI hardware according to the setup struct
             * 
             * @param[in]   setupStruct     Contains information on how to initialize SPI
             * @return Chimera::SPI::Status 
             */
            virtual Chimera::SPI::Status init(const Chimera::SPI::Setup& setupStruct) = 0;

            /**
             * @brief Writes data onto the SPI bus
             * 
             * @param[in]   txBuffer    Data buffer to be sent
             * @param[in]   length      Number of bytes to be sent
             * @param[in]   disableCS   Optionally disable the chip select line after transmission complete
             * @return Chimera::SPI::Status 
             */
            virtual Chimera::SPI::Status write(uint8_t * txBuffer, size_t length, const bool& disableCS = true) = 0;

            /**
             * @brief Reads data from the SPI bus
             * 
             * @param[in]   rxBuffer    Data buffer to read into 
             * @param[in]   length      Number of bytes to read 
             * @param[in]   disableCS   Optionally disable the chip select line after transmission complete
             * @return Chimera::SPI::Status 
             */
            virtual Chimera::SPI::Status read(uint8_t * rxBuffer, size_t length, const bool& disableCS = true) = 0;

            /**
             * @brief Transmits and receives data on the SPI bus
             * 
             * @param[in]   txBuffer    Data buffer to write from
             * @param[out]  rxBuffer    Data buffer to read into
             * @param[in]   length      Number of bytes to transfer
             * @param[in]   disableCS   Optionally disable the chip select line after transmission complete
             * @return Chimera::SPI::Status 
             */
            virtual Chimera::SPI::Status transfer(uint8_t * txBuffer, uint8_t * rxBuffer, size_t length, const bool& disableCS = true) = 0;

            /**
             * @brief Set the hardware peripheral operational mode. 
             * This should allow the user to choose if the hardware operates in blocking, interrupt, or DMA mode
             * 
             * @param[in]   periph      The peripheral to set the behavior on
             * @param[in]   mode        Desired operational mode of the peripheral
             * @return Chimera::SPI::Status 
             */
            virtual Chimera::SPI::Status setPeripheralMode(Chimera::SPI::SubPeripheral periph, Chimera::SPI::SubPeripheralMode mode) = 0;

            /**
             * @brief Change the frequency of the SPI output clock during runtime
             * 
             * @param[in]   freq    Desired SPI clock frequency in Hz
             * @return Chimera::SPI::Status 
             */
            virtual Chimera::SPI::Status setClockFrequency(uint32_t freq) = 0;

            /**
             * @brief Get the current SPI clock frequency
             * 
             * @param[out]  freq    Reported SPI clock
             * @return Chimera::SPI::Status
             */
            virtual Chimera::SPI::Status getClockFrequency(uint32_t& freq) = 0;

            /**
             * @brief Sets the chip select line logical state
             * 
             * @param[in]   value   The state to set the chip select to
             */
            virtual void writeSS(Chimera::GPIO::State value) = 0;

            //TODO: Implement freertos stub functions (attachThreadTrigger)

            //TODO: Add transfer complete/error callbacks

            //TODO: Add feature not supported Status code.
        };
    }
}

#endif /* !CHIMERA_INTERFACE_HPP */