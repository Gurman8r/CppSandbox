#ifndef _UNIFORM_H_
#define	_UNIFORM_H_

#include <MemeGraphics/Export.h>
#include <string>

namespace ml
{
	class ML_GRAPHICS_API Uniform final
	{
	public:
		enum ID : uint16_t
		{
			Color,
			Model,
			Proj,
			Texture,
			View,

			MAX_UNIFORM_ID
		};
		const static std::string UniformNames[Uniform::ID::MAX_UNIFORM_ID];
	};
}

#endif // !_UNIFORM_H_
