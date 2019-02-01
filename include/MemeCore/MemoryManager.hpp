#ifndef _MEMORY_MANAGER_HPP_
#define _MEMORY_MANAGER_HPP_

#include <MemeCore/ITrackable.hpp>
#include <MemeCore/ISingleton.hpp>
#include <MemeCore/Chunk.hpp>

// https://github.com/miguelperes/custom-malloc

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
		void	serializeChunk(std::ostream & out, const Chunk & c) const;

		template <typename T>
		inline T * allocate()
		{
			return static_cast<T *>(allocate(sizeof(T)));
		}

	//private:
		bool	isValidChunk(Chunk * value) const;
		void *	getBytes(size_t addr, size_t total);

		Chunk * writeChunk(size_t addr, size_t size);
		Chunk *	appendChunk(size_t size);

		Chunk *	findEmpty(size_t size) const;
		Chunk * readChunk(void * addr) const;

		Chunk * mergePrev(Chunk * value) const;
		Chunk * mergeNext(Chunk * value) const;
		Chunk *	splitChunk(Chunk * value, size_t size);

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

#endif // !_MEMORY_MANAGER_HPP_
