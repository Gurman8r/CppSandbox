#ifndef _IENUMERATOR_H_
#define _IENUMERATOR_H_

#include <MemeCore/BaseIterator.h>

namespace ml
{
	// Supports simple iteration
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

	protected:
		pointer m_first;
		pointer m_last;

	public:
		IEnumerable(pointer first, pointer last)
			: m_first(first)
			, m_last(last)
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
			return iterator(m_last);
		}

		inline const_iterator begin() const
		{
			return const_iterator(m_first);
		}
		inline const_iterator end() const
		{
			return const_iterator(m_last);
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
		
	};
}

#endif // !_IENUMERATOR_H_
