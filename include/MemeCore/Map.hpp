#ifndef _ML_MAP_HPP_
#define _ML_MAP_HPP_

#include <MemeCore/StandardLib.hpp>

namespace ml
{
	/* * * * * * * * * * * * * * * * * * * * */

	template <class K, class V> using Map		= typename std::map<K, V>;
	template <class K, class V> using MultiMap	= typename std::multimap<K, V>;
	template <class K, class V> using HashMap	= typename std::unordered_map<K, V>;
	template <class K, class V> using Pair		= typename std::pair<K, V>;

	/* * * * * * * * * * * * * * * * * * * * */
}

#endif // !_ML_MAP_HPP_
