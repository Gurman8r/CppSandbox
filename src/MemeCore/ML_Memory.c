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
#define BLOCK_SIZE (size_t)(sizeof(Block))

// Returns the space needed to store a Block and some data
#define SPACE_NEED(size) (size_t)(size + BLOCK_SIZE)

// Returns the end position of the given Block
#define BLOCK_NPOS(blck) (size_t)(((size_t)blck) + BLOCK_SIZE)

// Returns the address of the given block's data
#define BLOCK_DATA(blck) (void *)(BLOCK_NPOS(blck))

// Returns the address of a Block given the address of some data
#define FIND_BLOCK(data) (Block *)(((size_t)data) - BLOCK_SIZE)

/* * * * * * * * * * * * * * * * * * * * */

byte *			m_data;	// Pointer to byte array
size_t			m_size;	// Total bytes available
size_t			m_used;	// Number of bytes used
struct Block *	m_head;	// First block in list
struct Block *	m_tail;	// Last block in list

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
				
				ml_mergeBlockNext(block);
				
				ml_mergeBlockPrev(block);
				
				return true;
			}
		}
	}
	return false;
}

/* * * * * * * * * * * * * * * * * * * * */

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

size_t	ml_increment(size_t size)
{
	return (m_used += SPACE_NEED(size));
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
			block->size = (value->size - size);
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


void	ml_displayMemory()
{
	printf("Bytes Used: ( %u / %u )\n", m_used, m_size);

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

bool	ml_displayBlock(Block * value)
{
	if (value)
	{
		printf("[ size: %d | %s | addr: %p | prev: %p | next: %p | npos: %p ]\n", 
			value->size,
			(value->free ? "free" : "used"),
			(void *)value,
			(void *)value->prev,
			(void *)value->next,
			BLOCK_DATA(value));

		return true;
	}
	return false;
}


Block * ml_createBlock(size_t size)
{
	if ((m_used + size) <= m_size)
	{
		Block * block;
		if (block = (Block *)(&m_data[ml_increment(size)]))
		{
			block->size = size;
			block->free = false;
			block->prev = NULL;
			block->next = NULL;
			return m_tail;
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

Block * ml_findBlockByValue(void * value)
{
	if (value)
	{
		Block * found = NULL;

	}
	return NULL;
}

/* * * * * * * * * * * * * * * * * * * * */