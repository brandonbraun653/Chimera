/********************************************************************************
 *  File Name:
 *    adc_intf.hpp
 *
 *  Description:
 *    Models the Chimera ADC interface
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
     *
     */
    virtual Chimera::Status_t open( const HardwareInit &init ) = 0;

    virtual Chimera::Status_t close() = 0;

    virtual void setPowerState( const bool state ) = 0;

    virtual uint16_t sampleChannel( const Channel ch ) = 0;

    virtual uint16_t sampleInternal( const InternalSensor sensor ) = 0;

    virtual Chimera::Status_t startGroupConversion() = 0;

    virtual Chimera::Status_t assignGroupConversion( const Channel *const list, const size_t num ) = 0;

    virtual Chimera::Status_t setSampleTime( const Channel ch, const size_t cycles );

    virtual void onInterrupt( const Interrupt signal, ISRCallback cb ) = 0;

    virtual void setWatchdogThreshold( const Watchdog wd, const uint16_t low, const uint16_t high ) = 0;
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
