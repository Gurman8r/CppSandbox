#ifndef _MEMORY_MANAGER_H_
#define _MEMORY_MANAGER_H_

#include <MemeCore/ISingleton.h>
#include <MemeCore/Chunk.h>

#ifndef ML_POOL_CAPACITY
#define ML_POOL_CAPACITY 65535
#endif // !ML_POOL_CAPACITY

#define ML_Memory ml::MemoryManager::getInstance()

namespace ml
{
	class ML_CORE_API MemoryManager final
		: public ITrackable
		, public ISingleton<MemoryManager>
	{
		friend class ISingleton<MemoryManager>;

	public:
		using data_type			= uint8_t;
		using pointer			= data_type * ;
		using reference			= data_type & ;
		using const_pointer		= const data_type *;
		using const_reference	= const data_type &;

		enum : size_t { Capacity = ML_POOL_CAPACITY };

	public:
		void *	allocate(size_t size);
		bool	free(void * ptr);

	private:
		bool	hasSpace(size_t size) const;
		size_t	incrementAllocation(size_t value);
		Chunk *	createChunk(size_t size);
		Chunk *	findChunk(size_t size) const;

	public:
		void serialize(std::ostream & out) const override;

	private:
		MemoryManager();
		~MemoryManager();

		data_type	m_data[Capacity];
		size_t		m_size;
		Chunk *		m_head;
		Chunk *		m_tail;

	};
}

#endif // !_MEMORY_MANAGER_H_
