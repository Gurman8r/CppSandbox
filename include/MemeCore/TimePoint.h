#ifndef _TIME_POINT_H_
#define _TIME_POINT_H_

#include <MemeCore/IComparable.h>
#include <MemeCore/ISerializable.h>
#include <MemeCore/ITrackable.h>
#include <chrono>

namespace ml
{
	class ML_CORE_API TimePoint final 
		: public ITrackable
		, public IComparable<TimePoint>
		, public IComparable<uint64_t>
	{
	private:
		uint64_t m_value;

	public:
		using nanoseconds_t		= std::chrono::nanoseconds;
		using microseconds_t	= std::chrono::microseconds;
		using milliseconds_t	= std::chrono::milliseconds;
		using seconds_t			= std::chrono::seconds;
		using minutes_t			= std::chrono::minutes;
		using hours_t			= std::chrono::hours;

	public:
		TimePoint();
		TimePoint(uint64_t value);
		TimePoint(const nanoseconds_t & value);
		TimePoint(const milliseconds_t & value);
		TimePoint(const TimePoint & copy);
		~TimePoint();
		
	public:
		const uint64_t millis() const;		
		const uint64_t seconds() const;		
		const uint64_t minutes() const;		
		const uint64_t hours() const;

	public:
		friend TimePoint	operator+(const TimePoint & lhs, const TimePoint & rhs);
		friend TimePoint	operator-(const TimePoint & lhs, const TimePoint & rhs);
		friend TimePoint & operator+=(TimePoint & lhs, const TimePoint & rhs);
		friend TimePoint & operator-=(TimePoint & lhs, const TimePoint & rhs);
		friend TimePoint	operator+(const TimePoint & lhs, uint64_t rhs);
		friend TimePoint	operator-(const TimePoint & lhs, uint64_t rhs);
		friend TimePoint & operator+=(TimePoint & lhs, uint64_t rhs);
		friend TimePoint & operator-=(TimePoint & lhs, uint64_t rhs);

	public:
		inline bool equals(const TimePoint & other) const override
		{
			return millis() == other.millis();
		}
		inline bool lessThan(const TimePoint & other) const override
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

	public:
		inline operator uint64_t()	const { return millis(); }
		inline operator float()	const { return static_cast<float>(millis()); }
	
	};
}

#endif // !_TIME_POINT_H_
