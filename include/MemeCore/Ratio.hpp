#ifndef _ML_RATIO_HPP_
#define _ML_RATIO_HPP_

#include <MemeCore/STD.hpp>

namespace ml
{
	/* * * * * * * * * * * * * * * * * * * * */

	template <
		uint64_t N, // Numerator
		uint64_t D	// Denominator
	>
	using Ratio = typename std::ratio<N, D>;

	/* * * * * * * * * * * * * * * * * * * * */

	using Atto	= typename std::atto;	// 1, 1000000000000000000LL
	using Femto = typename std::femto;	// 1, 1000000000000000LL
	using Pico	= typename std::pico;	// 1, 1000000000000LL
	using Nano	= typename std::nano;	// 1, 1000000000
	using Micro = typename std::micro;	// 1, 1000000
	using Milli	= typename std::milli;	// 1, 1000
	using Centi = typename std::centi;	// 1, 100
	using Deci	= typename std::deci;	// 1, 10
	using Deca	= typename std::deca;	// 10, 1
	using Hecto = typename std::hecto;	// 100, 1
	using Kilo	= typename std::kilo;	// 1000, 1
	using Mega	= typename std::mega;	// 1000000, 1
	using Giga	= typename std::giga;	// 1000000000, 1
	using Tera	= typename std::tera;	// 1000000000000LL, 1
	using Peta	= typename std::peta;	// 1000000000000000LL, 1
	using Exa	= typename std::exa;	// 1000000000000000000LL, 1

	/* * * * * * * * * * * * * * * * * * * * */

	template <
		typename T, 
		uint64_t N, 
		uint64_t D
	>
	constexpr T ratio_cast(const T & value, const Ratio<N, D> & r)
	{
		return 
			((((T)(r.num) == (T)(1)) && ((T)(r.den) == (T)(1)))
				? (value)
				: ((((T)(r.num) != (T)(1)) && ((T)(r.den) == (T)(1)))
					? (value * (T)(r.num))
					: ((((T)(r.num) == (T)(1)) && ((T)(r.den) != (T)(1)))
						? (value / (T)(r.den))
						: (value * (T)(r.num) / (T)(r.den))
					)
				)
			);
	}

	/* * * * * * * * * * * * * * * * * * * * */
}

#endif // !_ML_RATIO_HPP_