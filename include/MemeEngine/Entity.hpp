#ifndef _ML_ENTITY_HPP_
#define _ML_ENTITY_HPP_

#include <MemeEngine/Export.hpp>
#include <MemeCore/ITrackable.hpp>
#include <MemeCore/IDisposable.hpp>
#include <MemeCore/IReadable.hpp>
#include <MemeCore/IWritable.hpp>
#include <MemeCore/Preprocessor.hpp>

/* * * * * * * * * * * * * * * * * * * * */

#define ML_assert_component(derived) ML_assert_is_base_of(ml::ITrackable, derived)

/* * * * * * * * * * * * * * * * * * * * */

namespace ml
{
	/* * * * * * * * * * * * * * * * * * * * */

	class ML_ENGINE_API Entity final
		: public ITrackable
		, public IDisposable
		, public IReadable
		, public IWritable
		, public INonCopyable
	{
	public: // Usings
		/* * * * * * * * * * * * * * * * * * * * */
		using value_type	= typename ITrackable *;
		using map_type		= typename HashMap<size_t, value_type>;
		using iterator		= typename map_type::iterator;
		using const_iterator= typename map_type::const_iterator;

	public: //
		/* * * * * * * * * * * * * * * * * * * * */
		Entity();
		~Entity();

	public: // Overrides
		/* * * * * * * * * * * * * * * * * * * * */
		bool dispose() override;
		bool loadFromFile(const String & filename) override;
		bool saveToFile(const String & filename) const override;

	public: // Functions
		/* * * * * * * * * * * * * * * * * * * * */
		template <
			class Component
		> inline Component * add()
		{
			ML_assert_component(Component);
			return ((this->find<Component>() == this->end())
				? (this->set(new Component()))
				: (NULL)
			);
		}

		template <
			class Component
		> inline Component * add(const Component & value)
		{
			ML_assert_component(Component);
			return ((this->find<Component>() == this->end())
				? (this->set(new Component(value)))
				: (NULL)
			);
		}

		/* * * * * * * * * * * * * * * * * * * * */

		template <
			class Component
		> inline value_type & at()
		{
			ML_assert_component(Component);
			return m_map[this->hash<Component>()];
		}

		template <
			class Component
		> inline const value_type & at() const
		{
			ML_assert_component(Component);
			return m_map.at(this->hash<Component>());
		}

		/* * * * * * * * * * * * * * * * * * * * */

		template <
			class Component
		> inline Component * get()
		{
			ML_assert_component(Component);
			iterator it;
			return (((it = this->find<Component>()) != this->end())
				? (reinterpret_cast<Component *>(it->second))
				: (NULL)
			);
		}

		template <
			class Component
		> inline const Component * get() const
		{
			ML_assert_component(Component);
			const_iterator it;
			return (((it = this->find<Component>()) != this->cend())
				? (reinterpret_cast<const Component *>(it->second))
				: (NULL)
			);
		}

		/* * * * * * * * * * * * * * * * * * * * */

		template <
			class Component
		> inline iterator find()
		{
			ML_assert_component(Component);
			return (iterator)(m_map.find(this->hash<Component>()));
		}

		template <
			class Component
		> inline const_iterator find() const
		{
			ML_assert_component(Component);
			return (const_iterator)(m_map.find(this->hash<Component>()));
		}

		/* * * * * * * * * * * * * * * * * * * * */

		template <
			class Component
		> inline size_t hash() const
		{
			ML_assert_component(Component);
			return (&typeid(Component))->hash_code();
		}

		/* * * * * * * * * * * * * * * * * * * * */

		template <
			class Component
		> inline Component * set(Component * value)
		{
			ML_assert_component(Component);
			return ((Component *)(this->at<Component>() = value));
		}

	public: // Iterators
		/* * * * * * * * * * * * * * * * * * * * */
		inline iterator			begin()			{ return m_map.begin(); }
		inline const_iterator	begin()	const	{ return m_map.begin(); }
		inline const_iterator	cbegin()const	{ return m_map.cbegin();}
		inline iterator			end()			{ return m_map.end();	}
		inline const_iterator	end()	const	{ return m_map.end();	}
		inline const_iterator	cend()	const	{ return m_map.cend();	}

	private: // Data
		/* * * * * * * * * * * * * * * * * * * * */
		map_type m_map;
	};

	/* * * * * * * * * * * * * * * * * * * * */
}

#endif // !_ML_ENTITY_HPP_