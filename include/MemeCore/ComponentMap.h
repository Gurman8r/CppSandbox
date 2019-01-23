#ifndef _COMPONENT_POOL_H_
#define _COMPONENT_POOL_H_

#include <MemeCore/Component.h>
#include <unordered_map>
#include <typeindex>

#define assert_typeof_component(T) \
static_assert(std::is_base_of<ml::Component, T>::value, "Type must inherit ml::Component")

namespace ml
{
	class ML_CORE_API ComponentMap
		: public ITrackable
	{
	public:
		using key_type		= const std::type_info*;
		using map_type		= std::unordered_map<key_type, Component*>;
		using iterator		= map_type::iterator;
		using const_iterator= map_type::const_iterator;

	public:
		ComponentMap()
			: m_map()
		{
		}
		ComponentMap(const ComponentMap& copy)
			: m_map(copy.m_map)
		{
		}
		~ComponentMap()
		{
			clear();
		}

	public:
		inline void clear()
		{
			for (auto pair : (*this))
			{
				delete pair.second;
				pair.second = NULL;
			}
			m_map.clear();
		}

	public:
		template <typename T> 
		inline T * add()
		{
			assert_typeof_component(T);
			if (!contains<T>())
			{
				m_map.insert({ &typeid(T), new T() });
				return get<T>();
			}
			return NULL;
		}
		
		template <typename T> 
		inline bool	contains() const
		{
			assert_typeof_component(T);
			return find<T>() != end();
		}
		
		template <typename T> 
		inline bool	erase()
		{
			assert_typeof_component(T);
			if (T* temp = get<T>())
			{
				delete temp;
				m_map.erase(&typeid(T));
				return !contains<T>();
			}
			return false;
		}
		
		template <typename T> 
		inline iterator	find()
		{
			assert_typeof_component(T);
			return m_map.find(&typeid(T));
		}
		
		template <typename T> 
		inline const_iterator find() const
		{
			assert_typeof_component(T);
			return m_map.find(&typeid(T));
		}
		
		template <typename T> 
		inline T * get()
		{
			assert_typeof_component(T);
			iterator it;
			if ((it = find<T>()) != end())
			{
				return static_cast<T*>(it->second);
			}
			return NULL;
		}
		
		template <typename T> 
		inline const T * get() const
		{
			assert_typeof_component(T);
			const_iterator it;
			if ((it = find<T>()) != end())
			{
				return static_cast<T*>(it->second);
			}
			return NULL;
		}
		
	public:
		inline ComponentMap::iterator		begin()			{ return m_map.begin(); };
		inline ComponentMap::const_iterator	begin()	const	{ return m_map.begin(); };
		inline ComponentMap::const_iterator	cbegin()const	{ return m_map.cbegin(); }
		inline ComponentMap::iterator		end()			{ return m_map.end(); };
		inline ComponentMap::const_iterator	end()	const	{ return m_map.end(); }
		inline ComponentMap::const_iterator	cend()	const	{ return m_map.cend(); }

	private:
		map_type m_map;
	};
}

#endif // !_COMPONENT_POOL_H_
