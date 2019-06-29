/********************************************************************************
 *   File Name:
 *    assert.hpp
 *
 *   Description:
 *    Lightweight exception definitions
 *
 *   2019 | Brandon Braun | brandonbraun653@gmail.com
 ********************************************************************************/

#pragma once
#ifndef CHIMERA_ASSERT_HPP
#define CHIMERA_ASSERT_HPP

namespace Chimera::Assert
{

#if defined( DEBUG) && ( DEBUG == 1 )
  #define DBG_ASSERT(x) if(!(x)){ asm("bkpt 255"); }
#else 
  #define DBG_ASSERT(x)
  #endif
}  // namespace Chimera::Assert

#endif /* !CHIMERA_ASSERT_HPP */