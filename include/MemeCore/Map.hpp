#ifndef _ML_MAP_HPP_
#define _ML_MAP_HPP_

/* * * * * * * * * * * * * * * * * * * * */

#include <MemeCore/STD.hpp>

/* * * * * * * * * * * * * * * * * * * * */

#define ML_MAKE_KV_TYPE(NAME, TYPE) \
template <class K, class V> \
using NAME = typename TYPE<K, V>;

/* * * * * * * * * * * * * * * * * * * * */

namespace ml
{
	/* * * * * * * * * * * * * * * * * * * * */

	ML_MAKE_KV_TYPE(Map,		std::map)
	ML_MAKE_KV_TYPE(MultiMap,	std::multimap)
	ML_MAKE_KV_TYPE(HashMap,	std::unordered_map)
	ML_MAKE_KV_TYPE(Pair,		std::pair)

	/* * * * * * * * * * * * * * * * * * * * */
}

#endif // !_ML_MAP_HPP_
