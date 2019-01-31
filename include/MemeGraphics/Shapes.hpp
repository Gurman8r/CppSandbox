#ifndef _SHAPES_H_
#define _SHAPES_H_

#include <MemeGraphics/Lists.hpp>

namespace ml
{
	class ML_GRAPHICS_API Shapes
	{
	public:
		struct ML_GRAPHICS_API Triangle final
		{
			const static VertexList	Vertices;
			const static IndexList	Indices;
		};

		struct ML_GRAPHICS_API Quad final
		{
			const static VertexList	Vertices;
			const static IndexList	Indices;
		};

		struct ML_GRAPHICS_API Cube final
		{
			const static VertexList	Vertices;
			const static IndexList	Indices;
		};
	};
}

#endif // !_SHAPES_H_
