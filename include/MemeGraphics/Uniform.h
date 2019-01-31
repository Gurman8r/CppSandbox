#ifndef _UNIFORM_H_
#define	_UNIFORM_H_

#include <MemeGraphics/Export.h>
#include <MemeCore/String.h>

namespace ml
{
	struct ML_GRAPHICS_API Uniform final
	{
		enum ID : uint16_t
		{
			Model,
			Proj,
			View,
			Color,
			Texture,
			CurveMode,
			MAX_UNIFORM_ID
		};
		static const String Names[Uniform::ID::MAX_UNIFORM_ID];
	};
}

#endif // !_UNIFORM_H_
