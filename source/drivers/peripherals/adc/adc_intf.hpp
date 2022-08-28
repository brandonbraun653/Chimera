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
 *  2020-2022 | Brandon Braun | brandonbraun653@gmail.com
 ********************************************************************************/

#pragma once
#ifndef CHIMERA_ADC_INTERFACE_HPP
#define CHIMERA_ADC_INTERFACE_HPP

/*-----------------------------------------------------------------------------
Includes
-----------------------------------------------------------------------------*/
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
    virtual Chimera::Status_t open( const DriverConfig &init ) = 0;
    virtual void close() = 0;
    virtual Chimera::Status_t setSampleTime( const Channel ch, const size_t cycles ) = 0;
    virtual Sample sampleChannel( const Channel ch ) = 0;
    virtual Chimera::Status_t configSequence( const SequenceInit &cfg ) = 0;
    virtual void startSequence() = 0;
    virtual void stopSequence() = 0;
    virtual bool nextSeqSample( const Channel ch, Sample &sample ) = 0;
    virtual size_t multiSeqSample( const Channel *ch_arr, Sample *sample_arr, const size_t size ) = 0;
    virtual void onInterrupt( const Interrupt signal, ISRCallback cb ) = 0;
    virtual float toVoltage( const Sample &sample ) = 0;
  };


  /**
   *  Virtual class to facilitate easy mocking of the driver
   */
#if defined( CHIMERA_VIRTUAL )
  class IADC : virtual public HWInterface, virtual public Chimera::Thread::LockableInterface
  {
  public:
    virtual ~IADC() = default;
  };
#else
  class IADC
  {
  };
#endif /* CHIMERA_VIRTUAL */

}  // namespace Chimera::ADC

#endif /* !CHIMERA_ADC_INTERFACE_HPP */
