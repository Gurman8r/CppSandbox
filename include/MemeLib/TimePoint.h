#ifndef _TIME_POINT_H_
#define _TIME_POINT_H_

#include <MemeLib/ITrackable.h>
#include <chrono>

namespace ml
{
	class ML_API TimePoint final 
		: public ITrackable
		, public IComparable<TimePoint>
		, public IComparable<uint64_t>
	{
	public:
		using duration_type = std::chrono::duration<uint64_t>;
		using millis_type = const std::chrono::milliseconds;


	public:
		TimePoint()
			: m_millis(0UL)
		{
		}
		TimePoint(uint64_t value)
			: m_millis(value)
		{
		}
		TimePoint(const duration_type & time)
			: m_millis(time.count())
		{
		}
		TimePoint(const millis_type & milliseconds)
			: m_millis(milliseconds.count())
		{
		}
		TimePoint(const TimePoint & copy)
			: m_millis(copy.m_millis)
		{
		}
		~TimePoint()
		{
		}

		inline const uint64_t millis() const
		{
			return m_millis;
		}
		inline const uint64_t seconds() const
		{
			return millis() / 1000UL;
		}
		inline const uint64_t minutes() const
		{
			return seconds() / 60UL;
		}
		inline const uint64_t hours() const
		{
			return minutes() / 24UL;
		}

		inline friend TimePoint	operator+(const TimePoint & lhs, const TimePoint & rhs)
		{
			return (lhs + rhs.millis());
		}
		inline friend TimePoint	operator-(const TimePoint & lhs, const TimePoint & rhs)
		{
			return (lhs - rhs.millis());
		}
		inline friend TimePoint	operator+(const TimePoint & lhs, uint64_t rhs)
		{
			return TimePoint(lhs.millis() + rhs);
		}
		inline friend TimePoint	operator-(const TimePoint & lhs, uint64_t rhs)
		{
			return TimePoint(lhs.millis() - rhs);
		}
		
		inline friend TimePoint & operator+=(TimePoint & lhs, const TimePoint & rhs)
		{
			return (lhs = (lhs + rhs));
		}
		inline friend TimePoint & operator-=(TimePoint & lhs, const TimePoint & rhs)
		{
			return (lhs = (lhs - rhs));
		}
		inline friend TimePoint & operator+=(TimePoint & lhs, uint64_t rhs)
		{
			return (lhs = (lhs + rhs));
		}
		inline friend TimePoint & operator-=(TimePoint & lhs, uint64_t rhs)
		{
			return (lhs = (lhs - rhs));
		}

	public:
		inline bool equals(const TimePoint & other) const override
		{
			return millis() == other.millis();
		}
		inline bool equals(const uint64_t & other) const override
		{
			return millis() == other;
		}
		
		inline bool lessThan(const TimePoint & other) const override
		{
			return millis() < other.millis();
		}
		inline bool lessThan(const uint64_t & other) const override
		{
			return millis() < other;
		}
	
	private:
		uint64_t m_millis;
	};
}

#endif // !_TIME_POINT_H_
