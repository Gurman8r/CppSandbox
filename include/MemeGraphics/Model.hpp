#ifndef _MODEL_HPP_
#define _MODEL_HPP_

#include <MemeGraphics/IRenderer.hpp>
#include <MemeGraphics/Mesh.hpp>
#include <MemeGraphics/Shader.hpp>
#include <MemeGraphics/VertexArray.hpp>
#include <MemeGraphics/VertexBuffer.hpp>
#include <MemeGraphics/IndexBuffer.hpp>

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
		VAO		m_vao;
		VBO		m_vbo;
		IBO		m_ibo;

	};
}

#endif // !_MODEL_HPP_
