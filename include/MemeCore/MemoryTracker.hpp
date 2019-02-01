#ifndef _MEMORY_TRACKER_HPP_
#define _MEMORY_TRACKER_HPP_

#include <MemeCore/ISingleton.hpp>
#include <MemeCore/ISerializable.hpp>

// Singleton to keep track of dynamically allocated ITrackables
// Handles memory allocation ( malloc / free )
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

#endif // !_MEMORY_TRACKER_HPP_