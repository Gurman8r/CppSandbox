#ifndef _VERTEX_BUFFER_H_
#define _VERTEX_BUFFER_H_

#include <MemeGraphics/GL.h>
#include <MemeCore/ITrackable.h>
#include <vector>

namespace ml
{
	class ML_GRAPHICS_API VertexBuffer final
		: public ITrackable
	{
	public:
		VertexBuffer();
		VertexBuffer(const VertexBuffer & copy);
		~VertexBuffer();

		VertexBuffer & clean();
		VertexBuffer & create(GL::Usage usage);

		VertexBuffer & bind();
		VertexBuffer & unbind();

		VertexBuffer & bufferData(const void * data, uint32_t size);
		VertexBuffer & bufferData(const std::vector<float> & data);

		VertexBuffer & bufferSubData(const void * data, uint32_t size, uint32_t offset = 0);
		VertexBuffer & bufferSubData(const std::vector<float> & data, uint32_t offset = 0);

		inline const uint32_t &		id()	const { return m_id; }
		inline const void *			data()	const { return m_data;}
		inline const GL::Usage &	usage() const { return m_usage; }
		inline const uint32_t &		size()	const { return m_size; }
		inline const int32_t &		count() const { return m_count; }

		inline operator bool() const { return (bool)id(); }

	private:
		uint32_t		m_id;
		const void *	m_data;
		GL::Usage		m_usage;
		uint32_t		m_size;		// total length of data
		int32_t			m_count;	// vertex count
	};

	using VBO = VertexBuffer;
}

#endif // !_VERTEX_BUFFER_H_
