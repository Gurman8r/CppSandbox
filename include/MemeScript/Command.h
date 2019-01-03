#ifndef _COMMAND_H_
#define _COMMAND_H_

#include <MemeScript/CommandInfo.h>
#include <MemeScript/Var.h>

namespace ml
{
	class ML_SCRIPT_API Command final
		: public ITrackable
	{
	public:
		using fn_type = Var(*)(Args &);

	public:
		Command();
		Command(const std::string& name, fn_type func);
		Command(const std::string& name, fn_type func, const CommandInfo& info);
		Command(fn_type func, const CommandInfo& info);
		Command(const Command& copy);
		~Command() {};

		std::string	name() const;
		CommandInfo info() const;

		Var	operator()(Args & args);

		friend std::ostream& operator<<(std::ostream& out, const Command& cmd);

	private:
		std::string m_name;
		fn_type		m_func;
		CommandInfo	m_info;
	};
}

#endif // !_COMMAND_H_
