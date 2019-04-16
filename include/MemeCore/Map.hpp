#ifndef _ML_MAP_HPP_
#define _ML_MAP_HPP_

#include <MemeCore/STD.hpp>

/* * * * * * * * * * * * * * * * * * * * */

#define ML_KEY_VAL template <typename Key, typename Val>

/* * * * * * * * * * * * * * * * * * * * */

namespace ml
{
	/* * * * * * * * * * * * * * * * * * * * */

	ML_KEY_VAL using Map		= std::map			<Key, Val>;
	ML_KEY_VAL using MultiMap	= std::multimap		<Key, Val>;
	ML_KEY_VAL using HashMap	= std::unordered_map<Key, Val>;
	ML_KEY_VAL using Pair		= std::pair			<Key, Val>;

	/* * * * * * * * * * * * * * * * * * * * */
}

#endif // !_ML_MAP_HPP_
