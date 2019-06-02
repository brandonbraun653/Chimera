/********************************************************************************
 *  File Name:
 *    imu_intf.hpp
 *
 *  Description:
 *    Describes an interface for interacting with generic IMU devices
 *
 *  2019 | Brandon Braun | brandonbraun653@gmail.com
 ********************************************************************************/

#pragma once
#ifndef CHIMERA_IMU_INTERFACE_HPP
#define CHIMERA_IMU_INTERFACE_HPP


/* Chimera Includes */
#include <Chimera/chimera.hpp>
#include <Chimera/modules/sensors/imu/imu_types.hpp>
#include <Chimera/types/common_types.hpp>

namespace Chimera::Modules::IMU
{
  class CommonInterface
  {
  public:
    virtual ~CommonInterface() = default;

    virtual Chimera::Status_t initialize() = 0;

    virtual Chimera::Status_t calibrate( const Sensor_t sensor ) = 0;

    virtual Chimera::Status_t reset() = 0;

    virtual Chimera::Status_t measure( const Sensor_t sensor, void *const measurement, const size_t size ) = 0;
  };

  class Interface9DOF : public CommonInterface
  {
  public:
    virtual ~Interface9DOF() = default;

    virtual Chimera::Status_t assignDOF( const Sensor_t sensor0 = Sensor_t::ACCEL, const Sensor_t sensor1 = Sensor_t::GYRO,
                                         const Sensor_t sensor2 = Sensor_t::MAG ) = 0;
  };
}  // namespace Chimera::Modules::IMU

#endif /* !CHIMERA_IMU_INTERFACE_HPP */