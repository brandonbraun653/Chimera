/******************************************************************************
 *  File Name:
 *    can_intf.hpp
 *
 *  Description:
 *    Models the Chimera CAN interface
 *
 *  2020-2022 | Brandon Braun | brandonbraun653@gmail.com
 *****************************************************************************/

#pragma once
#ifndef CHIMERA_CAN_INTERFACE_HPP
#define CHIMERA_CAN_INTERFACE_HPP

/*-----------------------------------------------------------------------------
Includes
-----------------------------------------------------------------------------*/
#include <Chimera/common>
#include <Chimera/thread>
#include <Chimera/event>
#include <Chimera/source/drivers/peripherals/can/can_types.hpp>

namespace Chimera::CAN
{
  /*---------------------------------------------------------------------------
  Public Functions
  ---------------------------------------------------------------------------*/
  namespace Backend
  {
    /**
     *  Registers the backend driver with Chimera
     *
     *  @param[in]  registry    Chimera's copy of the driver interface
     *  @return Chimera::Status_t
     */
    extern Chimera::Status_t registerDriver( DriverConfig &registry );
  }  // namespace Backend


  /*---------------------------------------------------------------------------
  Classes
  ---------------------------------------------------------------------------*/
  class HWInterface
  {
  public:
    virtual ~HWInterface() = default;

    virtual Chimera::Status_t open( const DriverConfig &cfg )                       = 0;
    virtual Chimera::Status_t close()                                               = 0;
    virtual CANStatus         getStatus()                                           = 0;
    virtual Chimera::Status_t send( const BasicFrame &frame )                       = 0;
    virtual Chimera::Status_t receive( BasicFrame &frame )                          = 0;
    virtual Chimera::Status_t filter( const Filter *const list, const size_t size ) = 0;
    virtual Chimera::Status_t flush( BufferType buffer )                            = 0;
    virtual size_t            available()                                           = 0;
  };


  /**
   *  Virtual class to facilitate easy mocking of the driver
   */
#if defined( CHIMERA_VIRTUAL )
  class ICAN : virtual public HWInterface,
               virtual public Chimera::Thread::AsyncIOInterface,
               virtual public Chimera::Thread::LockableInterface
  {
  public:
    virtual ~ICAN() = default;
  };
#else
  class ICAN
  {
  };
#endif /* CHIMERA_VIRTUAL */

}  // namespace Chimera::CAN

#endif /* !CHIMERA_CAN_INTERFACE_HPP */
