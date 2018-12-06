#ifndef CHIMERA_INTERFACE_HPP
#define CHIMERA_INTERFACE_HPP

/* C++ Includes */
#include <cstdlib>
#include <cstdint>
#include <array>

/* Chimera Includes */
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
             *  @brief Initializes the SPI hardware according to the setup struct
             *
             *  @param[in]   setupStruct     Contains information on how to initialize SPI
             *  @return Chimera::SPI::Status
             */
            virtual Chimera::SPI::Status init(const Chimera::SPI::Setup& setupStruct) = 0;

            /**
             *  @brief Sets the chip select GPIO to a logical state
             *
             *  @param[in]   value   The state to set the chip select to
             */
            virtual Chimera::SPI::Status setChipSelect(const Chimera::GPIO::State &value) = 0;

            /**
             *  @brief Instruct the chip select to behave in a specific manner
             *
             *  @param[in]  mode    The desired mode for the chip select to operate in
             *  @return Chimera::SPI::Status
             */
            virtual Chimera::SPI::Status setChipSelectControlMode(const Chimera::SPI::ChipSelectMode &mode) = 0;

            /**
             *  @brief Writes data onto the SPI bus
             *
             *  @param[in]  txBuffer    Data buffer to be sent
             *  @param[in]  length      Number of bytes to be sent
             *  @param[in]  disableCS   Optionally disable the chip select line after transmission complete
             *  @param[in]  autoRelease Optionally release the SPI HW lock should the caller hold ownership
             *  @param[in]  timeoutMS   If the hardware is not free, wait this amount of time before exiting
             *  @return Chimera::SPI::Status
             */
            virtual Chimera::SPI::Status writeBytes(const uint8_t *const txBuffer, size_t length,
                const bool &disableCS = true, const bool &autoRelease = false, uint32_t timeoutMS = 10) = 0;

            /**
             *  @brief Writes data on to the SPI bus
             *  Acts as a conventient mapping tool to allow the use of std::array to write data
             *  
             *  @param[in]  txBuffer    Data buffer to be sent
             *  @param[in]  disableCS   Optionally disable the chip select line after transmission complete
             *  @param[in]  autoRelease Optionally release the SPI HW lock should the caller hold ownership
             *  @param[in]  timeoutMS   If the hardware is not free, wait this amount of time before exiting
             *  @return Chimera::SPI::Status
             */
            template< typename T, std::size_t S >
				Chimera::SPI::Status writeBytes(const std::array<T, S>& txBuffer,
					const bool &disableCS = true, const bool &autoRelease = false, uint32_t timeoutMS = 10)
				{
					auto constexpr arr = static_cast<uint8_t*>(txBuffer.data());
					return writeBytes(arr, txBuffer.size(), disableCS, autoRelease, timeoutMS);   
				}

            /**
             *  @brief Write multiple buffers of data onto the bus
             *
             *  This is useful when you want to queue up multiple transmissions but the calling program
             *  needs to go off and do something else instead of managing the transfers.
             *
             *
             *  @return Chimera::SPI::Status
             */
            virtual Chimera::SPI::Status queueTransfers(const std::vector<uint8_t *const> &txBuffers)
            {
                //TODO: This likely will need to turn into a struct with transfer specific information

                return Chimera::SPI::Status::NOT_SUPPORTED;
            }

            /**
             *  @brief Write multiple buffers of data onto the bus
             *
             *  This is useful when you want to queue up multiple transmissions but the calling program
             *  needs to go off and do something else instead of managing the transfers.
             *
             *
             *  @return Chimera::SPI::Status
             */
            virtual Chimera::SPI::Status queueTransfers(const uint8_t **const txBuffers)
            {
                //TODO: Same as other, but with an array of the struct type (more memory efficient)

                return Chimera::SPI::Status::NOT_SUPPORTED;
            }

            /**
             *  @brief Write data to the slave queue that will be clocked out on the next transfer
             *
             *  @return Chimera::SPI::Status
             */
            virtual Chimera::SPI::Status writeSlaveQueue(const uint8_t *const txBuffer, size_t length)
            {
                return Chimera::SPI::Status::NOT_SUPPORTED;
            }

            /**
             *  @brief Reads data from the SPI bus
             *
             *  Internally,the function will call readWriteBytes() and use the rxBuffer as the source of
             *  bytes to transmit on the MOSI line.
             *
             *  Master
             *  ==================
             *  This section describes the behavior of readBytes() when SPI is configured as a master.
             *
             *      Blocking Mode
             *      ------------------
             *      When called in blocking mode, the SPI hardware will attempt to read a number of bytes from the bus
             *      with an enforced timeout. Should the requested number of bytes not arrive before the timeout expires,
             *      an error code is returned indicating the timeout.
             *
             *      Interrupt Mode
             *      ------------------
             *      When using interrupt mode, an ISR handles the transfer. If supported by the underlying driver, the
             *      user can attach callbacks to be notified when the transfer is complete [onReadCompleteCallback()] or
             *      when an error occurs [onErrorCallback()].
             *
             *      DMA Mode
             *      ------------------
             *      The DMA hardware will handle the transfer. If supported by the underlying driver, the
             *      user can attach callbacks to be notified when the transfer is complete [onReadCompleteCallback()] or
             *      when an error occurs [onErrorCallback()].
             *
             *
             *  Slave
             *  ==================
             *  This section describes the behavior of readBytes() when SPI is configured as a slave.
             *
             *      Blocking Mode
             *      ------------------
             *      When called in blocking mode, the SPI hardware will attempt to read a number of bytes from MOSI
             *      with an enforced timeout. The corresponding bytes clocked out on MISO will be any data contained
             *      in the receive buffer before the transfer starts. Should the requested number of bytes not arrive
             *      before the timeout expires or the slave select line is not asserted, an error code is returned
             *      indicating the timeout.
             *
             *      Interrupt Mode
             *      ------------------
             *
             *
             *      DMA Mode
             *      ------------------
             *
             *
             *  @param[out]  rxBuffer    Data buffer to read into
             *  @param[in]   length      Number of bytes to read
             *  @param[in]   disableCS   Optionally disable the chip select line after transmission complete
             *  @return Chimera::SPI::Status
             */
            virtual Chimera::SPI::Status readBytes(uint8_t * const rxBuffer, size_t length,
                const bool & disableCS = true, const bool &autoRelease = false, uint32_t timeoutMS = 10) = 0;
			
			/**
			 *	@brief A templated version of readBytes() for more modern C++ compatibility
		     *	@see readBytes()
			 */
			template< typename T, std::size_t S>
				Chimera::SPI::Status readBytes(std::array<T, S> rxBuffer,
					const bool & disableCS = true, const bool &autoRelease = false, uint32_t timeoutMS = 10)
				{
    				auto constexpr array = static_cast<uint8_t*>(rxBuffer.data());
    				return readBytes(array, rxBuffer.size(), disableCS, autoRelease, timeoutMS);
				}

            /**
             *  @brief Reads a number of bytes out from the internal slave receive buffer
             *
             *  @param[in]   rxBuffer    Data buffer to read into
             *  @param[in]   length      Number of bytes to read
             *  @return Chimera::SPI::Status
             */
            virtual Chimera::SPI::Status readSlaveQueue(uint8_t *const rxBuffer, size_t length)
            {
                return Chimera::SPI::Status::NOT_SUPPORTED;
            }

            /**
             *  @brief Transmits and receives data on the SPI bus
             *
             *  @param[in]   txBuffer    Data buffer to write from
             *  @param[out]  rxBuffer    Data buffer to read into
             *  @param[in]   length      Number of bytes to transfer
             *  @param[in]   disableCS   Optionally disable the chip select line after transmission complete
             *  @return Chimera::SPI::Status
             */
            virtual Chimera::SPI::Status readWriteBytes(const uint8_t * const txBuffer, uint8_t * const rxBuffer, size_t length,
                const bool& disableCS = true, const bool &autoRelease = false, uint32_t timeoutMS = 10) = 0;

            /**
             *  @brief Set the hardware peripheral operational mode.
             *  This should allow the user to choose if the hardware operates in blocking, interrupt, or DMA mode
             *
             *  @param[in]   periph      The peripheral to set the behavior on
             *  @param[in]   mode        Desired operational mode of the peripheral
             *  @return Chimera::SPI::Status
             */
            virtual Chimera::SPI::Status setPeripheralMode(const Chimera::SPI::SubPeripheral &periph, const Chimera::SPI::SubPeripheralMode &mode) = 0;

            /**
             *  @brief Change the frequency of the SPI output clock during runtime
             *
             *  @param[in]   freq    Desired SPI clock frequency in Hz
             *  @return Chimera::SPI::Status
             */
            virtual Chimera::SPI::Status setClockFrequency(const uint32_t &freq) = 0;

            /**
             *  @brief Get the current SPI clock frequency
             *
             *  @param[out]  freq    Reported SPI clock
             *  @return Chimera::SPI::Status
             */
            virtual Chimera::SPI::Status getClockFrequency(uint32_t * const freq) = 0;

            /**
             *  @brief Reserves the SPI hardware to allow unobstructed use
             *
             *  @param[in]  timeout_ms  How many milliseconds to wait for the hardware to become available
             *  @return Chimera::SPI::Status
             */
            virtual Chimera::SPI::Status reserve(const uint32_t &timeout_ms = 0u) = 0;

            /**
             *  @brief Releases a previous reservation
             *
             *  @param[in]  timeout_ms  How many milliseconds to wait for the hardware to release
             *  @return Chimera::SPI::Status
             */
            virtual Chimera::SPI::Status release(const uint32_t &timeout_ms = 0u ) = 0;

            /**
             *  @brief Allows the user to assign a callback function to the write complete event
             *
             *  @param[in]  func  Callback function
             *  @return Chimera::SPI::Status
             */
            virtual Chimera::SPI::Status onWriteCompleteCallback(const Chimera::void_func_void func)
            {
                return Chimera::SPI::Status::NOT_SUPPORTED;
            }

            /**
             *  @brief Allows the user to assign a callback function to the read complete event
             *
             *  @param[in]  func  Callback function
             *  @return Chimera::SPI::Status
             */
            virtual Chimera::SPI::Status onReadCompleteCallback(const Chimera::void_func_void func)
            {
                return Chimera::SPI::Status::NOT_SUPPORTED;
            }

            /**
             *  @brief Allows the user to assign a callback function to the read-write complete event
             *
             *  @param[in]  func  Callback function
             *  @return Chimera::SPI::Status
             */
            virtual Chimera::SPI::Status onReadWriteCompleteCallback(const Chimera::void_func_void func)
            {
                return Chimera::SPI::Status::NOT_SUPPORTED;
            }

            /**
             *  @brief Allows the user to assign a callback function on an error event
             *
             *  The function will be passed an error code indicating what happened
             *
             *  @param[in]  func  Callback function
             *  @return Chimera::SPI::Status
             */
            virtual Chimera::SPI::Status onErrorCallback(const Chimera::void_func_uint32_t func)
            {
                return Chimera::SPI::Status::NOT_SUPPORTED;
            }

#if defined(USING_FREERTOS)

            /**
             *  @brief Allows the user to have a semaphore given to when an event occurs
             *
             *  @param[in]  event   The event to be waiting on
             *  @param[in]  semphr  The semaphore to be given to upon event occurance
             *  @return Chimera::SPI::Status
             */
            virtual Chimera::SPI::Status attachEventWakeup(const Chimera::FreeRTOS::SPIEvent &event, const SemaphoreHandle_t *const semphr) = 0;

            /**
             *  @brief Removes a semaphore from the event wakeup list
             *
             *  @param[in]  semphr  The semaphore to be removed
             *  @return Chimera::SPI::Status
             */
            virtual Chimera::SPI::Status removeEventWakeup(const SemaphoreHandle_t *const semphr) = 0;

            #endif /* !USING_FREERTOS */
        };
    }
}

#endif /* !CHIMERA_INTERFACE_HPP */
