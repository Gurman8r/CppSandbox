#include <MemeEngine/Entity.hpp>
#include <MemeEngine/Lua.hpp>
#include <MemeCore/Debug.hpp>

namespace ml
{
	/* * * * * * * * * * * * * * * * * * * * */

	Entity::Entity()
		: m_cmp()
	{
	}

	Entity::~Entity()
	{
		cleanup();
	}

	/* * * * * * * * * * * * * * * * * * * * */

	bool Entity::cleanup()
	{
		for (auto pair : m_cmp)
		{
			if (static_cast<ITrackable *>(pair.second))
			{
				delete static_cast<ITrackable *>(pair.second);
			}
			else
			{
				delete pair.second;
			}
			pair.second = NULL;
		}
		m_cmp.clear();
		return true;
	}

	bool Entity::loadFromFile(const String & filename)
	{
		lua_State * lua = luaL_newstate();
		if (lua)
		{
			// try parsing Entities with Lua
		}
		lua_close(lua);
		return lua;
	}

	/* * * * * * * * * * * * * * * * * * * * */
}