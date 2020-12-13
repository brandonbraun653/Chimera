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
    HW_READY       = ( 1u << 0 ), /**< Physical hardware is ready to start sampling */
    EOC_PRI_LO     = ( 1u << 1 ), /**< End of conversion single low priority channel */
    EOC_PRI_LO_SEQ = ( 1u << 2 ), /**< End of conversion low priority group */
    EOC_PRI_HI     = ( 1u << 3 ), /**< End of conversion single high priority channel */
    EOC_PRI_HI_SEQ = ( 1u << 4 ), /**< End of conversion high priority group*/
    AWD_0_TRIGGER  = ( 1u << 5 ), /**< Analog watchdog 0 event */
    AWD_1_TRIGGER  = ( 1u << 6 ), /**< Analog watchdog 1 event*/
    AWD_2_TRIGGER  = ( 1u << 7 ), /**< Analog watchdog 2 event*/
    OVERRUN        = ( 1u << 8 ), /**< New sample overwrote unread old sample */

    NUM_OPTIONS = 9,
    NONE        = ( 1u << 15 )
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
  enum class Sensor : uint8_t
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
    OS_NONE,
    OS_2X,
    OS_4X,
    OS_8X,
    OS_16X,
    OS_32X,
    OS_64X,
    OS_128X,
    OS_256X,

    NUM_OPTIONS,
    UNKNOWN
  };

  /**
   *  Input clock prescaler options
   */
  enum class Prescaler : uint8_t
  {
    DIV_1,
    DIV_2,
    DIV_4,
    DIV_6,
    DIV_8,
    DIV_10,
    DIV_12,
    DIV_16,
    DIV_32,
    DIV_64,
    DIV_128,
    DIV_256,

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
   *  High level configuration options of an ADC peripheral. These
   *  settings will apply to all configured channels.
   */
  struct DriverConfig
  {
    Converter periph;                /**< Which peripheral instance is being configured */
    Interrupt bmISREnable;           /**< Bit mask of interrupts to enable */
    Oversampler oversampleRate;      /**< Over sampling rate, if any */
    TransferMode transferMode;       /**< Conversion result memory transfer method */
    Resolution resolution;           /**< Conversion resolution */
    Prescaler clockPrescale;         /**< Requested prescaler to drive ADC from system clock [1, 255] */
    Chimera::Clock::Bus clockSource; /**< Which clock drives the prescaler */

    void clear()
    {
      periph         = Converter::UNKNOWN;
      bmISREnable    = Interrupt::NONE;
      oversampleRate = Oversampler::OS_NONE;
      transferMode   = TransferMode::POLLING;
      clockPrescale  = Prescaler::DIV_1;
      clockSource    = Chimera::Clock::Bus::UNKNOWN_BUS;
    }
  };


  /**
   *  Initializes a group sampling sequence
   */
  struct GroupInit
  {
    SamplingMode sampleMode; /**< How should the user expect sampling to occur? */
    SampleGroup sampleGroup; /**< Priority of the group being configured */
    Channel *groupList;      /**< List of channels (in order) to be sampled */
    size_t groupLength;      /**< Number of elements in groupList */

    void clear()
    {
      groupLength = 0;
      groupList   = nullptr;
      sampleGroup = SampleGroup::UNKNOWN;
      sampleMode  = SamplingMode::UNKNOWN;
    }
  };


  /**
   *  Some helpful information that is passed into an interrupt
   *  event handler callback function.
   */
  struct InterruptDetail
  {
    Interrupt whichISR;   /**< ISR type that occurred */
    Channel whichChannel; /**< Channel the event occured on */

    void clear()
    {
      whichChannel = Channel::UNKNOWN;
      whichISR     = Interrupt::NONE;
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
