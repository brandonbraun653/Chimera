/******************************************************************************
 *  File Name:
 *    serial_user.hpp
 *
 *  Description:
 *    User interface for the serial driver
 *
 *  2022 | Brandon Braun | brandonbraun653@protonmail.com
 *****************************************************************************/

#pragma once
#ifndef CHIMERA_SERIAL_USER_HPP
#define CHIMERA_SERIAL_USER_HPP

/*-----------------------------------------------------------------------------
Includes
-----------------------------------------------------------------------------*/
#include <Chimera/common>
#include <Chimera/source/drivers/peripherals/peripheral_types.hpp>
#include <Chimera/source/drivers/serial/serial_types.hpp>

namespace Chimera::Serial
{
  /*---------------------------------------------------------------------------
  Public Functions
  ---------------------------------------------------------------------------*/
  Chimera::Status_t initialize();
  Chimera::Status_t attach( const Chimera::Peripheral::Type type, const Channel channel );
  Driver_rPtr getDriver( const Channel channel );

  /*---------------------------------------------------------------------------
  Classes
  ---------------------------------------------------------------------------*/
  class Driver
  {
  public:
    Driver();
    ~Driver();

    Chimera::Status_t open( const Config &config );
    Chimera::Status_t close();
    Chimera::Status_t flush( const Chimera::Hardware::SubPeripheral periph );
    int write( const void *const buffer, const size_t length );
    int read( void *const buffer, const size_t length );

  protected:
    friend Driver_rPtr getDriver( const Channel );

    void * mImpl;
  };
}  // namespace Chimera::Serial

#endif  /* !CHIMERA_SERIAL_USER_HPP */
