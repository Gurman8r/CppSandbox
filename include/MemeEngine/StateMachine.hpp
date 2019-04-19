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
		using key_type		= typename int32_t;
		using ret_type		= typename int32_t;
		using arg_type		= typename void *;
		using fun_type		= typename ret_type(*)(arg_type);
		using map_type		= typename HashMap<key_type, fun_type>;
		using pair_type		= typename Pair<key_type, fun_type>;
		using init_type		= typename std::initializer_list<pair_type>;
		using iterator		= typename map_type::iterator;
		using const_iterator= typename map_type::const_iterator;

	public:
		StateMachine();
		StateMachine(const map_type & states);
		StateMachine(const init_type & init);
		~StateMachine();

	public:
		fun_type get(const key_type key) const;
		fun_type set(const key_type key, fun_type && fun);
		ret_type run(const key_type key, arg_type data);

	private:
		map_type m_states;
	};

	/* * * * * * * * * * * * * * * * * * * * */
}

#endif // !_ML_STATE_MACHINE_HPP_
