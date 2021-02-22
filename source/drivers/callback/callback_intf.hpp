/********************************************************************************
 *  File Name:
 *    callback_intf.hpp
 *
 *  Description:
 *    Describes various kinds of callback interfaces that can be inherited from
 *    to build up a class's functionality and guarantee a particular interface.
 *
 *  2019-2021 | Brandon Braun | brandonbraun653@gmail.com
 ********************************************************************************/

#pragma once
#ifndef CHIMERA_CALLBACK_INTERFACE_HPP
#define CHIMERA_CALLBACK_INTERFACE_HPP

/* STL Includes */
#include <cstdint>
#include <cstddef>

/* ETL Includes */
#include <etl/delegate.h>
#include <etl/delegate_service.h>

/* Chimera Includes */
#include <Chimera/common>

namespace Chimera::Callback
{
  /*-------------------------------------------------------------------------------
  Class Definitions
  -------------------------------------------------------------------------------*/
  template<typename CRTPClass, typename CBType, typename WhatHappenedType = void*>
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
        mDelegateRegistry.register_unhandled_delegate( func );
      }
      else
      {
        mDelegateRegistry.register_delegate( id, func );
      }

      static_cast<CRTPClass *>( this )->unlock();
      return Chimera::Status::OK;
    }

    const WhatHappenedType &whatHappened()
    {
      return mDelegateWhatHappened;
    }

  protected:
    WhatHappenedType mDelegateWhatHappened;
    etl::delegate_service<CBType::CB_NUM_OPTIONS> mDelegateRegistry;
  };
}  // namespace Chimera::Callback

#endif /* !CHIMERA_CALLBACK_INTERFACE_HPP */
