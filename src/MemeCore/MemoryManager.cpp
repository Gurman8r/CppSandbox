#include <MemeCore/MemoryManager.h>
#include <MemeCore/BitHelper.h>
#include <MemeCore/DebugUtility.h>
#include <MemeCore/ML_Memory.h>

namespace ml
{
	MemoryManager::MemoryManager()
		: m_size(0)
		, m_head(NULL)
	{
		memset(m_data, NULL, MaxBytes);

		ml_prime(m_data, MaxBytes);
	}

	MemoryManager::~MemoryManager()
	{
	}
	
	
	void *	MemoryManager::allocate(size_t size)
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

	bool	MemoryManager::free(void * value)
	{
		if (value)
		{
			Chunk * ptr = (Chunk *)((size_t)value - sizeof(Chunk));

			if (ptr >= m_head)
			{
				if ((void *)ptr <= (&m_tail->npos))
				{
					ptr->free = true;
					
					mergeChunkNext(ptr);
					
					mergeChunkPrev(ptr);
					
					return true;
				}
			}
		}
		return false;
	}
	
	
	bool	MemoryManager::hasSpace(size_t size) const
	{
		return ((m_size + (size + sizeof(Chunk))) < MaxBytes);
	}

	size_t	MemoryManager::incrementAllocation(size_t size)
	{
		return (m_size += (size + sizeof(Chunk)));
	}

	
	Chunk * MemoryManager::createNewChunk(size_t size)
	{
		if (hasSpace(size))
		{
			if (Chunk * chunk = (Chunk *)(&m_data[incrementAllocation(size)]))
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
	

	bool	MemoryManager::mergeChunkPrev(Chunk * value)
	{
		if (value->prev && (value->prev)->free)
		{
			(value->prev)->size += value->size + sizeof(Chunk);

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
		if (value->next && (value->next)->free)
		{
			value->size += (value->next)->size + sizeof(Chunk);

			value->next = (value->next)->next;

			if ((value->next)->next)
			{
				((value->next)->next)->prev = value;

				return true;
			}
		}
		return false;
	}


	void	MemoryManager::serialize(std::ostream & out) const
	{
		out << "Memory: " 
			<< "( " << m_size << " / " << MaxBytes << " ) " 
			<< "( " << ((double)m_size / (double)MaxBytes) << " % )"
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