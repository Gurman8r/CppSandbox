#include <MemeCore/Duration.hpp>

namespace ml
{
	/* * * * * * * * * * * * * * * * * * * * */

	Duration::Duration()
		: m_nanoseconds(0ULL)
	{
	}
	
	Duration::Duration(const uint64_t value)
		: m_nanoseconds(value)
	{
	}

	Duration::Duration(const Duration & copy)
		: m_nanoseconds(copy.m_nanoseconds)
	{
	}
	
	Duration::~Duration()
	{
	}

	/* * * * * * * * * * * * * * * * * * * * */
	
	const uint64_t Duration::hours() const
	{
		return std::chrono::duration_cast<Hours>(
			Minutes(minutes())
		).count();
	}

	const uint64_t Duration::minutes() const
	{
		return std::chrono::duration_cast<Minutes>(
			Seconds(seconds())
		).count();
	}

	const uint64_t Duration::seconds() const
	{
		return std::chrono::duration_cast<Seconds>(
			Milliseconds(milliseconds())
		).count();
	}
	
	const uint64_t Duration::milliseconds() const
	{
		return std::chrono::duration_cast<Milliseconds>(
			Microseconds(microseconds())
		).count();
	}
	
	const uint64_t Duration::microseconds() const
	{
		return std::chrono::duration_cast<Microseconds>(
			Nanoseconds(nanoseconds())
		).count();
	}
	
	const uint64_t Duration::nanoseconds() const
	{
		return m_nanoseconds;
	}

	/* * * * * * * * * * * * * * * * * * * * */

	bool Duration::equals(const Duration & other) const
	{
		return (milliseconds() == other.milliseconds());
	}

	bool Duration::lessThan(const Duration & other) const
	{
		return (milliseconds() < other.milliseconds());
	}


	bool Duration::equals(const uint64_t & other) const
	{
		return (milliseconds() == other);
	}

	bool Duration::lessThan(const uint64_t & other) const
	{
		return (milliseconds() < other);
	}

	
	void Duration::serialize(std::ostream & out) const
	{
		const uint64_t min = minutes();
		const uint64_t sec = seconds();
		const uint64_t mil = milliseconds();
		
		out << ((min / 10) % 10)
			<< (min % 10)
			<< ':'
			<< (((sec % 60) / 10) % 10)
			<< ((sec % 60) % 10)
			<< ':' 
			<< ((mil % 1000) / 100)
			<< ((mil % 100) / 10);
	}

	void Duration::deserialize(std::istream & in)
	{
		in >> m_nanoseconds;
	}

	/* * * * * * * * * * * * * * * * * * * * */

	Duration operator+(const Duration & lhs, const Duration & rhs)
	{
		return (lhs + (uint64_t)rhs);
	}
	
	Duration operator-(const Duration & lhs, const Duration & rhs)
	{
		return (lhs - (uint64_t)rhs);
	}
	
	
	Duration operator+(const Duration & lhs, uint64_t rhs)
	{
		return (lhs.milliseconds() + rhs);
	}
	
	Duration operator-(const Duration & lhs, uint64_t rhs)
	{
		return (lhs.milliseconds() - rhs);
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

	/* * * * * * * * * * * * * * * * * * * * */
}