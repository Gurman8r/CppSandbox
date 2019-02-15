#ifndef _MODEL_HPP_
#define _MODEL_HPP_

#include <MemeGraphics/IDrawable.hpp>
#include <MemeGraphics/ITransformable.hpp>
#include <MemeGraphics/Mesh.hpp>
#include <MemeGraphics/Shader.hpp>
#include <MemeGraphics/VertexArray.hpp>
#include <MemeGraphics/VertexBuffer.hpp>
#include <MemeGraphics/IndexBuffer.hpp>

namespace ml
{
	class ML_GRAPHICS_API Model
		: public ITrackable
		, public IDrawable
		, public IResource
	{
	public:
		Model();
		Model(const Model & copy);
		~Model();

		bool cleanup() override;
		bool loadFromFile(const String & filename) override;

		void draw(RenderTarget & target, RenderBatch batch) const override;

	private:
		const Shader *	m_shader;
		const Texture * m_texture;
		const Mesh *	m_mesh;

		VAO		m_vao;
		VBO		m_vbo;
		IBO		m_ibo;

	};
}

#endif // !_MODEL_HPP_
