/******************************************************************************
 *  File Name:
 *    chimera_utilities.cpp
 *
 *  Description:
 *    Utility methods for Chimera
 *
 *  2020 | Brandon Braun | brandonbraun653@gmail.com
 *****************************************************************************/

/* STL Includes */
#include <limits>

/* Chimera Includes */
#include <Chimera/utility>

namespace Chimera::Utility
{
  /*---------------------------------------------------------------------------
  Public Functions
  ---------------------------------------------------------------------------*/
  size_t bf2Idx( size_t bf )
  {
    /*-------------------------------------------------------------------------
    Handle the single invalid case
    -------------------------------------------------------------------------*/
    if ( !bf )
    {
      return std::numeric_limits<size_t>::max();
    }

    /*-------------------------------------------------------------------------
    Return the first set bit as an index
    -------------------------------------------------------------------------*/
    size_t idx = 0;
    do
    {
      if ( bf & 0x1 )
      {
        break;
      }
      idx++;
    } while ( bf >> 1u );

    return idx;
  }
}  // namespace Chimera::Utility
