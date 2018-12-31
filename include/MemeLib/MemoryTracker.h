#ifndef _MEMORY_TRACKER_H_
#define _MEMORY_TRACKER_H_

#include <MemeLib/Export.h>
#include <MemeLib/ISingleton.h>
#include <map>
#include <iostream>

#define ML_MemoryTracker ml::MemoryTracker::getInstance()

namespace ml
{
	class ITrackable;

	class ML_CORE_API MemoryTracker final
		: public ISingleton<MemoryTracker>
	{
		friend ISingleton<MemoryTracker>;

	public:
		struct Allocation final
		{
			void*		addr;
			std::size_t	index;
			std::size_t	size;

			Allocation(void* addr, std::size_t index, std::size_t size)
				: addr(addr)
				, index(index)
				, size(size) 
			{}

		};

		inline friend std::ostream & operator<<(std::ostream & out, const Allocation & rhs)
		{
			return (out)
				<< " { addr: " << rhs.addr
				<< " | size: " << rhs.size
				<< " | indx: " << rhs.index
				<< " }";
		}

		using AllocationMap = std::map<ITrackable*, Allocation>;

	public:		
		ITrackable* newAllocation(ITrackable* ptr, std::size_t size);
		void		deleteAllocation(ITrackable* ptr);

		void displayFinalAllocations();
		void displayAllAllocations();

	private:
		MemoryTracker() {}
		~MemoryTracker() { displayFinalAllocations(); }

		AllocationMap	m_records;
		std::size_t m_guid;
	};
}

#endif // !_MEMORY_TRACKER_H_
