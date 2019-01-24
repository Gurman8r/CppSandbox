#ifndef _COMMAND_H_
#define _COMMAND_H_

#include <MemeScript/Var.h>

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
		Command(const std::string & name, fn_type func);
		Command(const Command & copy);
		~Command() {};

		inline const std::string & name() const { return m_name; }
		
		inline Var operator()(fn_args args)	{ return m_func(args); }

	private:
		std::string m_name;
		fn_type		m_func;
	};
}

#endif // !_COMMAND_H_
