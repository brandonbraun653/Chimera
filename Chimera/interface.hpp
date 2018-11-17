#ifndef CHIMERA_INTERFACE_HPP
#define CHIMERA_INTERFACE_HPP

#include <stdlib.h>
#include <cstdint>

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
             *  @brief Writes data onto the SPI bus
             *  
             *  @param[in]   txBuffer    Data buffer to be sent
             *  @param[in]   length      Number of bytes to be sent
             *  @param[in]   disableCS   Optionally disable the chip select line after transmission complete
             *  @return Chimera::SPI::Status 
             */
            virtual Chimera::SPI::Status writeBytes(const uint8_t *const txBuffer, size_t length, const bool &disableCS = true) = 0;

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

            virtual Chimera::SPI::Status queueTransfers(const uint8_t **const txBuffers)
            {
                //TODO: Same as other, but with an array of the struct type (more memory efficient)

                return Chimera::SPI::Status::NOT_SUPPORTED;
            }

            virtual Chimera::SPI::Status writeSlaveQueue(const uint8_t *const txBuffer, size_t length)
            {
                return Chimera::SPI::Status::NOT_SUPPORTED;
            }

            /**
             *  @brief Reads data from the SPI bus
             *  
             *  @param[in]   rxBuffer    Data buffer to read into 
             *  @param[in]   length      Number of bytes to read 
             *  @param[in]   disableCS   Optionally disable the chip select line after transmission complete
             *  @return Chimera::SPI::Status 
             */
            virtual Chimera::SPI::Status readBytes(uint8_t * const rxBuffer, size_t length, const bool & disableCS = true) = 0;

            /**
             *  @brief Reads a number of bytes out from the internal slave receive buffer
             *  
             *  @param[in]   rxBuffer    Data buffer to read into 
             *  @param[in]   length      Number of bytes to read
             *  @return Chimera::SPI::Status 
             */
            virtual Chimera::SPI::Status readSlaveQueue(const uint8_t *const rxBuffer, size_t length)
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
            virtual Chimera::SPI::Status readWriteBytes(const uint8_t * const txBuffer, uint8_t * const rxBuffer, size_t length, const bool& disableCS = true) = 0;

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

            /**
             *  @brief Allows the user to assign a callback function to execute when in slave mode and
             *         data arrives on the bus that needs to be handled. 
             *  
             *  @param[in]  func            Callback function
             *  @param[in]  byteThreshold   Set which byte the callback is executed on
             *  @return Chimera::SPI::Status
             */
            virtual Chimera::SPI::Status onSlaveReceiveCallback(const Chimera::void_func_void func, const size_t &byteThreshold = 1)
            {
                return Chimera::SPI::Status::NOT_SUPPORTED;
            }

            /**
             *  @brief Initializes the transmit FIFO queue
             *  
             *  This feature is used when buffering data in Slave mode is desireable. On
             *  some systems this may not be possible due to lack of memory management
             *  or physical flash size.
             *
             *  @param[in]  size    The number of bytes to initialize the queue to
             */
            virtual Chimera::SPI::Status setSlaveTransmitQueueSize(const size_t &size)
            {
                return Chimera::SPI::Status::NOT_SUPPORTED;
            }

            /**
             *  @brief Initializes the receive FIFO queue
             *  
             *  This feature is used when buffering data in Slave mode is desireable. On
             *  some systems this may not be possible due to lack of memory management
             *  or physical flash size.
             *
             *  @param[in]  size    The number of bytes to initialize the queue to
             */
            virtual Chimera::SPI::Status setSlaveReceiveQueueSize(const size_t &size)
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