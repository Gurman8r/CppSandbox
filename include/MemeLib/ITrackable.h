#ifndef _ITRACKABLE_H_
#define _ITRACKABLE_H_

#include <MemeLib/ISerializable.h>

namespace ml
{
	class ML_API ITrackable : public ISerializable
	{
	public:
		void*	operator new(std::size_t size);
		void	operator delete(void *ptr);
		void*	operator new[](std::size_t size);
		void	operator delete[](void *ptr);
	};
}

#endif // !_ITRACKABLE_H_