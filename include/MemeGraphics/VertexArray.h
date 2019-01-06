#ifndef _VERTEX_ARRAY_H_
#define _VERTEX_ARRAY_H_

#include <MemeGraphics/Vertex.h>

namespace ml
{
	class ML_GRAPHICS_API VertexArray final
		: public ITrackable
	{
	public:
		VertexArray();
		~VertexArray();

	private:
		std::vector<Vertex> m_values;
	};
}

#endif // !_VERTEX_ARRAY_H_
