/******************************************************************************
 *  File Name:
 *    register_optimizer.cpp
 *
 *  Description:
 *    Implements the register optimization functions
 *
 *  2019-2020 | Brandon Braun | brandonbraun653@gmail.com
 *****************************************************************************/

/* STL Includes */
#include <limits>
#include <cmath>

/* Chimera Includes */
#include <Chimera/source/drivers/algorithm/register_optimizer.hpp>

namespace Chimera::Algorithm
{
  namespace RegisterOptimization
  {
    Reg32_t findOptimalSetting( const RegOptimizerData &config, const Reg32_t defaultValue, void *const data )
    {
      /*-----------------------------------------------------------------------
      Input checks
      -----------------------------------------------------------------------*/
      if ( !config.actVals || !config.regVals )
      {
        return defaultValue;
      }

      /*-----------------------------------------------------------------------
      Lists are not required to be sorted, so the best
      we can do is O(n). Typically these option lists are
      not large anyways.
      -----------------------------------------------------------------------*/
      float bestFit  = std::numeric_limits<float>::max();
      float currFit  = std::numeric_limits<float>::max();
      size_t bestIdx = 0;

      for ( size_t x = 0; x < config.numOptions; x++ )
      {
        currFit = fabs( config.optimizer( config.desiredValue, config.actVals[ x ], data ) );

        if ( currFit < bestFit )
        {
          bestFit = currFit;
          bestIdx = x;
        }
      }

      return config.regVals[ bestIdx ];
    }
  }  // namespace RegisterOptimization
}  // namespace Chimera::Algorithm
