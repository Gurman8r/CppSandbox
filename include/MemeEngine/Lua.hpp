#ifndef _ML_LUA_HPP_
#define _ML_LUA_HPP_

/* * * * * * * * * * * * * * * * * * * * */

#include <MemeCore/Config.hpp>

/* * * * * * * * * * * * * * * * * * * * */

extern "C"
{
# include <lua/lua.h>
# include <lua/lauxlib.h>
# include <lua/lualib.h>
}

# ifdef ML_SYSTEM_WINDOWS
#	pragma comment(lib, "lua.lib")
# endif

/* * * * * * * * * * * * * * * * * * * * */

#endif // !_ML_LUA_HPP_