#ifndef _SHAPES_H_
#define _SHAPES_H_

#include <MemeGraphics/Export.h>
#include <MemeGraphics/Mesh.h>

namespace ml
{
	class ML_GRAPHICS_API Shapes
	{
	public:
		struct ML_GRAPHICS_API Triangle final
		{
			const static VertexList		Vertices;
			const static IndexList		Indices;
			const static Mesh			Mesh;
		};

		struct ML_GRAPHICS_API Quad final
		{
			const static VertexList		Vertices;
			const static IndexList		Indices;
			const static Mesh			Mesh;
		};

		struct ML_GRAPHICS_API Cube final
		{
			const static VertexList		Vertices;
			const static IndexList		Indices;
			const static Mesh			Mesh;
		};
	};
}

#endif // !_SHAPES_H_
