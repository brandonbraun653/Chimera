/********************************************************************************
 *  File Name:
 *    types.hpp
 *
 *  Description:
 *    Generic thread primitive native types
 *
 *  2020 | Brandon Braun | brandonbraun653@gmail.com
 ********************************************************************************/

#ifndef CHIMERA_THREADING_GENERIC_TYPES_HPP
#define CHIMERA_THREADING_GENERIC_TYPES_HPP

/* C++ Includes */
#include <cstdint>

namespace Chimera::Threading::detail
{
  using native_mutex = uint32_t;
}

#endif /* !CHIMERA_THREADING_GENERIC_TYPES_HPP */
