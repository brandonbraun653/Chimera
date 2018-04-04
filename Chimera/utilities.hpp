#pragma once
#ifndef CHIMERA_UTILITIES_HPP
#define CHIMERA_UTILITIES_HPP

#include <utility>
/* Common functions available to all */

namespace Chimera
{
	
	/* A lightweight implementation of a shared_ptr, sourced from here:
	 * https://stackoverflow.com/questions/7792011/alternative-to-boostshared-ptr-in-an-embedded-environment?utm_medium=organic&utm_source=google_rich_qa&utm_campaign=google_rich_qa */
	template<class T>
		class shared_ptr
		{
			template<class U>
				friend class my_shared_ptr;
		public:
			shared_ptr()
				: p()
				, c() {}
			explicit shared_ptr(T* s)
				: p(s)
				, c(new unsigned(1)) {}

			shared_ptr(const shared_ptr& s)
				: p(s.p)
				, c(s.c) { if (c) ++*c; }

			shared_ptr& operator=(const shared_ptr& s) 
			{ if (this != &s) { clear(); p = s.p; c = s.c; if (c) ++*c; } return *this; }

			template<class U>
				shared_ptr(const shared_ptr<U>& s)
					: p(s.p)
					, c(s.c) { if (c) ++*c; }

			~shared_ptr() { clear(); }

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
			unsigned* c;
		};
	
	template<class T, class... U>
		shared_ptr<T> make_shared(U&&... u)
		{ 
			return shared_ptr<T>(new T{ std::forward<U>(u)... });
		}
}


#endif