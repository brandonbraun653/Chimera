#pragma once
#ifndef CHIMERA_HPP
#define CHIMERA_HPP

#include <Chimera/preprocessor.hpp>
#include <Chimera/types.hpp>
#include <Chimera/utilities.hpp>

extern void ChimeraInit();

namespace Chimera
{
  uint32_t millis();
  void delayMilliseconds( uint32_t ms );
  void delayMicroseconds( uint32_t us );
}  // namespace Chimera

#endif
