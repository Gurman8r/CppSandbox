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
			reinterpret_cast<const void*>(offset * width)));
		glCheck(glEnableVertexAttribArray(index));
	}
}

namespace ml
{
	BufferLayout::BufferLayout()
		: BufferLayout(Elements())
	{
	}

	BufferLayout::BufferLayout(const Elements & elements)
		: m_elements(elements)
	{
	}

	BufferLayout::BufferLayout(const BufferLayout & copy)
		: BufferLayout(copy.m_elements)
	{
	}

	BufferLayout::~BufferLayout()
	{
	}


	void BufferLayout::use() const
	{
		for (const Element & e : elements())
		{
			e.use();
		}
	}

	BufferLayout & BufferLayout::push_back(const Element & value)
	{
		m_elements.push_back(value);
		return (*this);
	}
	
}