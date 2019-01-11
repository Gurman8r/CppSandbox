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
		VertexBuffer(GL::Usage usage, const void * data, uint32_t size);
		VertexBuffer(GL::Usage usage, const std::vector<float> & data);
		VertexBuffer(const VertexBuffer & copy);
		~VertexBuffer();

		VertexBuffer & clean();
		VertexBuffer & create(GL::Usage usage, const void * data, uint32_t size);
		VertexBuffer & create(GL::Usage usage, const std::vector<float> & data);
		VertexBuffer & update();

		void bind() const;
		void unbind() const;

		inline const uint32_t &		id() const		{ return m_id; }
		inline const void *			data() const	{ return m_data;}
		inline const GL::Usage &	usage() const	{ return m_usage; }

		inline operator bool() const { return (bool)id(); }

	private:
		uint32_t	 m_id;
		const void * m_data;
		GL::Usage	 m_usage;
		uint32_t	 m_size;
	};

	using VBO = VertexBuffer;
}

#endif // !_VERTEX_BUFFER_H_
