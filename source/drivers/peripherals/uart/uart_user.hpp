/********************************************************************************
 *  File Name:
 *    uart_user.hpp
 *
 *  Description:
 *    Implements an interface to create a Chimera UART peripheral
 *
 *  2020-2022 | Brandon Braun | brandonbraun653@gmail.com
 ********************************************************************************/

#pragma once
#ifndef CHIMERA_UART_HPP
#define CHIMERA_UART_HPP

/*-----------------------------------------------------------------------------
Includes
-----------------------------------------------------------------------------*/
#include <Chimera/common>
#include <Chimera/source/drivers/peripherals/uart/uart_intf.hpp>
#include <Chimera/source/drivers/peripherals/uart/uart_types.hpp>

namespace Chimera::UART
{
  /*---------------------------------------------------------------------------
  Public Functions
  ---------------------------------------------------------------------------*/
  Chimera::Status_t initialize();
  Chimera::Status_t reset();
  bool isChannelUART( const Chimera::Serial::Channel channel );
  Driver_rPtr getDriver( const Chimera::Serial::Channel channel );

  /*---------------------------------------------------------------------------
  Classes
  ---------------------------------------------------------------------------*/
  class Driver : public Chimera::Thread::Lockable<Driver>,
                 public Chimera::Thread::AsyncIO<Driver>,
                 public IUART
  {
  public:
    using Chimera::Thread::AsyncIO<Driver>::AsyncIO;

    Driver();
    ~Driver();

    Chimera::Status_t open( const Chimera::Serial::Config &config );
    Chimera::Status_t close();
    int               write( const void *const buffer, const size_t length );
    int               read( void *const buffer, const size_t length );

  private:
    friend Chimera::Thread::Lockable<Driver>;
    friend Chimera::Thread::AsyncIO<Driver>;
    void *mImpl;
  };
}  // namespace Chimera::UART

#endif  /* !CHIMERA_UART_HPP */
