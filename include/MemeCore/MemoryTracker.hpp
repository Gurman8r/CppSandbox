#ifndef _ML_MEMORY_TRACKER_HPP_
#define _ML_MEMORY_TRACKER_HPP_

#include <MemeCore/Map.hpp>
#include <MemeCore/ISingleton.hpp>
#include <MemeCore/ISerializable.hpp>

// Singleton to keep track of dynamically allocated ITrackables
// Handles memory allocation ( malloc / free )
#define ML_MemoryTracker ml::MemoryTracker::getInstance()

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

		using RecordMap = HashMap<ITrackable *, Record>;

		RecordMap	m_records;
		size_t		m_guid;
	};
}

#endif // !_ML_MEMORY_TRACKER_HPP_