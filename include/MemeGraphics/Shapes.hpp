#ifndef _SHAPES_HPP_
#define _SHAPES_HPP_

#include <MemeCore/Rect.hpp>
#include <MemeGraphics/Lists.hpp>

namespace ml
{
	class ML_GRAPHICS_API Shapes
	{
	public:
		static VertexList	genQuadVerts(const FloatRect & rect);
		static FloatList	genQuadFloats(const FloatRect & rect);


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

		struct ML_GRAPHICS_API Sky final
		{
			const static FloatList Vertices;
		};
	};
}

#endif // !_SHAPES_HPP_
