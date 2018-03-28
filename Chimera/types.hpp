#pragma once
#ifndef CHIMERA_TYPES_HPP
#define CHIMERA_TYPES_HPP
#include <map>
#include <string>

#include <boost/variant.hpp>

namespace Chimera
{
	namespace Types
	{
		template<typename... T>
		using OptionalInputs = std::map<std::string, boost::variant<T...>>;
	}
}



#endif 