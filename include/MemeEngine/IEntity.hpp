#ifndef _ML_I_ENTITY_HPP_
#define _ML_I_ENTITY_HPP_

#include <MemeEngine/IComponent.hpp>

namespace ml
{
	class ML_ENGINE_API IEntity
	{
	public:
		using ComponentMap	= typename HashMap<const std::type_info *, IComponent *>;
		using iterator		= typename ComponentMap::iterator;
		using const_iterator= typename ComponentMap::const_iterator;

	public:
		IEntity() 
			: m_map()
		{
		}
		virtual ~IEntity() {}

	public:
		template <class T> 
		inline T * addComponent()
		{
			if (!getComponent<T>())
			{
				m_map.insert({ &typeid(T), new T() });
				return get(&typeid(T));
			}
			return NULL;
		}
		
		template <class T> 
		inline T * getComponent()
		{
			iterator it;
			if ((it = m_map.find(&typeid(T))) != end())
			{
				return static_cast<T *>(it->second);
			}
			return NULL;
		}
		
		template <class T> 
		inline const T * getComponent() const
		{
			const_iterator it;
			if ((it = m_map.find(&typeid(T))) != end())
			{
				return static_cast<T *>(it->second);
			}
			return NULL;
		}
		
	public:
		inline iterator			begin()			{ return m_map.begin(); }
		inline const_iterator	begin()	const	{ return m_map.begin(); }
		inline const_iterator	cbegin()const	{ return m_map.cbegin();}
		inline iterator			end()			{ return m_map.end();	}
		inline const_iterator	end()	const	{ return m_map.end();	}
		inline const_iterator	cend()	const	{ return m_map.cend();	}

	private:
		ComponentMap m_map;
	};
}

#endif // !_ML_I_ENTITY_HPP_