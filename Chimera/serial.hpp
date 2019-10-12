/********************************************************************************
 *   File Name:
 *    serial.hpp
 *
 *   Description:
 *    Defines the interface to the UART/USART hardware
 *
 *   2019 | Brandon Braun | brandonbraun653@gmail.com
 ********************************************************************************/

#pragma once
#ifndef CHIMERA_SERIAL_HPP
#define CHIMERA_SERIAL_HPP

/* C++ Includes */
#include <cstdint>
#include <memory>

/* Chimera Includes */
#include <Chimera/constants/common.hpp>
#include <Chimera/interface/event_intf.hpp>
#include <Chimera/interface/serial_intf.hpp>
#include <Chimera/interface/threading_intf.hpp>

#include <Chimera/base/serial_base.hpp>

#include <Chimera/event.hpp>

#include "chimeraPort.hpp"

namespace Chimera::Serial
{
#if !defined( CHIMERA_INHERITED_SERIAL )
  using CHIMERA_INHERITED_SERIAL = SerialUnsupported;
#endif

  class SerialClass : public CHIMERA_INHERITED_SERIAL
  {
  public:
    SerialClass() : CHIMERA_INHERITED_SERIAL()
    {
    }
    ~SerialClass() = default;
  };

  /*------------------------------------------------
  Make sure the inherited driver implements the expected interfaces
  ------------------------------------------------*/
  static_assert( ( ( std::is_base_of<Interface, SerialClass>::value )
                   && ( std::is_base_of<Chimera::Event::ListenerInterface, SerialClass>::value )
                   && ( std::is_base_of<Chimera::Threading::AsyncIOBaseInterface, SerialClass>::value )
                   && ( std::is_base_of<Chimera::Threading::LockableInterface, SerialClass>::value ) ),
                 "Base class implements the wrong interface" );

}  // namespace Chimera::Serial

#endif
