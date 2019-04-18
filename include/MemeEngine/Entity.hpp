#ifndef _ML_I_ENTITY_HPP_
#define _ML_I_ENTITY_HPP_

#include <MemeEngine/Export.hpp>
#include <MemeCore/ITrackable.hpp>
#include <MemeCore/IDisposable.hpp>
#include <MemeCore/IReadable.hpp>
#include <MemeGraphics/IDrawable.hpp>

namespace ml
{
	/* * * * * * * * * * * * * * * * * * * * */

	class ECS;

	/* * * * * * * * * * * * * * * * * * * * */

	class ML_ENGINE_API Entity final
		: public ITrackable
		, public IDisposable
		, public IReadable
	{
		friend class ECS;

	public:
		using map_type		= typename HashMap<size_t, void *>;
		using iterator		= typename map_type::iterator;
		using const_iterator= typename map_type::const_iterator;

	public:
		Entity();
		~Entity();

	public:
		bool dispose() override;
		bool loadFromFile(const String & filename) override;

	public:
		/* * * * * * * * * * * * * * * * * * * * */

		template <
			class Component
		> inline Component * add()
		{
			return this->add(Component());
		}

		template <
			class Component
		> inline Component * add(const Component & value)
		{
			return ((this->find<Component>() == this->end())
				? (this->set(new Component(value)))
				: (NULL));
		}

		/* * * * * * * * * * * * * * * * * * * * */

		template <
			class Component
		> inline Component * get()
		{
			iterator it;
			return (((it = this->find<Component>()) != this->end())
				? (reinterpret_cast<Component *>(it->second))
				: (NULL));
		}

		template <
			class Component
		> inline const Component * get() const
		{
			const_iterator it;
			return (((it = this->find<Component>()) != this->cend())
				? (reinterpret_cast<const Component *>(it->second))
				: (NULL));
		}

		/* * * * * * * * * * * * * * * * * * * * */

		template <
			class Component
		> inline iterator find()
		{
			return (iterator)(m_cmp.find((&typeid(Component))->hash_code()));
		}

		template <
			class Component
		> inline const_iterator find() const
		{
			return (const_iterator)(m_cmp.find((&typeid(Component))->hash_code()));
		}

		/* * * * * * * * * * * * * * * * * * * * */

		template <
			class Component
		> inline Component * set(Component * value)
		{
			return ((Component *)(m_cmp[(&typeid(Component))->hash_code()] = value));
		}

		/* * * * * * * * * * * * * * * * * * * * */
		
	public:
		inline iterator			begin()			{ return m_cmp.begin(); }
		inline const_iterator	begin()	const	{ return m_cmp.begin(); }
		inline const_iterator	cbegin()const	{ return m_cmp.cbegin();}
		inline iterator			end()			{ return m_cmp.end();	}
		inline const_iterator	end()	const	{ return m_cmp.end();	}
		inline const_iterator	cend()	const	{ return m_cmp.cend();	}

	private:
		map_type m_cmp;
	};

	/* * * * * * * * * * * * * * * * * * * * */
}

#endif // !_ML_I_ENTITY_HPP_