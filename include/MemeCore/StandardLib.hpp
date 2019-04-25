#ifndef _ML_STANDARD_LIB_HPP_
#define _ML_STANDARD_LIB_HPP_

/* * * * * * * * * * * * * * * * * * * * */

#include <algorithm>
#include <array>
#include <cassert>
#include <cctype>
#include <chrono>
#include <csignal>
#include <cstddef>
#include <ctime>
#include <fstream>
#include <future>
#include <initializer_list>
#include <iomanip>
#include <iostream>
#include <iterator>
#include <limits>
#include <locale>
#include <map>
#include <mutex>
#include <queue>
#include <random>
#include <regex>
#include <stack>
#include <sstream>
#include <stdarg.h>
#include <stdint.h>
#include <string>
#include <thread>
#include <type_traits>
#include <typeindex>
#include <typeinfo>
#include <unordered_map>
#include <utility>
#include <vector>

/* * * * * * * * * * * * * * * * * * * * */

namespace ml
{
	template <class K, class V> using HashMap		= typename std::unordered_map<K, V>;
	template <class T>			using Initializer	= typename std::initializer_list<T>;
	template <class K, class V> using Map			= typename std::map<K, V>;
	template <class K, class V> using MultiMap		= typename std::multimap<K, V>;
	template <class K, class V> using Pair			= typename std::pair<K, V>;
}

/* * * * * * * * * * * * * * * * * * * * */

#endif // !_ML_STANDARD_LIB_HPP_