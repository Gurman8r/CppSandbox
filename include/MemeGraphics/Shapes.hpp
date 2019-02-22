#ifndef _SHAPES_HPP_
#define _SHAPES_HPP_

#include <MemeCore/Array.hpp>
#include <MemeCore/Rect.hpp>
#include <MemeGraphics/Lists.hpp>

namespace ml
{
	class ML_GRAPHICS_API Shapes
	{
	public:
		enum 
		{
			RectSize = Vertex::Size * 6
		};

		using RectQuad = Array<float, RectSize>;

		static RectQuad genSpriteQuad(const FloatRect & r);
		static RectQuad genGlyphQuad(const FloatRect & r);

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

	using RectQuad = typename Shapes::RectQuad;
}

#endif // !_SHAPES_HPP_
