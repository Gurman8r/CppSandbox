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
	struct ML_GRAPHICS_API RenderBatch final
		: public ITrackable
	{
		RenderBatch()
			: vao(NULL)
			, vbo(NULL)
			, shader(NULL)
		{
		}
		RenderBatch(VAO * vao, VBO * vbo, Shader * shader)
			: vao(vao)
			, vbo(vbo)
			, shader(shader)
		{
		}
		RenderBatch(const RenderBatch & copy)
			: vao(copy.vao)
			, vbo(copy.vbo)
			, shader(copy.shader)
			, texture(copy.texture)
			, vertices(copy.vertices)
			, color(copy.color)
			, proj(copy.proj)
		{
		}

		VAO		* vao;
		VBO		* vbo;
		Shader	* shader;

		const Texture	* texture;
		const FloatList * vertices;
		const vec4f		* color;
		const mat4f		* proj;
	};
}

#endif // !_RENDER_STATE_H_
