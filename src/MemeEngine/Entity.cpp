#include <MemeEngine/Entity.hpp>
#include <MemeEngine/Lua.hpp>
#include <MemeEngine/ECS.hpp>
#include <MemeGraphics/RenderTarget.hpp>
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
		for (auto pair : m_map)
		{
			if (ITrackable * trackable = reinterpret_cast<ITrackable *>(pair.second))
			{
				delete trackable;
			}
			else
			{
				delete pair.second;
			}
		}
		m_map.clear();
		return true;
	}

	bool Entity::loadFromFile(const String & filename)
	{
		lua_State * L = luaL_newstate();
		if (L)
		{
			// try parsing Entities with Lua
		}
		lua_close(L);
		return L;
	}

	/* * * * * * * * * * * * * * * * * * * * */
}