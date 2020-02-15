/********************************************************************************
 *  File Name:
 *    usart_intf.hpp
 *
 *	 Description:
 *    Interface to the USART peripheral
 *
 *  2020 | Brandon Braun | brandonbraun653@gmail.com
 *******************************************************************************/

#pragma once
#ifndef CHIMERA_USART_INTERFACE_HPP
#define CHIMERA_USART_INTERFACE_HPP

/* Chimera Includes */
#include <Chimera/common>

namespace Chimera::USART
{
  namespace Backend
  {
    extern Chimera::Status_t prjInitialize();
  }
}

#endif  /* !CHIMERA_USART_INTERFACE_HPP */
