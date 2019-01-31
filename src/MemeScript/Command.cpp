#include <MemeScript/Command.h>

namespace ml
{
	Command::Command()
		: m_name()
		, m_func(NULL)
	{
	}
	Command::Command(const string & name, fn_type func)
		: m_name(name)
		, m_func(func)
	{
	}
	Command::Command(const Command & copy)
		: m_name(copy.m_name)
		, m_func(copy.m_func)
	{
	}
}