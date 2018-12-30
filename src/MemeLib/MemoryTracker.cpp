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
		DisplayFinalAllocations(std::cout);
	}


	void MemoryTracker::AddAllocation(ITrackable * ptr, std::size_t size)
	{
		MemoryMap::iterator it = m_records.find(ptr);

		if (it != m_records.end())
		{
			std::cerr 
				<< ("Attempted to create an existing allocation")
				<< std::endl;
		}
		else
		{
			m_records.insert({ ptr, MemoryRecord(ptr, m_guid++, size) });
		}
	}

	void MemoryTracker::RemoveAllocation(ITrackable * ptr)
	{
		MemoryMap::iterator it = m_records.find(ptr);

		if (it == m_records.end())
		{
			std::cerr 
				<< ("Could not remove allocation")
				<< std::endl;
		}
		else
		{
			m_records.erase(it);
		}
	}

	void MemoryTracker::DisplayFinalAllocations(std::ostream & out)
	{
		std::cout
			<< ("Deleting Memory Tracker...") 
			<< std::endl;

		if (m_records.size() > 0)
		{
			std::cout 
				<< ("Final allocations follow:")
				<< std::endl;

			DisplayAllAllocations(out);

			system("pause");
		}
		else
		{
			std::cout
				<< ("There are no memory allocations") 
				<< std::endl;
		}
	}

	void MemoryTracker::DisplayAllAllocations(std::ostream & out)
	{
		std::cout 
			<< ("Current memory allocations:") 
			<< std::endl;

		MemoryMap::iterator it;
		for (it = m_records.begin(); it != m_records.end(); ++it)
		{
			DisplayAllocation(out, it);
		}

		std::cout << std::endl;
	}

	void MemoryTracker::DisplayAllocation(std::ostream & out, MemoryMap::iterator it)
	{
		out << (*it->first) << (it->second) << std::endl;
	}

}