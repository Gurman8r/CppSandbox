#ifndef _ML_WINDOW_EXPORT_HPP_
#define _ML_WINDOW_EXPORT_HPP_

#include <MemeCore/Config.hpp>

#if defined(ML_WINDOW_EXPORTS)
	#define ML_WINDOW_API ML_API_EXPORT
#else
	#define ML_WINDOW_API ML_API_IMPORT
#endif

#endif // !_ML_WINDOW_EXPORT_HPP_