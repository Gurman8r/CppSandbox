#include <MemeLib/ITrackable.h>
#include <MemeLib/MemoryTracker.h>

namespace ml
{
	void * ITrackable::operator new(std::size_t size)
	{
		return ML_MemoryTracker.newAllocation(static_cast<ITrackable*>(malloc(size)), size);
	}
	
	void ITrackable::operator delete(void *ptr)
	{
		return ML_MemoryTracker.deleteAllocation(static_cast<ITrackable*>(ptr));
	}
	
	void * ITrackable::operator new[](std::size_t size)
	{
		return ML_MemoryTracker.newAllocation(static_cast<ITrackable*>(malloc(size)), size);
	}
	
	void ITrackable::operator delete[](void *ptr)
	{
		return ML_MemoryTracker.deleteAllocation(static_cast<ITrackable*>(ptr));
	}	
}