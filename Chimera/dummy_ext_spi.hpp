#pragma once
#ifndef DUMMY_SPI_HPP
#define DUMMY_SPI_HPP

#include <stdlib.h>
#include <stdint.h>

#include <Chimera\chimera.hpp>

using namespace Chimera::SPI;

class DummySPI
{
public:

	Status init(uint8_t channel, const Setup& setupStruct)
	{

		return Status::SPI_OK;
	}


	DummySPI() = default;
	~DummySPI() = default;
private:

	int var = 0;

};



#endif 