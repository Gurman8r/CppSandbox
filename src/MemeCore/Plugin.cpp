#include <MemeCore/Plugin.hpp>
#include <MemeCore/Debug.hpp>
#include <MemeCore/Macros.hpp>

namespace ml
{
	/* * * * * * * * * * * * * * * * * * * * */

	Plugin::Plugin()
		: m_name(String())
		, m_inst(NULL)
		, m_main(NULL)
	{
	}

	Plugin::~Plugin()
	{
		cleanup();
	}

	/* * * * * * * * * * * * * * * * * * * * */

	bool Plugin::cleanup()
	{
		return ML_Lib.freeLibrary(m_inst);
	}

	bool Plugin::loadFromFile(const String & filename)
	{
		return 
			(!m_inst && (m_inst = ML_Lib.loadLibrary(m_name = filename))) &&
			(m_main = ML_Lib.loadFunction<PluginFun>(m_inst, ML_STR(ML_Plugin_Main)));
	}

	int32_t Plugin::entryPoint(void * data)
	{
		return ((m_main) ? (m_main(data)) : (-1));
	}

	/* * * * * * * * * * * * * * * * * * * * */
}