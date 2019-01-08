#ifndef _VERTEX_BUFFER_H_
#define _VERTEX_BUFFER_H_

#include <MemeGraphics/GL_Enum.h>
#include <MemeCore/ITrackable.h>
#include <vector>

namespace ml
{
	class ML_GRAPHICS_API VertexBuffer final
		: public ITrackable
	{
	public:
		VertexBuffer(Enum::Usage usage, const void * data, uint32_t size);
		VertexBuffer(Enum::Usage usage, const std::vector<float> & data);
		VertexBuffer(const VertexBuffer & copy);
		~VertexBuffer();

		VertexBuffer & clean();
		
		void bind() const;
		void unbind() const;

		inline const uint32_t &		id() const		{ return m_id; }
		inline const void *			data() const	{ return m_data;}
		inline const Enum::Usage &	usage() const	{ return m_usage; }

	private:
		uint32_t	 m_id;
		const void * m_data;
		Enum::Usage	 m_usage;
		uint32_t	 m_size;
	};

	using VBO = VertexBuffer;
}

#endif // !_VERTEX_BUFFER_H_
