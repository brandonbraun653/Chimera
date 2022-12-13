/******************************************************************************
 *  File Name:
 *    exti_types.hpp
 *
 *  Description:
 *    Types for the External Interrupt driver
 *
 *  2020 | Brandon Braun | brandonbraun653@gmail.com
 *****************************************************************************/

#pragma once
#ifndef CHIMERA_EXTI_TYPES_HPP
#define CHIMERA_EXTI_TYPES_HPP

/* STL Includes */
#include <cstdint>
#include <cstddef>

namespace Chimera::EXTI
{
  /*---------------------------------------------------------------------------
  Aliases
  ---------------------------------------------------------------------------*/
  using EventLine_t = uint8_t;

  /*---------------------------------------------------------------------------
  Enumerations
  ---------------------------------------------------------------------------*/
  enum class EdgeTrigger : uint8_t
  {
    RISING_EDGE,
    FALLING_EDGE,
    BOTH_EDGE,

    NUM_OPTIONS,
    UNKNOWN
  };


  /*---------------------------------------------------------------------------
  Structures
  ---------------------------------------------------------------------------*/
  namespace Backend
  {
    struct DriverConfig
    {
      bool isSupported; /**< A simple flag to let Chimera know if the driver is supported */

      /*-------------------------------------------------
      Interface function pointers. See exti_user.hpp for
      documentation on what each function should do.
      -------------------------------------------------*/
      Chimera::Status_t ( *open )();
      Chimera::Status_t ( *close )();
      Chimera::Status_t ( *attach )( const EventLine_t listener, const EdgeTrigger edge, Chimera::Function::vGeneric callback );
      Chimera::Status_t ( *detach )( const EventLine_t listener );
      Chimera::Status_t ( *trigger )( const EventLine_t listener );
      Chimera::Status_t ( *disable )( const EventLine_t listener );
      Chimera::Status_t ( *enable )( const EventLine_t listener );
      EventLine_t ( *numInterruptLines )();
    };
  }  // namespace Backend

}  // namespace Chimera::EXTI

#endif /* !CHIMERA_EXTI_TYPES_HPP */
