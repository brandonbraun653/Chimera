/********************************************************************************
 *   File Name:
 *    callback_intf.hpp
 *
 *   Description:
 *    Describes various kinds of callback interfaces that can be inherited from
 *    to build up a class's functionality and guarantee a particular interface.
 *
 *   2019 | Brandon Braun | brandonbraun653@gmail.com
 ********************************************************************************/

#pragma once
#ifndef CHIMERA_CALLBACK_INTERFACE_HPP
#define CHIMERA_CALLBACK_INTERFACE_HPP

/* C++ Includes */
#include <algorithm>
#include <unordered_map>
#include <vector>

/* Chimera Includes */
#include <Chimera/types/common_types.hpp>
#include <Chimera/types/callback_types.hpp>
#include <Chimera/types/event_types.hpp>

namespace Chimera::Callback
{
  /**
   *  Expands an inheriting class functionality with the ability to store
   *  callback functions that are attached to a particular event trigger.
   *  This is accomplished through the CRTP pattern.
   */
  template<typename DerivedClass, typename CallbackType>
  class Manager
  {
  public:
    ~Manager() = default;

    Chimera::Status_t attachCallback( const Chimera::Event::Trigger event, Callback<CallbackType> &cb )
    {
      cb.id = idCounter++;

      mappedCallbacks[ event ].push_back( cb );
      return Chimera::CommonStatusCodes::OK;
    }

    Chimera::Status_t detachCallback( const Chimera::Event::Trigger event, Callback<CallbackType> &cb )
    {
      auto iterator = mappedCallbacks.find( event );

      /*------------------------------------------------
      Finding a valid iterator within the map means there was 
      some kind of vector attached to the given event. Search
      it for the related callback to remove.
      ------------------------------------------------*/
      if ( iterator != mappedCallbacks.end() )
      {
        auto vector = iterator->second;
        auto ptr = vector.begin();

        for ( ptr = vector.begin(); ptr != vector.end(); ptr++ )
        {
          if ( ptr->id == cb.id )
          {
            break;
          }
        }

        vector.erase( ptr );
      }

      return Chimera::CommonStatusCodes::OK;
    }

  protected:
    friend DerivedClass;
    std::unordered_map<Chimera::Event::Trigger, std::vector<Callback<CallbackType>>> mappedCallbacks;

  private:
    size_t idCounter;

    Manager() : idCounter( 0 )
    {
    }
  };

}  // namespace Chimera::Callback

#endif /* !CHIMERA_CALLBACK_INTERFACE_HPP */