#include <MemeGraphics/Mesh.h>
#include <MemeGraphics/Shapes.h>

namespace ml
{
	const Mesh Mesh::Triangle(Primitive::Triangles, Shapes::TriangleVertices, Shapes::TriangleIndices);
	const Mesh Mesh::Quad(Primitive::Triangles, Shapes::QuadVertices, Shapes::QuadIndices);
	const Mesh Mesh::Cube(Primitive::Triangles, Shapes::CubeVertices, Shapes::CubeIndices);

	Mesh::Mesh()
		: m_vertices()
		, m_indices()
		, m_primitive(Primitive::Points)
	{
	}

	Mesh::Mesh(Primitive::Type primitive, const VertexArray & vertices, const IndexArray & indices)
		: m_vertices(vertices)
		, m_indices(indices)
		, m_primitive(primitive)
	{
	}

	Mesh::Mesh(const Mesh & copy)
		: m_vertices(copy.m_vertices)
		, m_indices(copy.m_indices)
		, m_primitive(copy.m_primitive)
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
	
	
	const VertexArray & Mesh::vertices() const
	{
		return m_vertices;
	}
	
	const IndexArray & Mesh::indices() const
	{
		return m_indices;
	}
	
	Primitive::Type Mesh::primitive() const
	{
		return m_primitive;
	}
}