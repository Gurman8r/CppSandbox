#ifndef _RENDER_STATE_H_
#define _RENDER_STATE_H_

#include <MemeGraphics/VertexArray.h>
#include <MemeGraphics/VertexBuffer.h>
#include <MemeGraphics/IndexBuffer.h>
#include <MemeGraphics/FrameBuffer.h>
#include <MemeGraphics/Shader.h>
#include <MemeGraphics/Mesh.h>

namespace ml
{
	struct ML_GRAPHICS_API RenderBatch final : public ITrackable
	{
		RenderBatch(
			VAO * vao, VBO * vbo, IBO * ibo,
			const mat4f * proj, const mat4f * view, const Transform * model,
			Shader * shader)
			: vao(vao)
			, vbo(vbo)
			, ibo(ibo)
			, fbo(NULL)
			, shader(shader)
			, proj(proj)
			, view(view)
			, model(model)
			, color(NULL)
			, texture(NULL)
			, vertices(NULL)
		{
		}
		RenderBatch(VAO * vao, VBO * vbo, IBO * ibo)
			: RenderBatch(vao, vbo, ibo, NULL, NULL, NULL, NULL)
		{
		}
		RenderBatch()
			: RenderBatch(NULL, NULL, NULL)
		{
		}
		RenderBatch(const RenderBatch & copy)
			: vao(copy.vao)
			, vbo(copy.vbo)
			, ibo(copy.ibo)
			, fbo(copy.fbo)
			, shader(copy.shader)
			, proj(copy.proj)
			, view(copy.view)
			, model(copy.model)
			, color(copy.color)
			, texture(copy.texture)
			, vertices(copy.vertices)
		{
		}

		VAO		* vao;
		VBO		* vbo;
		IBO		* ibo;
		FBO		* fbo;
		Shader	* shader;

		const mat4f		* proj;
		const mat4f		* view;
		const Transform	* model;
		const vec4f		* color;
		const Texture	* texture;
		const FloatList	* vertices;
	};
}

#endif // !_RENDER_STATE_H_
