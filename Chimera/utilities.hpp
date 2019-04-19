#pragma once
#ifndef CHIMERA_UTILITIES_HPP
#define CHIMERA_UTILITIES_HPP

/* C++ Includes */
#include <array>
#include <atomic>
#include <iterator>
#include <set>
#include <type_traits>
#include <utility>
#include <vector>

/* Chimera Includes */
#include <Chimera/preprocessor.hpp>

#ifdef CHIMERA_FREERTOS
#include <Chimera/threading.hpp>
#endif

#ifdef USING_BOOST
#include <boost/make_shared.hpp>
#include <boost/shared_ptr.hpp>
#endif

namespace Chimera
{
  namespace Utilities
  {
#if defined( USING_BOOST )
    // Conversion functions courtesy of:
    // https://stackoverflow.com/questions/12314967/cohabitation-of-boostshared-ptr-and-stdshared-ptr

    /**
     *   Converts an std::shared_ptr into a boost::shared_ptr
     */
    template<typename T>
    boost::shared_ptr<T> convert_shared_ptr( std::shared_ptr<T> &ptr )
    {
      return boost::shared_ptr<T>( ptr.get(), [ptr]( T * ) mutable { ptr.reset(); } );
    }

    /**
     *   Converts a boost::shared_ptr into an std::shared_ptr
     */
    template<typename T>
    std::shared_ptr<T> convert_shared_ptr( boost::shared_ptr<T> &ptr )
    {
      return std::shared_ptr<T>( ptr.get(), [ptr]( T * ) mutable { ptr.reset(); } );
    }
#endif /* !USING_BOOST */

    namespace Arrays
    {
#if __cplusplus >= 201402L
      /*-------------------------------------------------
      Use the "Indices Trick" to cast an std::array to another type. Not entirely sure
      I understand this one myself yet. If both arrays are constexpr, this can be done
      at compile time.

      References:
          1. https://stackoverflow.com/questions/14279821/cast-an-stdarray-to-another-data-type-at-compile-time
          2. https://stackoverflow.com/questions/31463388/can-someone-please-explain-the-indices-trick
      -------------------------------------------------*/
      template<std::size_t... Is>
      struct indices
      {
      };

      template<std::size_t N, std::size_t... Is>
      struct build_indices : build_indices<N - 1, N - 1, Is...>
      {
      };

      template<std::size_t... Is>
      struct build_indices<0, Is...> : indices<Is...>
      {
      };

      template<typename Output, typename Input, size_t Size, size_t... Is>
      constexpr auto array_cast_helper( const std::array<Input, Size> &x, indices<Is...> ) -> std::array<Output, Size>
      {
        return { { static_cast<Output>( std::get<Is>( x ) )... } };
      }

      /**
       *  Cast an std::array from one primitive type to another. Not sure that this
       *  works on anything other than POD types. Given that Chimera is an embedded systems
       *  library for MCUs, I'm not sure you would want/need to.
       *
       *   @param[in]  x   Some array you want converted
       *   @return The shiny new array.
       */
      template<typename Output, typename Input, size_t Size>
      constexpr auto array_cast( const std::array<Input, Size> &x ) -> std::array<Output, Size>
      {
        return array_cast_helper<Output>( x, build_indices<Size>() );
      }

#endif
    }  // namespace Arrays

    namespace Types
    {
#if __cplusplus >= 201402L
      template<typename...>
      struct is_one_of
      {
        static constexpr bool value = false;
      };

      template<typename F, typename S, typename... T>
      struct is_one_of<F, S, T...>
      {
        static constexpr bool value = std::is_same<F, S>::value || is_one_of<F, T...>::value;
      };

      template<typename Kind, typename... Kinds>
      constexpr bool isOneOf()
      {
        return is_one_of<Kind, Kinds...>::value;
      }

#endif
    }  // namespace Types

    namespace Pointers
    {
#if __cplusplus == 201103L
      template<typename T, typename... Args>
      std::unique_ptr<T> make_unique( Args &&... args )
      {
        return std::unique_ptr<T>( new T( std::forward<Args>( args )... ) );
      }
#endif /* __cplusplus == 201103L */

      /*------------------------------------------------
      Alternative to static_pointer_cast courtesy of:
      https://stackoverflow.com/questions/36120424/alternatives-of-static-pointer-cast-for-unique-ptr
      ------------------------------------------------*/
      template<typename TO, typename FROM>
      std::unique_ptr<TO> static_unique_pointer_cast( std::unique_ptr<FROM> &&old )
      {
        return std::unique_ptr<TO>{ static_cast<TO *>( old.release() ) };
      }
    }  // namespace Pointers
  }    // namespace Utilities
};     // namespace Chimera

#endif
