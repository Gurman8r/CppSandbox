#ifndef _ML_MODEL_HPP_
#define _ML_MODEL_HPP_

#include <MemeGraphics/IDrawable.hpp>
#include <MemeGraphics/Mesh.hpp>
#include <MemeGraphics/BufferLayout.hpp>
#include <MemeGraphics/VertexArray.hpp>
#include <MemeGraphics/VertexBuffer.hpp>
#include <MemeGraphics/IndexBuffer.hpp>

namespace ml
{
	/* * * * * * * * * * * * * * * * * * * * */

	// Model is a Drawable Mesh
	class ML_GRAPHICS_API Model final
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
		bool loadFromMemory(const List<float> & vertices);
		bool loadFromMemory(const VertexList & vertices);
		bool loadFromMemory(const VertexList & vertices, const List<uint32_t> & indices);
		bool loadFromMemory(const List<float> & vertices, const List<uint32_t> & indices);

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