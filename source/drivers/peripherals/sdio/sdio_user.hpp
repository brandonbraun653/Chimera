/******************************************************************************
 *  File Name:
 *    sdio_user.hpp
 *
 *  Description:
 *    User interface to the Chimera SDIO driver
 *
 *  2023 | Brandon Braun | brandonbraun653@protonmail.com
 *****************************************************************************/

#pragma once
#ifndef CHIMERA_SDIO_USER_HPP
#define CHIMERA_SDIO_USER_HPP

/*-----------------------------------------------------------------------------
Includes
-----------------------------------------------------------------------------*/
#include <Chimera/common>
#include <Chimera/source/drivers/peripherals/sdio/sdio_intf.hpp>
#include <Chimera/source/drivers/peripherals/sdio/sdio_types.hpp>

namespace Chimera::SDIO
{
  /*---------------------------------------------------------------------------
  Public Functions
  ---------------------------------------------------------------------------*/
  Chimera::Status_t initialize();
  Chimera::Status_t reset();
  Driver_rPtr       getDriver( const Chimera::SDIO::Channel channel );


  /*---------------------------------------------------------------------------
  Classes
  ---------------------------------------------------------------------------*/
  class Driver : public Chimera::Thread::Lockable<Driver>, public Chimera::Thread::AsyncIO<Driver>, public ISDIO
  {
  public:
    using Chimera::Thread::AsyncIO<Driver>::AsyncIO;

    Driver();
    ~Driver();

    Chimera::Status_t open( const Chimera::SDIO::HWConfig &init );
    void              close();
    int               write( const size_t address, const void *const buffer, const size_t length );
    int               read( const size_t address, void *const buffer, const size_t length );

  private:
    friend Chimera::Thread::Lockable<Driver>;
    friend Chimera::Thread::AsyncIO<Driver>;
    void *mImpl;
  };
}  // namespace Chimera::SDIO

#endif /* !CHIMERA_SDIO_USER_HPP */
