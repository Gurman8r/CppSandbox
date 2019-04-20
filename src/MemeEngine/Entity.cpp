#include <MemeEngine/Entity.hpp>
#include <MemeEngine/Lua.hpp>
#include <MemeCore/Debug.hpp>

namespace ml
{
	/* * * * * * * * * * * * * * * * * * * * */

	Entity::Entity()
		: m_map()
	{
	}

	Entity::~Entity()
	{
		dispose();
	}

	/* * * * * * * * * * * * * * * * * * * * */

	bool Entity::dispose()
	{
		for (auto & pair : m_map)
		{
			delete pair.second;
		}
		m_map.clear();
		return true;
	}

	bool Entity::loadFromFile(const String & filename)
	{
		if (lua_State * L = luaL_newstate())
		{
			lua_close(L);

			return true;
		}
		return false;
	}

	bool Entity::saveToFile(const String & filename) const
	{
		return true;
	}

	/* * * * * * * * * * * * * * * * * * * * */
}