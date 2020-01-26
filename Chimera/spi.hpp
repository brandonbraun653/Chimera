/********************************************************************************
 * File Name:
 *    spi.hpp
 *
 * Description:
 *    Implements an interface to create a Chimera SPI peripheral.
 *
 * 2019-2020 | Brandon Braun | brandonbraun653@gmail.com
 ********************************************************************************/

#pragma once
#ifndef CHIMERA_SPI_HPP
#define CHIMERA_SPI_HPP

/* C/C++ Includes */
#include <memory>

/* Chimera Includes */
#include <Chimera/interface/spi_intf.hpp>

namespace Chimera::SPI
{
  using SPI_sPtr = std::shared_ptr<HardwareDriverInterface>;
  using SPI_uPtr = std::unique_ptr<HardwareDriverInterface>;

  /**
   *  Initialize the driver memory and other resources for SPI
   *
   *  @return Chimera::Status_t
   */
  Chimera::Status_t initialize();

  SPI_sPtr create_shared_ptr();

  SPI_uPtr create_unique_ptr();

}  // namespace Chimera::SPI

#endif
