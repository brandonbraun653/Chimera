#pragma once
#ifndef CHIMERA_TYPES_HPP
#define CHIMERA_TYPES_HPP

#include <stdint.h>

#include <Chimera\preprocessor.hpp>


namespace Chimera
{
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

	namespace I2C
	{

	}

	namespace Serial
	{

	}

}



#endif 