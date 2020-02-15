/********************************************************************************
 *  File Name:
 *    chimera_spi.cpp
 *
 *  Description:
 *    Implements Chimera's SPI functionality.
 *
 *  2020 | Brandon Braun | brandonbraun653@gmail.com
 ********************************************************************************/

/* Chimera Includes */
#include "chimeraPort.hpp"
#include <Chimera/spi>


namespace Chimera::SPI
{
#if !defined( CHIMERA_INHERITED_SPI )
  using CHIMERA_INHERITED_SPI = SPIUnsupported;
#endif

  static_assert( std::is_base_of<HardwareDriverInterface, CHIMERA_INHERITED_SPI>::value, "Invalid interface" );
  static_assert( std::is_base_of<Chimera::Event::ListenerInterface, CHIMERA_INHERITED_SPI>::value, "Invalid interface" );
  static_assert( std::is_base_of<Chimera::Threading::AsyncIOInterface, CHIMERA_INHERITED_SPI>::value, "Invalid interface" );
  static_assert( std::is_base_of<Chimera::Threading::LockableInterface, CHIMERA_INHERITED_SPI>::value, "Invalid interface" );


  Chimera::Status_t initialize()
  {
    return Chimera::CommonStatusCodes::OK;
  }

  SPI_sPtr create_shared_ptr()
  {
    return std::make_shared<CHIMERA_INHERITED_SPI>();
  }

  SPI_uPtr create_unique_ptr()
  {
    return std::make_unique<CHIMERA_INHERITED_SPI>();
  }
}  // namespace Chimera::SPI