/********************************************************************************
 *   File Name:
 *    serial_types.hpp
 *
 *   Description:
 *    Chimera Serial types
 *
 *   2019 | Brandon Braun | brandonbraun653@gmail.com
 ********************************************************************************/

#pragma once
#ifndef CHIMERA_EVENT_TYPES_HPP
#define CHIMERA_EVENT_TYPES_HPP

/* C++ Includes */
#include <cstdint>
#include <cstdlib>
#include <limits>
#include <memory>
#include <variant>

/* Chimera Includes */
#include <Chimera/common>
#include <Chimera/callback>


namespace Chimera::Event
{
  class Listener;
  using Listener_sPtr = std::shared_ptr<Listener>;
  using Listener_uPtr = std::unique_ptr<Listener>;
  
  enum class Trigger : size_t
  {
    INVALID,            /**< Special case used for initialization */
    READ_COMPLETE,      /**< A read was completed */
    WRITE_COMPLETE,     /**< A write was completed */
    TRANSFER_COMPLETE,  /**< A transfer of some sort completed (bi-directional) */
    SYSTEM_ERROR,       /**< Catch all error case */
  };

  enum class ElementType : uint8_t
  {
    INVALID,
    ATOMIC_NOTIFIER_T,
    THREAD_NOTIFIER_T,
    CALLBACK_T
  };

  struct Actionable
  {
    Trigger trigger;  /**< Which kind of event triggers this Actionable */
    ElementType type; /**< What kind of Actionable is stored in the handle */
    size_t id;        /**< Uniquely identifies this Actionable */

    /**
     *  Stores the user defined element that an external 
     *  processing function will act upon. 
     */
    #if defined( USING_FREERTOS ) && ( CHIMERA_CFG_FREERTOS == 1 )
    std::variant<uint32_t *, SemaphoreHandle_t, Chimera::Callback::ISRCallbackFunction> element;
    #else
    std::variant<uint32_t *, void*, Chimera::Callback::ISRCallbackFunction> element;
    #endif 

    Actionable() :
        trigger( Trigger::INVALID ), type( ElementType::INVALID ), id( std::numeric_limits<size_t>::max() )
    {
    }
  };

}  // namespace Chimera::Event


#endif /* !CHIMERA_EVENT_TYPES_HPP */