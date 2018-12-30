#ifndef _IENUMERATOR_H_
#define _IENUMERATOR_H_

#include <MemeLib/BaseIterator.h>

namespace ml
{
	// Supports simple iteration
	template <typename T>
	class IEnumerator
	{
	public:
		using iterator		= BaseIterator<std::forward_iterator_tag, T>;
		using const_iterator= BaseIterator<std::forward_iterator_tag, const T>;

	protected:
		inline virtual T * _MyBegin() = 0;
		inline virtual T * _MyEnd() = 0;

	public:
		inline iterator			begin() { return iterator(_MyBegin()); }
		inline iterator			end()	{ return iterator(_MyEnd()); }		
		inline const_iterator	cbegin(){ return begin(); }
		inline const_iterator	cend()	{ return end(); }

		inline const T &	front() const { return (*begin()); }
		inline const T &	back() const { return (*end()); }
		inline const bool	empty() const { return begin() == end(); }
	};
}

#endif // !_IENUMERATOR_H_
