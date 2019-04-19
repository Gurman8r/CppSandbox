#ifndef _ML_MAP_HPP_
#define _ML_MAP_HPP_

#include <MemeCore/STD.hpp>
#include <MemeCore/Preprocessor.hpp>

namespace ml
{
	/* * * * * * * * * * * * * * * * * * * * */

	ML_GENERATE_KV_TYPE(Map,		std::map)
	ML_GENERATE_KV_TYPE(MultiMap,	std::multimap)
	ML_GENERATE_KV_TYPE(HashMap,	std::unordered_map)
	ML_GENERATE_KV_TYPE(Pair,		std::pair)

	/* * * * * * * * * * * * * * * * * * * * */
}

#endif // !_ML_MAP_HPP_
