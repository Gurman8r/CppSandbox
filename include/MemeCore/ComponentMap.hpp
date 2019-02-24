#ifndef _COMPONENT_MAP_HPP_
#define _COMPONENT_MAP_HPP_

#include <MemeCore/IComponent.hpp>

#define assert_typeof_component(T) \
static_assert(std::is_base_of<ml::IComponent, T>::value, "Type must derive ml::IComponent")

namespace ml
{
	class ML_CORE_API ComponentMap
		: public ITrackable
	{
	public:
		using key_type		= const std::type_info *;
		using value_type	= typename IComponent *;
		using map_type		= HashMap<key_type, value_type>;
		using iterator		= typename map_type::iterator;
		using const_iterator= typename map_type::const_iterator;

	public:
		ComponentMap()
			: m_map()
		{
		}
		ComponentMap(const ComponentMap & copy)
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
		template <class T> 
		inline T * add()
		{
			if (!contains<T>())
			{
				m_map.insert({ &typeid(T), new T() });
				return get<T>();
			}
			return NULL;
		}
		
		template <class T> 
		inline bool	contains() const
		{
			return find<T>() != end();
		}
		
		template <class T> 
		inline bool	erase()
		{
			if (T * temp = get<T>())
			{
				delete temp;
				m_map.erase(&typeid(T));
				return !contains<T>();
			}
			return false;
		}
		
		template <class T> 
		inline iterator	find()
		{
			return m_map.find(&typeid(T));
		}
		
		template <class T> 
		inline const_iterator find() const
		{
			assert_typeof_component(T);
			return m_map.find(&typeid(T));
		}
		
		template <class T> 
		inline T * get()
		{
			iterator it;
			if ((it = find<T>()) != end())
			{
				return static_cast<T *>(it->second);
			}
			return NULL;
		}
		
		template <class T> 
		inline const T * get() const
		{
			const_iterator it;
			if ((it = find<T>()) != end())
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
		map_type m_map;
	};
}

#endif // !_COMPONENT_MAP_HPP_
