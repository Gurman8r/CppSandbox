#ifndef _MEMORY_MANAGER_H_
#define _MEMORY_MANAGER_H_

#include <MemeCore/ITrackable.h>
#include <MemeCore/ISingleton.h>
#include <MemeCore/Chunk.h>

// https://github.com/miguelperes/custom-malloc

#define ML_CHUNK_SIZE	sizeof(ml::Chunk)	// size of struct Chunk
#define ML_NPOS_SIZE	sizeof(ml::byte *)	// size of data pointer

#define ML_Memory ml::MemoryManager::getInstance()

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
		void	serialize(std::ostream & out) const override;

	private:
		Chunk * writeChunk(size_t addr, size_t size);
		Chunk *	insertChunk(size_t size);

		Chunk *	findEmpty(size_t size) const;
		Chunk * readChunk(void * addr) const;

		bool	isValidChunk(Chunk * value) const;
		Chunk *	splitChunk(Chunk * value, size_t size);

		Chunk * mergePrev(Chunk * value) const;
		Chunk * mergeNext(Chunk * value) const;

	public:
		inline bool		good() const { return (m_data && m_size); }
		inline operator bool() const { return good(); }

	private:
		MemoryManager()
			: m_data(NULL)
			, m_size(NULL)
			, m_used(NULL)
			, m_head(NULL)
			, m_tail(NULL)
		{
		}
		~MemoryManager() {}

		byte *	m_data; // Pointer to byte array
		size_t	m_size; // Total bytes available
		size_t	m_used; // Number of bytes used
		Chunk *	m_head; // First chunk in list
		Chunk *	m_tail; // Last chunk in list
	};

}

#endif // !_MEMORY_MANAGER_H_
