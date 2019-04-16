#ifndef _ML_LUA_HPP_
#define _ML_LUA_HPP_

#include <MemeEngine/Export.hpp>

/* * * * * * * * * * * * * * * * * * * * */

extern "C"
{
#include <lua/lua.h>
#include <lua/lauxlib.h>
#include <lua/lualib.h>
}

# ifdef ML_SYSTEM_WINDOWS
#	pragma comment(lib, "lua.lib")
# endif

/* * * * * * * * * * * * * * * * * * * * */

#endif // !_ML_LUA_HPP_