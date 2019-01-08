#include <MemeCore/TimePoint.h>

namespace ml
{
	TimePoint::TimePoint()
		: m_value(0UL)
	{
	}
	
	TimePoint::TimePoint(uint64_t value)
		: m_value(value)
	{
	}
	
	TimePoint::TimePoint(const TimePoint & copy)
		: m_value(copy.m_value)
	{
	}
	
	TimePoint::~TimePoint()
	{
	}

	
	const uint64_t TimePoint::nanos() const
	{
		return micros() * 1000UL;
	}

	const uint64_t TimePoint::micros() const
	{
		return millis() * 1000UL;
	}

	const uint64_t TimePoint::millis() const
	{
		return (uint64_t)(*this);
	}
	
	const uint64_t TimePoint::seconds() const
	{
		return millis() / 1000UL;
	}
	
	const uint64_t TimePoint::minutes() const
	{
		return seconds() / 60UL;
	}
	
	const uint64_t TimePoint::hours() const
	{
		return minutes() / 24UL;
	}


	TimePoint operator+(const TimePoint & lhs, const TimePoint & rhs)
	{
		return lhs + (uint64_t)rhs;
	}
	
	TimePoint operator-(const TimePoint & lhs, const TimePoint & rhs)
	{
		return lhs - (uint64_t)rhs;
	}
	
	
	TimePoint operator+(const TimePoint & lhs, uint64_t rhs)
	{
		return (lhs.millis() + rhs);
	}
	
	TimePoint operator-(const TimePoint & lhs, uint64_t rhs)
	{
		return (lhs.millis() - rhs);
	}
	
	
	TimePoint & operator+=(TimePoint & lhs, const TimePoint & rhs)
	{
		return (lhs = (lhs + rhs));
	}
	
	TimePoint & operator-=(TimePoint & lhs, const TimePoint & rhs)
	{
		return (lhs = (lhs - rhs));
	}
	
	
	TimePoint & operator+=(TimePoint & lhs, uint64_t rhs)
	{
		return (lhs = (lhs + rhs));
	}
	
	TimePoint & operator-=(TimePoint & lhs, uint64_t rhs)
	{
		return (lhs = (lhs - rhs));
	}
}