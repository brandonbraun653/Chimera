#pragma once
#ifndef CHIMERA_TYPES_HPP
#define CHIMERA_TYPES_HPP

#include <boost/container/vector.hpp>

namespace Chimera
{
	namespace Types
	{

		enum DataType 
		{
			TYPE_INT,
			TYPE_CHAR,
			TYPE_FLOAT,
			TYPE_DOUBLE,
			TYPE_UINT8,
			TYPE_UINT16,
			TYPE_UINT32,
			TYPE_UINT64,
			TOTAL_TYPES_SUPPORTED
		};

		typedef struct Param
		{
			DataType type;
			union
			{
				int int_value;
				char char_value;
				float float_value;
				double double_value;
				uint8_t uint8_value;
				uint16_t uint16_value;
				uint32_t uint32_value;
				uint64_t uint64_value;
			};
		};

		/* Used for passing multiple unknown parameters into functions*/
		using ParamVec = boost::container::vector<Param>;
	}
}



#endif 