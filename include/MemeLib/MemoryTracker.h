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

	class ML_API MemoryTracker final
		: public ISingleton<MemoryTracker>
	{
		friend ISingleton<MemoryTracker>;

	public:
		struct MemoryRecord final
		{
			void*		addr;
			std::size_t	index;
			std::size_t	size;

			MemoryRecord(void* addr, std::size_t index, std::size_t size)
				: addr(addr)
				, index(index)
				, size(size) 
			{}

		};

		inline friend std::ostream & operator<<(std::ostream & out, const MemoryRecord & rhs)
		{
			return (out)
				<< " { addr: " << rhs.addr
				<< " | size: " << rhs.size
				<< " | indx: " << rhs.index
				<< " }";
		}

		using MemoryMap = std::map<ITrackable*, MemoryRecord>;

	public:		
		void AddAllocation(ITrackable* ptr, std::size_t size);
		void RemoveAllocation(ITrackable* ptr);
		void DisplayFinalAllocations(std::ostream& out);
		void DisplayAllAllocations(std::ostream& out);
		void DisplayAllocation(std::ostream& out, MemoryMap::iterator it);

	private:
		MemoryTracker();
		~MemoryTracker();

		MemoryMap	m_records;
		std::size_t m_guid;
	};
}

#endif // !_MEMORY_TRACKER_H_
