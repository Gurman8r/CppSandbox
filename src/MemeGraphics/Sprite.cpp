#include <MemeGraphics/Sprite.hpp>

namespace ml
{
	Sprite::Sprite()
	{
	}

	Sprite::~Sprite()
	{
	}

	bool Sprite::cleanup()
	{
		return false;
	}

	bool Sprite::loadFromFile(const String & filename)
	{
		return false;
	}

	
	void Sprite::draw(RenderTarget & target, RenderBatch batch) const
	{
	}
}