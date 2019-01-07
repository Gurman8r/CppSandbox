#include <MemeGraphics/BufferLayout.h>
#include <MemeGraphics/OpenGL.h>

namespace ml
{
	void BufferLayout::Element::use() const
	{
		glCheck(glVertexAttribPointer(
			index,
			size,
			type,
			normalized,
			(stride * width),
			(void*)(offset * width)));
		glCheck(glEnableVertexAttribArray(index));
	}
}

namespace ml
{
	BufferLayout::BufferLayout()
		: m_elements(Elements())
	{
	}

	BufferLayout::~BufferLayout()
	{
	}


	BufferLayout & BufferLayout::apply()
	{
		for (auto e : elements())
		{
			e.use();
		}
		return (*this);
	}

	BufferLayout & BufferLayout::push_back(const Element & value)
	{
		m_elements.push_back(value);
		return (*this);
	}
	
}