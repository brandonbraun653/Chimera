#pragma once
#ifndef DUMMY_SPI_HPP
#define DUMMY_SPI_HPP

#include <stdlib.h>
#include <stdint.h>
#include <map>
#include <string>
#include <iostream>

#include <boost/variant.hpp>

class DummySPI
{
public:

	int init(uint8_t channel, const std::map<std::string, boost::variant<int, double>>& options)
	{
		//Assume all the keys are there
		int speed = boost::get<int>(options.find("speed")->second);
		double opt1 = boost::get<double>(options.find("opt1")->second);

		return 1;
	}


	DummySPI() = default;
	~DummySPI() = default;
private:

	int var = 0;

};



#endif 