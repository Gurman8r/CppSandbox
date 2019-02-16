#ifndef _ML_MAP_HPP_
#define _ML_MAP_HPP_

#include <MemeCore/STD.hpp>

namespace ml
{
	template <typename Key, typename Val>
	using Map = std::map<Key, Val>;

	template <typename Key, typename Val>
	using HashMap = std::unordered_map<Key, Val>;

	template <typename Key, typename Val>
	using Pair = std::pair<Key, Val>;
}

#endif // !_ML_MAP_HPP_
