#ifndef _MEMORY_MANAGER_H_
#define _MEMORY_MANAGER_H_

#include <MemeCore/ITrackable.h>
#include <MemeCore/ISingleton.h>
#include <MemeCore/Chunk.h>

#ifndef ML_MEMORY_MAX_BYTES
#define ML_MEMORY_MAX_BYTES 65535
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
		enum : size_t { MaxBytes = ML_MEMORY_MAX_BYTES };

	public:
		void *	allocate(size_t size);
		bool	free(void * value);

	private:
		bool	hasSpace(size_t size) const;
		size_t	increment(size_t value);

		Chunk *	createNewChunk(size_t size);
		Chunk *	findEmptyChunk(size_t size) const;

		bool	mergeChunkPrev(Chunk * value);
		bool	mergeChunkNext(Chunk * value);

	public:
		void serialize(std::ostream & out) const override;

	private:
		MemoryManager();
		~MemoryManager();

		uint8_t	m_data[MaxBytes];
		size_t	m_size;
		Chunk *	m_head;
		Chunk *	m_tail;
	};
}

#endif // !_MEMORY_MANAGER_H_
