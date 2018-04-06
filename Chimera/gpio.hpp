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
		//TODO: Support alternate functions somehow

		class GPIOClass : public CHIMERA_INHERITED_GPIO
		{
		public:
			Status mode(Mode mode, bool pullup = false) { return CHIMERA_INHERITED_GPIO::mode(mode, pullup); }
			Status write(State state) { return CHIMERA_INHERITED_GPIO::write(state); }

			GPIOClass(const Port& port, const uint8_t& pin)
			{
				_port = port;
				_pin = pin;

				CHIMERA_INHERITED_GPIO::init(port, pin);
			}

			~GPIOClass() = default;
			
		private:
			Port _port;
			uint8_t _pin;
			
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
				typedef Chimera::GPIO::Status(T::*fptr)(Chimera::GPIO::Port, uint8_t);
			}
			;

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

		static_assert(has_mode<CHIMERA_INHERITED_GPIO>::value,
			"Please provide a function with the signature 'Chimera::GPIO::Status mode(Chimera::GPIO::Port, uint8_t)' in inherited GPIO class.");

		static_assert(has_mode<CHIMERA_INHERITED_GPIO>::value,
			"Please provide a function with the signature 'Chimera::GPIO::Status mode(Chimera::GPIO::Mode, bool)' in inherited GPIO class.");
		
		static_assert(has_write<CHIMERA_INHERITED_GPIO>::value,
			"Please provide a function with the signature 'Chimera::GPIO::Status mode(Chimera::GPIO::State)' in inherited GPIO class.");
		
	}
}

#endif