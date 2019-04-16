#include <MemeEngine/StateMachine.hpp>
#include <MemeCore/Debug.hpp>

namespace ml
{
	/* * * * * * * * * * * * * * * * * * * * */

	StateMachine::StateMachine()
		: m_states()
	{
	}

	StateMachine::StateMachine(const StateInit & states)
		: StateMachine()
	{
		for (auto it = states.begin(); it != states.end(); it++)
		{
			if (it->first > ML_STATE_INVALID)
			{
				m_states[it->first] = it->second;
			}
		}
	}

	StateMachine::~StateMachine()
	{
	}

	/* * * * * * * * * * * * * * * * * * * * */

	StateMachine::StateFun StateMachine::get(const StateKey key) const
	{
		const_iterator it;
		return ((key > ML_STATE_INVALID)
			? ((((it = m_states.find(key)) != m_states.end())
				? (it->second)
				: (NULL)))
			: (NULL));
	}

	StateMachine::StateFun StateMachine::set(const StateKey key, StateFun && fun)
	{
		iterator it;
		return ((key > ML_STATE_INVALID)
			? (((it = m_states.find(key)) == m_states.end()
				? (it->second = fun)
				: (NULL)))
			: (NULL));
	}

	StateMachine::StateRet StateMachine::run(const StateKey key, StateArg data)
	{
		StateFun fun;
		return ((key > ML_STATE_INVALID)
			? (((fun = get(key))
				? (fun(data))
				: (ML_STATE_INVALID)))
			: (ML_STATE_INVALID));
	}

	/* * * * * * * * * * * * * * * * * * * * */
}