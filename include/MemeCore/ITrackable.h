#ifndef _ITRACKABLE_H_
#define _ITRACKABLE_H_

#include <MemeCore/ISerializable.h>

namespace ml
{
	class ML_CORE_API ITrackable : public ISerializable
	{
	public:
		void*	operator new(std::size_t size);
		void	operator delete(void *ptr);
		void*	operator new[](std::size_t size);
		void	operator delete[](void *ptr);
	};
}

#endif // !_ITRACKABLE_H_