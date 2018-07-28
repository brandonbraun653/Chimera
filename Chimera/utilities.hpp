/** @file utilities.hpp */
#pragma once
#ifndef CHIMERA_UTILITIES_HPP
#define CHIMERA_UTILITIES_HPP

/* C/C++ Includes */
#include <atomic>
#include <utility>
#include <vector>
#include <set>
#include <iterator>

/* Chimera Includes */
#include <Chimera/preprocessor.hpp>

#ifdef CHIMERA_FREERTOS
#include <Chimera/threading.hpp>
#endif 

#ifdef USING_BOOST
#include <boost/shared_ptr.hpp>
#include <boost/make_shared.hpp>
#endif


/** @ingroup Global_Macros
 *	@{
 **/

/** A 'factory-like' macro that generates a compile time check for the existence of a specific 
 *	function signature within a given class. Code is not generated in flash and there is no overhead.
 *	Mostly this function is used in the various Chimera core perihperal module namespaces like GPIO, 
 *	UART, SPI, etc. to give the developer an error. If desired, a runtime check could be used simply
 *	by calling "InvocationName::value".
 *
 *	@param[in]	InvocationName	A unique name for the macro
 *	@param[in]	ClassToCheck	Name of the class to be checked
 *	@param[in]	FuncName		Name of the function to be checked 
 *	@param[in]	FuncReturn		The return type of the function to be checked
 *	@param[in]	FuncArgs...		A comma separated list of argument types for the function to be checked 
 **/
#define CLASS_METHOD_CHECKER(InvocationName, ClassToCheck, FuncName, FuncReturn, FuncArgs...)				\
	class InvocationName																					\
	{																										\
		typedef char Yes;																					\
		typedef long No;																					\
		template <typename T, T> struct TypeCheck;															\
																											\
		template <typename T> struct fsig																	\
		{																									\
			typedef FuncReturn(T::*fptr)(FuncArgs);															\
		};																									\
																											\
		template <typename T> static Yes has_func(TypeCheck< typename fsig<T>::fptr, &T::FuncName >*);		\
		template <typename T> static No  has_func(...);														\
																											\
	public:																									\
		static bool const value = (sizeof(has_func<ClassToCheck>(0)) == sizeof(Yes));						\
	};																										\
	static_assert(InvocationName::value, "Proper signature for function \"" #FuncName "\" in class \"" #ClassToCheck "\" could not be found.");
	

/** Returns the number of bytes in an array
 *	@param[in] x an array (not pointer)
 **/
#define cnt_of_array(x) (sizeof(x) / sizeof(*x))


/** @} */


namespace Chimera
{
	#if defined(USING_BOOST)
	//Conversion functions courtesy of: https://stackoverflow.com/questions/12314967/cohabitation-of-boostshared-ptr-and-stdshared-ptr
	
	/** Converts an std::shared_ptr into a boost::shared_ptr */
	template<typename T>
	boost::shared_ptr<T> convert_shared_ptr(std::shared_ptr<T>& ptr)
	{
		return boost::shared_ptr<T>(ptr.get(), [ptr](T*) mutable {ptr.reset(); });
	}

	/** Converts a boost::shared_ptr into an std::shared_ptr */
	template<typename T>
	std::shared_ptr<T> convert_shared_ptr(boost::shared_ptr<T>& ptr)
	{
		return std::shared_ptr<T>(ptr.get(), [ptr](T*) mutable {ptr.reset(); });
	}
	#endif	/* !USING_BOOST */


	/** C++11 doesn't provide a make_unique implementation, so we will! */
	template<typename T, typename ...Args>
	std::unique_ptr<T> make_unique(Args&& ...args)
	{
		return std::unique_ptr<T>(new T(std::forward<Args>(args)...));
	}

}



#endif