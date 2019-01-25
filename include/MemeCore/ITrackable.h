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

		inline void * operator new(size_t size)
		{
			return ML_Tracker.newAllocation(size);
		}
		
		inline void * operator new[](size_t size)
		{
			return ML_Tracker.newAllocation(size);
		}
		
		inline void operator delete(void * ptr)
		{
			return ML_Tracker.freeAllocation(ptr);
		}
		
		inline void operator delete[](void * ptr)
		{
			return ML_Tracker.freeAllocation(ptr);
		}
	};
}

#endif // !_ITRACKABLE_H_