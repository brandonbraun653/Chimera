/********************************************************************************
 *   File Name:
 *    preprocessor.hpp
 *
 *   Description:
 *    Does some preprocessor magic to figure out various configuration environments
 *
 *   2019 | Brandon Braun | brandonbraun653@gmail.com
 ********************************************************************************/

#pragma once
#ifndef CHIMERA_PREPROCESSOR_HPP
#define CHIMERA_PREPROCESSOR_HPP

/*------------------------------------------------
Check for supported compiler options:
https://clang.llvm.org/docs/LanguageExtensions.html
------------------------------------------------*/
#ifndef __has_include
#error FATAL ERROR: Please use a compiler that supports __has_include(), such as Clang or MSVC 2015 Update 2 or higher
#endif

/*------------------------------------------------
Test Environment
------------------------------------------------*/
#if defined( _WIN32 ) || defined( _WIN64 )
#define MOD_TEST
#define SIM
#endif

#if defined( MOD_TEST ) && !defined( NOMINMAX )
#error Please define NOMINMAX so that the Windows port of FreeRTOS does not try and overwrite the C++ STL
#endif

/*------------------------------------------------
Target CPU
------------------------------------------------*/
#if !defined( CHIMERA_LITTLE_ENDIAN ) && !defined( CHIMERA_BIG_ENDIAN )
#error Please define the target CPU endianness: CHIMERA_LITTLE_ENDIAN or CHIMERA_BIG_ENDIAN
#endif 

/*------------------------------------------------
FreeRTOS
------------------------------------------------*/
#if __has_include( "FreeRTOS.h" ) && !defined( USING_FREERTOS )
#define USING_FREERTOS
#endif

/*------------------------------------------------
Boost
------------------------------------------------*/
#if __has_include( "boost/shared_ptr.hpp" )
#define USING_BOOST
#endif

#endif  /* !CHIMERA_PREPROCESSOR_HPP */
