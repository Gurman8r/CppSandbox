#ifndef _ML_DURATION_HPP_
#define _ML_DURATION_HPP_

#include <MemeCore/Ratio.hpp>
#include <MemeCore/ITrackable.hpp>

namespace ml
{
	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	using Hours			= typename std::chrono::hours;
	using Minutes		= typename std::chrono::minutes;
	using Seconds		= typename std::chrono::seconds;
	using Milliseconds	= typename std::chrono::milliseconds;
	using Microseconds	= typename std::chrono::microseconds;
	using Nanoseconds	= typename std::chrono::nanoseconds;

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	class ML_CORE_API Duration final 
		: public ITrackable
		, public IComparable<Duration>
		, public IComparable<uint64_t>
	{
	public:
		/* * * * * * * * * * * * * * * * * * * * */
		Duration();
		Duration(const uint64_t value);
		Duration(const Duration & copy);
		~Duration();

		template <
			class Rep, 
			class Per
		> Duration(const std::chrono::duration<Rep, Per> & value)
			: Duration(std::chrono::duration_cast<Nanoseconds>(value).count())
		{
		}
		
	public:
		/* * * * * * * * * * * * * * * * * * * * */
		const uint64_t hours() const;
		const uint64_t minutes() const;	
		const uint64_t seconds() const;
		const uint64_t milliseconds() const;
		const uint64_t microseconds() const;
		const uint64_t nanoseconds() const;

	public:
		/* * * * * * * * * * * * * * * * * * * * */
		inline operator uint64_t() const { return nanoseconds(); }

		inline const float delta() const { return ((float)(*this) / 1000000000.0f); }

	public:
		/* * * * * * * * * * * * * * * * * * * * */
		bool equals(const Duration & other) const override;
		bool lessThan(const Duration & other) const override;
		
		bool equals(const uint64_t & other) const override;
		bool lessThan(const uint64_t & other) const override;

		void serialize(std::ostream & out) const override;
		void deserialize(std::istream & in) override;

	public:
		/* * * * * * * * * * * * * * * * * * * * */
		friend Duration	operator+(const Duration & lhs, const Duration & rhs);
		friend Duration	operator-(const Duration & lhs, const Duration & rhs);

		friend Duration	operator+(const Duration & lhs, uint64_t rhs);
		friend Duration	operator-(const Duration & lhs, uint64_t rhs);

		friend Duration & operator+=(Duration & lhs, const Duration & rhs);
		friend Duration & operator-=(Duration & lhs, const Duration & rhs);

		friend Duration & operator+=(Duration & lhs, uint64_t rhs);
		friend Duration & operator-=(Duration & lhs, uint64_t rhs);

	private:
		/* * * * * * * * * * * * * * * * * * * * */
		uint64_t m_count;
	};

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
}

#endif // !_ML_DURATION_HPP_