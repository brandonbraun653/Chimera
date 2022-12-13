/******************************************************************************
 *  File Name:
 *    usart_intf.hpp
 *
 *	 Description:
 *    Interface to the USART peripheral
 *
 *  2020-2022 | Brandon Braun | brandonbraun653@gmail.com
 *****************************************************************************/

#pragma once
#ifndef CHIMERA_USART_INTERFACE_HPP
#define CHIMERA_USART_INTERFACE_HPP

/*-----------------------------------------------------------------------------
Includes
-----------------------------------------------------------------------------*/
#include <Chimera/common>
#include <Chimera/source/drivers/peripherals/usart/usart_types.hpp>
#include <Chimera/source/drivers/serial/serial_intf.hpp>

namespace Chimera::USART
{
  /*---------------------------------------------------------------------------
  Public Functions
  ---------------------------------------------------------------------------*/
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


  /*---------------------------------------------------------------------------
  Classes
  ---------------------------------------------------------------------------*/
  /**
   *  Virtual class to facilitate easy mocking of the driver
   */
#if defined( CHIMERA_VIRTUAL )
  class IUSART : virtual public Chimera::Serial::ISerial
  {
  public:
    virtual ~IUSART() = default;
  };
#else
  class IUSART
  {
  };
#endif

}

#endif  /* !CHIMERA_USART_INTERFACE_HPP */
