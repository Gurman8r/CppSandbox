#ifndef _FORWARD_ITERATOR_H_
#define _FORWARD_ITERATOR_H_

#include <MemeLib/ITrackable.h>
#include <algorithm>
#include <cassert>
#include <cstddef>
#include <iterator>
#include <type_traits>
#include <utility>

// Source:
// https://codereview.stackexchange.com/questions/74609/custom-iterator-for-a-linked-list-class

namespace ml
{
	template <class IC, class TV, class UT = std::remove_cv_t<TV>, class DT = std::ptrdiff_t>
	class ArrayIterator
		: public ITrackable
		, public std::iterator<IC, UT, DT, TV*, TV&>
	{
	public:
		using iterator_category = IC;
		using value_type		= TV;
		using unqualified_type	= UT;
		using difference_type	= DT;
		using pointer			= value_type * ;
		using reference			= value_type & ;
		using self_type			= ArrayIterator<iterator_category, value_type, unqualified_type>;

	public:
		ArrayIterator()
			: m_ptr(NULL)
		{
		}
		ArrayIterator(const self_type & copy)
			: m_ptr(copy.m_ptr)
		{
		}
		explicit ArrayIterator(unqualified_type* ptr)
			: m_ptr(ptr)
		{
		}
		virtual ~ArrayIterator() {}

	public:
		void swap(ArrayIterator & value) noexcept
		{
			using std::swap;
			swap(m_ptr, value.m_ptr);
		}

	public:
		template <class U>
		inline bool operator==(const ArrayIterator<iterator_category, U> & rhs) const
		{
			return (m_ptr == rhs.m_ptr);
		}

		template <class U>
		inline bool operator!=(const ArrayIterator<iterator_category, U> & rhs) const
		{
			return !((*this) == rhs);
		}

		inline reference operator*() const
		{
			assert((m_ptr != NULL) && "Invalid iterator dereference!");
			return (*m_ptr);
		}
		inline reference operator->() const
		{
			assert((m_ptr != NULL) && "Invalid iterator dereference!");
			return (*m_ptr);
		}

		inline self_type & operator++()
		{
			assert((m_ptr != NULL) && "Out-of-bounds iterator increment!");
			(++m_ptr);
			return (*this);
		}
		inline self_type & operator--()
		{
			assert((m_ptr != NULL) && "Out-of-bounds iterator increment!");
			(--m_ptr);
			return (*this);
		}
		
		inline self_type operator++(int)
		{
			assert((m_ptr != NULL) && "Out-of-bounds iterator increment!");
			self_type tmp = (*this);
			operator++();
			return tmp;
		}
		inline self_type operator--(int)
		{
			assert((m_ptr != NULL) && "Out-of-bounds iterator increment!");
			self_type tmp = (*this);
			operator--();
			return tmp;
		}
				
		inline self_type operator+(const difference_type & value) const
		{
			return self_type(m_ptr + value);
		}
		inline self_type operator-(const difference_type & value) const
		{
			return self_type(m_ptr - value);
		}
		
		inline self_type & operator+=(const difference_type & value)
		{
			return (*this) = (*this) + value;
		}
		inline self_type & operator-=(const difference_type & value)
		{
			return (*this) = (*this) - value;
		}
		
		inline const difference_type operator+(const self_type & other) const
		{
			return (m_ptr + other.m_ptr);
		}
		inline const difference_type operator-(const self_type & other) const
		{
			return (m_ptr - other.m_ptr);
		}

		inline operator ArrayIterator<iterator_category, const value_type>() const
		{
			return ArrayIterator<iterator_category, const TV>(m_ptr);
		}

	private:
		unqualified_type* m_ptr;
	};
}

#endif // !_FORWARD_ITERATOR_H_
