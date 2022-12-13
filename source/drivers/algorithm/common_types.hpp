/******************************************************************************
 *  File Name:
 *    common_types.hpp
 *
 *  Description:
 *    Common types for Chimera::Algorithm
 *
 *  2020 | Brandon Braun | brandonbraun653@gmail.com
 *****************************************************************************/

/* STL Includes */
#include <cstddef>

#pragma once
#ifndef CHIMERA_ALGORITHM_COMMON_TYPES_HPP
#define CHIMERA_ALGORITHM_COMMON_TYPES_HPP

namespace Chimera::Algorithm
{
  /**
   *  A generic way to describe a set of options that can be searched through.
   *  @warning The entire list must be allocated using contiguous memory
   */
  struct OptionsList
  {
    void *pData;       /**< List data (must be contiguous memory) */
    size_t totalSize;  /**< Total size of the list (bytes) */
    size_t objectSize; /**< Size of each element (bytes) */
  };
}  // namespace Chimera::Algorithm

#endif  /* !CHIMERA_ALGORITHM_COMMON_TYPES_HPP */
