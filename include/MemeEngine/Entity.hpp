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

	class ML_ENGINE_API Entity
		: public ITrackable
		, public IDisposable
		, public IReadable
		, public IDrawable
	{
		friend class ECS;

	public:
		using TypeInfo		= typename const std::type_info *;
		using ComponentMap	= typename HashMap<TypeInfo, void *>;
		using iterator		= typename ComponentMap::iterator;
		using const_iterator= typename ComponentMap::const_iterator;

	public:
		Entity();
		virtual ~Entity();

	public:
		virtual bool dispose() override;
		virtual bool loadFromFile(const String & filename) override;

	public:
		virtual void update() const;
		virtual void draw(RenderTarget & target, RenderBatch batch) const override;

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
				? ((Component *)(it->second))
				: (NULL));
		}

		template <
			class Component
		> inline const Component * get() const
		{
			const_iterator it;
			return (((it = this->find<Component>()) != this->end())
				? ((const Component *)(it->second))
				: (NULL));
		}

		/* * * * * * * * * * * * * * * * * * * * */

		template <
			class Component
		> inline const_iterator find() const
		{
			return m_cmp.find(&typeid(Component));
		}

		template <
			class Component
		> inline iterator find()
		{
			return m_cmp.find(&typeid(Component));
		}

		/* * * * * * * * * * * * * * * * * * * * */

		template <
			class Component
		> inline Component * set(Component * value)
		{
			return ((Component *)(m_cmp[&typeid(Component)] = value));
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
		ComponentMap m_cmp;
	};

	/* * * * * * * * * * * * * * * * * * * * */
}

#endif // !_ML_I_ENTITY_HPP_