#ifndef _ML_SHAPES_HPP_
#define _ML_SHAPES_HPP_

#include <MemeCore/Array.hpp>
#include <MemeCore/Rect.hpp>
#include <MemeGraphics/VertexList.hpp>

namespace ml
{
	/* * * * * * * * * * * * * * * * * * * * */

	class ML_GRAPHICS_API Shapes final
	{
	public:
		struct ML_GRAPHICS_API RectQuad final
		{
			enum { Size = Vertex::Size * 6 };
			using  array_type = typename Array<float, Size>;
			static array_type genGlyphQuad(const FloatRect & r);
			static array_type genSpriteQuad(const FloatRect & r);
		};

		struct ML_GRAPHICS_API Triangle final
		{
			const static VertexList	Vertices;
			const static List<uint32_t>	Indices;
		};

		struct ML_GRAPHICS_API Quad final
		{
			const static VertexList	Vertices;
			const static List<uint32_t>	Indices;
		};

		struct ML_GRAPHICS_API Cube final
		{
			const static VertexList	Vertices;
			const static List<uint32_t>	Indices;
		};

		struct ML_GRAPHICS_API Sky final
		{
			const static List<float> Vertices;
		};
	};

	/* * * * * * * * * * * * * * * * * * * * */

	using RectQuadArray = typename Shapes::RectQuad::array_type;

	/* * * * * * * * * * * * * * * * * * * * */
}

#endif // !_ML_SHAPES_HPP_