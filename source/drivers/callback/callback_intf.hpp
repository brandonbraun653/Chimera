/******************************************************************************
 *  File Name:
 *    callback_intf.hpp
 *
 *  Description:
 *    Describes various kinds of callback interfaces that can be inherited from
 *    to build up a class's functionality and guarantee a particular interface.
 *
 *  2019-2023 | Brandon Braun | brandonbraun653@gmail.com
 *****************************************************************************/

#pragma once
#ifndef CHIMERA_CALLBACK_INTERFACE_HPP
#define CHIMERA_CALLBACK_INTERFACE_HPP

/*-----------------------------------------------------------------------------
Includes
-----------------------------------------------------------------------------*/
#include <Chimera/common>
#include <cstddef>
#include <cstdint>
#include <etl/delegate.h>
#include <etl/delegate_service.h>

namespace Chimera::Callback
{
  /*---------------------------------------------------------------------------
  Class Definitions
  ---------------------------------------------------------------------------*/
  template<typename CRTPClass, typename CBType, typename EventDataType = void *>
  class DelegateService
  {
  public:
    /**
     *  Register a callback to be invoked upon some event that occurs during
     *  the service processing.
     *
     *  @param[in]  id          Which event to register against
     *  @param[in]  func        The function to register
     *  @return Chimera::Status_t
     */
    Chimera::Status_t registerCallback( const CBType id, etl::delegate<void( size_t )> func )
    {
      /*-------------------------------------------------
      Input protection
      -------------------------------------------------*/
      if ( !( id < CBType::CB_NUM_OPTIONS ) )
      {
        return Chimera::Status::INVAL_FUNC_PARAM;
      }

      /*-------------------------------------------------
      Register the callback
      -------------------------------------------------*/
      static_cast<CRTPClass *>( this )->lock();
      if ( id == CBType::CB_UNHANDLED )
      {
        mCBService_registry.register_unhandled_delegate( func );
      }
      else
      {
        mCBService_registry.register_delegate( id, func );
      }

      static_cast<CRTPClass *>( this )->unlock();
      return Chimera::Status::OK;
    }

    /**
     * Sets the data associated with a particular callback event
     *
     * @param id                Which event to post data against
     * @param data              The actual data
     */
    void setCallbackData( const CBType id, EventDataType &data )
    {
      if ( id < CBType::CB_NUM_OPTIONS )
      {
        mCBService_data[ id ] = data;
      }
    }

    /**
     *  Gets the last known data that happened in a given callback event
     *
     *  @param[in]  id          The callback id to get info on
     *  @return const EventDataType *
     */
    const EventDataType *whatHappened( const CBType id )
    {
      if ( id < CBType::CB_NUM_OPTIONS )
      {
        return &mCBService_data[ id ];
      }
      else
      {
        return nullptr;
      }
    }

  protected:
    etl::array<EventDataType, CBType::CB_NUM_OPTIONS> mCBService_data;
    etl::delegate_service<CBType::CB_NUM_OPTIONS>     mCBService_registry;
  };
}  // namespace Chimera::Callback

#endif /* !CHIMERA_CALLBACK_INTERFACE_HPP */
