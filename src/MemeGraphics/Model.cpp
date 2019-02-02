#include <MemeGraphics/Model.hpp>

namespace ml
{
	Model::Model()
		: m_meshes()
	{
	}

	Model::Model(const Model & copy)
		: m_meshes(copy.m_meshes)
	{
	}

	Model::~Model()
	{
	}


	void Model::draw(RenderTarget & target, RenderBatch batch) const
	{
	}
}