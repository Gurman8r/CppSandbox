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
		return ((id == ML_STATE_INVALID)
			? (NULL)
			: (((it = m_states.find(id)) != m_states.end()
				? (it->second)
				: (NULL))));
	}

	StateMachine::StateFun StateMachine::set(const int32_t id, StateFun && fun)
	{
		return ((id == ML_STATE_INVALID)
			? (NULL)
			: (((m_states.find(id)) == m_states.end()
				? (m_states[id] = fun)
				: (NULL))));
	}

	StateMachine::StateRetv StateMachine::run(const int32_t id, StateArgs data)
	{
		StateFun fun;
		return ((id == ML_STATE_INVALID)
			? (ML_STATE_INVALID)
			: (((fun = get(id))
				? (fun(data))
				: (ML_STATE_INVALID))));
	}

	/* * * * * * * * * * * * * * * * * * * * */
}