#include <MemeLib/ITrackable.h>
#include <MemeLib/MemoryTracker.h>

namespace ml
{
	void * ITrackable::operator new(std::size_t size)
	{
		ITrackable* ptr = static_cast<ITrackable*>(malloc(size));
		ML_MemoryTracker.AddAllocation(ptr, size);
		return ptr;
	}
	
	void ITrackable::operator delete(void *ptr)
	{
		ML_MemoryTracker.RemoveAllocation(static_cast<ITrackable*>(ptr));
		free(ptr);
	}
	
	void * ITrackable::operator new[](size_t size)
	{
		ITrackable* ptr = static_cast<ITrackable*>(malloc(size));
		ML_MemoryTracker.AddAllocation(ptr, size);
		return ptr;
	}
	
	void ITrackable::operator delete[](void *ptr)
	{
		ML_MemoryTracker.RemoveAllocation(static_cast<ITrackable*>(ptr));
		free(ptr);
	}
	
}