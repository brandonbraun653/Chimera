/********************************************************************************
 *  File Name:
 *    imu_types.hpp
 *
 *  Description:
 *    Describes types for interacting with generic IMU devices
 *
 *  2019 | Brandon Braun | brandonbraun653@gmail.com
 ********************************************************************************/

#pragma once
#ifndef CHIMERA_IMU_TYPES_HPP
#define CHIMERA_IMU_TYPES_HPP

/* C++ Includes */
#include <cstdint>
#include <limits>

namespace Chimera::Modules::IMU
{
  template<typename T>
  struct Vector3DOF
  {
    T x = std::numeric_limits<T>::min();
    T y = std::numeric_limits<T>::min();
    T z = std::numeric_limits<T>::min();
  };

  template<typename T>
  struct Measurement9DOF
  {
    Vector3DOF<T> sensor0;
    Vector3DOF<T> sensor1;
    Vector3DOF<T> sensor2;
  };
  
  template<typename T>
  struct Measurement6DOF
  {
    Vector3DOF<T> sensor0;
    Vector3DOF<T> sensor1;
  };
  
  template<typename T>
  struct Measurement3DOF
  {
    Vector3DOF<T> sensor0;
  };

  enum class Sensor_t : uint8_t
  {
    ACCEL,
    GYRO,
    MAG,
    TEMP,
    ALL
  };

}  // namespace Chimera::Modules::IMU

#endif /* !CHIMERA_IMU_TYPES_HPP */