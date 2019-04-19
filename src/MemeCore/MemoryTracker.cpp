#include <MemeCore/MemoryTracker.hpp>
#include <MemeCore/ITrackable.hpp>
#include <MemeCore/Debug.hpp>

namespace ml
{
	/* * * * * * * * * * * * * * * * * * * * */

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

	/* * * * * * * * * * * * * * * * * * * * */
}

namespace ml
{
	/* * * * * * * * * * * * * * * * * * * * */

	MemoryTracker::MemoryTracker()
		: m_records()
		, m_guid(0)
	{
	}

	MemoryTracker::~MemoryTracker() 
	{
		if (Debug::log("Deleting Memory Tracker...") && (!m_records.empty()))
		{
			Debug::logError("Final allocations follow:");
			
			cerr << (*this);
			
			Debug::fatal();
		}
	}

	/* * * * * * * * * * * * * * * * * * * * */

	ITrackable * MemoryTracker::newAllocation(size_t size)
	{
		if (ITrackable * trackable = static_cast<ITrackable *>(std::malloc(size)))
		{
			RecordMap::iterator it;
			if ((it = m_records.find(trackable)) == m_records.end())
			{
				m_records.insert({ trackable, Record(trackable, (m_guid++), size) });
				return trackable;
			}
		}
		return NULL;
	}

	void MemoryTracker::freeAllocation(void * value)
	{
		if (value)
		{
			if (ITrackable * trackable = static_cast<ITrackable *>(value))
			{
				RecordMap::iterator it;
				if ((it = m_records.find(trackable)) != m_records.end())
				{
					m_records.erase(it);
					std::free(trackable);
					trackable = NULL;
				}
			}
		}
	}

	/* * * * * * * * * * * * * * * * * * * * */

	void MemoryTracker::serialize(std::ostream & out) const
	{
		RecordMap::const_iterator it;
		for (it = m_records.begin(); it != m_records.end(); ++it)
		{
			out << (*it->first) << (it->second) << ml::endl;
		}
		out << ml::endl;
	}

	/* * * * * * * * * * * * * * * * * * * * */
}