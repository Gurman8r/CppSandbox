#ifndef _ML_STATE_MACHINE_HPP_
#define _ML_STATE_MACHINE_HPP_

#include <MemeEngine/Export.hpp>
#include <MemeCore/ITrackable.hpp>

#define ML_STATE_INVALID -1

namespace ml
{
	/* * * * * * * * * * * * * * * * * * * * */

	template <
		class _Key
	> class StateMachine final
		: public ITrackable
		, public INonCopyable
	{
	public:
		using key_type		= typename _Key;
		using fun_type		= typename key_type(*)(void);

		using map_type		= typename HashMap<key_type, fun_type>;
		using pair_type		= typename Pair<key_type, fun_type>;
		using init_type		= typename Initializer<pair_type>;
		using iterator		= typename map_type::iterator;
		using const_iterator= typename map_type::const_iterator;

	public:
		StateMachine(const map_type & states)
			: m_states(states)
		{
		}

		StateMachine(const init_type & init)
			: m_states()
		{
			for (auto it = init.begin(); it != init.end(); it++)
			{
				if (it->second && (it->first > (static_cast<key_type>(ML_STATE_INVALID))))
				{
					m_states[it->first] = it->second;
				}
			}
		}

		~StateMachine() {}

	public:
		inline fun_type get(const key_type & key) const
		{
			const_iterator it;
			return ((key > (static_cast<key_type>(ML_STATE_INVALID)))
				? ((((it = m_states.find(key)) != m_states.end())
					? (it->second)
					: (NULL)))
				: (NULL)
			);
		}

		inline key_type run(const key_type & key)
		{
			fun_type fun;
			return ((key > static_cast<key_type>(ML_STATE_INVALID))
				? (((fun = get(key))
					? (fun())
					: (static_cast<key_type>(ML_STATE_INVALID))))
				: (static_cast<key_type>(ML_STATE_INVALID))
			);
		}

	private:
		map_type m_states;
	};

	/* * * * * * * * * * * * * * * * * * * * */
}

#endif // !_ML_STATE_MACHINE_HPP_
