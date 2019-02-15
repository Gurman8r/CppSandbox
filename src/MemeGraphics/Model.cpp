#include <MemeGraphics/Model.hpp>

namespace ml
{
	Model::Model()
		: m_shader	(NULL)
		, m_texture	(NULL)
		, m_mesh	(NULL)
	{
	}

	Model::Model(const Model & copy)
		: m_shader	(copy.m_shader)
		, m_texture	(copy.m_texture)
		, m_mesh	(copy.m_mesh)
	{
	}

	Model::~Model()
	{
	}

	bool Model::cleanup()
	{
		return false;
	}

	bool Model::loadFromFile(const String & filename)
	{
		return false;
	}


	void Model::draw(RenderTarget & target, RenderBatch batch) const
	{
	}
}