#pragma once
#ifndef CHIMERA_PREPROCESSOR_HPP
#define CHIMERA_PREPROCESSOR_HPP

/*-------------------------------------------
 * Check for supported compiler options:
 * https://clang.llvm.org/docs/LanguageExtensions.html
 *-------------------------------------------*/
#ifndef __has_include
#error FATAL ERROR: Please use a compiler that supports __has_include(), such as Clang or MSVC 2015 Update 2 or higher
#endif

/* Test Configuration */
#if defined( _WIN32 ) || defined( _WIN64 )
#define MOD_TEST
#define SIM
#endif

/* FreeRTOS Files */
#if __has_include( "FreeRTOS.h" )
#ifndef CHIMERA_FREERTOS
#define CHIMERA_FREERTOS
#endif

#if defined( MOD_TEST ) && !defined( NOMINMAX )
#error Please define NOMINMAX so that the Windows port of FreeRTOS does not try and overwrite the C++ STL
#endif
#endif

/* Boost Files */
#if __has_include( "boost/shared_ptr.hpp" )
#define USING_BOOST
#endif

#endif
