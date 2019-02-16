#ifndef _I_TRACKABLE_HPP_
#define _I_TRACKABLE_HPP_

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
			return ML_MemoryTracker.newAllocation(size);
		}
		
		inline void * operator new[](size_t size)
		{
			return ML_MemoryTracker.newAllocation(size);
		}
		
		inline void operator delete(void * ptr)
		{
			return ML_MemoryTracker.freeAllocation(ptr);
		}
		
		inline void operator delete[](void * ptr)
		{
			return ML_MemoryTracker.freeAllocation(ptr);
		}
	};
}

#endif // !_I_TRACKABLE_HPP_
