#pragma once
#ifndef CHIMERA_UTILITIES_HPP
#define CHIMERA_UTILITIES_HPP

/* STL Includes */
#include <array>
#include <algorithm>
#include <atomic>
#include <iterator>
#include <memory>
#include <set>
#include <type_traits>
#include <utility>
#include <vector>

/* Chimera Includes */
#include <Chimera/common>

namespace Chimera::Utilities
{
  /*-------------------------------------------------------------------------------
  Public Functions
  -------------------------------------------------------------------------------*/
  /**
   *  Given a bit field value, returns the corresponding position
   *  of the first set bit (zero indexed). For example, 0x80 would
   *  return 3, 0x200000 returns 21, 0xB04400 returns 14, and 0x1
   *  returns 0.
   *
   *  This function assumes positive logic in that an index is only
   *  valid if a bit is set. An input of zero is invalid and will
   *  return the max value of the return type.
   *
   *  @param[in]  bf    Bit-field value to convert
   *  @return size_t
   */
  size_t bf2Idx( size_t bf );

  /**
   *  Swaps the endianness of the 32-bit input value
   *
   *  @param[in]  val   Value to swap
   *  @return uint32_t
   */
  static inline constexpr uint32_t swap( const uint32_t val )
  {
    /* clang-format off */
    return ( ( ( val << 24 ) )
           | ( ( val << 8  ) & 0x00ff0000 )
           | ( ( val >> 8  ) & 0x0000ff00 )
           | ( ( val >> 24 ) & 0x000000ff ) );
    /* clang-format on */
  }

  /**
   *  Swaps the endianness of the 16-bit input value
   *
   *  @param[in]  val   Value to swap
   *  @return uint16_t
   */
  static inline constexpr uint16_t swap( const uint16_t val )
  {
    /* clang-format off */
    return ( ( ( val << 8  ) & 0xff00 )
           | ( ( val >> 8  ) & 0x00ff ) );
    /* clang-format on */
  }

};  // namespace Chimera::Utilities

#endif
