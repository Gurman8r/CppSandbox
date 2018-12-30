#ifndef _EXPORT_H_
#define _EXPORT_H_

#include <MemeLib/Config.h>

#if defined(ML_API_EXPORTS)
	#define ML_API ML_API_EXPORT
#else
	#define ML_API ML_API_IMPORT
#endif

#endif // !_EXPORT_H_