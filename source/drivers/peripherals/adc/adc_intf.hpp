/********************************************************************************
 *  File Name:
 *    adc_intf.hpp
 *
 *  Description:
 *    Models the Chimera ADC interface. This driver is focused on two primary
 *    modes of operation, single sample and group sample. The former measures a
 *    single channel in one-shot mode while the latter queues up a group of
 *    channels to be sampled in sequence.
 *
 *    Conversion results are available to the user either via direct calls in one
 *    shot mode, or by waiting for a callback event to signal that a group sample
 *    has completed.
 *
 *  2020 | Brandon Braun | brandonbraun653@gmail.com
 ********************************************************************************/

#pragma once
#ifndef CHIMERA_ADC_INTERFACE_HPP
#define CHIMERA_ADC_INTERFACE_HPP

/* Chimera Includes */
#include <Chimera/common>
#include <Chimera/thread>
#include <Chimera/source/drivers/peripherals/adc/adc_types.hpp>

namespace Chimera::ADC
{
  /*-------------------------------------------------------------------------------
  Public Functions
  -------------------------------------------------------------------------------*/
  namespace Backend
  {
    /**
     *  Registers the backend driver with Chimera
     *
     *  @param[in]  registry    Chimera's copy of the driver interface
     *  @return Chimera::Status_t
     */
    extern Chimera::Status_t registerDriver( DriverConfig &registry );
  }  // namespace Backend


  /*-------------------------------------------------------------------------------
  Classes
  -------------------------------------------------------------------------------*/
  class HWInterface
  {
  public:
    virtual ~HWInterface() = default;

    /**
     *  Initializes the driver with high level settings. Note that not
     *  all hardware may support a config option, so either pre-check
     *  that necessary features are supported, or check the return code
     *  of this method.
     *
     *  @note Usually there are many ADC channels available, so it's left
     *        up to the application to properly configure GPIO pins.
     *
     *  @param[in]  init          Settings to be applied
     *  @return Chimera::Status_t
     */
    virtual Chimera::Status_t open( const DriverConfig &init ) = 0;

    /**
     *  Closes the driver and resets the hardware back to defaults
     *  @return void
     */
    virtual void close() = 0;

    /**
     *  Manually controls the power state (on/off) of the hardware to
     *  reduce power consumption without deconfiguring the device.
     *
     *  @param[in]  state         What state to place the HW in (on/off)
     *  @return void
     */
    virtual void setPowerState( const bool state ) = 0;

    /**
     *  Samples a single hardware channel in one-shot mode
     *
     *  @param[in]  ch            Which channel to sample
     *  @return Sample_t
     */
    virtual Sample_t sampleChannel( const Channel ch ) = 0;

    /**
     *  Samples an internal/external sensor. Not all options may be available.
     *
     *  @param[in]  sensor        Which sensor to sample
     *  @return Sample_t
     */
    virtual Sample_t sampleSensor( const Sensor sensor ) = 0;

    /**
     *  Some hardware peripherals support grouping of channels so that they
     *  may all be sampled in sequence with a single command. This function
     *  initializes the hardware with the necessary information.
     *
     *  @param[in]  cfg           Group configuration data
     *  @return Chimera::Status_t
     */
    virtual Chimera::Status_t groupConfig( const GroupInit &cfg ) = 0;

    /**
     *  Start a sampling sequence for the given grouping. Must have been
     *  pre-configured already.
     *
     *  @see groupConfig
     *
     *  @param[in]  grp           Which group to trigger a sample for
     *  @return Chimera::Status_t
     */
    virtual Chimera::Status_t groupStartSample( const SampleGroup grp ) = 0;

    /**
     *  Gets the results of the last group sampling operation.
     *
     *  @note If using this method, the backend should implement some kind of cache
     *        to store individual channel conversions.
     *
     *  @param[in]  grp           Which group to get samples for
     *  @param[in]  out           Destination buffer for the sample data
     *  @param[in]  len           Length of the destination buffer in number of Sample_t elements
     *  @return Chimera::Status_t
     */
    virtual Chimera::Status_t groupGetSample( const SampleGroup grp, Sample_t *const out, const size_t len ) = 0;

    /**
     *  If using DMA transfers, this method assigns the buffer where DMA will
     *  write conversion data into.
     *
     *  @warning Using this method will disable groupGetSample as it effectively replaces
     *           the internal cache for intermediate conversion results.
     *
     *  @warning As long as the DMA functionality is in use, the buffer MUST
     *           exist or the application code is likely to fault.
     *
     *  @param[in]  grp           Which group the buffer is intended for
     *  @param[in]  out           Destination buffer for the sample data
     *  @param[in]  len           Length of the destination buffer in number of Sample_t elements
     *  @return Chimera::Status_t
     */
    virtual Chimera::Status_t groupSetDMABuffer( const SampleGroup grp, Sample_t *const out, const size_t len ) = 0;

    /**
     *  Assigns a number of sampling cycles to a channel. The actual number of
     *  cycles will get rounded to the closes supported value by the hardware.
     *
     *  @param[in]  ch            Which channel to configure
     *  @param[in]  cycles        Number of cycles
     */
    virtual Chimera::Status_t setSampleTime( const Channel ch, const size_t cycles ) = 0;

    /**
     *  Sets the watchdog interrupt thresholds, assuming the hardware supports
     *  this feature.
     *
     *  @param[in]  wd            Which watchdog to configure
     *  @param[in]  low           Low trigger threshold, set to zero to disable
     *  @param[in]  high          High trigger threshold, set to max to disable
     *  @return void
     */
    virtual void setWatchdogThreshold( const Watchdog wd, const Sample_t low, const Sample_t high ) = 0;

    /**
     *  When an interrupt event happens, execute some callback function
     *
     *  @param[in]  bmSignal      Which events the callback applies to (bit mask)
     *  @param[in]  cb            The callback function
     *  @return void
     */
    virtual void onInterrupt( const Interrupt bmSignal, ISRCallback cb ) = 0;

    /**
     *  Converts a raw sample to the equivalent voltage
     *
     *  @param[in]  sample        The raw sample value to convert
     *  @return float
     */
    virtual float sampleToVoltage( const Sample_t sample ) = 0;

    /**
     *  Converts a raw sample into the equivalent temperature for
     *  the on-chip temperature sensor (if supported). Results are
     *  reported in celcius.
     *
     *  @param[in]  sample        The raw sample value to convert
     *  @return float
     */
    virtual float sampleToJunctionTemperature( const Sample_t sample ) = 0;
  };


  /**
   *  Virtual class to facilitate easy mocking of the driver
   */
  class IADC : virtual public HWInterface, virtual public Chimera::Threading::LockableInterface
  {
  public:
    virtual ~IADC() = default;
  };

}  // namespace Chimera::ADC

#endif /* !CHIMERA_ADC_INTERFACE_HPP */
