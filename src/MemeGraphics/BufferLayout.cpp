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
		: BufferLayout(Elements(), false)
	{
	}

	BufferLayout::BufferLayout(const Elements & elements, bool dontUse)
		: m_elements(elements)
	{
		if (!dontUse)
		{
			use();
		}
	}

	BufferLayout::BufferLayout(const BufferLayout & copy, bool dontUse)
		: BufferLayout(copy.m_elements, dontUse)
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