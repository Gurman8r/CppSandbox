#ifndef _ML_ENGINE_EXPORT_HPP_
#define _ML_ENGINE_EXPORT_HPP_

/* * * * * * * * * * * * * * * * * * * * */

#include <MemeCore/Config.hpp>

/* * * * * * * * * * * * * * * * * * * * */

# if defined(ML_ENGINE_EXPORTS)
#	define ML_ENGINE_API ML_API_EXPORT
# else
#	define ML_ENGINE_API ML_API_IMPORT
# endif

/* * * * * * * * * * * * * * * * * * * * */

# ifdef ML_SYSTEM_WINDOWS
#	pragma comment(lib, "lua.lib")
# endif

/* * * * * * * * * * * * * * * * * * * * */

#endif // !_ML_ENGINE_EXPORT_HPP_