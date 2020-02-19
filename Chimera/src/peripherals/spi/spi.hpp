/********************************************************************************
 *  File Name:
 *    spi.hpp
 *
 *  Description:
 *    Implements an interface to create a Chimera SPI peripheral.
 *
 *  2019-2020 | Brandon Braun | brandonbraun653@gmail.com
 ********************************************************************************/

#pragma once
#ifndef CHIMERA_SPI_HPP
#define CHIMERA_SPI_HPP

/* STL Includes */
#include <memory>

/* Chimera Includes */
#include <Chimera/src/peripherals/spi/spi_intf.hpp>

namespace Chimera::SPI
{
  Chimera::Status_t initialize();

  Chimera::Status_t reset();
  
  SPI_sPtr create_shared_ptr();

  SPI_uPtr create_unique_ptr();

}  // namespace Chimera::SPI

#endif  /* !CHIMERA_SPI_HPP */
