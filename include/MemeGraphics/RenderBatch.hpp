#ifndef _RENDER_STATE_HPP_
#define _RENDER_STATE_HPP_

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

		VAO	* vao;
		VBO	* vbo;
		IBO	* ibo;
		FBO	* fbo;
		RBO	* rbo;
		
		UniformSet uniforms;

		const Shader * shader;

		const VertexList  * vertices;

		/* * * * * * * * * * * * * * * * * * * * */

		RenderBatch(VAO * vao, VBO * vbo, const Shader * shader, const UniformSet & uniforms)
			: vao		(vao)
			, vbo		(vbo)
			, ibo		(NULL)
			, fbo		(NULL)
			, rbo		(NULL)
			, shader	(shader)
			, vertices	(NULL)
			, uniforms	(uniforms)
		{
		}
		RenderBatch()
			: vao		(NULL)
			, vbo		(NULL)
			, ibo		(NULL)
			, fbo		(NULL)
			, rbo		(NULL)
			, shader	(NULL)
			, vertices	(NULL)
			, uniforms	(UniformSet())
		{
		}
		RenderBatch(const RenderBatch & copy)
			: vao		(copy.vao)
			, vbo		(copy.vbo)
			, ibo		(copy.ibo)
			, fbo		(copy.fbo)
			, rbo		(copy.rbo)
			, shader	(copy.shader)
			, vertices	(copy.vertices)
			, uniforms	(copy.uniforms)
		{
		}

		/* * * * * * * * * * * * * * * * * * * * */
	};
}

#endif // !_RENDER_STATE_HPP_
