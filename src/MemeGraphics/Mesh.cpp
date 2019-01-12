#include <MemeGraphics/Mesh.h>
#include <MemeGraphics/Shapes.h>

namespace ml
{
	Mesh::Mesh()
		: m_mode(GL::Points)
		, m_vertices()
		, m_indices()
		, m_flattened()
		, m_changed(true)
	{
	}

	Mesh::Mesh(GL::Mode mode, const VertexList & vertices, const IndexList & indices)
		: m_mode(mode)
		, m_vertices(vertices)
		, m_indices(indices)
		, m_flattened()
		, m_changed(true)
	{
	}

	Mesh::Mesh(const Mesh & copy)
		: m_mode(copy.m_mode)
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

	
	const GL::Mode & Mesh::mode() const
	{
		return m_mode;
	}

	const VertexList & Mesh::vertices() const
	{
		return m_vertices;
	}

	const IndexList & Mesh::indices() const
	{
		return m_indices;
	}

	const FloatList & Mesh::flattened() const
	{
		if (m_changed)
		{
			m_changed = false;

			update();
		}
		return m_flattened;
	}


	void Mesh::update() const
	{
		m_flattened = Vertex::Flatten(vertices());
	}


	Mesh & Mesh::mode(GL::Mode value)
	{
		if (value != m_mode)
		{
			m_mode = value;
			return changed(true);
		}
		return changed(false);
	}

	Mesh & Mesh::vertices(const VertexList & value)
	{
		m_vertices = value;
		return changed(true);
	}

	Mesh & Mesh::indices(const IndexList & value)
	{
		m_indices = value;
		return changed(false);
	}

}