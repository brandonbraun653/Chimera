#pragma once
#ifndef CHIMERA_TYPES_HPP
#define CHIMERA_TYPES_HPP

#include <stdint.h>

#include <Chimera/preprocessor.hpp>

/** @namespace Chimera */
namespace Chimera
{
    typedef void (*void_func_void)(void);
    typedef void (*void_func_void_ptr)(void *);
    typedef void (*void_func_uint32_t)(uint32_t);

    /** @namespace Chimera::GPIO */
	namespace GPIO
	{
		enum class Status : uint8_t
		{
			OK,
			ERROR_UNINITIALIZED,
			ERROR_INVALID_PIN,
			ERROR_INVALID_FUNCTION,
			ERROR_INVALID_PORT
		};

		enum class Mode : uint8_t
		{
			INPUT,
			OUTPUT_PUSH_PULL,
			OUTPUT_OPEN_DRAIN,
			ALTERNATE_PUSH_PULL,
			ALTERNATE_OPEN_DRAIN,
			ANALOG,
			HIZ
		};
		
		enum class State : bool 
		{
			HIGH = true,
			HI = true,
			ON = true,
			TRUE = true,
			LOW = false,
			LO = false,
			OFF = false,
			FALSE = false
		};
		
		enum class Port : uint8_t
		{
			PORTA,
			PORTB,
			PORTC,
			PORTD,
			PORTE,
			PORTF,
			PORTG,
			PORTH,
			PORTI,
			PORTJ,
			PORTK,
			PORTL
		};
	}

	/** @namespace Chimera::SPI */
	namespace SPI
	{
        enum class Status : int
		{
			INVALID_HARDWARE_PARAM = -1,
			FAILED_INITIALIZATION = -2,

			OK = 0,
			BUSY,
			LOCKED,
			NOT_INITIALIZED,
			ERROR,
			NOT_READY,
            NOT_SUPPORTED,
			TX_IN_PROGRESS,
			RX_IN_PROGRESS,
			PACKET_TOO_LARGE_FOR_BUFFER,
			TIMEOUT,
			UNKNOWN_STATUS_CODE
		};

		enum class Mode : uint8_t
		{
			MASTER,
			SLAVE
		};
		
		enum class BitOrder : uint8_t
		{
			MSB_FIRST,	
			LSB_FIRST	
		};

		enum class ClockMode : uint8_t
		{	
			MODE0,	/**< CPOL=0, CPHA=0 */
			MODE1,	/**< CPOL=0, CPHA=1 */
			MODE2,	/**< CPOL=1, CPHA=0 */
			MODE3	/**< CPOL=1, CPHA=1 */
		};

		enum class DataSize : uint8_t
		{
			SZ_8BIT,
			SZ_9BIT,
			SZ_10BIT,
			SZ_11BIT,
			SZ_12BIT,
			SZ_13BIT,
			SZ_14BIT,
			SZ_15BIT,
			SZ_16BIT,
		};

		enum class SubPeripheral : uint8_t
		{
			TX,
			RX,
			TXRX
		};

		enum class SubPeripheralMode : uint8_t
		{
			UNKOWN_MODE,
			BLOCKING,
			INTERRUPT,
			DMA
		};

		struct Setup
		{
            Mode mode				= Mode::MASTER;
            DataSize dataSize		= DataSize::SZ_8BIT;
            BitOrder bitOrder		= BitOrder::MSB_FIRST;
            ClockMode clockMode		= ClockMode::MODE0;
            uint32_t clockFrequency = 1000000;
		};
	}

	/** @namespace Chimera::I2C */
	namespace I2C
	{

	}

	/** @namespace Chimera::Serial */
	namespace Serial
	{
		enum class Status : uint8_t
		{
			OK,
			ERROR,
			LOCKED,
			NOT_INITIALIZED,
			TX_IN_PROGRESS,
			RX_IN_PROGRESS,
			NOT_READY,
			PACKET_TOO_LARGE_FOR_BUFFER,
			TIMEOUT,
			UNKNOWN_ERROR
		};

		enum class BaudRate : uint32_t
		{
			SERIAL_BAUD_110 = 100u,
			SERIAL_BAUD_150 = 150u,
			SERIAL_BAUD_300 = 300u,
			SERIAL_BAUD_1200 = 1200u,
			SERIAL_BAUD_2400 = 2400u,
			SERIAL_BAUD_4800 = 4800u,
			SERIAL_BAUD_9600 = 9600u,
			SERIAL_BAUD_19200 = 19200u,
			SERIAL_BAUD_38400 = 38400u,
			SERIAL_BAUD_57600 = 57600u,
			SERIAL_BAUD_115200 = 115200u,
			SERIAL_BAUD_230400 = 230400u,
			SERIAL_BAUD_460800 = 460800u,
			SERIAL_BAUD_921600 = 921600u
		};

		enum class Modes : uint8_t
		{
			MODE_UNDEFINED,
			BLOCKING,
			INTERRUPT,
			DMA
		};

		enum class SubPeripheral : bool
		{
			RX = false,
			TX = true
		};

	}

    /** @namespace Chimera::FreeRTOS */
    namespace FreeRTOS
    {
        enum class SPIEvent : uint8_t
        {
            RX_COMPLETE,            /**< Receive complete */
            QUEUED_RX_COMPLETE,     /**< Queued receive completed */
            TX_COMPLETE,            /**< Transmit complete */
            QUEUED_TX_COMPLETE,     /**< Queued transmit completed */
            TXRX_COMPLETE,          /**< Transmit receive operation complete */
            QUEUED_TX_RX_COMPLETE,  /**< Queued transfer completed */
            SLAVE_RX_BYTE,          /**< Slave receive buffer got a byte */
            SLAVE_RX_HALF,          /**< Slave receive buffer half full */
            SLAVE_RX_FULL,          /**< Slave receive buffer full */
            ERROR
        };
    }
}



#endif 