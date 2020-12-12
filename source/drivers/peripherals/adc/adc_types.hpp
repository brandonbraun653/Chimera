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
  using Sample_t    = uint16_t;

  /*-------------------------------------------------------------------------------
  Constants
  -------------------------------------------------------------------------------*/


  /*-------------------------------------------------------------------------------
  Enumerations
  -------------------------------------------------------------------------------*/
  /**
   *  Hardware implemented features that may or may not be supported
   *  on all devices. Use the driver's API to check first if a desired
   *  functionality is supported by your device.
   */
  enum class Feature : uint8_t
  {
    ANALOG_WATCHDOG,       /**< Hardware monitors for high/low input thresholds */
    REGULAR_SAMPLE_GROUP,  /**< Grouping of channels to be sampled at normal priority */
    PRIORITY_SAMPLE_GROUP, /**< Grouping of channels to be sampled at higher priority */


    NUM_OPTIONS,
    UNKNOWN
  };

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
    HW_READY       = ( 1u << 0 ),
    EOC_PRI_LO     = ( 1u << 1 ),
    EOC_PRI_LO_SEQ = ( 1u << 2 ),
    EOC_PRI_HI     = ( 1u << 3 ),
    EOC_PRI_HI_SEQ = ( 1u << 4 ),
    AWD_0_TRIGGER  = ( 1u << 5 ),
    AWD_1_TRIGGER  = ( 1u << 6 ),
    AWD_2_TRIGGER  = ( 1u << 7 ),
    OVERRUN        = ( 1u << 8 ),

    NUM_OPTIONS = 9,
    UNKNOWN     = ( 1u << 15 )
  };

  /**
   *  Describes the sampling behavior of hardware
   */
  enum class SamplingMode : uint8_t
  {
    POLLING,    /**< User must start sample. Once sample completes, a manual trigger must start another */
    CONTINUOUS, /**< The hardware starts a new sample after the last completes */

    NUM_OPTIONS,
    UNKNOWN
  };

  /**
   *  Instructs the driver implementation on how data should be moved
   *  from the ADC registers into user buffers.
   */
  enum class TransferMode : uint8_t
  {
    POLLING,   /**< User must manually get a sample. ONLY use for single-channel ADC */
    INTERRUPT, /**< Interrupts on End-of-Conversion will trigger the data transfer */
    DMA,       /**< DMA is used to trigger data transfers */

    NUM_OPTIONS,
    UNKNOWN
  };

  /**
   *  Possible internal sensors connected to the ADC
   */
  enum class InternalSensor : uint8_t
  {
    VBAT, /**< Measures an external battery voltage */
    VREF, /**< Internal ADC voltage reference */
    TEMP, /**< Internal temperature sensor */

    NUM_OPTIONS,
    UNKNOWN
  };

  /**
   *  Possible oversampling rates
   */
  enum class Oversampler : uint8_t
  {

    NUM_OPTIONS,
    UNKNOWN
  };

  /**
   *  ADC sampling resolution in bits
   */
  enum class Resolution : uint8_t
  {
    BIT_12,
    BIT_10,
    BIT_8,
    BIT_6,

    NUM_OPTIONS,
    UNKNOWN
  };

  /**
   *  If supported by hardware, allows for grouping of hardware
   *  channels in to priority structures.
   */
  enum class SampleGroup : uint8_t
  {
    PRI_LO,
    PRI_HI,

    NUM_OPTIONS,
    UNKNOWN
  };


  /*-------------------------------------------------------------------------------
  Structures
  -------------------------------------------------------------------------------*/
  /**
   *  High level configuration options of an ADC peripheral
   */
  struct HardwareInit
  {
    Converter periph;
    Interrupt bmISREnable;
    Oversampler oversampleRate;
    SamplingMode sampleMode;
    TransferMode transferMode;

    uint8_t clockPrescale;
    Chimera::Clock::Bus clockSource;

    void clear()
    {
    }
  };  // namespace Chimera::ADC

  /**
   *  Initializes a group sampling sequence
   */
  struct GroupInit
  {
    SamplingMode sampleMode; /**< How should the user expect sampling to occur? */
    SampleGroup sampleGroup; /**< Priority of the group being configured */
    Channel *groupList;      /**< List of channels (in order) to be sampled */
    size_t groupLength;      /**< Number of elements in groupList */
  };

  /**
   *  Some helpful information that is passed into an interrupt
   *  event handler callback function.
   */
  struct InterruptDetail
  {
    Interrupt whichISR;
    Channel whichChannel;
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

      /**
       *  Checks if a peripheral instance supports a given feature
       */
      bool ( *featureSupported )( const Converter periph, const Feature feature );
    };
  }  // namespace Backend


  /*-------------------------------------------------------------------------------
  Function Pointers
  -------------------------------------------------------------------------------*/
  using ISRCallback = void ( * )( const InterruptDetail &detail );

}  // namespace Chimera::ADC

#endif /* !CHIMERA_ADC_TYPES_HPP */
