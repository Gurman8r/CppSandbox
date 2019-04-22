#ifndef _ML_STATE_MACHINE_HPP_
#define _ML_STATE_MACHINE_HPP_

#include <MemeEngine/Export.hpp>
#include <MemeCore/ITrackable.hpp>

#define ML_STATE_INVALID -1

namespace ml
{
	/* * * * * * * * * * * * * * * * * * * * */

	template <
		class _Key, 
		class _Ret, 
		class _Arg
	> class StateMachine final
		: public ITrackable
		, public INonCopyable
	{
	public:
		using key_type		= typename _Key;
		using ret_type		= typename _Ret;
		using arg_type		= typename _Arg;
		using fun_type		= typename ret_type(*)(arg_type);

		using map_type		= typename HashMap<key_type, fun_type>;
		using pair_type		= typename Pair<key_type, fun_type>;
		using init_type		= typename std::initializer_list<pair_type>;
		
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
				if ((it->first > ML_STATE_INVALID) && it->second)
				{
					m_states[it->first] = it->second;
				}
			}
		}

		~StateMachine() {}

	public:
		inline fun_type get(const key_type key) const
		{
			const_iterator it;
			return ((key > ML_STATE_INVALID)
				? ((((it = m_states.find(key)) != m_states.end())
					? (it->second)
					: (NULL)))
				: (NULL)
			);
		}

		inline ret_type run(const key_type key, const arg_type & arg)
		{
			fun_type fun;
			return ((key > ML_STATE_INVALID)
				? (((fun = get(key))
					? (fun(arg))
					: (ML_STATE_INVALID)))
				: (ML_STATE_INVALID)
			);
		}

	private:
		map_type m_states;
	};

	/* * * * * * * * * * * * * * * * * * * * */
}

#endif // !_ML_STATE_MACHINE_HPP_
