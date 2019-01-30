#include <MemeCore/MemoryTracker.h>
#include <MemeCore/ITrackable.h>
#include <MemeCore/Debug.h>

namespace ml
{
	struct MemoryTracker::Record
		: public ISerializable
	{
		void *	addr;
		size_t	index;
		size_t	size;

		Record(void* addr, size_t index, size_t size)
			: addr(addr)
			, index(index)
			, size(size)
		{
		}

		void serialize(std::ostream & out) const override
		{
			out << " { addr: " << addr
				<< " | size: " << size
				<< " | indx: " << index
				<< " }";
		}
	};
}

namespace ml
{
	MemoryTracker::MemoryTracker()
		: m_map()
		, m_guid(0)
	{
	}

	MemoryTracker::~MemoryTracker() 
	{
		Debug::Log("Deleting Memory Tracker...");
		if (!m_map.empty())
		{
			Debug::LogWarning("Final allocations follow:");
			
			cerr << (*this);
			
			Debug::pause(EXIT_FAILURE);
		}
	}


	ITrackable * MemoryTracker::newAllocation(size_t size)
	{
		if (ITrackable * ptr = static_cast<ITrackable*>(malloc(size)))
		{
			RecordMap::iterator it;
			if ((it = m_map.find(ptr)) == m_map.end())
			{
				m_map.insert({ ptr, Record(ptr, m_guid++, size) });
			}
			return ptr;
		}
		return NULL;
	}

	void MemoryTracker::freeAllocation(void * value)
	{
		if (ITrackable * ptr = static_cast<ITrackable*>(value))
		{
			RecordMap::iterator it;
			if ((it = m_map.find(ptr)) != m_map.end())
			{
				m_map.erase(it);
			}
			return free(ptr);
		}
	}

	void MemoryTracker::serialize(std::ostream & out) const
	{
		RecordMap::const_iterator it;
		for (it = m_map.begin(); it != m_map.end(); ++it)
		{
			out << (*it->first) << (it->second) << std::endl;
		}
		out << std::endl;
	}

}