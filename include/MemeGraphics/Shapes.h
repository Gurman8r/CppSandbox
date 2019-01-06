#ifndef _SHAPES_H_
#define _SHAPES_H_

#include <MemeGraphics/VertexArray.h>

namespace ml
{
	class ML_GRAPHICS_API Shapes
	{
	public:
		const static VertexArray	TriangleVertices;
		const static IndexArray		TriangleIndices;

		const static VertexArray	QuadVertices;
		const static IndexArray		QuadIndices;

		const static VertexArray	CubeVertices;
		const static IndexArray		CubeIndices;
	};
}

#endif // !_SHAPES_H_
