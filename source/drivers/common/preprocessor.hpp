/********************************************************************************
 *  File Name:
 *    preprocessor.hpp
 *
 *  Description:
 *    Does some preprocessor magic to figure out various configuration environments
 *
 *  2019-2020 | Brandon Braun | brandonbraun653@gmail.com
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
FreeRTOS
------------------------------------------------*/
#if __has_include( "FreeRTOS.h" ) && __has_include( "tasks.c" )
#if !defined( USING_FREERTOS )
#define USING_FREERTOS
#endif
#endif

/*------------------------------------------------
Test Environment
------------------------------------------------*/
#if defined( WIN32 ) || defined( WIN64 )
#define USING_WINDOWS
#define MOD_TEST

#if !defined( CHIMERA_SIMULATOR )
#define CHIMERA_SIMULATOR
#endif
#endif

#if defined( MOD_TEST ) && defined( USING_FREERTOS) && !defined( NOMINMAX )
#error Please define NOMINMAX so that the Windows port of FreeRTOS does not try and overwrite the C++ STL
#endif


/*------------------------------------------------
Composite System Configuration
------------------------------------------------*/
#if defined( USING_WINDOWS ) || ( USING_LINUX )
/* Indicates to the code it can use the full C++ STL without the memory concerns of embedded systems */
#define FULL_CPP_STL_AVAILABLE
#else
#define USING_GENERIC_SYSTEM
#endif

#endif /* !CHIMERA_PREPROCESSOR_HPP */
