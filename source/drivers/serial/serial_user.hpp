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
#include <Chimera/source/drivers/threading/threading_extensions.hpp>

namespace Chimera::Serial
{
  /*---------------------------------------------------------------------------
  Public Functions
  ---------------------------------------------------------------------------*/
  Chimera::Status_t initialize();
  Chimera::Status_t reset();
  Driver_rPtr       getDriver( const Channel channel );

  /*---------------------------------------------------------------------------
  Classes
  ---------------------------------------------------------------------------*/
  class Driver : public Chimera::Thread::Lockable<Driver>, public Chimera::Thread::AsyncIO<Driver>, public virtual ISerial
  {
  public:
    Driver();
    ~Driver();

    Chimera::Status_t open( const Chimera::Serial::Config &config );
    Chimera::Status_t close();
    int               write( const void *const buffer, const size_t length, const size_t timeout = Chimera::Thread::TIMEOUT_DONT_WAIT );
    int               read( void *const buffer, const size_t length, const size_t timeout = Chimera::Thread::TIMEOUT_DONT_WAIT );

  protected:
    friend Chimera::Thread::Lockable<Driver>;
    friend Chimera::Thread::AsyncIO<Driver>;
    void *mImpl;
  };
}  // namespace Chimera::Serial

#endif /* !CHIMERA_SERIAL_USER_HPP */
