#ifndef _ML_DURATION_HPP_
#define _ML_DURATION_HPP_

#include <MemeCore/Ratio.hpp>
#include <MemeCore/ITrackable.hpp>

namespace ml
{
	/* * * * * * * * * * * * * * * * * * * * */

	using Hours			= typename std::chrono::duration<int32_t, Ratio<3600>>;
	using Minutes		= typename std::chrono::duration<int32_t, Ratio<60>>;
	using Seconds		= typename std::chrono::duration<int64_t>;
	using Milliseconds	= typename std::chrono::duration<int64_t, Milli>;
	using Microseconds	= typename std::chrono::duration<int64_t, Micro>;
	using Nanoseconds	= typename std::chrono::duration<int64_t, Nano>;

	/* * * * * * * * * * * * * * * * * * * * */

	class ML_CORE_API Duration final
	{
	public:
		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
		constexpr Duration()
			: m_nanos(0ULL)
		{
		}

		constexpr Duration(const intmax_t value)
			: m_nanos((value > 0) ? (uint64_t)value : 0ULL)
		{
		}

		constexpr Duration(const uint64_t value)
			: m_nanos(value)
		{
		}

		constexpr Duration(const Duration & copy)
			: m_nanos(copy.m_nanos)
		{
		}

		template <
			class Rep,
			class Per
		> constexpr Duration(const std::chrono::duration<Rep, Per> & value)
			: m_nanos(std::chrono::duration_cast<Nanoseconds>(value).count())
		{
		}

		~Duration() {}

	public:
		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
		constexpr const uint64_t hours() const
		{
			return std::chrono::duration_cast<Hours>(
				Minutes(this->minutes())).count();
		}

		constexpr const uint64_t minutes() const
		{
			return std::chrono::duration_cast<Minutes>(
				Seconds(this->seconds())).count();
		}

		constexpr const uint64_t seconds() const
		{
			return std::chrono::duration_cast<Seconds>(
				Milliseconds(this->milliseconds())).count();
		}

		constexpr const uint64_t milliseconds() const
		{
			return std::chrono::duration_cast<Milliseconds>(
				Microseconds(this->microseconds())).count();
		}

		constexpr const uint64_t microseconds() const
		{
			return std::chrono::duration_cast<Microseconds>(
				Nanoseconds(this->nanoseconds())).count();
		}

		constexpr const uint64_t nanoseconds() const
		{
			return m_nanos;
		}


	public:
		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
		constexpr operator uint64_t() const 
		{
			return this->nanoseconds();
		}

		constexpr float delta() const 
		{
			return ((float)this->milliseconds() / (float)Milli::den); 
		}

	
	public:
		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
		template <class T>
		constexpr friend bool operator==(const Duration & lhs, const T & rhs)
		{
			return ((uint64_t)lhs == (uint64_t)(Duration(rhs)));
		}

		template <class T>
		constexpr friend bool operator <(const Duration & lhs, const T & rhs)
		{
			return ((uint64_t)lhs < (uint64_t)(Duration(rhs)));
		}

		template <class T>
		constexpr friend bool operator!=(const Duration & lhs, const T & rhs)
		{
			return !(lhs == rhs);
		}

		template <class T>
		constexpr friend bool operator >(const Duration & lhs, const T & rhs)
		{
			return !(lhs < rhs);
		}

		template <class T>
		constexpr friend bool operator>=(const Duration & lhs, const T & rhs)
		{
			return (lhs > rhs) || (lhs == rhs);
		}

		template <class T>
		constexpr friend bool operator<=(const Duration & lhs, const T & rhs)
		{
			return (lhs < rhs) || (lhs == rhs);
		}

		/* * * * * * * * * * * * * * * * * * * * */

		template <class T>
		constexpr friend Duration operator+(const Duration & lhs, const T & rhs)
		{
			return Duration((uint64_t)lhs + (uint64_t)rhs);
		}

		template <class T>
		constexpr friend Duration operator-(const Duration & lhs, const T & rhs)
		{
			return Duration((uint64_t)lhs - (uint64_t)rhs);
		}

		/* * * * * * * * * * * * * * * * * * * * */
		
		inline friend std::ostream & operator<<(std::ostream & out, const Duration & value)
		{
			return out 
				<< (((value.hours() % 24) / 10) % 10)
				<< ((value.hours() % 24) % 10)
				<< ':'
				<< (((value.minutes() % 60) / 10) % 10)
				<< ((value.minutes() % 60) % 10)
				<< ':'
				<< (((value.seconds() % 60) / 10) % 10)
				<< ((value.seconds() % 60) % 10)
				<< ':'
				<< ((value.milliseconds() % 1000) / 100)
				<< ((value.milliseconds() % 100) / 10);
		}

		inline friend std::istream & operator>>(std::istream & in, Duration & value)
		{
			return in >> value.m_nanos;
		}


	private:
		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
		uint64_t m_nanos;
	};

	/* * * * * * * * * * * * * * * * * * * * */
}

#endif // !_ML_DURATION_HPP_