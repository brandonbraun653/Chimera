/********************************************************************************
 *  File Name:
 *    uart_intf.hpp
 *
 *  Description:
 *    Interface to the UART peripheral
 *
 *  2020-2022 | Brandon Braun | brandonbraun653@gmail.com
 *******************************************************************************/

#pragma once
#ifndef CHIMERA_UART_INTERFACE_HPP
#define CHIMERA_UART_INTERFACE_HPP

/* Chimera Includes */
#include <Chimera/common>
#include <Chimera/source/drivers/peripherals/uart/uart_types.hpp>
#include <Chimera/source/drivers/serial/serial_intf.hpp>

namespace Chimera::UART
{
  /*-------------------------------------------------------------------------------
  Public Functions
  -------------------------------------------------------------------------------*/
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


  /*-------------------------------------------------------------------------------
  Classes
  -------------------------------------------------------------------------------*/
  /**
   *  Virtual class to facilitate easy mocking of the driver
   */
#if defined( CHIMERA_VIRTUAL )
  class IUART : virtual public Chimera::Serial::ISerial
  {
  public:
    virtual ~IUART() = default;
  };
#else
  class IUART
  {
  };
#endif

}  // namespace Chimera::UART

#endif /* !CHIMERA_UART_INTERFACE_HPP */
