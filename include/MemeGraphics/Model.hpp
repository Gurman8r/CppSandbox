#ifndef _MODEL_HPP_
#define _MODEL_HPP_

#include <MemeGraphics/Mesh.hpp>
#include <MemeGraphics/IRenderer.hpp>

namespace ml
{
	class ML_GRAPHICS_API Model
		: public ITrackable
		, public IRenderer
	{
	public:
		Model();
		Model(const Model & copy);
		~Model();

		void draw(RenderTarget & target, RenderBatch batch) const override;

	private:
	};
}

#endif // !_MODEL_HPP_
