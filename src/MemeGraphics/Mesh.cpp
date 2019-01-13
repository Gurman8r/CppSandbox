#include <MemeGraphics/Mesh.h>
#include <MemeGraphics/Shapes.h>

namespace ml
{
	Mesh::Mesh()
		: m_vertices()
		, m_indices()
		, m_textures()
		, m_changed(true)
		, m_contiguous()
	{
	}

	Mesh::Mesh(const Mesh & copy)
		: m_vertices()
		, m_indices()
		, m_textures()
		, m_changed(copy.m_changed)
		, m_contiguous(copy.m_contiguous)
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

	
	Mesh & Mesh::vertices(const VertexList & value)
	{
		if (m_vertices != value)
		{
			return changed(true);
		}
		return changed(false);
	}

	Mesh & Mesh::indices(const IndexList & value)
	{
		if(m_indices != value)
		{
			return changed(true);
		}
		return changed(false);
	}

	Mesh & Mesh::textures(const TextureList & value)
	{
		if(m_textures != value)
		{
			return changed(true);
		}
		return changed(false);
	}


	const FloatList & Mesh::update() const
	{
		if (m_changed)
		{
			m_changed = false;

			m_contiguous = Vertex::Flatten(vertices());
		}
		return m_contiguous;
	}

}