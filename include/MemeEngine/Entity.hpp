#ifndef _ML_I_ENTITY_HPP_
#define _ML_I_ENTITY_HPP_

#include <MemeCore/IReadable.hpp>
#include <MemeEngine/Export.hpp>
#include <MemeCore/ITrackable.hpp>

namespace ml
{
	class ML_ENGINE_API Entity
		: public ITrackable
		, public IReadable
	{
	public:
		using TypeInfo		= typename const std::type_info *;
		using ComponentMap	= typename HashMap<TypeInfo, void *>;
		using iterator		= typename ComponentMap::iterator;
		using const_iterator= typename ComponentMap::const_iterator;

	public:
		Entity();
		virtual ~Entity();

	public:
		virtual bool cleanup() override;
		virtual bool loadFromFile(const String & filename) override;

	public:
		template <typename T>
		inline T * add()
		{
			return (m_cmp.find(&typeid(T)) == m_cmp.end())
				? ((T *)(m_cmp[&typeid(T)] = new T()))
				: (NULL);
		}

		template <typename T>
		inline T * add(const T & value)
		{
			return (m_cmp.find(&typeid(T)) == m_cmp.end())
				? ((T *)(m_cmp[&typeid(T)] = new T(value)))
				: (NULL);
		}

		template <typename T>
		inline T * get()
		{
			iterator it;
			return ((it = m_cmp.find(&typeid(T))) != end())
				? ((T *)(it->second))
				: (NULL);
		}

		template <typename T>
		inline const T * get() const
		{
			const_iterator it;
			return ((it = m_cmp.find(&typeid(T))) != end())
				? ((const T *)(it->second))
				: (NULL);
		}
		
	public:
		inline iterator			begin()			{ return m_cmp.begin(); }
		inline const_iterator	begin()	const	{ return m_cmp.begin(); }
		inline const_iterator	cbegin()const	{ return m_cmp.cbegin();}
		inline iterator			end()			{ return m_cmp.end();	}
		inline const_iterator	end()	const	{ return m_cmp.end();	}
		inline const_iterator	cend()	const	{ return m_cmp.cend();	}

	private:
		ComponentMap m_cmp;
	};
}

#endif // !_ML_I_ENTITY_HPP_