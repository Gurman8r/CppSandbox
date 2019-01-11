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
		m_flattened = Flatten(vertices());
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