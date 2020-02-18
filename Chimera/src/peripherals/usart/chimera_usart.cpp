/********************************************************************************
 *  File Name:
 *    chimera_usart.cpp
 *
 *	 Description:
 *    Implements the Chimera USART driver
 *
 *  2020 | Brandon Braun | brandonbraun653@gmail.com
 *******************************************************************************/

/* STL Includes */
#include <memory>

/* Chimera Includes */
#include "chimeraPort.hpp"
#include <Chimera/usart>
#include <Chimera/src/serial/serial_base.hpp>
#include <Chimera/src/serial/serial_intf.hpp>


namespace Chimera::USART
{
#if !defined( CHIMERA_INHERITED_USART )
  using CHIMERA_INHERITED_USART = ::Chimera::Serial::SerialUnsupported;
  #pragma message( "USART driver is unsupported" )
#endif

  static_assert( std::is_base_of<::Chimera::Serial::ISerial, CHIMERA_INHERITED_USART>::value, "Invalid interface" );

  Chimera::Status_t initialize()
  {
    return Chimera::CommonStatusCodes::OK;
  }

  USART_sPtr create_shared_ptr()
  {
    return std::make_shared<CHIMERA_INHERITED_USART>();
  }

  USART_uPtr create_unique_ptr()
  {
    return std::make_unique<CHIMERA_INHERITED_USART>();
  }
}  // namespace Chimera::USART