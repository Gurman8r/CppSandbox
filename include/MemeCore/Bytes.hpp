#ifndef _BYTES_HPP_
#define _BYTES_HPP_

#include <MemeCore/Ratio.hpp>

namespace ml
{
	/* * * * * * * * * * * * * * * * * * * * */

	struct Bytes final
		: public ITrackable
		, public IComparable<uint64_t>
	{
		/* * * * * * * * * * * * * * * * * * * * */

		uint64_t	count;	// Scaled display size
		CString		unit;	// Unit of display size
		uint64_t	real;	// Real size in bytes

		/* * * * * * * * * * * * * * * * * * * * */

		Bytes(const Bytes & copy)
			: count(copy.count)
			, unit(copy.unit)
			, real(copy.real)
		{
		}

		template <
			intmax_t N, // Numerator
			intmax_t D	// Denominator
		>
		Bytes(const uint64_t value, const Ratio<N, D> & r)
			: Bytes(ratio_cast(value, r))
		{
		}

		Bytes(const uint64_t value)
			: real(value)
		{
			if (value == 0) // 
			{
				count = 0;
				unit = "-";
			}
			else if (value < Kilo::num) // Bytes
			{
				count = ratio_cast(value, Ratio<1, 1>());
				unit = "B";
			}
			else if (value < Mega::num) // Kilobytes
			{
				count = ratio_cast(value, Milli());
				unit = "kB";
			}
			else if (value < Giga::num) // Megabytes
			{
				count = ratio_cast(value, Micro());
				unit = "MB";
			}
			else if (value < Tera::num) // Gigabytes
			{
				count = ratio_cast(value, Nano());
				unit = "GB";
			}
			else if (value < Peta::num) // Terabytes
			{
				count = ratio_cast(value, Pico());
				unit = "TB";
			}
			else if (value < Exa::num) // Petabytes
			{
				count = ratio_cast(value, Femto());
				unit = "TB";
			}
			else // Exabytes
			{
				count = ratio_cast(value, Atto());
				unit = "EB";
			}
		}

		/* * * * * * * * * * * * * * * * * * * * */

		inline void serialize(std::ostream & out) const override
		{
			if (count)
			{
				out << count;
			}
			out << unit;
		}

		inline void deserialize(std::istream & in) override
		{
			uint64_t temp;
			in >> temp;
			(*this) = Bytes(temp);
		}

		/* * * * * * * * * * * * * * * * * * * * */

		inline operator uint64_t() const
		{
			return real;
		}

		inline bool equals(const uint64_t & other) const override
		{
			return real == other;
		}

		inline bool lessThan(const uint64_t & other) const override
		{
			return real < other;
		}

		/* * * * * * * * * * * * * * * * * * * * */
	};

	/* * * * * * * * * * * * * * * * * * * * */

	constexpr uint64_t operator "" _kB(const uint64_t value)
	{
		return ratio_cast(value, Kilo());
	}

	constexpr uint64_t operator "" _MB(const uint64_t value)
	{
		return ratio_cast(value, Mega());
	}

	constexpr uint64_t operator "" _GB(const uint64_t value)
	{
		return ratio_cast(value, Giga());
	}

	constexpr uint64_t operator "" _TB(const uint64_t value)
	{
		return ratio_cast(value, Tera());
	}

	constexpr uint64_t operator "" _PB(const uint64_t value)
	{
		return ratio_cast(value, Peta());
	}

	constexpr uint64_t operator "" _EB(const uint64_t value)
	{
		return ratio_cast(value, Exa());
	}

	/* * * * * * * * * * * * * * * * * * * * */
}

#endif // !_BYTES_HPP_
