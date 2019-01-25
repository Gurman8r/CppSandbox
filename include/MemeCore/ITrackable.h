#ifndef _ITRACKABLE_H_
#define _ITRACKABLE_H_

#include <MemeCore/MemoryTracker.h>

namespace ml
{
	class ML_CORE_API ITrackable
		: public ISerializable
	{
	public:
		virtual ~ITrackable() {}

		inline void * operator new(std::size_t size)
		{
			return ML_MemoryTracker.newAllocation(static_cast<ITrackable*>(malloc(size)), size);
		}
		
		inline void * operator new[](std::size_t size)
		{
			return ML_MemoryTracker.newAllocation(static_cast<ITrackable*>(malloc(size)), size);
		}
		
		inline void operator delete(void * ptr)
		{
			return ML_MemoryTracker.deleteAllocation(static_cast<ITrackable*>(ptr));
		}
		
		inline void operator delete[](void * ptr)
		{
			return ML_MemoryTracker.deleteAllocation(static_cast<ITrackable*>(ptr));
		}
	};
}

#endif // !_ITRACKABLE_H_