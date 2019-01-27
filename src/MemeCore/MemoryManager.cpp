#include <MemeCore/MemoryManager.h>
#include <MemeCore/DebugUtility.h>

namespace ml
{
	void *	MemoryManager::allocate(size_t size)
	{
		if (!m_head && (m_head = createChunk(size)))
		{
			m_tail = m_head;
			
			return m_head->npos;
		}
		else if(Chunk * eChunk = findAvailableChunk(size))
		{
			eChunk->size = size;
			
			eChunk->free = false;
			
			return eChunk->npos;
		}
		else if(Chunk * nChunk = createChunk(size))
		{
			m_tail->next = nChunk;
			
			nChunk->prev = m_tail;
			
			m_tail = nChunk;
			
			return m_tail->npos;
		}
		return NULL;
	}

	bool	MemoryManager::free(void * value)
	{
		if (Chunk * chunk = findChunkByValue(value))
		{
			if (chunk >= m_head)
			{
				if (((void *)chunk) <= (&m_tail->npos))
				{
					chunk->free = true;

					mergeChunkNext(chunk);

					mergeChunkPrev(chunk);

					return true;
				}
			}
		}
		return false;
	}

	bool	MemoryManager::prime(byte * data, size_t size)
	{
		if (!m_data && (data && size))
		{
			m_data = data;
			m_size = size;
			m_used = 0;
			m_head = NULL;
			m_tail = NULL;
			
			return true;
		}
		return false;
	}

	
	Chunk * MemoryManager::createChunk(size_t size)
	{
		if (size)
		{
			const size_t need = (size + sizeof(Chunk));

			if ((m_used + need) < m_size)
			{
				if (Chunk * chunk = (Chunk *)(&m_data[(m_used += need)]))
				{
					chunk->size = need;
					chunk->free = false;
					chunk->prev = NULL;
					chunk->next = NULL;
					return chunk;
				}
			}
		}
		return NULL;
	}

	Chunk * MemoryManager::findChunkByValue(void * value) const
	{
		if (value)
		{
			return (Chunk *)((size_t)value - sizeof(Chunk) + sizeof(uint8_t *));
		}
		return NULL;
	}

	Chunk * MemoryManager::findAvailableChunk(size_t size) const
	{
		if (size)
		{
			Chunk * ptr = m_head;
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
		return NULL;
	}
	

	bool	MemoryManager::mergeChunkPrev(Chunk * value)
	{
		if (value && (value->prev && (value->prev)->free))
		{
			(value->prev)->size += value->size;

			(value->prev)->next = value->next;

			if (value->next)
			{
				(value->next)->prev = value->prev;

				return true;
			}
		}
		return false;
	}

	bool	MemoryManager::mergeChunkNext(Chunk * value)
	{
		if (value && (value->next && (value->next)->free))
		{
			value->size += (value->next)->size;

			value->next = (value->next)->next;

			if ((value->next)->next)
			{
				((value->next)->next)->prev = value;

				return true;
			}
		}
		return false;
	}

	bool	MemoryManager::splitChunk(Chunk * value, size_t size)
	{
		if (value && size)
		{

		}
		return false;
	}

	
	void	MemoryManager::serialize(std::ostream & out) const
	{
		out << "Memory Usage: " 
			<< "( " << m_used << " / " << m_size << " ) " 
			<< "( " << ((double)m_used / (double)m_size) << "% )"
			<< std::endl;

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