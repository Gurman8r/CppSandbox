#ifndef _UNIFORM_HPP_
#define	_UNIFORM_HPP_

#include <MemeGraphics/Export.hpp>
#include <MemeCore/String.hpp>

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
			Tex,
			CurveMode,
			MAX_UNIFORM_ID
		};
		static const String Names[Uniform::ID::MAX_UNIFORM_ID];
	};
}

#endif // !_UNIFORM_HPP_
