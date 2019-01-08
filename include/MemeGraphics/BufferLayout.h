#ifndef _BUFFER_LAYOUT_H_
#define _BUFFER_LAYOUT_H_

#include <MemeGraphics/GL_Enum.h>
#include <MemeCore/ITrackable.h>
#include <vector>

namespace ml
{
	class ML_GRAPHICS_API BufferLayout final
		: public ITrackable
	{
	public:
		struct ML_GRAPHICS_API Element final
		{
			uint32_t	index;
			uint32_t	size;
			GL::Type	type;
			bool		normalized;
			uint32_t	stride;
			uint32_t	offset;
			uint32_t	width;

			void use() const;
		};

		using Elements = std::vector<Element>;

	public:
		BufferLayout();
		BufferLayout(const Elements & elements);
		BufferLayout(const BufferLayout & copy);
		~BufferLayout();

		void use() const;

		BufferLayout & push_back(const Element & value);
		
		inline const Elements & elements() const { return m_elements; }

	private:
		Elements m_elements;
	};
}

#endif // !_BUFFER_LAYOUT_H_
