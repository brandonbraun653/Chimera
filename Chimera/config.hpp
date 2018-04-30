#pragma once
#ifndef CHIMERA_CONFIG_HPP
#define CHIMERA_CONFIG_HPP

/* All includes for peripheral drivers go here */
#include <Thor\include\thor.h>
#include <Thor\include\spi.h>
#include <Thor\include\gpio.h>
#include <Thor\include\serial.h>

/* Defines the classes for Chimera to inherit from */
#define CHIMERA_INHERITED_GPIO		Thor::Peripheral::GPIO::GPIOClass
#define CHIMERA_INHERITED_SPI		Thor::Peripheral::SPI::SPIClass
#define CHIMERA_INHERITED_SERIAL	Thor::Peripheral::Serial::SerialClass

#endif 