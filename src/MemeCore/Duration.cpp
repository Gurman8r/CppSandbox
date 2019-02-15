#include <MemeCore/Duration.hpp>

namespace ml
{
	Duration::Duration()
		: m_ms(0UL)
	{
	}
	
	Duration::Duration(uint64_t value)
		: m_ms(value)
	{
	}

	Duration::Duration(const Duration & copy)
		: m_ms(copy.m_ms)
	{
	}
	
	Duration::~Duration()
	{
	}

	
	const uint64_t Duration::nanos() const
	{
		return micros() * 1000UL;
	}

	const uint64_t Duration::micros() const
	{
		return millis() * 1000UL;
	}

	const uint64_t Duration::millis() const
	{
		return (uint64_t)(*this);
	}
	
	const uint64_t Duration::seconds() const
	{
		return millis() / 1000UL;
	}
	
	const uint64_t Duration::minutes() const
	{
		return seconds() / 60UL;
	}
	
	const uint64_t Duration::hours() const
	{
		return minutes() / 24UL;
	}


	Duration operator+(const Duration & lhs, const Duration & rhs)
	{
		return lhs + (uint64_t)rhs;
	}
	
	Duration operator-(const Duration & lhs, const Duration & rhs)
	{
		return lhs - (uint64_t)rhs;
	}
	
	
	Duration operator+(const Duration & lhs, uint64_t rhs)
	{
		return lhs.millis() + rhs;
	}
	
	Duration operator-(const Duration & lhs, uint64_t rhs)
	{
		return lhs.millis() - rhs;
	}
	
	
	Duration & operator+=(Duration & lhs, const Duration & rhs)
	{
		return (lhs = (lhs + rhs));
	}
	
	Duration & operator-=(Duration & lhs, const Duration & rhs)
	{
		return (lhs = (lhs - rhs));
	}
	
	
	Duration & operator+=(Duration & lhs, uint64_t rhs)
	{
		return (lhs = (lhs + rhs));
	}
	
	Duration & operator-=(Duration & lhs, uint64_t rhs)
	{
		return (lhs = (lhs - rhs));
	}
}