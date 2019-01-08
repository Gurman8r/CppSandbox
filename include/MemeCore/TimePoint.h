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
		TimePoint();
		TimePoint(uint64_t value);
		TimePoint(const TimePoint & copy);
		~TimePoint();

		template <class R, class P>
		TimePoint(const std::chrono::duration<R, P> & value)
			: m_value(std::chrono::duration_cast<std::chrono::milliseconds>(value).count())
		{
		}

	public:
		inline operator uint64_t()	const { return m_value; }
		inline operator float()		const { return static_cast<float>(m_value); }
		
		template <class R, class P>
		inline operator std::chrono::duration<R, P>() const
		{
			return std::chrono::milliseconds(m_value);
		}
		
	public:
		const uint64_t nanos() const;
		const uint64_t micros() const;
		const uint64_t millis() const;		
		const uint64_t seconds() const;		
		const uint64_t minutes() const;		
		const uint64_t hours() const;

	public:
		friend TimePoint	operator+(const TimePoint & lhs, const TimePoint & rhs);
		friend TimePoint	operator-(const TimePoint & lhs, const TimePoint & rhs);
		
		friend TimePoint	operator+(const TimePoint & lhs, uint64_t rhs);
		friend TimePoint	operator-(const TimePoint & lhs, uint64_t rhs);
		
		friend TimePoint &	operator+=(TimePoint & lhs, const TimePoint & rhs);
		friend TimePoint &	operator-=(TimePoint & lhs, const TimePoint & rhs);
		
		friend TimePoint &	operator+=(TimePoint & lhs, uint64_t rhs);
		friend TimePoint &	operator-=(TimePoint & lhs, uint64_t rhs);

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
		
	};
}

#endif // !_TIME_POINT_H_
