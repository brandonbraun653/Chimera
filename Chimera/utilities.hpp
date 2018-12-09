#pragma once
#ifndef CHIMERA_UTILITIES_HPP
#define CHIMERA_UTILITIES_HPP

/* C++ Includes */
#include <atomic>
#include <utility>
#include <vector>
#include <set>
#include <iterator>
#include <type_traits>

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

	/**
    *   Converts an std::shared_ptr into a boost::shared_ptr
    */
	template<typename T>
	boost::shared_ptr<T> convert_shared_ptr(std::shared_ptr<T>& ptr)
	{
		return boost::shared_ptr<T>(ptr.get(), [ptr](T*) mutable {ptr.reset(); });
	}

	/**
    *   Converts a boost::shared_ptr into an std::shared_ptr
    */
	template<typename T>
	std::shared_ptr<T> convert_shared_ptr(boost::shared_ptr<T>& ptr)
	{
		return std::shared_ptr<T>(ptr.get(), [ptr](T*) mutable {ptr.reset(); });
	}
	#endif	/* !USING_BOOST */


	/**
    *   C++11 doesn't provide a make_unique implementation, so we will!
    */
    #if __cplusplus == 201103L
	template<typename T, typename ...Args>
	std::unique_ptr<T> make_unique(Args&& ...args)
	{
		return std::unique_ptr<T>(new T(std::forward<Args>(args)...));
	}
    #endif

    #if __cplusplus >= 201402L
    namespace Array
    {
        /*-------------------------------------------------
        Use the "Indices Trick" to cast an std::array to another type. Not entirely sure I understand
        this one myself yet. If both arrays are constexpr, this can be done at compile time.

        References:
            1. https://stackoverflow.com/questions/14279821/cast-an-stdarray-to-another-data-type-at-compile-time
            2. https://stackoverflow.com/questions/31463388/can-someone-please-explain-the-indices-trick
        -------------------------------------------------*/
        template <std::size_t... Is>
        struct indices{};

        template<std::size_t N, std::size_t... Is>
        struct build_indices: build_indices<N-1, N-1, Is...>{};

        template<std::size_t... Is>
        struct build_indices<0, Is...>: indices<Is...>{};

        template<typename Output, typename Input, size_t Size, size_t... Is>
        constexpr auto array_cast_helper(const std::array<Input,Size> &x, indices<Is...>) -> std::array<Output,Size>
        {
            return {{static_cast<Output>(std::get<Is>(x))...}};
        }

        /**
        *   Cast an std::array from one primitive type to another. Not sure that this works on more
        *   complex types. Given that Chimera is an embedded systems library for MCUs, I'm not sure
        *   you would want/need to.
        *
        *   @param[in]  x   Some array you want converted
        *   @return The shiny new array.
        */
        template<typename Output, typename Input, size_t Size>
        constexpr auto cast(const std::array<Input,Size> &x) -> std::array<Output,Size>
        {
            return array_cast_helper<Output>(x, build_indices<Size>());
        }
    }

    namespace Types
    {
        template <typename...>
        struct is_one_of { static constexpr bool value = false; };

        template <typename F, typename S, typename... T>
        struct is_one_of<F, S, T...>
        {
            static constexpr bool value = std::is_same<F, S>::value || is_one_of<F, T...>::value;
        };

        template< typename Kind, typename... Kinds>
        constexpr bool isOneOf() { return is_one_of<Kind, Kinds...>::value; }
    }

    #endif

};

#endif
