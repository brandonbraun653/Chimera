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
#include <Chimera/src/peripherals/usart/usart_types.hpp>
#include <Chimera/src/serial/serial_intf.hpp>

namespace Chimera::USART
{
  namespace Backend
  {
    /**
     *  Registers the backend driver with Chimera
     *
     *  @param[in]  registry    Chimera's copy of the driver interface
     *  @return Chimera::Status_t
     */
    extern Chimera::Status_t registerDriver( DriverConfig &registry );
  }  // namespace Backend

  class IUSART : virtual public Chimera::Serial::ISerial
  {
  public:
    virtual ~IUSART() = default;
  };
}

#endif  /* !CHIMERA_USART_INTERFACE_HPP */
