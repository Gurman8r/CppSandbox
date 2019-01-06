#include <MemeGraphics/VertexArray.h>

namespace ml
{
	VertexArray::VertexArray()
		: m_values()
		, m_primitive(Primitive::Points)
	{
	}

	VertexArray::VertexArray(Primitive::Type primitive)
		: m_values()
		, m_primitive(primitive)
	{
	}

	VertexArray::VertexArray(const vector_type & values)
		: m_values(values)
		, m_primitive(Primitive::Points)
	{
	}

	VertexArray::VertexArray(Primitive::Type primitive, const vector_type & values)
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

	Primitive::Type VertexArray::primitive() const
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


	FloatArray VertexArray::Flatten(const VertexArray & value)
	{
		FloatArray out(value.size() * Vertex::Size);

		for (std::size_t i = 0; i < out.size(); i++)
		{
			out[i * ml::Vertex::Size + 0] = value[i].position()[0];	// 0 v x
			out[i * ml::Vertex::Size + 1] = value[i].position()[1];	// 1 v y
			out[i * ml::Vertex::Size + 2] = value[i].position()[2];	// 2 v z
			out[i * ml::Vertex::Size + 3] = value[i].color()[0];	// 3 c r
			out[i * ml::Vertex::Size + 4] = value[i].color()[1];	// 4 c g
			out[i * ml::Vertex::Size + 5] = value[i].color()[2];	// 5 c b
			out[i * ml::Vertex::Size + 6] = value[i].color()[3];	// 6 c a
			out[i * ml::Vertex::Size + 7] = value[i].texcoords()[0];// 7 t x
			out[i * ml::Vertex::Size + 8] = value[i].texcoords()[1];// 8 t y
		}

		return out;
	}
}