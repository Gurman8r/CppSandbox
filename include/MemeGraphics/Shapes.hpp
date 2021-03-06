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
			enum 
			{ 
				Size	= (Vertex::Size * 6), 
				NumUV	= (Size * 2),
			};

			using RectUVs = typename std::array<float, Size * 2>;

			static VertexList genQuadVerts(const FloatRect & r, const RectUVs & uv);

			static Array<float, Size> genQuad(const FloatRect & r, const RectUVs & uv);
			static Array<float, Size> genGlyphQuad(const FloatRect & r);
			static Array<float, Size> genSpriteQuad(const FloatRect & r);
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
}

#endif // !_ML_SHAPES_HPP_