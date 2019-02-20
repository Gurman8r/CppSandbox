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

	Duration::Duration(float value, uint64_t ratio)
		: m_ms((uint64_t)(value) * ratio)
	{
	}

	Duration::Duration(const Duration & copy)
		: m_ms(copy.m_ms)
	{
	}
	
	Duration::~Duration()
	{
	}

	
	const uint64_t Duration::hours() const
	{
		return minutes() / 24ULL;
	}

	const uint64_t Duration::minutes() const
	{
		return seconds() / 60ULL;
	}

	const uint64_t Duration::seconds() const
	{
		return millis() / 1000ULL;
	}
	
	const uint64_t Duration::millis() const
	{
		return (uint64_t)(*this);
	}
	
	const uint64_t Duration::micros() const
	{
		return millis() * 1000ULL;
	}
	
	const uint64_t Duration::nanos() const
	{
		return micros() * 1000ULL;
	}

	void Duration::serialize(std::ostream & out) const
	{
		if (uint64_t h = hours())
		{
			out << h / 10 % 10
				<< h % 10
				<< ':';
		}

		out << (minutes()) / 10 % 10
			<< (minutes()) % 10
			<< ':'
			<< (seconds() % 60) / 10 % 10
			<< (seconds() % 60) % 10
			<< ':'
			<< (millis()) % 1000 / 100
			<< (millis()) % 100 / 10;
	}

	void Duration::deserialize(std::istream & in)
	{
		in >> m_ms;
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