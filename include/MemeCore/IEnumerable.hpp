#ifndef _IENUMERABLE_HPP_
#define _IENUMERABLE_HPP_

#include <MemeCore/Iterator.hpp>

namespace ml
{
	// Supports simple iteration of a fixed array
	template <typename T>
	class IEnumerable
	{
	public:
		using value_type		= T;
		using pointer			= value_type *;
		using reference			= value_type &;
		using const_pointer		= const value_type *;
		using const_reference	= const value_type &;
		using iterator			= ForwardIterator<value_type>;
		using const_iterator	= ForwardIterator<const value_type>;

	public:
		IEnumerable(pointer first, pointer last)
			: m_first(first)
			, m_tail(last)
		{
		}		
		virtual ~IEnumerable() {}

	public:
		inline iterator begin()
		{
			return iterator(m_first);
		}
		inline iterator end()
		{
			return iterator(m_tail);
		}

		inline const_iterator begin() const
		{
			return const_iterator(m_first);
		}
		inline const_iterator end() const
		{
			return const_iterator(m_tail);
		}

		inline const_iterator cbegin() const
		{
			return begin();
		}
		inline const_iterator cend() const
		{
			return end();
		}


	public:
		inline reference front()
		{
			return (*begin());
		}
		inline reference back()
		{
			return (*(end() - 1));
		}

		inline const_reference front() const
		{
			return (*cbegin());
		}
		inline const_reference back() const
		{
			return (*(cend() - 1));
		}

		inline bool empty() const
		{
			return (begin() == end());
		}

		inline const_reference operator[](size_t i) const
		{
			return *(cbegin() + i);
		}

		inline reference operator[](size_t i)
		{
			return *(begin() + i);
		}

	private:
		pointer m_first;
		pointer m_tail;
	};
}

#endif // !_IENUMERABLE_HPP_