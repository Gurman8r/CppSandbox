#ifndef _MEMORY_MANAGER_H_
#define _MEMORY_MANAGER_H_

#include <MemeCore/ITrackable.h>
#include <MemeCore/ISingleton.h>
#include <MemeCore/Chunk.h>

#ifndef ML_MAX_BYTES
#define ML_MAX_BYTES 4096
#endif

#define ML_Memory ml::MemoryManager::getInstance()

// https://github.com/miguelperes/custom-malloc

namespace ml
{
	class ML_CORE_API MemoryManager final
		: public ITrackable
		, public ISingleton<MemoryManager>
	{
		friend class ISingleton<MemoryManager>;

	public:
		void *	allocate(size_t size);
		bool	free(void * value);
		bool	prime(byte * data, size_t size);

	private:
		Chunk *	createChunk(size_t size);
		Chunk * findChunkByValue(void * value) const;
		Chunk *	findAvailableChunk(size_t size) const;

		bool	mergeChunkPrev(Chunk * value);
		bool	mergeChunkNext(Chunk * value);
		bool	splitChunk(Chunk * value, size_t size);

	public:
		void serialize(std::ostream & out) const override;

	private:
		MemoryManager() {}
		~MemoryManager() {}

		byte *	m_data; // Pointer to byte array
		size_t	m_size; // Total bytes available
		size_t	m_used; // Number of bytes used
		Chunk *	m_head; // First chunk in list
		Chunk *	m_tail; // Last chunk in list
	};
}

#endif // !_MEMORY_MANAGER_H_
