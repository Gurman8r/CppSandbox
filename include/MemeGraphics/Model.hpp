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
		std::vector<Mesh> m_meshes;
	};
}

#endif // !_MODEL_HPP_
