/******************************************************************************
 *  File Name:
 *    interrupt_types.hpp
 *
 *  Description:
 *    Interrupt type definitions
 *
 *  2021 | Brandon Braun | brandonbraun653@gmail.com
 *****************************************************************************/

#pragma once
#ifndef CHIMERA_INTERRUPT_TYPES_HPP
#define CHIMERA_INTERRUPT_TYPES_HPP

/* STL Includes */
#include <cstdint>
#include <limits>

/* Chimera Includes */
#include <Chimera/common>
#include <Chimera/peripheral>

/* ETL Includes */
#include <etl/delegate.h>

namespace Chimera::Interrupt
{
  /*---------------------------------------------------------------------------
  Forward Declarations
  ---------------------------------------------------------------------------*/
  struct SignalCallback;

  /*---------------------------------------------------------------------------
  Aliases
  ---------------------------------------------------------------------------*/
  using Signal_t = int16_t;
  using ISRCallback = etl::delegate<void(void)>;
  using UserCallback = etl::delegate<void(const SignalCallback *const event)>;

  /*---------------------------------------------------------------------------
  Constants
  ---------------------------------------------------------------------------*/
  static constexpr Signal_t SIGNAL_INVALID = std::numeric_limits<Signal_t>::min();

  /*---------------------------------------------------------------------------
  Structures
  ---------------------------------------------------------------------------*/
  struct SignalCallback
  {
    void *data;      /**< Possible event data from the ISR */
    uint16_t size;   /**< Number of bytes contained in the 'data' pointer */
    Signal_t signal; /**< Which signal is associated with this data */

    /**
     *  A fast callback will execute directly inside the ISR. This should
     *  be kept extremely short and **not** access any kind of system
     *  resource, except those tagged with the 'fromISR' keywords.
     *
     *  @return void
     */
    ISRCallback isrCallback;

    /**
     *  A thread aware callback will execute inside a high priority thread
     *  registered with the peripheral interupt. This allows far more complex
     *  and time consuming tasks to execute, but at the cost of latency.
     *
     *  @param[in]  event     Pointer to this structure
     *  @return void
     */
     UserCallback userCallback;
  };

  namespace Backend
  {
    struct DriverConfig
    {
      bool isSupported;
      Chimera::Status_t ( *initialize )( void );
      Chimera::Status_t ( *reset )( void );

      /**
       *  Registers a set of handler functions to be invoked on a particular
       *  peripheral ISR signal.
       *
       *  @param[in]  type      Which peripheral to register this against
       *  @param[in]  signal    The ISR event being listened to
       *  @param[in]  callback  The callback structure being registered
       *  @return Chimera::Status_t
       */
      Chimera::Status_t ( *registerISRHandler )( const Peripheral::Type type, const Signal_t signal,
                                                 const SignalCallback &callback );

      Chimera::Status_t ( *disableISR )( const Signal_t signal );
      Chimera::Status_t ( *enableISR )( const Signal_t signal );
    };
  }  // namespace Backend

  /*---------------------------------------------------------------------------
  Aliases
  ---------------------------------------------------------------------------*/
  /**
   *  Allows a peripheral driver to hold a number of ISR callback
   *  structures, indexed by a driver defined lookup value.
   */
  template<const size_t SIZE>
  using CallbackArray = std::array<SignalCallback, SIZE>;

}  // namespace Chimera::Interrupt

#endif  /* !CHIMERA_INTERRUPT_TYPES_HPP */
