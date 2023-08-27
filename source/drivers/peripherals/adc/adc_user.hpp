/******************************************************************************
 *  File Name:
 *    adc_user.hpp
 *
 *  Description:
 *    Implements an interface to create a Chimera ADC peripheral
 *
 *  2020-2023 | Brandon Braun | brandonbraun653@gmail.com
 *****************************************************************************/

#pragma once
#ifndef CHIMERA_ADC_HPP
#define CHIMERA_ADC_HPP

/*-----------------------------------------------------------------------------
Includes
-----------------------------------------------------------------------------*/
#include <Chimera/source/drivers/peripherals/adc/adc_intf.hpp>
#include <Chimera/source/drivers/peripherals/adc/adc_types.hpp>
#include <Chimera/source/drivers/threading/threading_extensions.hpp>
#include <memory>

namespace Chimera::ADC
{
  /*---------------------------------------------------------------------------
  Public Functions
  ---------------------------------------------------------------------------*/
  Chimera::Status_t initialize();
  Chimera::Status_t reset();
  Driver_rPtr       getDriver( const Peripheral periph );
  bool              featureSupported( const Peripheral periph, const Feature feature );

  /*---------------------------------------------------------------------------
  Classes
  ---------------------------------------------------------------------------*/
  class Driver : public Thread::Lockable<Driver>, public IADC
  {
  public:
    Driver();
    ~Driver();

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
    Chimera::Status_t open( const DriverConfig &init );

    /**
     *  Closes the driver and resets the hardware back to defaults
     *  @return void
     */
    void close();

    /**
     *  Assigns a number of sampling cycles to a channel. The actual number of
     *  cycles will get rounded to the closes supported value by the hardware.
     *
     *  @param[in]  ch            Which channel to configure
     *  @param[in]  cycles        Number of cycles
     */
    Chimera::Status_t setSampleTime( const Channel ch, const size_t cycles );

    /**
     * @brief Gets the sample time cycles assigned to a channel
     *
     * @param ch  Which channel to query
     * @return size_t
     */
    size_t getSampleCycle( const Channel ch );

    /**
     * @brief Computes the total sample time + conversion, in nanoseconds.
     *
     * @param cycles  Number of cycles used for sampling
     * @return size_t
     */
    size_t totalMeasureTime( const size_t cycles );

    /**
     *  Samples a single hardware channel in one-shot mode
     *
     *  @param[in]  ch            Which channel to sample
     *  @return Sample
     */
    Sample sampleChannel( const Channel ch );

    /**
     *  Some hardware peripherals support grouping of channels so that they
     *  may all be sampled in sequence with a single command.
     *
     *  @param[in]  cfg           Group configuration data
     *  @return Chimera::Status_t
     */
    Chimera::Status_t configSequence( const SequenceInit &cfg );

    /**
     *  Starts the sequence conversions
     *  @return void
     */
    void startSequence();

    /**
     *  Starts the sequence conversions
     *  @return void
     */
    void stopSequence();

    /**
     * @brief Grab the next sample from the ADC channel
     *
     * This does not perform an immediate sample, but rather queries the
     * hardware driver queues for any new data.
     *
     * @param ch        Which channel to request
     * @param sample    Where to write the sample data
     * @return true     Retrieval was successful
     * @return false    Retrieval failed (nothing available)
     */
    bool nextSeqSample( const Channel ch, Sample &sample );

    /**
     * @brief Retrieves pending data for multiple channels at a time
     * @note Channels do not need to be unique
     *
     * If a channel's data is unable to be retrieved, the associated index in the
     * sample array will be cleared to defaults.
     *
     * @param ch_arr      Array of channels to get data for
     * @param sample_arr  Array of sample entries to write data into
     * @param size        Number of elements in both arrays
     * @return size_t     Number of elements successfully retrieved
     */
    size_t multiSeqSample( const Channel *ch_arr, Sample *sample_arr, const size_t size );

    /**
     *  When an interrupt event happens, execute some callback function. This is
     *  how the driver expects data to get out to the user.
     *
     *  @param[in]  signal      Which events the callback applies to (bit mask)
     *  @param[in]  cb            The callback function
     *  @return void
     */
    void onInterrupt( const Interrupt signal, ISRCallback cb );

    /**
     *  Converts a raw sample to the equivalent voltage
     *
     *  @param[in]  sample        The raw sample value to convert
     *  @return float
     */
    float toVoltage( const Sample &sample );

    /**
     * @brief Sets a channel to be monitored with a dedicated HW watchdog circuit
     *
     * For the high and low threshold voltages, this will take into account the current ADC
     * configuration to ensure conversion resolution and other properties are accurate.
     *
     * @param cfg   Watchdog configuration
     * @return Chimera::Status_t
     */
    Chimera::Status_t monitorChannel( const WatchdogConfig &cfg );

    /**
     * @brief Gets the analog reference voltage used for conversions
     * @return float  Volts
     */
    float analogReference() const;

  private:
    friend Chimera::Thread::Lockable<Driver>;
    void *mImpl; /**< Implementation details of the driver */
  };
}  // namespace Chimera::ADC

#endif /* !CHIMERA_ADC_HPP */
