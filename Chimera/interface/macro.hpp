/********************************************************************************
 *   File Name:
 *    macro.hpp
 *
 *   Description:
 *    Provides macros useful for interacting with the Interface system
 *
 *   2019 | Brandon Braun | brandonbraun653@gmail.com
 ********************************************************************************/

#pragma once 
#ifndef CHIMERA_INTERFACE_MACROS_HPP
#define CHIMERA_INTERFACE_MACROS_HPP


/**
 *  A wonderful macro to generate a compile time warning, courtesy of:
 *  https://stackoverflow.com/questions/8936063/does-there-exist-a-static-warning
 */
#if defined(__GNUC__)
#define DEPRECATE(foo, msg) foo __attribute__((deprecated(msg)))
#elif defined(_MSC_VER)
#define DEPRECATE(foo, msg) __declspec(deprecated(msg)) foo
#else
#error This compiler is not supported
#endif

#define PP_CAT(x,y) PP_CAT1(x,y)
#define PP_CAT1(x,y) x##y

namespace detail
{
    struct true_type {};
    struct false_type {};
    template <int test> struct converter : public true_type {};
    template <> struct converter<0> : public false_type {};
}

#define STATIC_WARNING(cond, msg) \
struct PP_CAT(static_warning,__LINE__) { \
  DEPRECATE(void _(::detail::false_type const& ),msg) {}; \
  void _(::detail::true_type const& ) {}; \
  PP_CAT(static_warning,__LINE__)() {_(::detail::converter<(cond)>());} \
}

// Note: using STATIC_WARNING_TEMPLATE changes the meaning of a program in a small way.
// It introduces a member/variable declaration.  This means at least one byte of space
// in each structure/class instantiation.  STATIC_WARNING should be preferred in any 
// non-template situation.
//  'token' must be a program-wide unique identifier.
#define STATIC_WARNING_TEMPLATE(token, cond, msg) \
    STATIC_WARNING(cond, msg) PP_CAT(PP_CAT(_localvar_, token),__LINE__)

#endif /* !CHIMERA_INTERFACE_MACROS_HPP */