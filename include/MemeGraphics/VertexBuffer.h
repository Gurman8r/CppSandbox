#ifndef _VERTEX_BUFFER_H_
#define _VERTEX_BUFFER_H_

#include <MemeGraphics/Export.h>
#include <MemeCore/ITrackable.h>

namespace ml
{
	class ML_GRAPHICS_API VertexBuffer final
		: public ITrackable
	{
	public:
		enum Usage
		{
			Stream	= 0x88E0,
			Dynamic = 0x88E8,
			Static	= 0x88E4,
		};

	public:
		VertexBuffer(Usage usage, const void * data, uint32_t size);
		~VertexBuffer();

		VertexBuffer & clean();
		VertexBuffer & bind();
		VertexBuffer & unbind();

		inline const uint32_t & id() const { return m_id; }
		inline const Usage & usage() const { return m_usage; }

	private:
		uint32_t	m_id;
		Usage		m_usage;
	};
}

#endif // !_VERTEX_BUFFER_H_
