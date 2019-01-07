#include <MemeGraphics/VertexArray.h>
#include <MemeGraphics/OpenGL.h>

namespace ml
{
	VertexArray::VertexArray()
		: m_values()
		, m_primitive(Enum::Primitive::Points)
	{
	}

	VertexArray::VertexArray(Enum::Primitive primitive)
		: m_values()
		, m_primitive(primitive)
	{
	}

	VertexArray::VertexArray(const Vertices & values)
		: m_values(values)
		, m_primitive(Enum::Primitive::Points)
	{
	}

	VertexArray::VertexArray(Enum::Primitive primitive, const Vertices & values)
		: m_values(values)
		, m_primitive(primitive)
	{
	}

	VertexArray::VertexArray(const VertexArray & copy)
		: m_values(copy.m_values)
		, m_primitive(copy.m_primitive)
	{
	}

	VertexArray::~VertexArray()
	{
	}
	

	bool VertexArray::empty() const
	{
		return m_values.empty();
	}
	
	std::size_t VertexArray::size() const
	{
		return m_values.size();
	}

	uint32_t VertexArray::count() const
	{
		return (uint32_t)size() * Vertex::Size;
	}

	Enum::Primitive VertexArray::primitive() const
	{
		return m_primitive;
	}

	const Vertex * VertexArray::ptr() const
	{
		return &m_values[0];
	}
	

	VertexArray & VertexArray::append(const Vertex & value)
	{
		m_values.push_back(value);
		return (*this);
	}

	VertexArray & VertexArray::clear()
	{
		m_values.clear();
		return (*this);
	}


	const FloatArray & VertexArray::Flatten(const VertexArray & value)
	{
		static FloatArray out;
		std::size_t imax = value.size();
		out.resize(imax * Vertex::Size);		
		for (std::size_t i = 0; i < imax; i++)
		{
			out[i * Vertex::Size + 0] = value[i].position()[0];	// [0] p.x
			out[i * Vertex::Size + 1] = value[i].position()[1];	// [1] p.y
			out[i * Vertex::Size + 2] = value[i].position()[2];	// [2] p.z
			out[i * Vertex::Size + 3] = value[i].color()[0];	// [3] c.r
			out[i * Vertex::Size + 4] = value[i].color()[1];	// [4] c.g
			out[i * Vertex::Size + 5] = value[i].color()[2];	// [5] c.b
			out[i * Vertex::Size + 6] = value[i].color()[3];	// [6] c.a
			out[i * Vertex::Size + 7] = value[i].texcoords()[0];// [7] t.x
			out[i * Vertex::Size + 8] = value[i].texcoords()[1];// [8] t.y
		}
		return out;
	}	
	
	void VertexArray::GenVAO(uint32_t count, uint32_t & vao)
	{
		glCheck(glGenVertexArrays(count, &vao));
		glCheck(glBindVertexArray(vao));
	}
}