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
#include <Chimera/interface.hpp>

namespace Chimera
{
  namespace Serial
  {

  class SerialClass : public CHIMERA_INHERITED_SERIAL
    {
    public:
      SerialClass()  = default;
      ~SerialClass() = default;

      /**
       *  Special ctor for specifying the size of internal buffers
       */
      SerialClass( const size_t bufferSize ) : CHIMERA_INHERITED_SERIAL( bufferSize ) {}
    };

    typedef std::shared_ptr<SerialClass> SerialClass_sPtr;
    typedef std::unique_ptr<SerialClass> SerialClass_uPtr;

    // TODO: For some reason this causes an assert even though all virtual methods
    // are implmented?
    // static_assert(std::is_base_of<Chimera::Serial::Interface,
    // Chimera::Serial::SerialClass>::value, "CHIMERA: Base class does not implement
    // the correct interface");
  }  // namespace Serial
}  // namespace Chimera

#endif
