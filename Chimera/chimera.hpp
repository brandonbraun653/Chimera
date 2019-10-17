#pragma once
#ifndef CHIMERA_HPP
#define CHIMERA_HPP

#include <Chimera/preprocessor.hpp>
#include <Chimera/utilities.hpp>

extern void ChimeraInit();

namespace Chimera
{
  size_t millis();
  void delayMilliseconds( size_t ms );
  void delayMicroseconds( size_t us );
}  // namespace Chimera

#endif
