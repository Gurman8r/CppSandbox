#include <MemeCore/MemoryTracker.hpp>
#include <MemeCore/ITrackable.hpp>
#include <MemeCore/Debug.hpp>

namespace ml
{
	struct MemoryTracker::Record
		: public ISerializable
	{
		void *	addr;
		size_t	index;
		size_t	size;

		Record(void * addr, size_t index, size_t size)
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
		Debug::log("Deleting Memory Tracker...");
		if (!m_map.empty())
		{
			Debug::logWarning("Final allocations follow:");
			
			cerr << (*this);
			
			Debug::pause(EXIT_FAILURE);
		}
	}


	ITrackable * MemoryTracker::newAllocation(size_t size)
	{
		if (ITrackable * ptr = static_cast<ITrackable *>(malloc(size)))
		{
#ifdef ML_DEBUG
			RecordMap::iterator it;
			if ((it = m_map.find(ptr)) == m_map.end())
			{
				m_map.insert({ ptr, Record(ptr, m_guid++, size) });
			}
#endif // ML_DEBUG
			return ptr;
		}
		return NULL;
	}

	void MemoryTracker::freeAllocation(void * value)
	{
		if (ITrackable * ptr = static_cast<ITrackable *>(value))
		{
#ifdef ML_DEBUG
			RecordMap::iterator it;
			if ((it = m_map.find(ptr)) != m_map.end())
			{
				m_map.erase(it);
			}
#endif // ML_DEBUG
			return free(ptr);
		}
	}

	void MemoryTracker::serialize(std::ostream & out) const
	{
		RecordMap::const_iterator it;
		for (it = m_map.begin(); it != m_map.end(); ++it)
		{
			out << (*it->first) << (it->second) << ml::endl;
		}
		out << ml::endl;
	}

}