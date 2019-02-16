#include <MemeGraphics/Model.hpp>
#include <MemeGraphics/RenderTarget.hpp>

namespace ml
{
	/* * * * * * * * * * * * * * * * * * * * */

	Model::Model()
		: m_vao		()
		, m_vbo		()
		, m_ibo		()
	{
	}

	Model::Model(const Model & copy)
		: m_vao		(copy.m_vao)
		, m_vbo		(copy.m_vbo)
		, m_ibo		(copy.m_ibo)
	{
	}

	Model::~Model()
	{
	}

	/* * * * * * * * * * * * * * * * * * * * */

	bool Model::cleanup()
	{
		return false;
	}

	bool Model::loadFromFile(const String & filename)
	{
		Mesh mesh;
		return mesh.loadFromFile(filename) && loadFromMemory(mesh);
	}

	bool Model::loadFromMemory(const Mesh & mesh)
	{
		return loadFromMemory(mesh.contiguous());
	}

	bool Model::loadFromMemory(const VertexList & vertices)
	{
		return loadFromMemory(vertices.contiguous());
	}

	bool Model::loadFromMemory(const FloatList & vertices)
	{
		m_vao.create(ml::GL::Triangles);
		m_vao.bind();
		
		m_vbo.create(ml::GL::StaticDraw);
		m_vbo.bind();
		m_vbo.bufferData(vertices);
		
		ml::BufferLayout::Default.bind();
		
		m_vbo.unbind();
		m_vao.unbind();
		
		return (m_vao && m_vbo);
	}

	bool Model::loadFromMemory(const VertexList & vertices, const IndexList & indices)
	{
		return loadFromMemory(vertices.contiguous(), indices);
	}

	bool Model::loadFromMemory(const FloatList & vertices, const IndexList & indices)
	{
		m_vao.create(GL::Triangles);
		m_vao.bind();
		
		m_vbo.create(GL::StaticDraw);
		m_vbo.bind();
		m_vbo.bufferData(vertices);
		
		m_ibo.create(GL::StaticDraw, GL::UnsignedInt);
		m_ibo.bind();
		m_ibo.bufferData(indices);
		
		BufferLayout::Default.bind();
		
		m_ibo.unbind();
		m_vbo.unbind();
		m_vao.unbind();
		
		return (m_vao && m_vbo && m_ibo);
	}

	/* * * * * * * * * * * * * * * * * * * * */

	void Model::draw(RenderTarget & target, RenderBatch batch) const
	{
		if (m_vao && m_vbo)
		{
			if (m_ibo)
			{
				target.draw(m_vao, m_vbo, m_ibo);
			}
			else
			{
				target.draw(m_vao, m_vbo);
			}
		}
	}

	/* * * * * * * * * * * * * * * * * * * * */
}