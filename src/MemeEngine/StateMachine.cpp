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
			m_states[it->first] = it->second;
		}
	}

	StateMachine::~StateMachine()
	{
	}

	/* * * * * * * * * * * * * * * * * * * * */

	StateMachine::StateFun StateMachine::get(const int32_t id) const
	{
		const_iterator it;
		return ((id > ML_STATE_INVALID)
			? ((((it = m_states.find(id)) != m_states.end())
				? (it->second)
				: (NULL)))
			: (NULL));
	}

	StateMachine::StateFun StateMachine::set(const int32_t id, StateFun && fun)
	{
		iterator it;
		return ((id > ML_STATE_INVALID)
			? (((it = m_states.find(id)) == m_states.end()
				? (it->second = fun)
				: (NULL)))
			: (NULL));
	}

	StateMachine::StateRet StateMachine::run(const int32_t id, StateArg data)
	{
		StateFun fun;
		return ((id > ML_STATE_INVALID)
			? (((fun = get(id))
				? (fun(data))
				: (ML_STATE_INVALID)))
			: (ML_STATE_INVALID));
	}

	/* * * * * * * * * * * * * * * * * * * * */
}