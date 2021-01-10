/********************************************************************************
 *  File Name:
 *    adc_user.hpp
 *
 *  Description:
 *    Implements an interface to create a Chimera ADC peripheral
 *
 *  2020 | Brandon Braun | brandonbraun653@gmail.com
 ********************************************************************************/

#pragma once
#ifndef CHIMERA_ADC_HPP
#define CHIMERA_ADC_HPP

/* STL Includes */
#include <memory>

/* Chimera Includes */
#include <Chimera/source/drivers/peripherals/adc/adc_intf.hpp>
#include <Chimera/source/drivers/peripherals/adc/adc_types.hpp>

namespace Chimera::ADC
{
  /*-------------------------------------------------------------------------------
  Public Functions
  -------------------------------------------------------------------------------*/
  Chimera::Status_t initialize();
  Chimera::Status_t reset();
  Driver_rPtr getDriver( const Converter periph );
  bool featureSupported( const Converter periph, const Feature feature );


  /*-------------------------------------------------------------------------------
  Classes
  -------------------------------------------------------------------------------*/
  /**
   *  Concrete class declaration that promises to implement the virtual one, to
   *  avoid paying the memory penalty of a v-table lookup. Implemented project side
   *  using the Bridge pattern.
   */
  class Driver
  {
  public:
    Driver();
    ~Driver();

    /*-------------------------------------------------
    Interface: Hardware
    -------------------------------------------------*/
    Chimera::Status_t open( const DriverConfig &init );
    void close();
    void setPowerState( const bool state );
    Sample_t sampleChannel( const Channel ch );
    Sample_t sampleSensor( const Sensor sensor );
    Chimera::Status_t groupConfig( const GroupInit &cfg );
    Chimera::Status_t groupStartSample( const SampleGroup grp );
    Chimera::Status_t groupGetSample( const SampleGroup grp, Sample_t *const out, const size_t len );
    Chimera::Status_t groupSetDMABuffer( const SampleGroup grp, Sample_t *const out, const size_t len );
    Chimera::Status_t setSampleTime( const Channel ch, const size_t cycles );
    void setWatchdogThreshold( const Watchdog wd, const Sample_t low, const Sample_t high );
    void onInterrupt( const Interrupt bmSignal, ISRCallback cb );
    float sampleToVoltage( const Sample_t sample );
    float sampleToJunctionTemperature( const Sample_t sample );

    /*-------------------------------------------------
    Interface: Lockable
    -------------------------------------------------*/
    void lock();
    void lockFromISR();
    bool try_lock_for( const size_t timeout );
    void unlock();
    void unlockFromISR();

  private:
    void *mDriver; /**< Instance of the implementer's ADC driver */
  };
}  // namespace Chimera::ADC

#endif /* !CHIMERA_ADC_HPP */
