#pragma once
#ifndef CHIMERA_GPIO_HPP
#define CHIMERA_GPIO_HPP

/* C/C++ Includes */
#include <type_traits>


/* Boost Includes */
#include <boost/shared_ptr.hpp>
#include <boost/make_shared.hpp>

/* Chimera Includes */
#include <Chimera/chimera.hpp>
#include <Chimera/config.hpp>

namespace Chimera
{
	namespace GPIO
	{
		class GPIOClass : public CHIMERA_INHERITED_GPIO
		{
		public:
			//Specialized Chimera only functions here? 
			//Maybe something to do with RTOS or properties of the pin or idk...
			
			GPIOClass() = default;
			~GPIOClass() = default;
			
		private:
			
		};
		typedef boost::shared_ptr<GPIOClass> GPIOClass_sPtr;
		
		
		/* Checks for the proper init function in inherited GPIO class */
		template <class Type>
			class has_init
			{
				typedef char Yes;
				typedef long No;
				template <typename T, T> struct TypeCheck;

				
				template <typename T> struct fsig
				{
					typedef Chimera::GPIO::Status(T::*fptr)(uint32_t);
				};

				template <typename T> static Yes has_func(TypeCheck< typename fsig<T>::fptr, &T::init >*);
				template <typename T> static No  has_func(...);

			public:
				static bool const value = (sizeof(has_func<Type>(0)) == sizeof(Yes));
			};
		
		/* Checks for the proper mode function in inherited GPIO class */
		template <class Type>
			class has_mode
			{
				typedef char Yes;
				typedef long No;
				template <typename T, T> struct TypeCheck;

				
				template <typename T> struct fsig
				{
					typedef Chimera::GPIO::Status(T::*fptr)(Chimera::GPIO::Mode, bool);
				}
				;

				template <typename T> static Yes has_func(TypeCheck< typename fsig<T>::fptr, &T::mode >*);
				template <typename T> static No  has_func(...);

			public:
				static bool const value = (sizeof(has_func<Type>(0)) == sizeof(Yes));
			};	
		
		/* Checks for the proper write function in inherited GPIO class */
		template <class Type>
			class has_write
			{
				typedef char Yes;
				typedef long No;
				template <typename T, T> struct TypeCheck;

				
				template <typename T> struct fsig
				{
					typedef Chimera::GPIO::Status(T::*fptr)(Chimera::GPIO::State);
				}
				;

				template <typename T> static Yes has_func(TypeCheck< typename fsig<T>::fptr, &T::write >*);
				template <typename T> static No  has_func(...);

			public:
				static bool const value = (sizeof(has_func<Type>(0)) == sizeof(Yes));
			};
		
		
		
		
		static_assert(has_init<CHIMERA_INHERITED_GPIO>::value,
			"Please provide a function with the signature 'Chimera::GPIO::Status init(uint32_t)' in inherited GPIO class.");
		
		static_assert(has_mode<CHIMERA_INHERITED_GPIO>::value,
			"Please provide a function with the signature 'Chimera::GPIO::Status mode(Chimera::GPIO::Mode, bool)' in inherited GPIO class.");
		
		static_assert(has_write<CHIMERA_INHERITED_GPIO>::value,
			"Please provide a function with the signature 'Chimera::GPIO::Status mode(Chimera::GPIO::State)' in inherited GPIO class.");
		
	}
}

#endif