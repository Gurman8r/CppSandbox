#include <MemeEngine/LuaScript.hpp>
#include <MemeEngine/Lua.hpp>

namespace ml
{
	/* * * * * * * * * * * * * * * * * * * * */

	LuaScript::LuaScript()
	{
	}

	LuaScript::~LuaScript()
	{
		dispose();
	}

	/* * * * * * * * * * * * * * * * * * * * */

	bool LuaScript::dispose()
	{
		return m_file.dispose();
	}

	bool LuaScript::loadFromFile(const String & filename)
	{
		return m_file.loadFromFile(filename);
	}

	/* * * * * * * * * * * * * * * * * * * * */
}