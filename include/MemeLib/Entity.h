#ifndef _ENTITY_H_
#define _ENTITY_H_

#include <MemeLib/ComponentPool.h>

namespace ml
{
	class Entity
		: public ITrackable
	{
	public:
		Entity() : m_pool() {};
		virtual ~Entity() {}

	protected:
		inline const ComponentPool & pool() const
		{
			return m_pool;
		}
		inline ComponentPool & pool()
		{
			return m_pool;
		}

	public:
		template <typename T> 
		inline T* addComponent()
		{
			return pool().add<T>();
		}
		
		template <typename T> 
		inline T* getComponent()
		{
			return pool().get<T>();
		}
		
		template <typename T> 
		inline const T* getComponent() const
		{
			return pool().get<T>();
		}
		
		template <typename T>
		inline bool hasComponent() const
		{
			return pool().contains<T>();
		}
		
		template <typename T> 
		inline T* setComponent(const T& value)
		{
			return pool().set<T>(value);
		}

	private:
		ComponentPool m_pool;
	};
}

#endif // !_ENTITY_H_
