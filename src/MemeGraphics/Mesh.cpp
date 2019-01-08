#include <MemeGraphics/Mesh.h>
#include <MemeGraphics/Shapes.h>

namespace ml
{
	Mesh::Mesh()
		: m_primitive(GL::Points)
		, m_vertices()
		, m_indices()
		, m_flattened()
		, m_changed(true)
	{
	}

	Mesh::Mesh(GL::Primitive primitive, const VertexList & vertices, const IndexList & indices)
		: m_primitive(primitive)
		, m_vertices(vertices)
		, m_indices(indices)
		, m_flattened()
		, m_changed(true)
	{
	}

	Mesh::Mesh(const Mesh & copy)
		: m_primitive(copy.m_primitive)
		, m_vertices(copy.m_vertices)
		, m_indices(copy.m_indices)
		, m_flattened(copy.m_flattened)
		, m_changed(copy.m_changed)
	{
	}

	Mesh::~Mesh()
	{
	}

	
	bool Mesh::cleanup()
	{
		return false;
	}
	
	bool Mesh::loadFromFile(const std::string & filename)
	{
		return false;
	}

	const FloatList & Mesh::Flatten(const VertexList & value)
	{
		static FloatList out;
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
}