#ifndef _ML_STATE_MACHINE_HPP_
#define _ML_STATE_MACHINE_HPP_

#include <MemeEngine/Export.hpp>
#include <MemeCore/ITrackable.hpp>

#define ML_STATE_INVALID (-1)

namespace ml
{
	/* * * * * * * * * * * * * * * * * * * * */

	class ML_ENGINE_API StateMachine final
		: public ITrackable
		, public INonCopyable
	{
	public:
		using StateKey		= typename int32_t;
		using StateRet		= typename int32_t;
		using StateArg		= typename void *;
		using StateFun		= typename StateRet(*)(StateArg);
		using StateMap		= typename HashMap<StateKey, StateFun>;
		using StatePair		= typename Pair<StateKey, StateFun>;
		using StateInit		= typename std::initializer_list<StatePair>;

		using iterator		= typename StateMap::iterator;
		using const_iterator= typename StateMap::const_iterator;

	public:
		StateMachine();
		StateMachine(const StateInit & states);
		~StateMachine();

	public:
		StateFun get(const StateKey key) const;
		StateFun set(const StateKey key, StateFun && fun);
		StateRet run(const StateKey key, StateArg data);

	private:
		StateMap m_states;
	};

	/* * * * * * * * * * * * * * * * * * * * */
}

#endif // !_ML_STATE_MACHINE_HPP_
