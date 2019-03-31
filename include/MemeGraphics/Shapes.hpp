#ifndef _ML_SHAPES_HPP_
#define _ML_SHAPES_HPP_

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

		inline static RectQuad genSpriteQuad(const FloatRect & r);
		inline static RectQuad genGlyphQuad(const FloatRect & r);

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

#endif // !_ML_SHAPES_HPP_