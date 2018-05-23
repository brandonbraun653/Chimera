#pragma once
#ifndef CHIMERA_CONFIG_HPP
#define CHIMERA_CONFIG_HPP

/* All includes for peripheral drivers go here */
#include <Thor\include\thor.hpp>
#include <Thor\include\spi.hpp>
#include <Thor\include\gpio.hpp>
#include <Thor\include\serial.hpp>

/* Defines the classes for Chimera to inherit from */
#define CHIMERA_INHERITED_GPIO		Thor::Peripheral::GPIO::GPIOClass
#define CHIMERA_INHERITED_SPI		Thor::Peripheral::SPI::SPIClass
#define CHIMERA_INHERITED_SERIAL	Thor::Peripheral::Serial::SerialClass

#endif 