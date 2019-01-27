#include <MemeCore/MemoryManager.h>
#include <MemeCore/DebugUtility.h>

#define ML_CHUNK_SIZE	sizeof(ml::Chunk)	// Size of Chunk
#define ML_ENDPTR_SIZE	sizeof(ml::byte *)	// Size of data pointer

namespace ml
{
	void *	MemoryManager::allocate(size_t size)
	{
		if (!m_head && (m_head = appendChunk(size))) // create head
		{
			m_tail = m_head;

			return m_head->data;
		}
		else if (Chunk * e = findEmpty(size)) // return free empty chunk
		{
			e->size = size;

			e->free = false;

			if (e->size > size)
			{
				splitChunk(e, size);
			}

			return e->data;
		}
		else if (Chunk * n = appendChunk(size)) // push back new chunk
		{
			m_tail->next = n;

			n->prev = m_tail;

			m_tail = n;

			return m_tail->data;
		}
		return NULL;
	}

	bool	MemoryManager::free(void * value)
	{
		if (Chunk * chunk = readChunk(value))
		{
			chunk->free = true;

			mergeNext(chunk);

			mergePrev(chunk);

			return true;
		}
		return false;
	}

	bool	MemoryManager::prime(byte * data, size_t size)
	{
		if (!good() && (data && size))
		{
			m_data = data;
			m_size = size;
		}
		return good();
	}
	
	void	MemoryManager::serialize(std::ostream & out) const
	{
		out << "Bytes Used: "
			<< "( " << m_used << " / " << m_size << " ) "
			<< "( " << (((double)m_used / m_size) * 100.0) << " % )"
			<< std::endl;

		if (Chunk * chunk = m_head)
		{
			while (chunk)
			{
				serializeChunk(out, (*chunk));

				chunk = chunk->next;
			}
		}
		else
		{
			out << "Empty" << std::endl;
		}
	}

	void	MemoryManager::serializeChunk(std::ostream & out, const Chunk & c) const
	{
		const std::type_info & info(typeid(c));
		out << FG::White << "[ " << FG::Gray << info.name() << FG::White << " ]"
			<< FG::White << " { " << FG::Green << "size: " << FG::Yellow << std::setw(4) << (c.size)
			<< FG::White << " | " << (c.free ? FG::Cyan : FG::Red) << (c.free ? "free" : "used")
			<< FG::White << " | " << FG::Green << "addr: " << FG::Yellow << (&c)
			<< FG::White << " | " << FG::Green << "prev: " << FG::Yellow << (c.prev)
			<< FG::White << " | " << FG::Green << "next: " << FG::Yellow << (c.next)
			<< FG::White << " | " << FG::Green << "npos: " << FG::Yellow << (&c.data)
			<< FG::White << " | " << FG::Green << "indx: " << FG::Yellow << ((int)c.data[0])
			<< FG::White << " }"
			<< FMT()
			<< std::endl;
	}

	
	bool	MemoryManager::isValidChunk(Chunk * value) const
	{
		return good()
			&& (value) 
			&& (value >= m_head)
			&& (((void *)value) <= (&m_tail->data));
	}
	

	Chunk * MemoryManager::writeChunk(size_t addr, size_t size)
	{
		if (good() && size)
		{
			const size_t need = (size + ML_CHUNK_SIZE + ML_ENDPTR_SIZE);

			if ((addr + need) < m_size)
			{
				if (Chunk * chunk = (Chunk *)(&m_data[(addr + need)]))
				{
					chunk->size = need;
					chunk->free = false;
					chunk->prev = NULL;
					chunk->next = NULL;

					if (chunk->size > size)
					{
						splitChunk(chunk, size);
					}

					return chunk;
				}
			}
		}
		return NULL;
	}

	Chunk * MemoryManager::appendChunk(size_t size)
	{
		if (Chunk * chunk = writeChunk(m_used, size))
		{
			m_used += (size + (ML_CHUNK_SIZE + ML_ENDPTR_SIZE));

			return chunk;
		}
		return NULL;
	}


	Chunk * MemoryManager::findEmpty(size_t size) const
	{
		if (good() && size)
		{
			Chunk * chunk = m_head;
			while (chunk)
			{
				if (chunk->free && (chunk->size >= (size + ML_CHUNK_SIZE)))
				{
					return chunk;
				}
				chunk = chunk->next;
			}
			return chunk;
		}
		return NULL;
	}

	Chunk * MemoryManager::readChunk(void * addr) const
	{
		if (good() && addr)
		{
			if (Chunk * chunk = (Chunk *)((((size_t)addr) - ML_CHUNK_SIZE) + ML_ENDPTR_SIZE))
			{
				if (isValidChunk(chunk))
				{
					return chunk;
				}
			}
		}
		return NULL;
	}

	
	Chunk *	MemoryManager::mergePrev(Chunk * value) const
	{
		if (good())
		{
			if (value && (value->prev && (value->prev)->free))
			{
				(value->prev)->size += value->size;

				(value->prev)->next = value->next;

				if (value->next)
				{
					(value->next)->prev = value->prev;
				}
				
				return value;
			}
		}
		return NULL;
	}

	Chunk *	MemoryManager::mergeNext(Chunk * value) const
	{
		if (good())
		{
			if (value && (value->next && (value->next)->free))
			{
				value->size += (value->next)->size;

				value->next = (value->next)->next;

				if ((value->next)->next)
				{
					((value->next)->next)->prev = value;
				}

				return value;
			}
		}
		return NULL;
	}

	Chunk *	MemoryManager::splitChunk(Chunk * value, size_t size)
	{
		if (0 && good() && (value && size))
		{
			if (Chunk * chunk = readChunk(value->data))
			{
				chunk->size = value->size - size;
				chunk->free = true;
				chunk->next = value->next;
				chunk->prev = value;

				if (chunk->next)
				{
					(chunk->next)->prev = chunk;
				}

				value->size = size;
				value->free = false;
				value->next = chunk;

				Debug::Log("Split: Success");

				return chunk;
			}
		}
		return NULL;
	}

}