/********************************************************************************
 *   File Name:
 *
 *
 *   Description:
 *
 *
 *   2019 | Brandon Braun | brandonbraun653@gmail.com
 ********************************************************************************/
#pragma once
#ifndef CHIMERA_PORT_MOCK_HPP
#define CHIMERA_PORT_MOCK_HPP

#include <Chimera/mock/delay.hpp>
#include <Chimera/mock/watchdog.hpp>

namespace BACKEND_NAMESPACE = Chimera::Mock;

#define CHIMERA_INHERITED_WATCHDOG Chimera::Mock::Watchdog::SimWatchdog

#endif /* !CHIMERA_PORT_MOCK_HPP */
