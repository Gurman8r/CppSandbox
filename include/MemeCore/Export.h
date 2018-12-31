#ifndef _ML_CORE_EXPORT_H_
#define _ML_CORE_EXPORT_H_

#include <MemeCore/Config.h>

#if defined(ML_CORE_EXPORTS)
	#define ML_CORE_API ML_API_EXPORT
#else
	#define ML_CORE_API ML_API_IMPORT
#endif

#endif // !_ML_CORE_EXPORT_H_