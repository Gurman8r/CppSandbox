#include <MemeEngine/Plugin.hpp>
#include <MemeCore/Debug.hpp>

namespace ml
{
	/* * * * * * * * * * * * * * * * * * * * */

	Plugin::Plugin()
		: m_file()
		, m_inst(NULL)
	{
	}

	Plugin::~Plugin()
	{
		dispose();
	}

	/* * * * * * * * * * * * * * * * * * * * */

	bool Plugin::dispose()
	{
		return ML_Lib.freeLibrary(m_inst) || !(m_inst = NULL);
	}

	bool Plugin::loadFromFile(const String & filename)
	{
		return dispose() && (m_inst = ML_Lib.loadLibrary(m_file = filename));
	}

	void * Plugin::call(const String & name, void * data)
	{
		PluginFun func;
		return ((func = (PluginFun)ML_Lib.loadFunction(m_inst, name))
			? (func(data))
			: (NULL));
	}

	/* * * * * * * * * * * * * * * * * * * * */
}