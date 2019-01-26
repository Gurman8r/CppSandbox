/* * * * * * * * * * * * * * * * * * * * */

#include <MemeCore/ML_Memory.h>

/* * * * * * * * * * * * * * * * * * * * */

#ifndef ML_CPP
#define true	1
#define false	0
#endif

/* * * * * * * * * * * * * * * * * * * * */

byte *			m_data;		// Pointer to byte array
size_t			m_capacity;	// Total bytes available
size_t			m_size;		// Number of bytes used
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

			return (block + BLOCK_SIZE);
		}
		else if (block = ml_createBlock(size))
		{
			m_tail->next = block;

			block->prev = m_tail;

			m_tail = block;

			return (m_tail + BLOCK_SIZE);
		}
	}
	return NULL;
}

bool	ml_free(void * value)
{
	if (value)
	{
		Block * block;
		if (block = ml_getBlockAt((size_t)value))
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
	return (m_size += (size + BLOCK_SIZE));
}

bool	ml_prime(byte * data, size_t capacity)
{
	static bool checked = false;
	if (!checked && (!m_head && (data && capacity)))
	{
		checked = true;

		m_data		= data;
		m_capacity	= capacity;
		m_size		= 0;
		m_head		= ml_createBlock(capacity);
		m_tail		= m_head;

		size_t i;
		for (i = BLOCK_SIZE; i < m_capacity; i++)
		{
			m_data[i] = NULL;
		}
	}
	return checked;
}


Block * ml_createBlock(size_t size)
{
	if ((m_size + (size + BLOCK_SIZE)) < m_capacity)
	{
		return (Block *)(&m_data[ml_increment(size)]);
	}
	return NULL;
}

Block * ml_findEmptyBlock(size_t size)
{
	Block * ptr = (m_head);
	while (ptr)
	{
		if (ptr->free && (ptr->size >= (size + BLOCK_SIZE)))
		{
			return ptr;
		}
		ptr = ptr->next;
	}
	return ptr;
}

Block * ml_getBlockAt(size_t index)
{
	return (Block *)(((size_t)index) - BLOCK_SIZE);
}

/* * * * * * * * * * * * * * * * * * * * */