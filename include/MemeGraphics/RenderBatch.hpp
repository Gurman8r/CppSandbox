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
		
		const VertexList * vertices;
		const Shader * shader;
		UniformSet * uniforms;

		/* * * * * * * * * * * * * * * * * * * * */

		RenderBatch(VAO * vao, VBO * vbo, const Shader * shader, UniformSet * uniforms)
			: vao		(vao)
			, vbo		(vbo)
			, vertices	(NULL)
			, shader	(shader)
			, uniforms	(uniforms)
		{
		}
		RenderBatch()
			: vao		(NULL)
			, vbo		(NULL)
			, vertices	(NULL)
			, shader	(NULL)
			, uniforms	(NULL)
		{
		}
		RenderBatch(const RenderBatch & copy)
			: vao		(copy.vao)
			, vbo		(copy.vbo)
			, vertices	(copy.vertices)
			, shader	(copy.shader)
			, uniforms	(copy.uniforms)
		{
		}

		/* * * * * * * * * * * * * * * * * * * * */
	};
}

#endif // !_RENDER_STATE_HPP_
