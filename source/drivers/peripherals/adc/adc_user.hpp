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
  Driver_rPtr getDriver( const Peripheral periph );
  bool featureSupported( const Peripheral periph, const Feature feature );


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
    Chimera::Status_t setSampleTime( const Channel ch, const size_t cycles );
    Sample sampleChannel( const Channel ch );
    Chimera::Status_t configSequence( const SequenceInit &cfg );
    void startSequence();
    void stopSequence();
    bool nextSample( const Channel ch, Sample &sample );
    void onInterrupt( const Interrupt bmSignal, ISRCallback cb );
    float sampleToVoltage( const Sample sample );

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
