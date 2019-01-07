#ifndef _INDEX_BUFFER_H_
#define _INDEX_BUFFER_H_

#include <MemeGraphics/Enum.h>
#include <MemeCore/ITrackable.h>
#include <vector>

namespace ml
{
	class ML_GRAPHICS_API IndexBuffer final
		: public ITrackable
	{
	public:
		IndexBuffer(Enum::Usage usage, const uint32_t * data, uint32_t size);
		IndexBuffer(Enum::Usage usage, const std::vector<uint32_t> & data);
		~IndexBuffer();

		IndexBuffer & clean();

		void bind() const;
		void unbind() const;

		inline const uint32_t &		id() const { return m_id; }
		inline const uint32_t *		data() const { return m_data; }
		inline const uint32_t &		size() const { return m_count; }
		inline const Enum::Usage &	usage() const { return m_usage; }

	private:
		uint32_t		 m_id;
		const uint32_t * m_data;
		uint32_t		 m_count;
		Enum::Usage		 m_usage;
	};

	using IBO = IndexBuffer;
}

#endif // !_INDEX_BUFFER_H_
