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
		inline friend bool operator==(const IComparable<T> & lhs, const T & rhs)
		{
			return lhs.equals(rhs);
		};
		
		inline friend bool operator!=(const IComparable<T> & lhs, const T & rhs)
		{
			return !(lhs == rhs);
		};
		
		inline friend bool operator>=(const IComparable<T> & lhs, const T & rhs)
		{
			return (lhs == rhs) || (lhs > rhs);
		};
		
		inline friend bool operator<=(const IComparable<T> & lhs, const T & rhs)
		{
			return (lhs == rhs) || (lhs < rhs);
		};
		
		inline friend bool operator >(const IComparable<T> & lhs, const T & rhs)
		{
			return !(lhs < rhs);
		};
		
		inline friend bool operator <(const IComparable<T> & lhs, const T & rhs)
		{
			return lhs.lessThan(rhs);
		};

	};

}

#endif // !_ICOMPARABLE_H_
