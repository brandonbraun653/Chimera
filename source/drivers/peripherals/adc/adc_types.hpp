/******************************************************************************
 *  File Name:
 *    adc_types.hpp
 *
 *  Description:
 *    Chimera ADC types
 *
 *  2020-2022 | Brandon Braun | brandonbraun653@gmail.com
 *****************************************************************************/

#pragma once
#ifndef CHIMERA_ADC_TYPES_HPP
#define CHIMERA_ADC_TYPES_HPP

/*-----------------------------------------------------------------------------
Includes
-----------------------------------------------------------------------------*/
#include <Aurora/utility>
#include <Chimera/clock>
#include <Chimera/common>
#include <cstdint>
#include <cstring>
#include <etl/delegate.h>
#include <limits>

namespace Chimera::ADC
{
  /*---------------------------------------------------------------------------
  Forward Declarations
  ---------------------------------------------------------------------------*/
  class Driver;
  struct Sample;
  struct InterruptDetail;

  /*---------------------------------------------------------------------------
  Aliases
  ---------------------------------------------------------------------------*/
  using Driver_rPtr = Driver *;

  /*---------------------------------------------------------------------------
  Constants
  ---------------------------------------------------------------------------*/
  static constexpr float VOLTAGE_OOR = std::numeric_limits<float>::max();

  /*---------------------------------------------------------------------------
  Enumerations
  ---------------------------------------------------------------------------*/
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
  enum class Peripheral : uint8_t
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
    HW_READY,     /**< Physical hardware is ready to start sampling */
    EOC_SINGLE,   /**< End of conversion single channel */
    EOC_SEQUENCE, /**< End of conversion sequence sample */
    ANALOG_WD,    /**< Analog watchdog event */
    OVERRUN,      /**< New sample overwrote unread old sample */

    NUM_OPTIONS,
    NONE
  };
  ENUM_CLS_BITWISE_OPERATOR( Interrupt, & );
  ENUM_CLS_BITWISE_OPERATOR( Interrupt, | );

  /**
   *  Describes the sampling behavior of hardware
   */
  enum class SamplingMode : uint8_t
  {
    ONE_SHOT,   /**< User must start sample. Once sample completes, a manual trigger must start another */
    CONTINUOUS, /**< The hardware starts a new sample after the last completes */
    TRIGGER,    /**< External/internal signal can trigger the sample */

    NUM_OPTIONS,
    UNKNOWN
  };

  /**
   *  Instructs the driver implementation on how data should be moved
   *  from the ADC registers into user buffers.
   */
  enum class TransferMode : uint8_t
  {
    ONE_SHOT,  /**< User must manually get a sample. ONLY use for single-channel ADC */
    INTERRUPT, /**< Interrupts on End-of-Conversion will trigger the data transfer */
    DMA,       /**< DMA is used to trigger data transfers */

    NUM_OPTIONS,
    UNKNOWN
  };

  /**
   * @brief Determines edge detection for a hardware trigger
   */
  enum class TriggerMode : uint8_t
  {
    RISING_EDGE,
    FALLING_EDGE,
    BOTH_EDGE,

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
  enum class OverSampler : uint8_t
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


  enum class OverSampleShift : uint8_t
  {
    OS_NONE,
    OS_1_BIT,
    OS_2_BIT,
    OS_3_BIT,
    OS_4_BIT,
    OS_5_BIT,
    OS_6_BIT,
    OS_7_BIT,
    OS_8_BIT,

    NUM_OPTIONS,
    UNKNOWN
  };


  /**
   *  Input clock prescaler options
   */
  enum class PreScaler : uint8_t
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
   * @brief Channel grouping selections
   */
  enum class SequenceGroup : uint8_t
  {
    REGULAR,    /**< Regular channel grouping */
    INJECTED,   /**< Injected channel sequence group */

    NUM_OPTIONS,
    UNKNOWN
  };

  /*---------------------------------------------------------------------------
  Aliases
  ---------------------------------------------------------------------------*/
  using SampleList  = std::array<Sample, 19>;
  using ChannelList = std::array<Channel, 19>;

  /*---------------------------------------------------------------------------
  Structures
  ---------------------------------------------------------------------------*/
  struct Sample
  {
    size_t us;     /**< Timestamp in microseconds */
    size_t counts; /**< Measured ADC counts */

    void clear()
    {
      us     = 0;
      counts = 0;
    }

    bool inline isCleared()
    {
      return ( !us && !counts );
    }
  };

  /**
   *  High level configuration options of an ADC peripheral. These
   *  settings will apply to all configured channels.
   */
  struct DriverConfig
  {
    Peripheral          periph;              /**< Which peripheral instance is being configured */
    Interrupt           bmISREnable;         /**< Bit mask of interrupts to enable */
    OverSampler         overSampleRate;      /**< Over sampling rate, if any */
    OverSampleShift     overSampleShift;     /**< How much to right shift after oversampling, if any */
    TransferMode        transferMode;        /**< Conversion result memory transfer method */
    Resolution          resolution;          /**< Conversion resolution */
    PreScaler           clockPrescale;       /**< Requested prescaler to drive ADC from system clock [1, 255] */
    Chimera::Clock::Bus clockSource;         /**< Which clock drives the prescaler */
    size_t              defaultSampleCycles; /**< Default number of clock cycles each channel will sample for */

    void clear()
    {
      periph              = Peripheral::UNKNOWN;
      bmISREnable         = Interrupt::NONE;
      overSampleRate      = OverSampler::OS_NONE;
      overSampleShift     = OverSampleShift::OS_NONE;
      transferMode        = TransferMode::ONE_SHOT;
      clockPrescale       = PreScaler::DIV_1;
      clockSource         = Chimera::Clock::Bus::UNKNOWN_BUS;
      defaultSampleCycles = 100;
    }
  };


  /**
   *  Initializes a group sampling sequence
   */
  struct SequenceInit
  {
    SamplingMode seqMode;     /**< How should the user expect sampling to occur? */
    SequenceGroup seqGroup;    /**< Which channel grouping to map the sequence to */
    TriggerMode  trigMode;    /**< Hardware trigger mode, if SamplingMode == TRIGGER */
    size_t       trigChannel; /**< Which trigger channel to use. HW target specific. */
    ChannelList *channels;    /**< List of channels (in order) to be sampled */
    size_t       numChannels; /**< How many channels are in the sequence */

    SequenceInit() :
        seqMode( SamplingMode::UNKNOWN ), seqGroup( SequenceGroup::UNKNOWN ), trigMode( TriggerMode::UNKNOWN ), trigChannel( 0 ), channels( nullptr ),
        numChannels( 0 )

    {
    }

    void clear()
    {
      seqMode     = SamplingMode::UNKNOWN;
      seqGroup    = SequenceGroup::UNKNOWN;
      trigMode    = TriggerMode::UNKNOWN;
      numChannels = 0;
      trigChannel = 0;

      if ( channels )
      {
        channels->fill( Channel::UNKNOWN );
      }
    }
  };


  /**
   *  Some helpful information that is passed into an interrupt
   *  event handler callback function.
   */
  struct InterruptDetail
  {
    Interrupt isr;         /**< ISR type that occurred */
    Channel   channel;     /**< Channel the event occurred on (single conversion only) */
    uint16_t *samples;     /**< Data that was sampled */
    uint16_t  num_samples; /**< How many samples are available */
    float     resolution;  /**< ADC resolution at sample time */
    float     vref;        /**< Voltage reference used with the sample */

    void clear()
    {
      channel     = Channel::UNKNOWN;
      isr         = Interrupt::NONE;
      samples     = nullptr;
      num_samples = 0;
      resolution  = 0.0f;
      vref        = 0.0f;
    }
  };


  /*---------------------------------------------------------------------------
  Aliases
  ---------------------------------------------------------------------------*/
  using ISRCallback   = etl::delegate<void( const InterruptDetail   &)>;
  using CallbackArray = std::array<ISRCallback, EnumValue( Interrupt::NUM_OPTIONS )>;


  /**
   * @brief Configuration structure for analog watchdog monitoring
   */
  struct WatchdogConfig
  {
    Channel                   adcChannel;    /**< ADC channel to monitor */
    Watchdog                  wdgChannel;    /**< Watchdog channel that should do the monitoring */
    uint32_t                  highThreshold; /**< High threshold of the monitor  in ADC counts */
    uint32_t                  lowThreshold;  /**< Low threshold of the monitor in ADC counts */
    Chimera::ADC::ISRCallback callback;      /**< Callback to execute on watchdog trip event */

    void clear()
    {
      adcChannel    = Channel::UNKNOWN;
      wdgChannel    = Watchdog::UNKNOWN;
      highThreshold = 0;
      lowThreshold  = 0;
      callback      = {};
    }
  };

  /*---------------------------------------------------------------------------
  Backend Driver Namespace
  ---------------------------------------------------------------------------*/
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
       *  Factory function that gets a pointer to an instance of the backend
       *  driver, as long as it conforms to the expected interface.
       */
      Driver_rPtr ( *getDriver )( const Peripheral periph );

      /**
       *  Checks if a peripheral instance supports a given feature
       */
      bool ( *featureSupported )( const Peripheral periph, const Feature feature );
    };
  }  // namespace Backend

}  // namespace Chimera::ADC

#endif /* !CHIMERA_ADC_TYPES_HPP */
