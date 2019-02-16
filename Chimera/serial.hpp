#pragma once
#ifndef CHIMERA_SERIAL_HPP
#define CHIMERA_SERIAL_HPP

/* C++ Includes */
#include <cstdint>
#include <cstdlib>
#include <memory>

/* Chimera Includes */
#include "chimeraPort.hpp"
#include <Chimera/chimera.hpp>
#include <Chimera/config.hpp>
#include <Chimera/interface.hpp>

namespace Chimera
{
  namespace Serial
  {
#ifndef MOD_TEST
    class SerialClass : public CHIMERA_INHERITED_SERIAL
    {
    public:
      SerialClass( const uint8_t channel ) : CHIMERA_INHERITED_SERIAL( channel )
      {
        serialChannel = channel;
      }

      ~SerialClass() = default;

    private:
      uint8_t serialChannel;
    };

    typedef std::shared_ptr<SerialClass> SerialClass_sPtr;
    typedef std::unique_ptr<SerialClass> SerialClass_uPtr;
#endif
    // TODO: For some reason this causes an assert even though all virtual methods
    // are implmented?
    // static_assert(std::is_base_of<Chimera::Serial::Interface,
    // Chimera::Serial::SerialClass>::value, "CHIMERA: Base class does not implement
    // the correct interface");
  }  // namespace Serial
}  // namespace Chimera

#endif
