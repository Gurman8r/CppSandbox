#include <MemeCore/MemoryTracker.hpp>
#include <MemeCore/ITrackable.hpp>
#include <MemeCore/Debug.hpp>

namespace ml
{
	struct MemoryTracker::Record final
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
		: m_records()
		, m_guid(0)
	{
	}

	MemoryTracker::~MemoryTracker() 
	{
		Debug::log("Deleting Memory Tracker...");
		if (!m_records.empty())
		{
			Debug::logError("Final allocations follow:");
			
			cerr << (*this);
			
#ifdef ML_DEBUG
			Debug::pause(EXIT_FAILURE);
			Debug::terminate();
#endif // ML_DEBUG
		}
	}


	ITrackable * MemoryTracker::newAllocation(size_t size)
	{
		if (ITrackable * ptr = static_cast<ITrackable *>(malloc(size)))
		{
			RecordMap::iterator it;
			if ((it = m_records.find(ptr)) == m_records.end())
			{
				m_records.insert({ ptr, Record(ptr, m_guid++, size) });
			}
			return ptr;
		}
		return NULL;
	}

	void MemoryTracker::freeAllocation(void * value)
	{
		if (ITrackable * ptr = static_cast<ITrackable *>(value))
		{
			RecordMap::iterator it;
			if ((it = m_records.find(ptr)) != m_records.end())
			{
				m_records.erase(it);
			}
			free(ptr);
			ptr = NULL;
		}
	}

	void MemoryTracker::serialize(std::ostream & out) const
	{
		RecordMap::const_iterator it;
		for (it = m_records.begin(); it != m_records.end(); ++it)
		{
			out << (*it->first) << (it->second) << ml::endl;
		}
		out << ml::endl;
	}

}