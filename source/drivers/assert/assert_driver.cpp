/******************************************************************************
 *  File Name:
 *    assert_driver.cpp
 *
 *  Description:
 *    Assert implementation
 *
 *  2021 | Brandon Braun | brandonbraun653@gmail.com
 *****************************************************************************/

/* Aurora Includes */
#include <Aurora/logging>

/* Chimera Includes */
#include <Chimera/common>
#include <Chimera/assert>
#include <Chimera/system>

namespace Chimera::Assert
{
  /*---------------------------------------------------------------------------
  Public Functions
  ---------------------------------------------------------------------------*/
  void hardAssert( const bool assertion, const char *file, const uint32_t line  )
  {
    if( !assertion )
    {
      LOG_ERROR( "Failed assertion -- %s, Line: %d\r\n", file, line );
      Chimera::System::softwareReset();
    }
  }
}  // namespace Chimera::Assert
