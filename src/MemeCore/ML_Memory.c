/* * * * * * * * * * * * * * * * * * * * */

#include <MemeCore/ML_Memory.h>
#include <stdio.h>

/* * * * * * * * * * * * * * * * * * * * */

#ifndef ML_CPP
	#define true	((bool)(1))
	#define false	((bool)(0))
#endif

/* * * * * * * * * * * * * * * * * * * * */

// Size of struct Block
#define BLOCK_SIZE (size_t)(sizeof(Block)) // 20

// Size of data pointer
#define NPOS_SIZE (size_t)(sizeof(byte *)) // 4

// Not yet implemented
#define NYI false

/* * * * * * * * * * * * * * * * * * * * */

byte *			m_data;	// Pointer to byte array
size_t			m_size;	// Total bytes available
size_t			m_used;	// Number of bytes used
struct Block *	m_head;	// First block in list
struct Block *	m_tail;	// Last block in list

/* * * * * * * * * * * * * * * * * * * * */

inline static bool good()
{
	return m_data && m_size;
}

/* * * * * * * * * * * * * * * * * * * * */

ML_CORE_API void * ml_allocate(size_t size)
{
	if (!m_head && (m_head = ml_appendBlock(size)))
	{
		m_tail = m_head;

		return m_head->npos;
	}

	Block * block;
	if (block = ml_findBlock(size))
	{
		block->size = size;

		block->free = false;

		if (block->size > size)
		{
			ml_splitBlock(block, size);
		}

		return block->npos;
	}
	else if (block = ml_appendBlock(size))
	{
		m_tail->next = block;

		block->prev = m_tail;

		m_tail = block;

		return m_tail->npos;
	}
	return NULL;
}

ML_CORE_API bool ml_free(void * value)
{
	Block * block;
	if (block = ml_readBlock(value))
	{
		block->free = true;

		ml_mergeBlockNext(block);

		ml_mergeBlockPrev(block);

		return true;
	}
	return false;
}

ML_CORE_API bool ml_prime(byte * data, size_t size)
{
	if (!good() && (data && size))
	{
		m_data = data;
		m_size = size;
	}
	return good();
}


ML_CORE_API void ml_displayMemory()
{
	printf("Bytes Used: ( %u / %u ) ( %f %% )\n",
		m_used, 
		m_size,
		(((double)m_used / m_size) * 100.0));

	Block * block;
	if (block = (m_head))
	{
		while (ml_displayBlock(block))
		{
			block = block->next;
		}
	}

	printf("\n");
}

ML_CORE_API bool ml_displayBlock(Block * value)
{
	if (value)
	{
		printf("[ struct Block ] ");
		printf("{ size: %d | %s | addr: %p | prev: %p | next: %p | npos: %p | indx: %d }\n",
			value->size,
			(value->free ? "free" : "used"),
			(void *)value,
			(void *)value->prev,
			(void *)value->next,
			(void *)value->npos,
			(int)value->npos[0]);

		return true;
	}
	return false;
}

/* * * * * * * * * * * * * * * * * * * * */

ML_CORE_API bool ml_isValidBlock(Block * value)
{
	return good()
		&& (value)
		&& (value >= m_head)
		&& (((void *)value) <= (void *)(&m_tail->npos));
}


ML_CORE_API Block * ml_writeBlock(size_t addr, size_t size)
{
	if (good() && size)
	{
		const size_t need = (size + BLOCK_SIZE);

		if ((addr + need) < m_size)
		{
			Block * block;
			if (block = (Block *)(&m_data[(addr + need)]))
			{
				block->size = need;
				block->free = false;
				block->prev = NULL;
				block->next = NULL;

				if (block->size > size)
				{
					ml_splitBlock(block, size);
				}

				return block;
			}
		}
	}
	return NULL;
}

ML_CORE_API Block * ml_appendBlock(size_t size)
{
	Block * block;
	if (block = ml_writeBlock(m_used, size))
	{
		m_used += (size + BLOCK_SIZE);

		return block;
	}
	return NULL;
}


ML_CORE_API Block * ml_findBlock(size_t size)
{
	if (good() && size)
	{
		Block * block = m_head;
		while (block)
		{
			if (block->free && (block->size >= (size + BLOCK_SIZE)))
			{
				return block;
			}
			block = block->next;
		}
		return block;
	}
	return NULL;
}

ML_CORE_API Block * ml_readBlock(void * addr)
{
	if (good() && addr)
	{
		Block * block;
		if (block = (Block *)(((size_t)addr - BLOCK_SIZE) + NPOS_SIZE))
		{
			if (ml_isValidBlock(block))
			{
				return block;
			}
		}
	}
	return NULL;
}


ML_CORE_API Block * ml_mergeBlockPrev(Block * value)
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

ML_CORE_API Block * ml_mergeBlockNext(Block * value)
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

ML_CORE_API Block * ml_splitBlock(Block * value, size_t size)
{
	if (NYI && good() && (value && size))
	{
		Block * block;
		if (block = ml_readBlock(value + size))
		{
			block->size = value->size - size;
			block->free = true;
			block->next = value->next;
			block->prev = value;

			if (block->next)
			{
				(block->next)->prev = block;
			}

			value->size = size;
			value->free = false;
			value->next = block;

			return block;
		}
	}
	return NULL;
}

/* * * * * * * * * * * * * * * * * * * * */