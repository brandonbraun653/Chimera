#include "chimeraPort.hpp"
#include <Chimera/chimera.hpp>
#include <Chimera/config.hpp>

#ifdef CHIMERA_FREERTOS
#include <Chimera/threading.hpp>
#endif

void ChimeraInit()
{
#ifdef MOD_TEST

#else
  cSystemInit();
#endif
}

namespace Chimera
{
  uint32_t millis()
  {
    return BACKEND_NAMESPACE::millis();
  }

  void delayMilliseconds( uint32_t ms )
  {
    BACKEND_NAMESPACE::delayMilliseconds( ms );
  }

  void delayMicroseconds( uint32_t us )
  {
    BACKEND_NAMESPACE::delayMicroseconds( us );
  }
}  // namespace Chimera
