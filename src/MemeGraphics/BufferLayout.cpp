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
		: BufferLayout(std::vector<Element>())
	{
	}

	BufferLayout::BufferLayout(const std::vector<Element> & elements)
		: m_elements(elements)
	{
	}

	BufferLayout::BufferLayout(const std::initializer_list<Element> & elements)
		: m_elements(elements.begin(), elements.end())
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
		BufferLayout::bind(*this);
	}

	void BufferLayout::bind(const BufferLayout & value)
	{
		for (const Element & e : value.elements())
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