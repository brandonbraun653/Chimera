/********************************************************************************
 *  File Name:
 *    uart_intf.hpp
 *
 *	Description:
 *    Interface to the UART peripheral
 *
 *  2020 | Brandon Braun | brandonbraun653@gmail.com
 *******************************************************************************/

#pragma once
#ifndef CHIMERA_UART_INTERFACE_HPP
#define CHIMERA_UART_INTERFACE_HPP

/* Chimera Includes */
#include <Chimera/common>
#include <Chimera/src/peripherals/uart/uart_types.hpp>
#include <Chimera/src/serial/serial_intf.hpp>

namespace Chimera::UART
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

  class IUART : virtual public Chimera::Serial::ISerial
  {
  public:
    virtual ~IUART() = default;
  };
}  // namespace Chimera::UART

#endif /* !CHIMERA_UART_INTERFACE_HPP */
