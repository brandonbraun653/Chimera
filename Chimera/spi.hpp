#pragma once
#ifndef CHIMERA_SPI_HPP
#define CHIMERA_SPI_HPP

#include <map>
#include <string>
#include <type_traits>
#include <typeinfo>

#include "loki_config.hpp"
#include "types.hpp"
#include "assertion.hpp"

namespace Chimera
{
	using namespace Chimera::Types;

	class SPI : public CHIMERA_SPI_INHERITED
	{
	public:
		template<typename... T>
		int init(uint8_t channel, const OptionalInputs<T...>& options)
		{
			//static_assert(is_callable_with<decltype(channel), decltype(options)>(&LOKI_SPI_INHERITED::init), "oops");

			std::cout << typeid(channel).name() << std::endl;

			return CHIMERA_SPI_INHERITED::init(channel, options);
		}


		SPI() = default;
		~SPI() = default;

	private:

	};
}




#endif 