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
	/* * * * * * * * * * * * * * * * * * * * */

	struct ML_GRAPHICS_API RenderBatch final
		: public ITrackable
	{
		/* * * * * * * * * * * * * * * * * * * * */

		const VAO *		vao;
		const VBO *		vbo;
		const Shader *	shader;
		UniformSet		uniforms;

		/* * * * * * * * * * * * * * * * * * * * */

		RenderBatch(
			const VAO		* vao, 
			const VBO		* vbo, 
			const Shader	* shader, 
			const UniformSet& uniforms)
			: vao		(vao)
			, vbo		(vbo)
			, shader	(shader)
			, uniforms	(uniforms)
		{
		}

		RenderBatch()
			: RenderBatch(NULL, NULL, NULL, UniformSet())
		{
		}

		RenderBatch(const RenderBatch & copy)
			: RenderBatch(copy.vao, copy.vbo, copy.shader, copy.uniforms)
		{
		}

		/* * * * * * * * * * * * * * * * * * * * */
	};

	/* * * * * * * * * * * * * * * * * * * * */
}

#endif // !_ML_RENDER_BATCH_HPP_