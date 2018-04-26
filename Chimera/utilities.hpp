/** @file utilities.hpp */
#pragma once
#ifndef CHIMERA_UTILITIES_HPP
#define CHIMERA_UTILITIES_HPP

#include <atomic>
#include <utility>

#include <vector>
#include <set>

#include <iterator>
#include <boost/range/iterator_range.hpp>

#ifdef CHIMERA_FREERTOS
#include <Chimera/threading.hpp>
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
			typedef FuncReturn(T::*fptr)(FuncArgs);														\
		};																									\
																											\
		template <typename T> static Yes has_func(TypeCheck< typename fsig<T>::fptr, &T::FuncName >*);	\
		template <typename T> static No  has_func(...);														\
																											\
	public:																									\
		static bool const value = (sizeof(has_func<ClassToCheck>(0)) == sizeof(Yes));						\
	};																										\
	static_assert(InvocationName::value, "Proper signature for function \"" #FuncName "\" in class \"" #ClassToCheck "\" could not be found.");
	
/** @} */


namespace Chimera
{
	//StackOverflow: https://goo.gl/ad1kT1
	template<class T>
	class shared_ptr
	{
		// Allows for implicit conversion ptr<T>(const ptr<U>&)
		template<class U>
		friend class shared_ptr;
			
	public:
		shared_ptr() : p(), c() {}
		explicit shared_ptr(T* s) : p(s), c(new std::atomic<unsigned>(1)) {}

		shared_ptr(const shared_ptr& s) : p(s.p) , c(s.c) 
		{ 
			if (c) ++*c;
		}
		
		~shared_ptr() 
		{ 
			clear(); 
		}

		shared_ptr& operator=(const shared_ptr& s) 
		{ 
			if (this != &s) 
			{ 
				clear(); 
				p = s.p; 
				c = s.c; 
				if (c) ++*c; 
			} 
			return *this; 
		}

		template<class U>
		shared_ptr(const shared_ptr<U>& s) : p(s.p) , c(s.c) 
		{ 
			if (c) ++*c; 
		}

		void clear() 
		{ 
			if (c)
			{
				if (*c == 1) delete p; 
				if (!--*c) delete c; 
			} 
			c = 0; p = 0; 
		}

		T* get() const { return (c) ? p : 0; }
		T* operator->() const { return get(); }
		T& operator*() const { return *get(); }

	private:
		T* p;
		std::atomic<unsigned>* c;
	};
	
	template<class T, class... U>
	shared_ptr<T> make_shared(U&&... u)
	{ 
		return shared_ptr<T>(new T{ std::forward<U>(u)... });
	}
	
	
	/* Intended to provide an easy way to iterate over a contiguous enumeration */
	//StackOverflow: https://goo.gl/wcXF26
	//Coliru Live Code: https://goo.gl/gkG99X
	template<class enum_type>
		class enum_iterator {
		private:
			enum_type value;
			typedef typename std::underlying_type<enum_type>::type under;
		public:   
			typedef std::size_t size_type;
			typedef std::ptrdiff_t difference_type;
			typedef enum_type value_type;
			typedef enum_type reference;
			typedef enum_type* pointer;
			typedef std::random_access_iterator_tag iterator_category;
    
			constexpr enum_iterator()
				: value() {}
			constexpr enum_iterator(const enum_iterator& rhs) noexcept(true)
				: value(rhs.value) {}
			constexpr explicit enum_iterator(enum_type value_) noexcept(true)
				: value(value_) {}
			~enum_iterator() noexcept(true) {}
			enum_iterator& operator=(const enum_iterator& rhs) noexcept(true) {value = rhs.valud; return *this;}
			enum_iterator& operator++() noexcept(true) {value = (enum_type)(under(value) + 1); return *this;}
			enum_iterator operator++(int) noexcept(true) {enum_iterator r(*this); ++*this; return r;}
			enum_iterator& operator+=(size_type o) noexcept(true) {value = (enum_type)(under(value) + o); return *this;}
			friend constexpr enum_iterator operator+(const enum_iterator& it, size_type o) noexcept(true) {return enum_iterator((enum_type)(under(it) + o));}
			friend constexpr enum_iterator operator+(size_type o, const enum_iterator& it) noexcept(true) {return enum_iterator((enum_type)(under(it) + o));}
			enum_iterator& operator--() noexcept(true) {value = (enum_type)(under(value) - 1); return *this;}
			enum_iterator operator--(int) noexcept(true) {enum_iterator r(*this); --*this; return r;}
			enum_iterator& operator-=(size_type o) noexcept(true) {value = (enum_type)(under(value) + o); return *this;}
			friend constexpr enum_iterator operator-(const enum_iterator& it, size_type o) noexcept(true) {return enum_iterator((enum_type)(under(it) - o));}
			friend constexpr difference_type operator-(enum_iterator lhs, enum_iterator rhs) noexcept(true) {return under(lhs.value) - under(rhs.value);}
			constexpr reference operator*() const noexcept(true) {return value;}
			constexpr reference operator[](size_type o) const noexcept(true) {return (enum_type)(under(value) + o);}
			constexpr const enum_type* operator->() const noexcept(true) {return &value;}
			constexpr friend bool operator==(const enum_iterator& lhs, const enum_iterator& rhs) noexcept(true) {return lhs.value == rhs.value;}
			constexpr friend bool operator!=(const enum_iterator& lhs, const enum_iterator& rhs) noexcept(true) {return lhs.value != rhs.value;} 
			constexpr friend bool operator<(const enum_iterator& lhs, const enum_iterator& rhs) noexcept(true) {return lhs.value < rhs.value;}
			constexpr friend bool operator>(const enum_iterator& lhs, const enum_iterator& rhs) noexcept(true) {return lhs.value > rhs.value;}
			constexpr friend bool operator<=(const enum_iterator& lhs, const enum_iterator& rhs) noexcept(true) {return lhs.value <= rhs.value;}
			constexpr friend bool operator>=(const enum_iterator& lhs, const enum_iterator& rhs) noexcept(true) {return lhs.value >= rhs.value;}
			friend void swap(const enum_iterator& lhs, const enum_iterator& rhs) noexcept(true) {std::swap(lhs.value, rhs.value); }
		};
	template<class enum_type> constexpr boost::iterator_range<enum_iterator<enum_type>> get_range() noexcept(true) 
	{return boost::make_iterator_range(enum_iterator<enum_type>(enum_type::First), enum_iterator<enum_type>(enum_type::Last));}
	
	
}



#endif