#include <MemeCore/MemoryManager.h>
#include <MemeCore/BitHelper.h>
#include <MemeCore/DebugUtility.h>

namespace ml
{
	MemoryManager::MemoryManager()
		: m_size(0)
		, m_head(NULL)
	{
		memset(m_data, NULL, MaxBytes);
	}

	MemoryManager::~MemoryManager()
	{
	}
	
	
	void * MemoryManager::allocate(size_t size)
	{
		if (!m_head && (m_head = createNewChunk(size)))
		{
			m_tail = m_head;
			
			return m_head->npos;
		}
		else if(Chunk * eChunk = findEmptyChunk(size))
		{
			eChunk->size = size;
			
			eChunk->free = false;
			
			return eChunk->npos;
		}
		else if(Chunk * nChunk = createNewChunk(size))
		{
			m_tail->next = nChunk;
			
			nChunk->prev = m_tail;
			
			m_tail = nChunk;
			
			return m_tail->npos;
		}
		return NULL;
	}

	bool MemoryManager::free(void * value)
	{
		if (value)
		{
			Chunk * ptr = (Chunk *)((size_t)value - sizeof(Chunk));

			if (ptr >= m_head)
			{
				if ((void *)ptr <= (&m_tail->npos))
				{
					ptr->free = true;
					//ptr->mergeNext();
					//ptr->mergePrev();
					return true;
				}
			}
		}
		return false;
	}
	
	
	bool MemoryManager::hasSpace(size_t size) const
	{
		return ((m_size + (size + sizeof(Chunk))) < MaxBytes);
	}

	size_t MemoryManager::increment(size_t size)
	{
		return (m_size += (size + sizeof(Chunk)));
	}

	
	Chunk * MemoryManager::createNewChunk(size_t size)
	{
		if (hasSpace(size))
		{
			if (Chunk * chunk = (Chunk *)(&m_data[increment(size)]))
			{
				chunk->size = size + sizeof(Chunk);
				chunk->free = (!size);
				chunk->prev = NULL;
				chunk->next = NULL;
				return chunk;
			}
		}
		return NULL;
	}

	Chunk * MemoryManager::findEmptyChunk(size_t size) const
	{
		Chunk * ptr = (m_head);
		while (ptr)
		{
			if (ptr->free && (ptr->size >= (size + sizeof(Chunk))))
			{
				return ptr;
			}

			ptr = ptr->next;
		}
		return ptr;
	}
	

	void MemoryManager::serialize(std::ostream & out) const
	{
		if (Chunk * ptr = (Chunk *)m_head)
		{
			while (ptr)
			{
				out << (*ptr) << std::endl;

				ptr = ptr->next;
			}
		}
		else
		{
			out << "Empty" << std::endl;
		}
	}
}