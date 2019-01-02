#ifndef _FORWARD_ITERATOR_H_
#define _FORWARD_ITERATOR_H_

#include <MemeCore/ITrackable.h>
#include <MemeCore/IComparable.h>
#include <algorithm>
#include <cassert>
#include <cstddef>
#include <iterator>
#include <utility>

// Source:
// https://codereview.stackexchange.com/a/74647/86601

namespace ml
{
	// IC : iterator category
	// VT : value type
	// UT : unqualified type
	// DT : difference type
	template <class IC, class VT, class UT, class DT>
	class BaseIterator
		: public ITrackable
		, public IComparable<BaseIterator<IC, VT, UT, DT>>
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
			: m_handle(NULL)
		{
		}
		
		BaseIterator(const self_type & copy)
			: m_handle(copy.m_handle)
		{
		}
		
		explicit BaseIterator(unqualified_type* ptr)
			: m_handle(ptr)
		{
		}
		
		virtual ~BaseIterator() {}

	public:
		virtual bool equals(const self_type & value) const override
		{
			return m_handle == value.m_handle;
		}
		
		virtual bool lessThan(const self_type & value) const override
		{
			return m_handle < value.m_handle;
		}

	public:
		inline reference operator*() const
		{
			assert((m_handle != NULL) && "Invalid iterator dereference!");
			return (*m_handle);
		}
		
		inline reference operator->() const
		{
			assert((m_handle != NULL) && "Invalid iterator dereference!");
			return (*m_handle);
		}


		inline self_type & operator++()
		{
			assert((m_handle != NULL) && "Out-of-bounds iterator increment!");
			(++m_handle);
			return (*this);
		}
		
		inline self_type & operator--()
		{
			assert((m_handle != NULL) && "Out-of-bounds iterator increment!");
			(--m_handle);
			return (*this);
		}
		

		inline self_type operator++(int)
		{
			assert((m_handle != NULL) && "Out-of-bounds iterator increment!");
			self_type tmp = (*this);
			operator++();
			return tmp;
		}
		
		inline self_type operator--(int)
		{
			assert((m_handle != NULL) && "Out-of-bounds iterator increment!");
			self_type tmp = (*this);
			operator--();
			return tmp;
		}
		

		inline self_type operator+(const difference_type & value) const
		{
			return self_type(m_handle + value);
		}
		
		inline self_type operator-(const difference_type & value) const
		{
			return self_type(m_handle - value);
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
			return (m_handle + other.m_handle);
		}
		
		inline const difference_type operator-(const self_type & other) const
		{
			return (m_handle - other.m_handle);
		}


		inline operator const_self_type() const
		{
			return BaseIterator<iterator_category, const VT>(m_handle);
		}

	private:
		unqualified_type* m_handle;
	};

	template <class VT, class UT = std::remove_cv_t<VT>, class DT = std::ptrdiff_t>
	using ForwardIterator = BaseIterator<std::forward_iterator_tag, VT, UT, DT>;
}

#endif // !_FORWARD_ITERATOR_H_
