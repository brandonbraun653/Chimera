/********************************************************************************
 *  File Name:
 *    chimeraPort.hpp
 *
 *  Description:
 *    Implements the Chimera back end driver redirects into the mocking framework
 *
 *   2019 | Brandon Braun | brandonbraun653@gmail.com
 ********************************************************************************/
#pragma once
#ifndef CHIMERA_PORT_MOCK_HPP
#define CHIMERA_PORT_MOCK_HPP

#include <Chimera/mock/gpio.hpp>
#include <Chimera/mock/spi.hpp>

namespace BACKEND_NAMESPACE = Chimera::Mock;

#define CHIMERA_INHERITED_GPIO Chimera::Mock::GPIOMock
#define CHIMERA_INHERITED_SPI Chimera::Mock::SPIMock

#endif /* !CHIMERA_PORT_MOCK_HPP */
