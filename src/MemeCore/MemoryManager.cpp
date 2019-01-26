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
		if (!m_head && (m_head = createChunk(size)))
		{
			m_tail = m_head;

			return m_head->npos;
		}
		//else if(Chunk * empty = findChunk(size))
		//{
		//	empty->size = size;
		//	empty->free = false;
		//	return empty->npos;
		//}
		else if(Chunk * chunk = createChunk(size))
		{
			m_tail->next = chunk;

			chunk->prev = m_tail;

			m_tail = chunk;

			return m_tail->npos;
		}
		return NULL;
	}

	bool MemoryManager::free(void * ptr)
	{
		if (ptr)
		{
			if (Chunk * toFree = ((Chunk *)ptr - sizeof(Chunk)))
			{
				if ((toFree >= m_head) && (toFree < m_tail))
				{
					toFree->free = true;
					toFree->mergeNext();
					toFree->mergePrev();
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

	
	Chunk * MemoryManager::createChunk(size_t size)
	{
		if (hasSpace(size))
		{
			if (Chunk * chunk = (Chunk *)(&m_data[increment(size)]))
			{
				chunk->size = size;
				chunk->free = (!size);
				chunk->prev = NULL;
				chunk->next = NULL;
				return chunk;
			}
		}
		return NULL;
	}

	Chunk * MemoryManager::findChunk(size_t size) const
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