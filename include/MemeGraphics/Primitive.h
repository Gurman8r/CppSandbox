#ifndef _PRIMITIVE_H_
#define _PRIMITIVE_H_

#include <MemeGraphics/Export.h>

namespace ml
{
	class ML_GRAPHICS_API Primitive
	{
	public:
		enum Type
		{
			Points,			// List of individual points
			Lines,			// List of individual lines
			LineLoop,		// List of individual lines but lines automatically close
			LineStrip,		// List of connected lines, a point uses the previous point to form a line
			Triangles,		// List of individual triangles
			TriangleStrip,	// List of connected triangles, a point uses the two previous points to form a triangle
			TriangleFan,	// List of connected triangles, a point uses the common center and the previous point to form a triangle
			
			MAX_PRIMITIVE_TYPE
		};
	};
}

#endif // !_PRIMITIVE_H_
