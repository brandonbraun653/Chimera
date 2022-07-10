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
     *  Assigns a number of sampling cycles to a channel. The actual number of
     *  cycles will get rounded to the closes supported value by the hardware.
     *
     *  @param[in]  ch            Which channel to configure
     *  @param[in]  cycles        Number of cycles
     */
    virtual Chimera::Status_t setSampleTime( const Channel ch, const size_t cycles ) = 0;

    /**
     *  Samples a single hardware channel in one-shot mode
     *
     *  @param[in]  ch            Which channel to sample
     *  @return Sample
     */
    virtual Sample sampleChannel( const Channel ch ) = 0;

    /**
     *  Some hardware peripherals support grouping of channels so that they
     *  may all be sampled in sequence with a single command.
     *
     *  @param[in]  cfg           Group configuration data
     *  @return Chimera::Status_t
     */
    virtual Chimera::Status_t configSequence( const SequenceInit &cfg ) = 0;

    /**
     *  Starts the sequence conversions
     *  @return void
     */
    virtual void startSequence() = 0;

    /**
     *  Starts the sequence conversions
     *  @return void
     */
    virtual void stopSequence() = 0;

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
    virtual bool nextSeqSample( const Channel ch, Sample &sample ) = 0;

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
    virtual size_t multiSeqSample( const Channel *ch_arr, Sample *sample_arr, const size_t size ) = 0;

    /**
     *  When an interrupt event happens, execute some callback function. This is
     *  how the driver expects data to get out to the user.
     *
     *  @param[in]  signal      Which events the callback applies to (bit mask)
     *  @param[in]  cb            The callback function
     *  @return void
     */
    virtual void onInterrupt( const Interrupt signal, ISRCallback cb ) = 0;

    /**
     *  Converts a raw sample to the equivalent voltage
     *
     *  @param[in]  sample        The raw sample value to convert
     *  @return float
     */
    virtual float toVoltage( const Sample &sample ) = 0;
  };


  /**
   *  Virtual class to facilitate easy mocking of the driver
   */
  class IADC : virtual public HWInterface, virtual public Chimera::Thread::LockableInterface
  {
  public:
    virtual ~IADC() = default;
  };

}  // namespace Chimera::ADC

#endif /* !CHIMERA_ADC_INTERFACE_HPP */
