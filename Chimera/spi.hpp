/********************************************************************************
 * File Name:
 *    spi.hpp
 *
 * Description:
 *    Implements a common wrapper interface to the Chimera SPI peripheral.
 *
 * 2019 | Brandon Braun | brandonbraun653@gmail.com
 ********************************************************************************/

#pragma once
#ifndef CHIMERA_SPI_HPP
#define CHIMERA_SPI_HPP

/* C/C++ Includes */
#include <memory>

/* Chimera Includes */
#include <Chimera/chimera.hpp>

#include "chimeraPort.hpp"

namespace Chimera
{
  namespace SPI
  {
    /**
     *  A simple wrapper to provide a common SPI class type for programs built
     *  with Chimera. The runtime behavior of this class is defined by the user
     *  provided class type CHIMERA_INHERITED_SPI in chimeraPort.hpp.
     *
     *  If no user class is provided, a default disabled behavior version will be
     *  substituted in its place.
     */
    class SPIClass : public CHIMERA_INHERITED_SPI
    {
    public:
      template<typename T>
      SPIClass( T &var ) : CHIMERA_INHERITED_SPI( var ){};

      SPIClass() : CHIMERA_INHERITED_SPI(){};
      ~SPIClass() = default;
    };

    using SPIClass_sPtr = std::shared_ptr<SPIClass>;
    using SPIClass_uPtr = std::unique_ptr<SPIClass>;

    static_assert( std::is_base_of<Interface, SPIClass>::value, "Base class implements the wrong interface" );

  }  // namespace SPI
}  // namespace Chimera

#endif
