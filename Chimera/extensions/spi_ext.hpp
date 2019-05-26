/********************************************************************************
 *   File Name:
 *    spi_ext.hpp
 *
 *   Description:
 *    Models additional interface specs that are built on top of an SPI object
 *
 *   2019 | Brandon Braun | brandonbraun653@gmail.com
 ********************************************************************************/

#pragma once 
#ifndef CHIMERA_SPI_INTERFACE_HPP
#define CHIMERA_SPI_INTERFACE_HPP

/* C++ Includes */
#include <cstdint>

/* Chimera Includes */
#include <Chimera/chimera.hpp>
#include <Chimera/spi.hpp>

namespace Chimera::SPI
{
  /**
   *  Additional model that enforces a specific interface on classes that 
   *  want to accept objects that adhere to the Chimera::SPI::Interface spec.
   */
  class SPIAcceptor
  {
  public:
    virtual ~SPIAcceptor() = default;

    /**
     *  Attaches an SPI instance to the inheriting class. Passing by const-reference
     *  ensures the internal instance counter isn't incremented and forces the user
     *  to copy without modification into their own internal shared_ptr.
     *
     *  @param[in]  spi     SPI object to be shared with the inheriting class
     *  @return Chimera::Status_t
     *
     *  |   Return Value   |                        Explanation                        |
     *  |:----------------:|:---------------------------------------------------------:|
     *  |               OK | The operation completed successfully                      |
     *  |             FAIL | The operation failed                                      |
     *  | INVAL_FUNC_PARAM | One or more of the initialization parameters were invalid |
     */
    virtual Chimera::Status_t attachSPI( const SPIClass_sPtr &spi )
    {
      return Chimera::CommonStatusCodes::NOT_SUPPORTED;
    }

    /**
     *  Attaches an SPI instance to the inheriting class. It is intended that the 
     *  user std::move() the object so that the function takes ownership.
     *
     *  @param[in]  spi     SPI object to be moved into the inheriting class
     *  @return Chimera::Status_t
     *
     *  |   Return Value   |                        Explanation                        |
     *  |:----------------:|:---------------------------------------------------------:|
     *  |               OK | The operation completed successfully                      |
     *  |             FAIL | The operation failed                                      |
     *  | INVAL_FUNC_PARAM | One or more of the initialization parameters were invalid |
     */
    virtual Chimera::Status_t attachSPI( SPIClass_uPtr spi )
    {
      return Chimera::CommonStatusCodes::NOT_SUPPORTED;
    }
  };
}  // namespace Chimera::SPI

#endif /* !CHIMERA_SPI_INTERFACE_HPP */