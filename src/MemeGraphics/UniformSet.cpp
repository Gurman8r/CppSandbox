#include <MemeGraphics/UniformSet.hpp>

namespace ml
{
	UniformSet::UniformSet()
		: UniformSet(map_type())
	{
	}

	UniformSet::UniformSet(const map_type & value)
		: m_map(value)
	{
	}

	UniformSet::UniformSet(const List<Uniform> & value)
		: UniformSet()
	{
		for (const Uniform & u : value)
		{
			this->insert(u);
		}
	}

	UniformSet::UniformSet(const std::initializer_list<Uniform>& value)
		: UniformSet()
	{
		for (auto it = value.begin(); it != value.end(); it++)
		{
			this->insert(*it);
		}
	}

	UniformSet::UniformSet(const UniformSet & copy)
		: UniformSet(copy.m_map)
	{
	}

	UniformSet::~UniformSet()
	{
		m_map.clear();
	}
	
	
	void UniformSet::clear()
	{
		m_map.clear();
	}

	bool UniformSet::empty() const
	{
		return m_map.empty();
	}

	bool UniformSet::insert(const Uniform & value)
	{
		if (!find(value.name))
		{
			m_map.insert({ value.name, value });
			return true;
		}
		return false;
	}

	Uniform * UniformSet::find(const String & name)
	{
		iterator it;
		if ((it = m_map.find(name)) != end())
		{
			return &it->second;
		}
		return NULL;
	}
}