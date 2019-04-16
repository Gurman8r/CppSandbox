#ifndef _ML_MODEL_HPP_
#define _ML_MODEL_HPP_

#include <MemeGraphics/IDrawable.hpp>
#include <MemeGraphics/ITransformable.hpp>
#include <MemeGraphics/Mesh.hpp>
#include <MemeGraphics/Material.hpp>
#include <MemeGraphics/BufferLayout.hpp>
#include <MemeGraphics/VertexArray.hpp>
#include <MemeGraphics/VertexBuffer.hpp>
#include <MemeGraphics/IndexBuffer.hpp>

namespace ml
{
	/* * * * * * * * * * * * * * * * * * * * */

	// Model is a Drawable Mesh
	class ML_GRAPHICS_API Model
		: public ITrackable
		, public IDisposable
		, public IDrawable
		, public IReadable
	{
	public:
		Model();
		Model(const Model & copy);
		~Model();

	public:
		bool dispose() override;
		bool loadFromFile(const String & filename) override;
		bool loadFromMemory(const Mesh & mesh);
		bool loadFromMemory(const FloatList & vertices);
		bool loadFromMemory(const VertexList & vertices);
		bool loadFromMemory(const VertexList & vertices, const IndexList & indices);
		bool loadFromMemory(const FloatList & vertices, const IndexList & indices);

	public:
		void draw(RenderTarget & target, RenderBatch batch) const override;

	private:
		VAO	m_vao;
		VBO	m_vbo;
		IBO	m_ibo;
	};

	/* * * * * * * * * * * * * * * * * * * * */
}

#endif // !_ML_MODEL_HPP_