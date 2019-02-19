#ifndef _SHAPES_HPP_
#define _SHAPES_HPP_

#include <MemeCore/Rect.hpp>
#include <MemeGraphics/Lists.hpp>

namespace ml
{
	class ML_GRAPHICS_API Shapes
	{
	public:
		inline static VertexList genQuadVerts(const FloatRect & rect)
		{
			return {
				{ { rect.left(),  rect.bot(), 0.0f }, Color::White, { 1, 1 } },
				{ { rect.left(),  rect.top(), 0.0f }, Color::White, { 1, 0 } },
				{ { rect.right(), rect.top(), 0.0f }, Color::White, { 0, 0 } },
				{ { rect.left(),  rect.bot(), 0.0f }, Color::White, { 1, 1 } },
				{ { rect.right(), rect.top(), 0.0f }, Color::White, { 0, 0 } },
				{ { rect.right(), rect.bot(), 0.0f }, Color::White, { 0, 1 } },
			};
		}
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
