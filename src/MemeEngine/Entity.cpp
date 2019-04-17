#include <MemeEngine/Entity.hpp>
#include <MemeEngine/Lua.hpp>
#include <MemeEngine/ECS.hpp>
#include <MemeGraphics/RenderTarget.hpp>
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
		dispose();
	}

	/* * * * * * * * * * * * * * * * * * * * */

	bool Entity::dispose()
	{
		for (auto pair : m_cmp)
		{
			if (ITrackable * trackable = (ITrackable *)(pair.second))
			{
				delete trackable;
			}
			else
			{
				delete pair.second;
			}
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