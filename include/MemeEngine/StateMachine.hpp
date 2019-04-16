#ifndef _ML_STATE_MACHINE_HPP_
#define _ML_STATE_MACHINE_HPP_

#include <MemeEngine/Export.hpp>
#include <MemeCore/ITrackable.hpp>

#define ML_STATE_INVALID -1

namespace ml
{
	/* * * * * * * * * * * * * * * * * * * * */

	class ML_ENGINE_API StateMachine final
		: public ITrackable
		, public INonCopyable
	{
	public:
		using StateRetv		= typename int32_t;
		using StateArgs		= typename void *;
		using StateFun		= typename StateRetv(*)(StateArgs);
		using StateMap		= typename HashMap<int32_t, StateFun>;
		using StatePair		= typename Pair<int32_t, StateFun>;
		using StateInit		= typename std::initializer_list<StatePair>;

		using iterator		= typename StateMap::iterator;
		using const_iterator= typename StateMap::const_iterator;

	public:
		StateMachine();
		StateMachine(const StateInit & states);
		~StateMachine();

	public:
		StateFun	get(const int32_t id) const;
		StateFun	set(const int32_t id, StateFun && fun);
		StateRetv	run(const int32_t id, StateArgs data);

	private:
		StateMap m_states;
	};

	/* * * * * * * * * * * * * * * * * * * * */
}

#endif // !_ML_STATE_MACHINE_HPP_
