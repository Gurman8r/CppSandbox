#ifndef _ICOMPARABLE_H_
#define _ICOMPARABLE_H_

namespace ml
{
	template <typename T>
	class IComparable
	{
	public:
		virtual bool equals(const T & value) const = 0;
		virtual bool lessThan(const T & value) const = 0;

	public:
		inline bool notEquals(const T & value) const
		{
			return !equals(value);
		}
		inline bool greaterThan(const T & value) const
		{
			return !lessThan(value);
		}

	public:
		inline friend bool operator==(const IComparable<T> & lhs, const T & rhs)
		{
			return lhs.equals(rhs);
		};
		
		inline friend bool operator!=(const IComparable<T> & lhs, const T & rhs)
		{
			return lhs.notEquals(rhs);
		};
		
		inline friend bool operator >(const IComparable<T> & lhs, const T & rhs)
		{
			return lhs.greaterThan(rhs);
		};
		
		inline friend bool operator <(const IComparable<T> & lhs, const T & rhs)
		{
			return lhs.lessThan(rhs);
		};

		inline friend bool operator>=(const IComparable<T> & lhs, const T & rhs)
		{
			return lhs.equals(rhs) || lhs.greaterThan(rhs);
		};

		inline friend bool operator<=(const IComparable<T> & lhs, const T & rhs)
		{
			return lhs.equals(rhs) || lhs.lessThan(rhs);
		};

	};

}

#endif // !_ICOMPARABLE_H_
