#pragma once
#ifndef CHIMERA_TYPES_HPP
#define CHIMERA_TYPES_HPP

#include <stdint.h>

#include <Chimera\preprocessor.hpp>

/** @namespace Chimera */
namespace Chimera
{
	/** @namespace Chimera::GPIO */
	namespace GPIO
	{
		enum Status : uint8_t
		{
			GPIO_OK,
			GPIO_ERROR_UNINITIALIZED,
			GPIO_ERROR_INVALID_PIN,
			GPIO_ERROR_INVALID_FUNCTION,
			GPIO_ERROR_INVALID_PORT
		};

		enum Mode : uint8_t
		{
			INPUT,
			OUTPUT,
			HIZ
		};
		
		enum State : bool 
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
		
		enum Port : uint8_t
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

		enum Status
		{
			INVALID_HARDWARE_PARAM = -1,
			FAILED_INITIALIZATION = -2,

			SPI_OK = 0,
			SPI_BUSY
		};

		enum BitOrder
		{
			MSB_FIRST,	/* Most significant bit sent first */
			LSB_FIRST	/* Least significant bit sent first */
		};

		enum Mode
		{
			MODE0,	
			MODE1,
			MODE2,
			MODE3,
			MODE4
		};
		
		enum TXRXMode
		{
			BLOCKING,
			INTERRUPT,
			DMA
		};

		struct Setup
		{
			uint32_t clockFrequency;
			BitOrder bitOrder;
			Mode mode;
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
			SERIAL_OK,
			SERIAL_ERROR,
			SERIAL_LOCKED,
			SERIAL_NOT_INITIALIZED,
			SERIAL_TX_IN_PROGRESS,
			SERIAL_RX_IN_PROGRESS,
			SERIAL_NOT_READY,
			SERIAL_PACKET_TOO_LARGE_FOR_BUFFER,
			SERIAL_TIMEOUT,
			SERIAL_UNKNOWN_ERROR
		};

		/** Supported communication baudrates */
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

		/** Common peripheral transmit and receive communication modes */
		enum class Modes : uint8_t
		{
			MODE_UNDEFINED,
			BLOCKING,
			INTERRUPT,
			DMA
		};

		/** Explicitly defines a peripheral type for different member functions of SerialClass */
		enum class SubPeripheral : bool
		{
			RX = false,
			TX = true
		};

	}

}



#endif 