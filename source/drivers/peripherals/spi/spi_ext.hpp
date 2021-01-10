/********************************************************************************
 *  File Name:
 *    spi_ext.hpp
 *
 *  Description:
 *    Models additional interface specs that are built on top of an SPI object
 *
 *  2019-2020 | Brandon Braun | brandonbraun653@gmail.com
 ********************************************************************************/

#pragma once
#ifndef CHIMERA_SPI_EXTENSIONS_HPP
#define CHIMERA_SPI_EXTENSIONS_HPP

/* STL Includes */
#include <cstdint>

/* Chimera Includes */
#include <Chimera/common>
#include <Chimera/gpio>
#include <Chimera/source/drivers/peripherals/spi/spi_user.hpp>

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
     *  Attaches an SPI instance to the inheriting class.
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
    virtual Chimera::Status_t attachSPI( Chimera::SPI::Driver_rPtr &spi ) = 0;

    /**
     *  Attaches an SPI instance to the inheriting class. Setup information is intentionally
     *  non-const to allow the underlying driver to make modifications specific to the system.
     *
     *  @note Assume the implementing function could initialize the hardware in this call.
     *
     *  @param[in]  spi     SPI object to be shared with the inheriting class
     *  @param[in]  setup   SPI setup configuration object
     *  @return Chimera::Status_t
     *
     *  |   Return Value   |                        Explanation                        |
     *  |:----------------:|:---------------------------------------------------------:|
     *  |               OK | The operation completed successfully                      |
     *  |             FAIL | The operation failed                                      |
     *  | INVAL_FUNC_PARAM | One or more of the initialization parameters were invalid |
     */
    virtual Chimera::Status_t attachSPI( Chimera::SPI::Driver_rPtr &spi, Chimera::SPI::DriverConfig &setup ) = 0;

    virtual Chimera::Status_t attachCS( Chimera::GPIO::PinInit &CSConfig ) = 0;

    virtual Chimera::Status_t attachCS( Chimera::GPIO::Driver_rPtr &CSPin ) = 0;
  };
}  // namespace Chimera::SPI

#endif /* !CHIMERA_SPI_EXTENSIONS_HPP */