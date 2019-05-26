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
#ifndef CHIMERA_PORT_SIM_HPP
#define CHIMERA_PORT_SIM_HPP

#include <mock/Chimera/spi.hpp>
#include <mock/Chimera/watchdog.hpp>

#define CHIMERA_INHERITED_SPI         Chimera::Mock::SPI::SPIMock
#define CHIMERA_INHERITED_WATCHDOG    Chimera::Mock::Watchdog::WatchdogMock

#endif /* !CHIMERA_PORT_MOCK_HPP */
