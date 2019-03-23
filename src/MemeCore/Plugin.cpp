#include <MemeCore/Plugin.hpp>
#include <MemeCore/Debug.hpp>

namespace ml
{
	/* * * * * * * * * * * * * * * * * * * * */

	Plugin::Plugin()
		: m_name(String())
		, m_inst(NULL)
	{
	}

	Plugin::~Plugin()
	{
		cleanup();
	}

	/* * * * * * * * * * * * * * * * * * * * */

	bool Plugin::cleanup()
	{
		if (ML_Lib.freeLibrary(m_inst))
		{
			m_inst = NULL;
		}
		return !(m_inst);
	}

	bool Plugin::loadFromFile(const String & filename)
	{
		return (cleanup()) && (m_inst = ML_Lib.loadLibrary(m_name = filename));
	}

	bool Plugin::call(const String & func, void * data)
	{
		PluginFun fun;
		if (fun = (PluginFun)ML_Lib.loadFunction(m_inst, func))
		{
			fun(data);
		}
		return (bool)(fun);
	}

	bool Plugin::main(void * data)
	{
		return call(ML_LITERAL(ML_PluginMain), data);
	}

	/* * * * * * * * * * * * * * * * * * * * */
}