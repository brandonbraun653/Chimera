#pragma once
#ifndef CHIMERA_CONFIG_HPP
#define CHIMERA_CONFIG_HPP

/* All includes for peripheral drivers go here */
#include <Thor/include/thor.hpp>
#include <Thor/include/spi.hpp>
#include <Thor/include/gpio.hpp>
#include <Thor/include/serial.hpp>

/* */
#define CHIMERA_INHERITED_GLOBAL_NAMESPACE Thor

/* Defines the classes for Chimera to inherit from */
typedef Thor::Peripheral::GPIO::ChimeraGPIO		CHIMERA_INHERITED_GPIO;
typedef Thor::Peripheral::SPI::ChimeraSPI		CHIMERA_INHERITED_SPI;
typedef	Thor::Peripheral::Serial::SerialClass	CHIMERA_INHERITED_SERIAL;

#endif
