#ifndef _ML_MEMORY_TRACKER_HPP_
#define _ML_MEMORY_TRACKER_HPP_

#include <MemeCore/Map.hpp>
#include <MemeCore/ISingleton.hpp>
#include <MemeCore/ISerializable.hpp>

/* * * * * * * * * * * * * * * * * * * * */

#define ML_MemoryTracker ml::MemoryTracker::getInstance()

/* * * * * * * * * * * * * * * * * * * * */

#define ML_new(size) ML_MemoryTracker.newAllocation(size)
#define ML_free(ptr) ML_MemoryTracker.freeAllocation(ptr)

/* * * * * * * * * * * * * * * * * * * * */

namespace ml
{
	/* * * * * * * * * * * * * * * * * * * * */

	class ITrackable;

	/* * * * * * * * * * * * * * * * * * * * */

	class ML_CORE_API MemoryTracker final
		: public ISerializable
		, public ISingleton<MemoryTracker>
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

		using RecordMap = typename HashMap<ITrackable *, Record>;

		RecordMap	m_records;
		size_t		m_guid;
	};

	/* * * * * * * * * * * * * * * * * * * * */
}

#endif // !_ML_MEMORY_TRACKER_HPP_