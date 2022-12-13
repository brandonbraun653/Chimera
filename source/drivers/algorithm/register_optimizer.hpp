/******************************************************************************
 *  File Name:
 *    register_optimizer.hpp
 *
 *  Description:
 *    Various algorithms for commonly used register initialization functionality
 *
 *  2019-2020 | Brandon Braun | brandonbraun653@gmail.com
 *****************************************************************************/

#pragma once
#ifndef CHIMERA_ALGORITHM_REGISTER_OPTIMIZER_HPP
#define CHIMERA_ALGORITHM_REGISTER_OPTIMIZER_HPP

/* Chimera Includes */
#include <Chimera/common>

namespace Chimera::Algorithm
{
  namespace RegisterOptimization
  {
    /**
     *  Declares a signature for the function that calculates the performance of a
     *  register configuration value versus the desired goal.
     *
     *  @param[in]  goal      The desired value to be achieved
     *  @param[in]  actVal    The register configuration value currently being evaluated
     *  @param[in]  data      Additional data that may be needed for the performance calculation
     *  @return float         How well the 'actVal' did at configuring for 'goal'. Values closer
     *                        to zero, positive or negative, are considered higher performing.
     */
    using PerformanceFunc_t = float ( * )( const size_t goal, const size_t actVal, void *const data );

    struct RegOptimizerData
    {
      const Reg32_t *regVals;      /**< A list of register level configuration options */
      const size_t *actVals;       /**< A list of what each register config option actually means */
      size_t numOptions;           /**< How many options there are to search through */
      size_t desiredValue;         /**< The desired outcome to achieve */
      PerformanceFunc_t optimizer; /**< The optimization performance function */
    };

    /**
     *  Finds the best register configuration setting
     *
     *  @param[in]  config    The optimizer configuration information
     *  @param[in]  data      Additional data needed by the performance function
     *  @return Reg32_t       The best register configuration option
     */
    Reg32_t findOptimalSetting( const RegOptimizerData &config, const Reg32_t defaultValue, void *const data );

  }  // namespace RegisterOptimization
}  // namespace Chimera::Algorithm

#endif /* CHIMERA_ALGORITHM_REGISTER_OPTIMIZER_HPP */
