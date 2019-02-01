#ifndef _ITRACKABLE_HPP_
#define _ITRACKABLE_HPP_

#include <MemeCore/MemoryTracker.hpp>

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

#endif // !_ITRACKABLE_HPP_
