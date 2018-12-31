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
	// IC : iterator category
	// VT : value type
	// UT : unqualified type
	// DT : difference type
	template <class IC, class VT, class UT = std::remove_cv_t<VT>, class DT = std::ptrdiff_t>
	class BaseIterator
		: public ITrackable
		, public std::iterator<IC, UT, DT, VT*, VT&>
	{
	public:
		using iterator_category = IC;
		using value_type		= VT;
		using unqualified_type	= UT;
		using difference_type	= DT;
		using pointer			= value_type * ;
		using reference			= value_type & ;
		using self_type			= BaseIterator<iterator_category, value_type, unqualified_type, difference_type>;
		using const_self_type	= BaseIterator<iterator_category, const value_type, unqualified_type, difference_type>;

	public:
		BaseIterator()
			: m_ptr(NULL)
		{
		}
		BaseIterator(const self_type & copy)
			: m_ptr(copy.m_ptr)
		{
		}
		explicit BaseIterator(unqualified_type* ptr)
			: m_ptr(ptr)
		{
		}
		virtual ~BaseIterator() {}

	public:
		template <class U>
		inline bool operator==(const BaseIterator<iterator_category, U> & rhs) const
		{
			return (m_ptr == rhs.m_ptr);
		}

		template <class U>
		inline bool operator!=(const BaseIterator<iterator_category, U> & rhs) const
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

		inline operator const_self_type() const
		{
			return BaseIterator<iterator_category, const VT>(m_ptr);
		}

	private:
		unqualified_type* m_ptr;
	};

	template <class VT, class UT = std::remove_cv_t<VT>, class DT = std::ptrdiff_t>
	using ForwardIterator = BaseIterator<std::forward_iterator_tag, VT, UT, DT>;
}

#endif // !_FORWARD_ITERATOR_H_
