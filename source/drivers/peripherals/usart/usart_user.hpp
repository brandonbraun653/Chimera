/******************************************************************************
 *  File Name:
 *    usart_user.hpp
 *
 *  Description:
 *    Implements an interface to create a Chimera USART peripheral
 *
 *  2020-2024 | Brandon Braun | brandonbraun653@gmail.com
 *****************************************************************************/

#pragma once
#ifndef CHIMERA_USART_USER_HPP
#define CHIMERA_USART_USER_HPP

/*-----------------------------------------------------------------------------
Includes
-----------------------------------------------------------------------------*/
#include <Chimera/common>
#include <Chimera/source/drivers/peripherals/usart/usart_intf.hpp>
#include <Chimera/source/drivers/peripherals/usart/usart_types.hpp>

namespace Chimera::USART
{
  /*---------------------------------------------------------------------------
  Public Functions
  ---------------------------------------------------------------------------*/
  Chimera::Status_t initialize();
  Chimera::Status_t reset();
  bool isChannelUSART( const Chimera::Serial::Channel channel );
  Driver_rPtr getDriver( const Chimera::Serial::Channel channel );


  /*---------------------------------------------------------------------------
  Classes
  ---------------------------------------------------------------------------*/
  class Driver : public Chimera::Thread::Lockable<Driver>,
                 public Chimera::Thread::AsyncIO<Driver>,
                 public IUSART
  {
  public:
    using Chimera::Thread::AsyncIO<Driver>::AsyncIO;

    Driver();
    ~Driver();

    Chimera::Status_t open( const Chimera::Serial::Config &config );
    Chimera::Status_t close();
    int               write( const void *const buffer, const size_t length, const size_t timeout = Chimera::Thread::TIMEOUT_DONT_WAIT );
    int               read( void *const buffer, const size_t length, const size_t timeout = Chimera::Thread::TIMEOUT_DONT_WAIT );

  private:
    friend Chimera::Thread::Lockable<Driver>;
    friend Chimera::Thread::AsyncIO<Driver>;
    void *mImpl;
  };
}  // namespace Chimera::USART

#endif /* !CHIMERA_USART_USER_HPP */
