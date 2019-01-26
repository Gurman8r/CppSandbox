#ifndef _MEMORY_TRACKER_H_
#define _MEMORY_TRACKER_H_

#include <MemeCore/ISingleton.h>
#include <MemeCore/ISerializable.h>
#include <map>

// Singleton to keep track of dynamically allocated ITrackables
// Handles memory allocation/deallocation
#define ML_Tracker ml::MemoryTracker::getInstance()

namespace ml
{
	class ITrackable;

	class ML_CORE_API MemoryTracker final
		: public ISingleton<MemoryTracker>
		, public ISerializable
	{	
		friend ISingleton<MemoryTracker>;

	public:
		ITrackable * newAllocation(size_t size);

		void freeAllocation(void * ptr);

		void serialize(std::ostream & out) const override;

	private:
		MemoryTracker();
		~MemoryTracker();

		struct Record;
		using RecordMap = std::map<ITrackable *, Record>;

		RecordMap	m_map;
		size_t		m_guid;
	};
}

#endif // !_MEMORY_TRACKER_H_
