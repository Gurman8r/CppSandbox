#ifndef _ITERATOR_HPP_
#define _ITERATOR_HPP_

#include <MemeCore/ITrackable.hpp>

// https://codereview.stackexchange.com/a/74647/86601

namespace ml
{
	// IC : iterator category
	// VT : value type
	// UT : unqualified type
	// DT : difference type
	template <class IC, class VT, class UT, class DT>
	class Iterator
		: public std::iterator<IC, UT, DT, VT *, VT &>
		, public IComparable<Iterator<IC, VT, UT, DT>>
	{
	public:
		using iterator_category = IC;
		using Value		= VT;
		using unqualified_type	= UT;
		using difference_type	= DT;
		using pointer			= Value * ;
		using reference			= Value & ;
		using const_pointer		= const Value * ;
		using const_reference	= const Value & ;
		using Self			= Iterator<iterator_category, Value, unqualified_type, difference_type>;
		using const_self_type	= Iterator<iterator_category, const Value, unqualified_type, difference_type>;

	public:
		Iterator()
			: m_handle(NULL)
		{
		}
		
		Iterator(const Self & copy)
			: m_handle(copy.m_handle)
		{
		}
		
		explicit Iterator(unqualified_type* ptr)
			: m_handle(ptr)
		{
		}
		
		virtual ~Iterator() {}

	public:
		virtual bool equals(const Self & value) const override
		{
			return m_handle == value.m_handle;
		}
		
		virtual bool lessThan(const Self & value) const override
		{
			return m_handle < value.m_handle;
		}

	public:
		inline operator const_self_type() const
		{
			return Iterator<iterator_category, const VT>(m_handle);
		}


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


		inline Self & operator++()
		{
			assert((m_handle != NULL) && "Out-of-bounds iterator increment!");
			(++m_handle);
			return (*this);
		}
		
		inline Self & operator--()
		{
			assert((m_handle != NULL) && "Out-of-bounds iterator increment!");
			(--m_handle);
			return (*this);
		}
		

		inline Self operator++(int32_t)
		{
			assert((m_handle != NULL) && "Out-of-bounds iterator increment!");
			Self tmp = (*this);
			operator++();
			return tmp;
		}
		
		inline Self operator--(int32_t)
		{
			assert((m_handle != NULL) && "Out-of-bounds iterator increment!");
			Self tmp = (*this);
			operator--();
			return tmp;
		}
		

		inline Self operator+(const difference_type & value) const
		{
			return Self(m_handle + value);
		}
		
		inline Self operator-(const difference_type & value) const
		{
			return Self(m_handle - value);
		}
		

		inline Self & operator+=(const difference_type & value)
		{
			return (*this) = (*this) + value;
		}
		
		inline Self & operator-=(const difference_type & value)
		{
			return (*this) = (*this) - value;
		}
		

		inline const difference_type operator+(const Self & other) const
		{
			return (m_handle + other.m_handle);
		}
		
		inline const difference_type operator-(const Self & other) const
		{
			return (m_handle - other.m_handle);
		}

	private:
		unqualified_type * m_handle;
	};

	template <class VT, class UT = std::remove_cv_t<VT>, class DT = std::ptrdiff_t>
	using ForwardIterator = Iterator<std::forward_iterator_tag, VT, UT, DT>;
}

#endif // !_ITERATOR_HPP_
