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
		return cleanup() && (m_inst = ML_Lib.loadLibrary(m_name = filename));
	}

	bool Plugin::call(const String & name, void * data)
	{
		if (m_inst)
		{
			if (PluginFun func = (PluginFun)ML_Lib.loadFunction(m_inst, name))
			{
				func(data);

				return true;
			}
		}
		return false;
	}

	/* * * * * * * * * * * * * * * * * * * * */

	bool Plugin::call_main(void * data)
	{
		return call(ML_LITERAL(ML_PluginMain), data);
	}

	/* * * * * * * * * * * * * * * * * * * * */
}