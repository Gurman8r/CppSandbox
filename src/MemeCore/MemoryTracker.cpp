#include <MemeCore/MemoryTracker.h>
#include <MemeCore/ITrackable.h>

namespace ml
{
	ITrackable * MemoryTracker::newAllocation(ITrackable * ptr, std::size_t size)
	{
		AllocationMap::iterator it;
		if ((it = m_records.find(ptr)) == m_records.end())
		{
			m_records.insert({ ptr, Allocation(ptr, m_guid++, size) });
		}
		return ptr;
	}

	void MemoryTracker::deleteAllocation(ITrackable * ptr)
	{
		AllocationMap::iterator it;
		if ((it = m_records.find(ptr)) != m_records.end())
		{
			m_records.erase(it);
		}
		free(ptr);
	}


	void MemoryTracker::displayFinalAllocations()
	{
		std::cout << ("Deleting Memory Tracker...")  << std::endl;
		if (m_records.size() > 0)
		{
			std::cout << ("Final allocations follow:") << std::endl;
			displayAllAllocations();
#ifdef ML_SYSTEM_WINDOWS
			system("pause");
#else
			// pause ?
#endif
		}
	}

	void MemoryTracker::displayAllAllocations()
	{
		for (AllocationMap::iterator it = m_records.begin(); it != m_records.end(); ++it)
		{
			std::cout << (*it->first) << (it->second) << std::endl;
		}
		std::cout << std::endl;
	}

}