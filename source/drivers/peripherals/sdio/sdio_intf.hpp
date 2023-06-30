/******************************************************************************
 *  File Name:
 *    sdio_intf.hpp
 *
 *  Description:
 *    Models the SDIO driver interface
 *
 *  2023 | Brandon Braun | brandonbraun653@protonmail.com
 *****************************************************************************/

#pragma once
#ifndef CHIMERA_SDIO_INTERFACE_HPP
#define CHIMERA_SDIO_INTERFACE_HPP

/*-----------------------------------------------------------------------------
Includes
-----------------------------------------------------------------------------*/
#include <Chimera/common>
#include <Chimera/thread>
#include <Chimera/source/drivers/peripherals/sdio/sdio_types.hpp>

namespace Chimera::SDIO
{
  /*---------------------------------------------------------------------------
  Public Functions
  ---------------------------------------------------------------------------*/
  namespace Backend
  {
    /**
     *  Registers the backend driver with Chimera
     *
     *	@param registry    Chimera's copy of the driver interface
     *	@return Chimera::Status_t
     */
    extern Chimera::Status_t registerDriver( DriverConfig &registry );
  }  // namespace Backend

  /*---------------------------------------------------------------------------
  Classes
  ---------------------------------------------------------------------------*/
  class HWInterface
  {
  public:
    virtual ~HWInterface()                                                                                   = default;
    virtual Chimera::Status_t open( const HWConfig &init )                                                   = 0;
    virtual Chimera::Status_t connect()                                                                      = 0;
    virtual void              close()                                                                        = 0;
    virtual Chimera::Status_t write( const uint32_t address, const void *const buffer, const size_t length ) = 0;
    virtual Chimera::Status_t read( const uint32_t address, void *const buffer, const size_t length )        = 0;
    virtual Chimera::Status_t getCardStatus( CardStatus &status )                                            = 0;
    virtual Chimera::Status_t getCardIdentity( CardIdentity &identity )                                      = 0;
    virtual Chimera::Status_t getCardSpecificData( CardSpecificData &data )                                  = 0;
  };

#if defined( CHIMERA_VIRTUAL )
  class ISDIO : virtual public HWInterface, virtual public Chimera::Thread::LockableInterface
  {
  public:
    virtual ~ISDIO() = default;
  };
#else
  class ISDIO
  {
  };
#endif /* CHIMERA_VIRTUAL */

}  // namespace Chimera::SDIO

#endif /* !CHIMERA_SDIO_INTERFACE_HPP */
