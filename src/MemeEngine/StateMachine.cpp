#include <MemeEngine/StateMachine.hpp>
#include <MemeCore/Debug.hpp>

namespace ml
{
	/* * * * * * * * * * * * * * * * * * * * */

	StateMachine::StateMachine()
		: m_states()
	{
	}

	StateMachine::StateMachine(const map_type & states)
		: m_states(states)
	{
	}

	StateMachine::StateMachine(const init_type & init)
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

	StateMachine::~StateMachine()
	{
	}

	/* * * * * * * * * * * * * * * * * * * * */

	StateMachine::fun_type StateMachine::get(const key_type key) const
	{
		const_iterator it;
		return ((key > ML_STATE_INVALID)
			? ((((it = m_states.find(key)) != m_states.end())
				? (it->second)
				: (NULL)))
			: (NULL));
	}

	StateMachine::fun_type StateMachine::set(const key_type key, fun_type && fun)
	{
		iterator it;
		return ((key > ML_STATE_INVALID)
			? (((it = m_states.find(key)) == m_states.end()
				? (it->second = fun)
				: (NULL)))
			: (NULL));
	}

	StateMachine::ret_type StateMachine::run(const key_type key, arg_type data)
	{
		fun_type fun;
		return ((key > ML_STATE_INVALID)
			? (((fun = get(key))
				? (fun(data))
				: (ML_STATE_INVALID)))
			: (ML_STATE_INVALID));
	}

	/* * * * * * * * * * * * * * * * * * * * */
}