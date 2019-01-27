/* * * * * * * * * * * * * * * * * * * * */

#include <MemeCore/ML_Memory.h>

/* * * * * * * * * * * * * * * * * * * * */

#ifndef ML_CPP
	#define true	((bool)(1))
	#define false	((bool)(0))
#endif

/* * * * * * * * * * * * * * * * * * * * */

// Size of block
#define BLOCK_SIZE sizeof(Block)

// Returns the given size plus sizeof(Block) ( BLOCK_SIZE )
#define SPACE_NEED(size) (size + BLOCK_SIZE)

// Returns the data of the given block_ptr
#define BLOCK_DATA(block_ptr) ((void *)(block_ptr + BLOCK_SIZE))

// Returns the block at the given data_ptr
#define BLOCK_ADDR(data_ptr) ((Block *)((size_t)data_ptr) - BLOCK_SIZE)

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
		if (block = BLOCK_ADDR(value))
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
	if (!checked && (!m_head && (data && size)))
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


Block * ml_createBlock(size_t size)
{
	if ((m_used + SPACE_NEED(size)) < m_size)
	{
		return (Block *)(&m_data[ml_increment(size)]);
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