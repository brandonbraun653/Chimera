#pragma once
#ifndef LOKI_ASSERTION_HPP
#define LOKI_ASSERTION_HPP
//#include <type_traits>

// struct can_call_test
// {
// 	template<typename F, typename... A>
// 	static decltype(std::declval<F>()(std::declval<A>()...),
// std::true_type()) 		f(int);
//
// 	template<typename F, typename... A>
// 	static std::false_type
// 		f(...);
// };

// template<typename F, typename... A>
// struct can_call : decltype(can_call_test::f<F, A...>(0)) { };
//
// template<typename F, typename... A>
// struct can_call <F(A...)> : can_call <F, A...> { };
//
// template<typename... A, typename F>
// constexpr can_call<F, A...>
// is_callable_with(F&&) { return can_call<F(A...)>{}; }

#endif