#pragma once
#ifndef CHIMERA_SPI_HPP
#define CHIMERA_SPI_HPP

/* C/C++ Includes */
#include <memory>

/* Chimera Includes */
#include <Chimera/chimera.hpp>
#include <Chimera/config.hpp>

#include "chimeraPort.hpp"

namespace Chimera
{
  namespace SPI
  {
    class SPIClass : public CHIMERA_INHERITED_SPI
    {
    public:
      SPIClass( const int &channel ) : CHIMERA_INHERITED_SPI( channel ){};
      ~SPIClass() = default;
    };

    typedef std::shared_ptr<SPIClass> SPIClass_sPtr;
    typedef std::unique_ptr<SPIClass> SPIClass_uPtr;

    static_assert( std::is_base_of<Interface, SPIClass>::value,
                   "CHIMERA: Base class does not implement the correct interface" );

  }  // namespace SPI
}  // namespace Chimera

#endif
