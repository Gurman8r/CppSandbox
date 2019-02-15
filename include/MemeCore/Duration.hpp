#ifndef _TIME_POINT_HPP_
#define _TIME_POINT_HPP_

#include <MemeCore/ITrackable.hpp>

#define ml_duration_cast(T, value) \
(uint64_t)(std::chrono::duration_cast<T>(value).count()) \

namespace ml
{
	/* * * * * * * * * * * * * * * * * * * * */

	using Hours		= typename std::chrono::hours;			// _h
	using Minutes	= typename std::chrono::minutes;		// _m
	using Seconds	= typename std::chrono::seconds;		// _s
	using Millis	= typename std::chrono::milliseconds;	// _ms
	using Micros	= typename std::chrono::microseconds;	// _us
	using Nanos		= typename std::chrono::nanoseconds;	// _ns

	/* * * * * * * * * * * * * * * * * * * * */

	class ML_CORE_API Duration final 
		: public ITrackable
		, public IComparable<Duration>
		, public IComparable<uint64_t>
	{
	private:
		uint64_t m_ms;

	public:
		Duration();
		Duration(uint64_t value);
		Duration(const Duration & copy);
		~Duration();

		template <class Rep, class Per>
		Duration(const std::chrono::duration<Rep, Per> & value)
			: Duration(ml_duration_cast(Millis, value))
		{
		}

	public:
		inline operator uint64_t() const { return m_ms; }

		inline operator float() const { return static_cast<float>(m_ms); }

		inline const float delta() const { return (float)(*this) / 1000.f; }
		
	public:
		const uint64_t nanos() const;
		const uint64_t micros() const;
		const uint64_t millis() const;		
		const uint64_t seconds() const;		
		const uint64_t minutes() const;		
		const uint64_t hours() const;

	public:
		friend Duration	operator+(const Duration & lhs, const Duration & rhs);
		friend Duration	operator-(const Duration & lhs, const Duration & rhs);
		
		friend Duration	operator+(const Duration & lhs, uint64_t rhs);
		friend Duration	operator-(const Duration & lhs, uint64_t rhs);
		
		friend Duration & operator+=(Duration & lhs, const Duration & rhs);
		friend Duration & operator-=(Duration & lhs, const Duration & rhs);
		
		friend Duration & operator+=(Duration & lhs, uint64_t rhs);
		friend Duration & operator-=(Duration & lhs, uint64_t rhs);

	public:
		inline bool equals(const Duration & other) const override
		{
			return millis() == other.millis();
		}
		inline bool lessThan(const Duration & other) const override
		{
			return millis() < other.millis();
		}

		inline bool equals(const uint64_t & other) const override
		{
			return millis() == other;
		}
		inline bool lessThan(const uint64_t & other) const override
		{
			return millis() < other;
		}

		inline void serialize(std::ostream & out) const override
		{
			out << (minutes()) / 10 % 10
				<< (minutes()) % 10
				<< ':'
				<< (seconds() % 60) / 10 % 10
				<< (seconds() % 60) % 10
				<< ':'
				<< (millis()) / 10 % 10
				<< (millis()) % 10;
		}
		inline void deserialize(std::istream & in) override
		{
			in >> m_ms;
		}
		
	};

	/* * * * * * * * * * * * * * * * * * * * */

	constexpr Hours		operator "" _h (uint64_t value) { return Hours(value);	 }
	constexpr Minutes	operator "" _m (uint64_t value) { return Minutes(value); }
	constexpr Seconds	operator "" _s (uint64_t value) { return Seconds(value); }
	constexpr Millis	operator "" _ms(uint64_t value) { return Millis(value);  }
	constexpr Micros	operator "" _us(uint64_t value) { return Micros(value);  }
	constexpr Nanos		operator "" _ns(uint64_t value) { return Nanos(value);	 }

	/* * * * * * * * * * * * * * * * * * * * */
}

#endif // !_TIME_POINT_HPP_
