#include <MemeLib/ComponentPool.h>

namespace ml
{
	ComponentPool::ComponentPool()
		: m_map()
	{
	}
	
	ComponentPool::ComponentPool(const ComponentPool & copy)
		: m_map(copy.m_map)
	{
	}
	
	ComponentPool::~ComponentPool()
	{
		for (auto pair : (*this))
		{
			delete pair.second;
			pair.second = NULL;
		}
		m_map.clear();
	}
}