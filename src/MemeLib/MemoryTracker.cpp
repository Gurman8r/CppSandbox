#include <MemeLib/MemoryTracker.h>
#include <MemeLib/ITrackable.h>
#include <iostream>

namespace ml
{
	MemoryTracker::MemoryTracker()
	{
	}

	MemoryTracker::~MemoryTracker()
	{
		displayFinalAllocations(std::cout);
	}


	ITrackable * MemoryTracker::newAllocation(ITrackable * ptr, std::size_t size)
	{
		MemoryMap::iterator it;
		if ((it = m_records.find(ptr)) != m_records.end())
		{
			std::cerr << ("Attempted to create an existing allocation") << std::endl;
		}
		else
		{
			m_records.insert({ ptr, MemoryRecord(ptr, m_guid++, size) });
		}
		return ptr;
	}

	void MemoryTracker::deleteAllocation(ITrackable * ptr)
	{
		MemoryMap::iterator it;

		if ((it = m_records.find(ptr)) == m_records.end())
		{
			std::cerr << ("Could not remove allocation")<< std::endl;
		}
		else
		{
			m_records.erase(it);
			free(ptr);
		}
	}


	void MemoryTracker::displayFinalAllocations(std::ostream & out)
	{
		std::cout << ("Deleting Memory Tracker...")  << std::endl;

		if (m_records.size() > 0)
		{
			std::cout << ("Final allocations follow:") << std::endl;

			displayAllAllocations(out);

#ifdef ML_SYSTEM_WINDOWS
			system("pause");
#endif
		}
		else
		{
			std::cout << ("There are no memory allocations") << std::endl;
		}
	}

	void MemoryTracker::displayAllAllocations(std::ostream & out)
	{
		std::cout << ("Current memory allocations:") << std::endl;

		for (MemoryMap::iterator it = m_records.begin(); it != m_records.end(); ++it)
		{
			displayAllocation(out, it);
		}

		std::cout << std::endl;
	}

	void MemoryTracker::displayAllocation(std::ostream & out, MemoryMap::iterator it)
	{
		out << (*it->first) << (it->second) << std::endl;
	}

}