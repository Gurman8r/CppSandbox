/* * * * * * * * * * * * * * * * * * * * */

#include <MemeCore/ML_Memory.h>

/* * * * * * * * * * * * * * * * * * * * */

#ifndef ML_CPP
	#define true	((bool)(1))
	#define false	((bool)(0))
#endif

/* * * * * * * * * * * * * * * * * * * * */

// Size of struct Block
#define BLOCK_SIZE ((size_t)(sizeof(Block)))

// Space needed for a Block plus the given size
#define SPACE_NEED(size) ((size_t)(size + BLOCK_SIZE))

// Returns the end position of the given Block
#define BLOCK_NPOS(block) ((size_t)((size_t)(block)) + BLOCK_SIZE)

// Returns the address of the given block's data
#define BLOCK_DATA(block) ((void *)(BLOCK_NPOS(block)))

// Returns the Block at the given address
#define FIND_BLOCK(addr) ((Block *)((size_t)(addr)) - BLOCK_SIZE)

/* * * * * * * * * * * * * * * * * * * * */

byte *			m_data;		// Pointer to byte array
size_t			m_size;		// Total bytes available
size_t			m_used;		// Number of bytes used
struct Block *	m_head;		// First block in list
struct Block *	m_tail;		// Last block in list

/* * * * * * * * * * * * * * * * * * * * */

void *	ml_allocate(size_t size)
{
	if (m_head)
	{
		Block * block = NULL;

		if (block = ml_findEmptyBlock(size))
		{
			block->size = size;
			
			block->free = false;
			
			return BLOCK_DATA(block);
		}
		else if (block = ml_createBlock(size))
		{
			m_tail->next = block;
			
			block->prev = m_tail;
			
			m_tail = block;
			
			return BLOCK_DATA(m_tail);
		}
	}
	return NULL;
}

bool	ml_free(void * value)
{
	if (value)
	{
		Block * block;
		if (block = FIND_BLOCK(value))
		{
			if ((block >= m_head) && (block <= (m_tail + BLOCK_SIZE)))
			{
				block->free = true;
				
				return true;
			}
		}
	}
	return false;
}


size_t	ml_increment(size_t size)
{
	return (m_used += SPACE_NEED(size));
}

bool	ml_prime(byte * data, size_t size)
{
	static bool checked = false;
	if (!checked && (!m_head && data && size))
	{
		checked = true;
		
		m_data	= data;
		m_size	= size;
		m_used	= 0;
		m_head	= ml_createBlock(size);
		m_tail	= m_head;

		size_t i;
		for (i = BLOCK_SIZE; i < m_size; i++)
		{
			m_data[i] = 0;
		}
	}
	return checked;
}


bool	ml_mergeBlockPrev(Block * value)
{
	if (value->prev && (value->prev)->free)
	{
		(value->prev)->size += value->size;

		(value->prev)->next = value->next;

		if (value->next)
		{
			(value->next)->prev = value->prev;
		}

		return true;
	}
	return false;
}

bool	ml_mergeBlockNext(Block * value)
{
	if (value->next && (value->next)->free)
	{
		value->size += (value->next)->size;

		value->next = (value->next)->next;

		if ((value->next)->next)
		{
			((value->next)->next)->prev = value;
		}

		return true;
	}
	return false;
}

bool	ml_splitBlock(Block * value, size_t size)
{
	if (value)
	{
		Block * block;

		if (block = (Block *)(BLOCK_NPOS(value) + size))
		{
			block->size = (value->size - size - BLOCK_SIZE);
			block->free = true;
			block->prev = value;
			block->next = value->next;

			if (block->next)
			{
				(block->next)->prev = block;
			}

			value->size = size;
			value->free = false;
			value->next = block;

			return true;
		}
	}
	return false;
}


Block * ml_createBlock(size_t size)
{
	if ((m_used + SPACE_NEED(size)) < m_size)
	{
		Block * block;
		if (block = (Block *)(&m_data[ml_increment(size)]))
		{
			block->size = size;
			block->free = false;
			block->prev = NULL;
			block->next = NULL;
			return block;
		}
	}
	return NULL;
}

Block * ml_findEmptyBlock(size_t size)
{
	Block * block = (m_head);
	while (block)
	{
		if (block->free && (block->size >= SPACE_NEED(size)))
		{
			return block;
		}
		block = block->next;
	}
	return block;
}


/* * * * * * * * * * * * * * * * * * * * */