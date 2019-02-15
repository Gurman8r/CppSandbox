#ifndef _MODEL_HPP_
#define _MODEL_HPP_

#include <MemeGraphics/IDrawable.hpp>
#include <MemeGraphics/ITransformable.hpp>
#include <MemeGraphics/Mesh.hpp>
#include <MemeGraphics/Shader.hpp>
#include <MemeGraphics/BufferLayout.hpp>
#include <MemeGraphics/VertexArray.hpp>
#include <MemeGraphics/VertexBuffer.hpp>
#include <MemeGraphics/IndexBuffer.hpp>

namespace ml
{
	class ML_GRAPHICS_API Model
		: public ITrackable
		, public IDrawable
		, public IResource
		, public ITransformable
	{
	public:
		Model();
		Model(const Model & copy);
		~Model();

	public:
		bool cleanup() override;
		bool loadFromFile(const String & filename) override;
		bool loadFromMemory(const Mesh & mesh);
		bool loadFromMemory(const FloatList & vertices);
		bool loadFromMemory(const VertexList & vertices);
		bool loadFromMemory(const VertexList & vertices, const IndexList & indices);
		bool loadFromMemory(const FloatList & vertices, const IndexList & indices);

	public:
		void draw(RenderTarget & target, RenderBatch batch) const override;

	private:
		mutable VAO	m_vao;
		mutable VBO	m_vbo;
		mutable IBO	m_ibo;
	};
}

#endif // !_MODEL_HPP_
