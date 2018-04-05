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

/* FreeRTOS Files */
#if __has_include("FreeRTOS.h")
	#ifndef CHIMERA_FREERTOS
	#define CHIMERA_FREERTOS
	#endif
#endif


/*-------------------------------------------
 * Chimera Runtime Behavior 
 *------------------------------------------*/
#define CHIMERA_RTOS_FREEZE_ON_FAIL		1		/* Freezes the system if any of the functions in Chimera/threading.hpp return pdFAIL */


#endif 
