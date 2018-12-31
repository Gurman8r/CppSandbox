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
		, public IComparable<uint64_t>
		, public IComparable<TimePoint>
	{
	public:
		using duration_type = std::chrono::duration<uint64_t>;
		using nanoseconds	= std::chrono::nanoseconds;
		using milliseconds	= std::chrono::milliseconds;

	public:
		TimePoint()
			: m_ms(0UL)
		{
		}
		
		TimePoint(uint64_t value)
			: m_ms(value)
		{
		}
		
		TimePoint(const nanoseconds & value)
			: TimePoint(std::chrono::duration_cast<milliseconds>(value))
		{
		}
		
		TimePoint(const milliseconds & value)
			: m_ms(value.count())
		{
		}
		
		TimePoint(const TimePoint & copy)
			: m_ms(copy.m_ms)
		{
		}
		
		~TimePoint()
		{
		}
		
	public:
		inline const uint64_t millis() const
		{
			return m_ms;
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

	public:
		inline friend TimePoint	operator+(const TimePoint & lhs, const TimePoint & rhs)
		{
			return lhs + (uint64_t)rhs;
		}
		
		inline friend TimePoint	operator-(const TimePoint & lhs, const TimePoint & rhs)
		{
			return lhs - (uint64_t)rhs;
		}


		inline friend TimePoint & operator+=(TimePoint & lhs, const TimePoint & rhs)
		{
			return (lhs = (lhs + rhs));
		}
		
		inline friend TimePoint & operator-=(TimePoint & lhs, const TimePoint & rhs)
		{
			return (lhs = (lhs - rhs));
		}


		inline friend TimePoint	operator+(const TimePoint & lhs, uint64_t rhs)
		{
			return (lhs.millis() + rhs);
		}
		
		inline friend TimePoint	operator-(const TimePoint & lhs, uint64_t rhs)
		{
			return (lhs.millis() - rhs);
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
		inline bool equals(const uint64_t & other) const override
		{
			return millis() == other;
		}
		
		inline bool lessThan(const uint64_t & other) const override
		{
			return millis() < other;
		}
		

		inline bool equals(const TimePoint & other) const override
		{
			return (*this) == (uint64_t)other;
		}
		
		inline bool lessThan(const TimePoint & other) const override
		{
			return (*this) < (uint64_t)other;
		}
		

		inline operator uint64_t() const
		{
			return millis();
		}
	
	private:
		uint64_t m_ms;
	};
}

#endif // !_TIME_POINT_H_
