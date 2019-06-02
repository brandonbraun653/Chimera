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

/* Eigen Includes */
#include <Eigen/Core>

/* Chimera Includes */
#include <Chimera/constants/physics.hpp>

namespace Chimera::Modules::IMU
{

  
  template<typename T>
  struct Measurement9DOF
  {
    Eigen::Matrix<T, 3, 1> sensor0;
    Eigen::Matrix<T, 3, 1> sensor1;
    Eigen::Matrix<T, 3, 1> sensor2;
  };
  
  template<typename T>
  struct Measurement6DOF
  {
    Eigen::Matrix<T, 3, 1> sensor0;
    Eigen::Matrix<T, 3, 1> sensor1;
  };
  
  template<typename T>
  struct Measurement3DOF
  {
    Eigen::Matrix<T, 3, 1> sensor0;
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