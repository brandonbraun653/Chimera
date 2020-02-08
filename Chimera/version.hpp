/********************************************************************************
*  File Name:
*    version.hpp
*
*  Description:
*    Chimera version information
*
*  2020 | Brandon Braun | brandonbraun653@gmail.com
********************************************************************************/

#pragma once
#ifndef CHIMERA_VERSION_HPP
#define CHIMERA_VERSION_HPP

/* C++ Includes */
#include <string_view>

namespace Chimera
{
  static constexpr std::string_view version_major = "1";
  static constexpr std::string_view version_minor = "0";
  static constexpr std::string_view version_patch = "1";

  static constexpr std::string_view version = "1.0.1";
}

#endif	/* !CHIMERA_VERSION_HPP */
