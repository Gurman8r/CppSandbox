#include <MemeGraphics/Model.hpp>

namespace ml
{
	Model::Model()
	{
	}

	Model::Model(const Model & copy)
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