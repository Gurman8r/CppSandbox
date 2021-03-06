#include <MemeEngine/Plugin.hpp>
#include <MemeCore/Debug.hpp>

namespace ml
{
	/* * * * * * * * * * * * * * * * * * * * */

	Plugin::Plugin()
	{
	}

	Plugin::~Plugin()
	{
		dispose();
	}

	/* * * * * * * * * * * * * * * * * * * * */

	bool Plugin::dispose()
	{
		return m_lib.dispose();
	}

	bool Plugin::loadFromFile(const String & filename)
	{
		return m_lib.loadFromFile(filename);
	}

	/* * * * * * * * * * * * * * * * * * * * */
}