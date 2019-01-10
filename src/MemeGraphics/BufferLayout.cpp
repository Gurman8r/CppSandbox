#include <MemeGraphics/BufferLayout.h>
#include <MemeGraphics/OpenGL.h>

namespace ml
{
	void BufferLayout::Element::use() const
	{
		OpenGL::vertexAttribPointer(
			index,
			size,
			type,
			normalized,
			stride,
			offset,
			width);
		OpenGL::enableVertexAttribArray(index);
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