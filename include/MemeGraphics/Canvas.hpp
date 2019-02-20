#ifndef _CANVAS_HPP_
#define _CANVAS_HPP_

#include <MemeGraphics/VertexArray.hpp>
#include <MemeGraphics/VertexBuffer.hpp>
#include <MemeGraphics/Shader.hpp>
#include <MemeGraphics/IDrawable.hpp>

namespace ml
{
	class ML_GRAPHICS_API Canvas
		: public ITrackable
		, public INonCopyable
	{
	public:
		Canvas();
		~Canvas();

		bool create();

		bool setShader(const Shader * value);
		bool setUniforms(const UniformSet * value);

	public:
		inline const Shader * shader() const { return m_shader; }
		inline const UniformSet * uniforms() const { return m_uniforms; }

	private:
		const Shader *		m_shader;
		const UniformSet *	m_uniforms;

		VertexList m_vertices;

		mutable VAO m_vao;
		mutable VBO m_vbo;
	};
}

#endif // !_CANVAS_HPP_
