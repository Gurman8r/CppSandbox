#ifndef _ENTITY_HPP_
#define _ENTITY_HPP_

#include <MemeCore/ComponentMap.hpp>

namespace ml
{
	class Entity
		: public ITrackable
	{
	public:
		Entity() 
			: m_components() 
		{
		}
		virtual ~Entity() {}

	protected:
		inline const ComponentMap & components() const
		{
			return m_components;
		}
		inline ComponentMap & components()
		{
			return m_components;
		}

	public:
		template <class T> 
		inline T * addComponent()
		{
			return components().add<T>();
		}
		
		template <class T> 
		inline T * getComponent()
		{
			return components().get<T>();
		}
		
		template <class T> 
		inline const T * getComponent() const
		{
			return components().get<T>();
		}
		
		template <class T>
		inline bool hasComponent() const
		{
			return components().contains<T>();
		}
		
		template <class T> 
		inline T * setComponent(const T & value)
		{
			return components().set<T>(value);
		}

	private:
		ComponentMap m_components;
	};
}

#endif // !_ENTITY_HPP_
