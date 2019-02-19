#ifndef _COMMAND_HPP_
#define _COMMAND_HPP_

#include <MemeScript/Var.hpp>

namespace ml
{
	class ML_SCRIPT_API Command final
		: public ITrackable
	{
	public:
		using fn_args = Args & ;
		using fn_type = Var(*)(fn_args);

	public:
		Command();
		Command(const String & name, fn_type func);
		Command(const Command & copy);
		~Command() {};

		inline const String & name() const { return m_name; }
		
		inline Var operator()(fn_args args)	{ return m_func(args); }

	private:
		String	m_name;
		fn_type	m_func;
	};
}

#endif // !_COMMAND_HPP_
