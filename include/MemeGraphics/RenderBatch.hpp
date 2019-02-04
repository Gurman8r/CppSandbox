#ifndef _RENDER_STATE_HPP_
#define _RENDER_STATE_HPP_

#include <MemeGraphics/VertexArray.hpp>
#include <MemeGraphics/VertexBuffer.hpp>
#include <MemeGraphics/IndexBuffer.hpp>
#include <MemeGraphics/FrameBuffer.hpp>
#include <MemeGraphics/RenderBuffer.hpp>
#include <MemeGraphics/Shader.hpp>
#include <MemeGraphics/Lists.hpp>

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

		const Shader	 * shader;
		const Transform  * proj;
		const Transform  * view;
		const Transform	 * model;
		const vec4f		 * color;
		const Texture	 * texture;
		const VertexList * vertices;

		/* * * * * * * * * * * * * * * * * * * * */

		RenderBatch(
			VAO * vao, VBO * vbo, const Transform * proj, const Shader * shader)
			: vao		(vao)
			, vbo		(vbo)
			, ibo		(NULL)
			, fbo		(NULL)
			, rbo		(NULL)
			, shader	(shader)
			, proj		(proj)
			, view		(NULL)
			, model		(NULL)
			, color		(NULL)
			, texture	(NULL)
			, vertices	(NULL)
		{
		}
		RenderBatch()
			: vao		(NULL)
			, vbo		(NULL)
			, ibo		(NULL)
			, fbo		(NULL)
			, rbo		(NULL)
			, shader	(NULL)
			, proj		(NULL)
			, view		(NULL)
			, model		(NULL)
			, color		(NULL)
			, texture	(NULL)
			, vertices	(NULL)
		{
		}
		RenderBatch(const RenderBatch & copy)
			: vao		(copy.vao)
			, vbo		(copy.vbo)
			, ibo		(copy.ibo)
			, fbo		(copy.fbo)
			, rbo		(copy.rbo)
			, shader	(copy.shader)
			, proj		(copy.proj)
			, view		(copy.view)
			, model		(copy.model)
			, color		(copy.color)
			, texture	(copy.texture)
			, vertices	(copy.vertices)
		{
		}

		/* * * * * * * * * * * * * * * * * * * * */
	};
}

#endif // !_RENDER_STATE_HPP_
