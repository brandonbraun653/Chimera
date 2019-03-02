#include "chimeraPort.hpp"
#include <Chimera/chimera.hpp>
#include <Chimera/config.hpp>

#ifdef CHIMERA_FREERTOS
#include <Chimera/threading.hpp>
#endif

#ifdef SIM
/* Boost Includes */
#include <boost/thread.hpp>
#include <boost/chrono.hpp>
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
    #ifdef BACKEND_NAMESPACE
    return BACKEND_NAMESPACE::millis();
    #else
    return 0u;
    #endif
  }

  void delayMilliseconds( uint32_t ms )
  {
    #ifdef BACKEND_NAMESPACE
    BACKEND_NAMESPACE::delayMilliseconds( ms );
    #else
    boost::this_thread::sleep_for( boost::chrono::milliseconds( ms ) );
    #endif
  }

  void delayMicroseconds( uint32_t us )
  {
    #ifdef BACKEND_NAMESPACE
    BACKEND_NAMESPACE::delayMicroseconds( us );
    #else
    boost::this_thread::sleep_for( boost::chrono::microseconds( us ) );
    #endif
  }
}  // namespace Chimera
