/********************************************************************************
 *  File Name:
 *    chimera_serial.cpp
 *
 *	 Description:
 *    Implements the Chimera Serial interface
 *
 *  2020 | Brandon Braun | brandonbraun653@gmail.com
 *******************************************************************************/

/* STL Includes */
#include <memory>

/* Chimera Includes */
#include <Chimera/src/serial/serial.hpp>
#include <Chimera/src/serial/serial_base.hpp>
#include <Chimera/src/serial/serial_intf.hpp>
#include <Chimera/src/serial/serial_types.hpp>


namespace Chimera::Serial
{
#if !defined( CHIMERA_INHERITED_USART ) && !defined( CHIMERA_INHERITED_UART ) && !defined( CHIMERA_INHERITED_SERIAL )
  using CHIMERA_INHERITED_SERIAL = SerialUnsupported;
#endif

  static_assert( std::is_base_of<ISerial, CHIMERA_INHERITED_SERIAL>::value, "Invalid interface" );

  Chimera::Status_t initialize()
  {
    return Chimera::CommonStatusCodes::OK;
  }

  Serial_sPtr create_shared_ptr()
  {
    return std::make_shared<CHIMERA_INHERITED_SERIAL>();
  }

  Serial_uPtr create_unique_ptr()
  {
    return std::make_unique<CHIMERA_INHERITED_SERIAL>();
  }
}