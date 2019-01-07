#include <MemeGraphics/Mesh.h>
#include <MemeGraphics/Shapes.h>

namespace ml
{
	Mesh::Mesh()
		: m_vertices()
		, m_indices()
	{
	}

	Mesh::Mesh(const VertexArray & vertices, const IndexArray & indices)
		: m_vertices(vertices)
		, m_indices(indices)
	{
	}

	Mesh::Mesh(const Mesh & copy)
		: m_vertices(copy.m_vertices)
		, m_indices(copy.m_indices)
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
}