#pragma once
#ifndef CHIMERA_CONFIG_HPP
#define CHIMERA_CONFIG_HPP

/* All code for peripheral drivers go here */
#include <Thor\include\spi.h>


#define CH_INHRT_SPI Thor::Peripheral::SPI::SPIClass
#define CHIMERA_I2C_INHERITED null
#define CHIMERA_UART_INHERITED null


#endif 