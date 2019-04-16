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
		return lib().dispose();
	}

	bool Plugin::loadFromFile(const String & filename)
	{
		return lib().loadFromFile(filename);
	}

	/* * * * * * * * * * * * * * * * * * * * */
}