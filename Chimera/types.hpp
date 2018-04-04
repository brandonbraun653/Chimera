#pragma once
#ifndef CHIMERA_TYPES_HPP
#define CHIMERA_TYPES_HPP

#include <stdint.h>

#include <Chimera\preprocessor.hpp>


namespace Chimera
{
	
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