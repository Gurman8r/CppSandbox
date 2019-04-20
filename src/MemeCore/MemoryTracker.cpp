#include <MemeCore/MemoryTracker.hpp>
#include <MemeCore/ITrackable.hpp>
#include <MemeCore/Debug.hpp>

namespace ml
{
	/* * * * * * * * * * * * * * * * * * * * */

	MemoryTracker::Record::Record(void * addr, const size_t indx, const size_t size)
		: addr	(addr)
		, indx	(indx)
		, size	(size)
	{
	}

	MemoryTracker::Record::Record(const Record & copy)
		: addr	(copy.addr)
		, indx	(copy.indx)
		, size	(copy.size)
	{
	}

	void MemoryTracker::Record::serialize(std::ostream & out) const
	{
		out << " { addr: " << addr
			<< " | size: " << size
			<< " | indx: " << indx
			<< " }";
	}

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

			Debug::pause(EXIT_FAILURE);

			Debug::fatal();
		}
	}

	/* * * * * * * * * * * * * * * * * * * * */

	ITrackable * MemoryTracker::newAllocation(const size_t size)
	{
		if (ITrackable * trackable = static_cast<ITrackable *>(std::malloc(size)))
		{
			iterator it;
			if ((it = m_records.find(trackable)) == m_records.end())
			{
				return m_records.insert({
					trackable, { trackable, (m_guid++), size }
				}).first->first;
			}
		}
		return NULL;
	}

	void MemoryTracker::freeAllocation(void * value)
	{
		if (ITrackable * trackable = static_cast<ITrackable *>(value))
		{
			iterator it;
			if ((it = m_records.find(trackable)) != m_records.end())
			{
				m_records.erase(it);
				std::free(trackable);
				trackable = NULL;
			}
		}
	}

	/* * * * * * * * * * * * * * * * * * * * */

	void MemoryTracker::serialize(std::ostream & out) const
	{
		const_iterator it;
		for (it = m_records.begin(); it != m_records.end(); ++it)
		{
			out << (*it->first) << (it->second) << ml::endl;
		}
		out << ml::endl;
	}

	/* * * * * * * * * * * * * * * * * * * * */
}