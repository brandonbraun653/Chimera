#pragma once
#ifndef CHIMERA_CONFIG_HPP
#define CHIMERA_CONFIG_HPP

/* All code for peripheral drivers go here */
#include "dummy_ext_spi.hpp"


#define CHIMERA_SPI_INHERITED DummySPI
#define CHIMERA_I2C_INHERITED null
#define CHIMERA_UART_INHERITED null


#endif 