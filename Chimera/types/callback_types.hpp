/********************************************************************************
 *   File Name:
 *    callback_types.hpp
 *
 *   Description:
 *    Various types of callback signatures used in Chimera. This is surprisingly
 *    difficult to keep track of the naming, so an incredibly generic format is 
 *    used. Whether or not this is a good idea is up for debate.
 *
 *   2019 | Brandon Braun | brandonbraun653@gmail.com
 ********************************************************************************/

#pragma once
#ifndef CHIMERA_CALLBACK_TYPES_HPP
#define CHIMERA_CALLBACK_TYPES_HPP

/* C++ Includes */
#include <cstdint>
#include <functional>
#include <unordered_map>
#include <vector>

/* Boost Includes */
#include <boost/function.hpp>

/* Chimera Includes */
#include <Chimera/types/event_types.hpp>

namespace Chimera::Callback
{
  using CB1 = boost::function<void( void )>;
  using CB1Map = std::unordered_map<Chimera::Event::Trigger, std::vector<CB1>>;

  template<typename Functor>
  struct Callback
  {
    Functor func;
    size_t id;
  };

  /**
   *  A generic callback type that can be used across a wide variety of
   *  peripheral ISR events. Sometimes the callbacks need data and sometimes
   *  they do not, so a 'handle' like function signature was used to give the
   *  most flexibility. The const data specifier was intentionally left removed
   *  so that in a unique scenario the callbacks can talk directly with the handle.
   *
   *  @param[in]  handle    Some data structure that the ISR wants to give to callbacks
   *  @param[in]  size      The size of the handle structure in bytes
   *  @return void
   */
  using ISRCallbackFunction = std::function<void( void *handle, size_t size )>;
  using ISRCallback = Callback<ISRCallbackFunction>;

}  // namespace Chimera::Callback

#endif /* !CHIMERA_CALLBACK_TYPES_HPP */