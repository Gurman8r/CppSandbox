#ifndef _ML_RENDER_BATCH_HPP_
#define _ML_RENDER_BATCH_HPP_

#include <MemeGraphics/VertexArray.hpp>
#include <MemeGraphics/VertexBuffer.hpp>
#include <MemeGraphics/IndexBuffer.hpp>
#include <MemeGraphics/FrameBuffer.hpp>
#include <MemeGraphics/RenderBuffer.hpp>
#include <MemeGraphics/Shader.hpp>
#include <MemeGraphics/UniformSet.hpp>

namespace ml
{
	struct ML_GRAPHICS_API RenderBatch final
		: public ITrackable
	{
		/* * * * * * * * * * * * * * * * * * * * */

		const VAO * vao;
		const VBO * vbo;
		
		const VertexList * vertices;
		const Shader * shader;
		UniformSet * uniforms;

		/* * * * * * * * * * * * * * * * * * * * */

		RenderBatch(const VAO * vao, const VBO * vbo, const Shader * shader, UniformSet * uniforms)
			: vao		(vao)
			, vbo		(vbo)
			, shader	(shader)
			, uniforms	(uniforms)
			, vertices	(NULL)
		{
		}
		RenderBatch()
			: vao		(NULL)
			, vbo		(NULL)
			, shader	(NULL)
			, uniforms	(NULL)
			, vertices	(NULL)
		{
		}
		RenderBatch(const RenderBatch & copy)
			: vao		(copy.vao)
			, vbo		(copy.vbo)
			, shader	(copy.shader)
			, uniforms	(copy.uniforms)
			, vertices	(copy.vertices)
		{
		}

		/* * * * * * * * * * * * * * * * * * * * */
	};
}

#endif // !_ML_RENDER_BATCH_HPP_