#ifndef _ML_I_ENTITY_HPP_
#define _ML_I_ENTITY_HPP_

#include <MemeCore/IReadable.hpp>
#include <MemeEngine/Component.hpp>

namespace ml
{
	class ML_ENGINE_API Entity
		: public ITrackable
		, public IReadable
	{
	public:
		using TypeInfo		= typename const std::type_info *;
		using ComponentMap	= typename HashMap<TypeInfo, Component *>;
		using iterator		= typename ComponentMap::iterator;
		using const_iterator= typename ComponentMap::const_iterator;

	public:
		Entity();
		virtual ~Entity();

	public:
		bool cleanup() override;
		bool loadFromFile(const String & filename) override;

	public:
		template <typename T> 
		inline T * addComponent()
		{
			return (m_map.find(&typeid(T)) == m_map.end())
				? ((T *)(m_map[&typeid(T)] = new T()))
				: (NULL);
		}

		template <typename T>
		inline T * getComponent()
		{
			iterator it;
			return ((it = m_map.find(&typeid(T))) != end())
				? ((T *)(it->second))
				: (NULL);
		}

		template <typename T>
		inline const T * getComponent() const
		{
			const_iterator it;
			return ((it = m_map.find(&typeid(T))) != end())
				? ((const T *)(it->second))
				: (NULL);
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