#ifndef _ICOMPARABLE_H_
#define _ICOMPARABLE_H_

namespace ml
{
	template <typename T>
	class IComparable
	{
	public:
		using comp_type = T;
		using self_type = IComparable<comp_type>;

	public:
		virtual bool equals(const comp_type & value) const = 0;
		virtual bool lessThan(const comp_type & value) const = 0;

	public:
		inline bool notEquals(const comp_type & value) const
		{
			return !equals(value);
		}

		inline bool greaterThan(const comp_type & value) const
		{
			return !lessThan(value);
		}

	public:
		inline friend bool operator==(const self_type & lhs, const comp_type & rhs)
		{
			return lhs.equals(rhs);
		}
		
		inline friend bool operator!=(const self_type & lhs, const comp_type & rhs)
		{
			return lhs.notEquals(rhs);
		}
		
		inline friend bool operator >(const self_type & lhs, const comp_type & rhs)
		{
			return lhs.greaterThan(rhs);
		}
		
		inline friend bool operator <(const self_type & lhs, const comp_type & rhs)
		{
			return lhs.lessThan(rhs);
		}

		inline friend bool operator>=(const self_type & lhs, const comp_type & rhs)
		{
			return lhs.equals(rhs) || lhs.greaterThan(rhs);
		}

		inline friend bool operator<=(const self_type & lhs, const comp_type & rhs)
		{
			return lhs.equals(rhs) || lhs.lessThan(rhs);
		}

	};

}

#endif // !_ICOMPARABLE_H_
