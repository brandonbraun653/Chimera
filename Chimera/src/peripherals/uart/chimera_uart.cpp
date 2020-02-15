/********************************************************************************
 *  File Name:
 *    chimera_uart.cpp
 *
 *	 Description:
 *    Implements the Chimera UART driver
 *
 *  2020 | Brandon Braun | brandonbraun653@gmail.com
 *******************************************************************************/

/* STL Includes */
#include <memory>

/* Chimera Includes */
#include "chimeraPort.hpp"
#include <Chimera/uart>
#include <Chimera/src/serial/serial_base.hpp>
#include <Chimera/src/serial/serial_intf.hpp>


namespace Chimera::UART
{
#if !defined( CHIMERA_INHERITED_UART )
  using CHIMERA_INHERITED_UART = ::Chimera::Serial::SerialUnsupported;
#endif

  static_assert( std::is_base_of<::Chimera::Serial::HWInterface, CHIMERA_INHERITED_UART>::value, "Invalid interface" );

  Chimera::Status_t initialize()
  {
    return Chimera::CommonStatusCodes::OK;
  }

  UART_sPtr create_shared_ptr()
  {
    return std::make_shared<CHIMERA_INHERITED_UART>();
  }

  UART_uPtr create_unique_ptr()
  {
    return std::make_unique<CHIMERA_INHERITED_UART>();
  }
}  // namespace Chimera::UART