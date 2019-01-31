#ifndef _ML_NETWORK_EXPORT_H_
#define _ML_NETWORK_EXPORT_H_

#include <MemeCore/Config.hpp>

#if defined(ML_NETWORK_EXPORTS)
	#define ML_NETWORK_API ML_API_EXPORT
#else
	#define ML_NETWORK_API ML_API_IMPORT
#endif

#endif // !_ML_NETWORK_EXPORT_H_