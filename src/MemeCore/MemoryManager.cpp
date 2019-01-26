#include <MemeCore/MemoryManager.h>
#include <MemeCore/BitHelper.h>
#include <MemeCore/DebugUtility.h>

namespace ml
{
	MemoryManager::MemoryManager()
		: m_size(0)
		, m_head(NULL)
		, m_tail(NULL)
	{
		memset(m_data, NULL, Capacity);
	}

	MemoryManager::~MemoryManager()
	{
	}
	
	
	void *	MemoryManager::allocate(size_t size)
	{
		if (!m_head && (m_head = createChunk(size)))
		{
			return m_head->npos;
		}
		else if(Chunk * empty = findChunk(size))
		{
			empty->size = size;
			empty->free = false;
			return empty->npos;
		}
		else if(Chunk * alloc = createChunk(size))
		{
			return alloc->npos;
		}
		return NULL;
	}

	bool	MemoryManager::free(void * ptr)
	{
		if (ptr)
		{
			if (Chunk * toFree = ((Chunk *)ptr - sizeof(Chunk)))
			{
				if (toFree >= m_head)
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
	
	
	bool	MemoryManager::hasSpace(size_t size) const
	{
		return ((m_size + (size + sizeof(Chunk))) < Capacity);
	}

	size_t	MemoryManager::incrementAllocation(size_t size)
	{
		return (m_size += (size + sizeof(Chunk)));
	}

	
	Chunk * MemoryManager::createChunk(size_t size)
	{
		if (hasSpace(size))
		{
			if (Chunk * chunk = (Chunk *)(&m_data[incrementAllocation(size)]))
			{
				chunk->size = size;
				chunk->free = false;
				chunk->next = NULL;
				chunk->prev = NULL;
				return (m_tail = chunk);
			}
		}
		return nullptr;
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
				out << ptr << std::endl;

				ptr = ptr->next;
			}
		}
		else
		{
			out << "Empty" << std::endl;
		}
	}
}