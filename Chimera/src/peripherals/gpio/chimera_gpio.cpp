/********************************************************************************
*   File Name:
*     chimera_gpio.cpp
*
*   Description:
*     Implements Chimera GPIO
*
*   2020 | Brandon Braun | brandonbraun653@gmail.com
********************************************************************************/

/* STL Includes */
#include <memory>

/* Chimera Includes */
#include <Chimera/gpio>

namespace Chimera::GPIO
{
#if !defined( CHIMERA_INHERITED_GPIO )
  using CHIMERA_INHERITED_GPIO = GPIOUnsupported;
#endif

  static_assert( std::is_base_of<HWInterface, CHIMERA_INHERITED_GPIO>::value, "Invalid interface" );

  Chimera::Status_t initialize()
  {
    return Chimera::CommonStatusCodes::OK;
  }

  GPIO_sPtr create_shared_ptr()
  {
    return std::make_shared<CHIMERA_INHERITED_GPIO>();
  }

  GPIO_uPtr create_unique_ptr()
  {
    return std::make_unique<CHIMERA_INHERITED_GPIO>();
  }
}