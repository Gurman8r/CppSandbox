#ifndef _TIME_POINT_H_
#define _TIME_POINT_H_

#include <MemeCore/IComparable.h>
#include <MemeCore/ISerializable.h>
#include <MemeCore/ITrackable.h>
#include <chrono>

namespace ml
{
	class ML_CORE_API Duration final 
		: public ITrackable
		, public IComparable<Duration>
		, public IComparable<uint64_t>
	{
	private:
		uint64_t m_millis;

	public:
		Duration();
		Duration(uint64_t value);
		Duration(const Duration & copy);
		~Duration();

		template <class Rep, class Period>
		Duration(const std::chrono::duration<Rep, Period> & value)
			: Duration(std::chrono::duration_cast<std::chrono::milliseconds>(value).count())
		{
		}

	public:
		inline operator uint64_t() const { return m_millis; }
		
		template <class R, class P>
		inline operator std::chrono::duration<R, P>() const
		{
			return std::chrono::milliseconds(m_nanos);
		}
		
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
		
		friend Duration &	operator+=(Duration & lhs, const Duration & rhs);
		friend Duration &	operator-=(Duration & lhs, const Duration & rhs);
		
		friend Duration &	operator+=(Duration & lhs, uint64_t rhs);
		friend Duration &	operator-=(Duration & lhs, uint64_t rhs);

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
		
	};
}

#endif // !_TIME_POINT_H_
