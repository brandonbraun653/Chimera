/********************************************************************************
 *  File Name:
 *    uart_intf.hpp
 *
 *	 Description:
 *    Interface to the UART peripheral
 *
 *  2020 | Brandon Braun | brandonbraun653@gmail.com
 *******************************************************************************/

#pragma once
#ifndef CHIMERA_UART_INTERFACE_HPP
#define CHIMERA_UART_INTERFACE_HPP

/* Chimera Includes */
#include <Chimera/common>

namespace Chimera::UART
{
  namespace Backend
  {
    extern Chimera::Status_t prjInitialize();
  }
}

#endif  /* !CHIMERA_UART_INTERFACE_HPP */
