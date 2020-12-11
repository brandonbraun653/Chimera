/********************************************************************************
 *  File Name:
 *    adc_types.hpp
 *
 *  Description:
 *    Chimera ADC types
 *
 *  2020 | Brandon Braun | brandonbraun653@gmail.com
 ********************************************************************************/

#pragma once
#ifndef CHIMERA_ADC_TYPES_HPP
#define CHIMERA_ADC_TYPES_HPP

/* STL Includes */
#include <cstdint>
#include <cstring>
#include <memory>

/* Chimera Includes */
#include <Chimera/common>
#include <Chimera/clock>

namespace Chimera::ADC
{
  /*-------------------------------------------------------------------------------
  Forward Declarations
  -------------------------------------------------------------------------------*/
  class Driver;


  /*-------------------------------------------------------------------------------
  Aliases
  -------------------------------------------------------------------------------*/
  using Driver_sPtr = std::shared_ptr<Driver>;


  /*-------------------------------------------------------------------------------
  Constants
  -------------------------------------------------------------------------------*/


  /*-------------------------------------------------------------------------------
  Enumerations
  -------------------------------------------------------------------------------*/
  /**
   *  Represents a physical ADC hardware peripheral
   */
  enum class Converter : uint8_t
  {
    ADC_0,
    ADC_1,
    ADC_2,
    ADC_3,

    NUM_OPTIONS,
    UNKNOWN
  };

  /**
   *  Represents the multiplexed channels per peripheral
   */
  enum class Channel : uint8_t
  {
    ADC_CH_0,
    ADC_CH_1,
    ADC_CH_2,
    ADC_CH_3,
    ADC_CH_4,
    ADC_CH_5,
    ADC_CH_6,
    ADC_CH_7,
    ADC_CH_8,
    ADC_CH_9,
    ADC_CH_10,
    ADC_CH_11,
    ADC_CH_12,
    ADC_CH_13,
    ADC_CH_14,
    ADC_CH_15,
    ADC_CH_16,
    ADC_CH_17,
    ADC_CH_18,
    ADC_CH_19,

    NUM_OPTIONS,
    ALL,
    UNKNOWN
  };

  /**
   *  Analog hw watchdog devices (not always supported)
   */
  enum class Watchdog : uint8_t
  {
    ANALOG_0,
    ANALOG_1,
    ANALOG_2,

    NUM_OPTIONS,
    UNKNOWN
  };

  /**
   *  Bit field to enable/disable interupts in the config
   */
  enum class Interrupt : uint16_t
  {
    HW_READY         = ( 1u << 0 ),
    EOC_REGULAR      = ( 1u << 1 ),
    EOC_SEQ_REGULAR  = ( 1u << 2 ),
    EOC_INJECTED     = ( 1u << 3 ),
    EOC_SEQ_INJECTED = ( 1u << 4 ),
    AWD_0_TRIGGER    = ( 1u << 5 ),
    AWD_1_TRIGGER    = ( 1u << 6 ),
    AWD_2_TRIGGER    = ( 1u << 7 ),
    OVERRUN          = ( 1u << 8 ),

    NUM_OPTIONS = 9,
    UNKNOWN     = ( 1u << 15 )
  };

  /**
   *  Output conversion data alignment
   */
  enum class Align : uint8_t
  {
    ALIGN_RIGHT,
    ALIGN_LEFT,

    NUM_OPTIONS,
    UNKNOWN
  };

  /**
   *  Data conversion mode
   */
  enum class ConversionMode : uint8_t
  {
    POLLING,
    CONTINUOUS,

    NUM_OPTIONS,
    UNKNOWN
  };

  /**
   *  Possible internal sensors connected to the ADC
   */
  enum class InternalSensor : uint8_t
  {
    VBAT,
    TEMP,
    BANDGAP,

    NUM_OPTIONS,
    UNKNOWN
  };

  enum class Oversampler : uint8_t
  {

    NUM_OPTIONS,
    UNKNOWN
  };

  enum class Resolution : uint8_t
  {
    BIT_12,
    BIT_10,
    BIT_8,
    BIT_6,

    NUM_OPTIONS,
    UNKNOWN
  };


  /*-------------------------------------------------------------------------------
  Structures
  -------------------------------------------------------------------------------*/
  struct HardwareInit
  {
    Converter periph;
    Interrupt bmISREnable;
    Oversampler oversampleRate;
    bool dmaEnabled;

    uint8_t clockPrescale;
    Chimera::Clock::Bus clockSource;

    void clear()
    {
      dmaEnabled = false;
    }
  };

  namespace Backend
  {
    struct DriverConfig
    {
      bool isSupported; /**< A simple flag to let Chimera know if the driver is supported */

      /**
       *  Function pointer that initializes the backend driver's
       *  memory. Should really only call once for initial set up.
       */
      Chimera::Status_t ( *initialize )();

      /**
       *  Resets the backend driver hardware to default configuration
       *  settings, but does not wipe out any memory.
       */
      Chimera::Status_t ( *reset )();

      /**
       *  Factory function that creates a shared_ptr instance of the backend
       *  driver, as long as it conforms to the expected interface.
       */
      Driver_sPtr ( *getDriver )( const Converter periph );
    };
  }  // namespace Backend


  /*-------------------------------------------------------------------------------
  Function Pointers
  -------------------------------------------------------------------------------*/
  using ISRCallback = void ( * )( const Interrupt, const Channel );

}  // namespace Chimera::ADC

#endif /* !CHIMERA_ADC_TYPES_HPP */
