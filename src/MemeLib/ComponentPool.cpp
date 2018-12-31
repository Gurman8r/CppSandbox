#include <MemeLib/ComponentPool.h>

namespace ml
{
	ComponentPool::ComponentPool()
		: m_value()
	{
	}
	
	ComponentPool::ComponentPool(const ComponentPool & copy)
		: m_value(copy.m_value)
	{
	}
	
	ComponentPool::~ComponentPool()
	{
		clear();
	}


	const bool ComponentPool::empty() const
	{
		return m_value.empty();
	}

	ComponentPool & ComponentPool::clear()
	{
		for (auto pair : m_value)
		{
			if (pair.second)
			{
				delete pair.second;
				pair.second = NULL;
			}
		}
		m_value.clear();

		return (*this);
	}
}