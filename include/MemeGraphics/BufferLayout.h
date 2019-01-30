#ifndef _BUFFER_LAYOUT_H_
#define _BUFFER_LAYOUT_H_

#include <MemeGraphics/GL.h>
#include <MemeCore/ITrackable.h>

namespace ml
{
	class ML_GRAPHICS_API BufferLayout final
		: public ITrackable
	{
	public:
		struct Element final
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

	public:
		BufferLayout();
		BufferLayout(const std::vector<Element> & elements);
		BufferLayout(const std::initializer_list<Element> & elements);
		BufferLayout(const BufferLayout & copy);
		~BufferLayout();

		void bind() const;

		static void bind(const BufferLayout & value);

		BufferLayout & push_back(const Element & value);
		
		inline const std::vector<Element> & elements() const { return m_elements; }

	private:
		std::vector<Element> m_elements;
	};
}

#endif // !_BUFFER_LAYOUT_H_
