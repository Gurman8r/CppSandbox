#ifndef _INDEX_BUFFER_H_
#define _INDEX_BUFFER_H_

#include <MemeCore/ITrackable.h>
#include <MemeGraphics/GL.h>
#include <vector>

namespace ml
{
	class ML_GRAPHICS_API IndexBuffer final
		: public ITrackable
	{
	public:
		IndexBuffer();
		IndexBuffer(const IndexBuffer & copy);
		~IndexBuffer();

		IndexBuffer & clean();
		IndexBuffer & create();

		IndexBuffer & bind();
		IndexBuffer & unbind();

		IndexBuffer & update(GL::Usage usage, const uint32_t * data, uint32_t count);
		IndexBuffer & update(GL::Usage usage, const std::vector<uint32_t> & data);

		inline const uint32_t &		id() const { return m_id; }
		inline const uint32_t *		data() const { return m_data; }
		inline const uint32_t &		count() const { return m_count; }
		inline const GL::Usage &	usage() const { return m_usage; }

		inline operator bool() const { return (bool)id(); }

	private:
		uint32_t		 m_id;
		const uint32_t * m_data;
		uint32_t		 m_count;
		GL::Usage		 m_usage;
	};

	using IBO = IndexBuffer;
}

#endif // !_INDEX_BUFFER_H_
