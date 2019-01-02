#include <MemeScript/Command.h>

namespace ml
{
	Command::Command()
		: m_name()
		, m_func(NULL)
		, m_info()
	{
	}
	Command::Command(const std::string& name, fn_type func)
		: m_name(name)
		, m_func(func)
		, m_info()
	{
	}
	Command::Command(const std::string& name, fn_type func, const CommandInfo& info)
		: m_name(name)
		, m_func(func)
		, m_info(info)
	{
	}
	Command::Command(fn_type func, const CommandInfo & info)
		: m_name(info.name)
		, m_func(func)
		, m_info(info)
	{
	}
	Command::Command(const Command& copy)
		: m_name(copy.m_name)
		, m_func(copy.m_func)
		, m_info(copy.m_info)
	{
	}


	std::string	 Command::name() const
	{
		return m_name;
	}

	CommandInfo	 Command::info() const
	{
		return m_info;
	}


	Var	 Command::operator()(Args args)
	{
		return m_func(args);
	}

	std::ostream & operator<<(std::ostream & out, const Command & cmd)
	{
		return cmd.info().formatManual(out);
	}

}