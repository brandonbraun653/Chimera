/********************************************************************************
 *  File Name:
 *    assert_driver.cpp
 *
 *  Description:
 *    Assert implementation
 *
 *  2021 | Brandon Braun | brandonbraun653@gmail.com
 *******************************************************************************/

/* Chimera Includes */
#include <Chimera/common>
#include <Chimera/assert>
#include <Chimera/system>

namespace Chimera::Assert
{
  /*-------------------------------------------------------------------------------
  Public Functions
  -------------------------------------------------------------------------------*/
  void hardAssert( const bool assertion )
  {
    if( !assertion )
    {
      Chimera::System::softwareReset();
    }
  }
}  // namespace Chimera::Assert
