#ifndef _COMPONENT_POOL_H_
#define _COMPONENT_POOL_H_

#include <MemeLib/Component.h>
#include <unordered_map>

#define assert_is_base_of_component(T) static_assert(std::is_base_of<ml::Component, T>::value, "Type must inherit ml::Component")

namespace ml
{
	class ML_API ComponentPool
		: public ITrackable
	{
	public: // types
		using key_type = const std::type_info*;
		using map_type = std::unordered_map<key_type, Component*>;
		using iterator = map_type::iterator;
		using c_iterator = map_type::const_iterator;

	public:
		ComponentPool();
		ComponentPool(const ComponentPool& copy);
		~ComponentPool();

		const bool		empty() const;
		ComponentPool&	clear();

	public: // templates
		template <typename T> inline T*			add()
		{
			assert_is_base_of_component(T);

			if (!contains<T>())
			{
				return set<T>();
			}

			return NULL;
		}
		template <typename T> inline bool		contains() const
		{
			assert_is_base_of_component(T);

			return find<T>() != end();
		}
		template <typename T> inline bool		erase()
		{
			assert_is_base_of_component(T);

			if (T* tmp = get<T>())
			{
				delete tmp;
				m_value.erase(&typeid(T));
				return !contains<T>();
			}

			return false;
		}
		template <typename T> inline iterator	find()
		{
			assert_is_base_of_component(T);

			return m_value.find(&typeid(T));
		}
		template <typename T> inline c_iterator find() const
		{
			assert_is_base_of_component(T);

			return m_value.find(&typeid(T));
		}
		template <typename T> inline T*			get()
		{
			assert_is_base_of_component(T);

			auto it = find<T>();
			if (it != end())
			{
				return static_cast<T*>(it->second);
			}

			return NULL;
		}
		template <typename T> inline const T*	get() const
		{
			assert_is_base_of_component(T);

			auto it = find<T>();
			if (it != end())
			{
				return static_cast<T*>(it->second);
			}

			return NULL;
		}
		template <typename T> inline T*			set()
		{
			assert_is_base_of_component(T);

			if (!get<T>())
			{
				m_value.insert({ &typeid(T), new T() });

				return get<T>();
			}

			return NULL;
		}

	public: // iterators
		inline ComponentPool::iterator			begin()
		{
			return m_value.begin();
		};
		inline ComponentPool::iterator			end()
		{
			return m_value.end();
		};
		inline ComponentPool::c_iterator		begin() const
		{
			return m_value.begin();
		};
		inline ComponentPool::c_iterator		end() const
		{
			return m_value.end();
		}
		inline ComponentPool::c_iterator		cbegin() const
		{
			return m_value.cbegin();
		}
		inline ComponentPool::c_iterator		cend() const
		{
			return m_value.cend();
		}

	private: // data
		map_type m_value;
	};
}

#endif // !_COMPONENT_POOL_H_
