#ifndef _ML_AUDIO_EXPORT_H_
#define _ML_AUDIO_EXPORT_H_

#include <MemeCore/Config.h>

#if defined(ML_AUDIO_EXPORTS)
	#define ML_AUDIO_API ML_API_EXPORT
#else
	#define ML_AUDIO_API ML_API_IMPORT
#endif

#endif // !_ML_AUDIO_EXPORT_H_