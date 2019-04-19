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
#include <Chimera/interface/macro.hpp>
#include <Chimera/interface/serial_intf.hpp>
#include "chimeraPort.hpp"

namespace Chimera
{
  namespace Serial
  {
#if !defined( CHIMERA_INHERITED_SERIAL )
    using CHIMERA_INHERITED_SERIAL = SerialUnsupported;
#endif
    
    class SerialClass : public CHIMERA_INHERITED_SERIAL
    {
    public:
      SerialClass()  = default;
      ~SerialClass() = default;

      SerialClass( const size_t bufferSize ) : CHIMERA_INHERITED_SERIAL( bufferSize )
      {
      }
    };
    
    static_assert( std::is_base_of<Interface, SerialClass>::value, "Base class implements the wrong interface" );
    
#if !defined( CHIMERA_DISABLE_INHERITANCE_WARNINGS )
    STATIC_WARNING( !( std::is_base_of<SerialUnsupported, SerialClass>::value ),
                    "No Serial interface defined in backend driver. You can disable these warnings by defining "
                    "CHIMERA_DISABLE_INHERITANCE_WARNINGS in the preprocessor." );
#endif
    
  }  // namespace Serial
}  // namespace Chimera

#endif
