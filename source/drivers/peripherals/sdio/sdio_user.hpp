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
#include <Chimera/source/drivers/callback/callback_intf.hpp>

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
  class Driver : public Chimera::Thread::Lockable<Driver>,
                 public Chimera::Thread::AsyncIO<Driver>,
                 public Chimera::Callback::DelegateService<Driver, CallbackId>,
                 public ISDIO
  {
  public:
    using Chimera::Thread::AsyncIO<Driver>::AsyncIO;

    Driver();
    ~Driver();

    Chimera::Status_t open( const HWConfig &init );
    Chimera::Status_t connect();
    void              close();
    Chimera::Status_t writeBlock( const uint32_t blockAddress, const size_t blockCount, const void *const buffer, const size_t size );
    Chimera::Status_t readBlock( const uint32_t blockAddress, const size_t blockCount, void *const buffer, const size_t size );
    Chimera::Status_t eraseBlock( const uint32_t blockAddress, const size_t blockCount );
    CardState         getCardState();
    Chimera::Status_t getCardStatus( CardStatus &status );
    Chimera::Status_t getCardIdentity( CardIdentity &identity );
    Chimera::Status_t getCardSpecificData( CardSpecificData &data );
    Chimera::Status_t getCardInfo( CardInfo &info );

  private:
    friend Chimera::Thread::Lockable<Driver>;
    friend Chimera::Thread::AsyncIO<Driver>;
		friend Chimera::Callback::DelegateService<Driver, CallbackId>;
    void *mImpl;
  };
}  // namespace Chimera::SDIO

#endif /* !CHIMERA_SDIO_USER_HPP */
