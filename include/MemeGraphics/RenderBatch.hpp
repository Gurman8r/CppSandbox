#ifndef _ML_RENDER_BATCH_HPP_
#define _ML_RENDER_BATCH_HPP_

#include <MemeGraphics/VertexArray.hpp>
#include <MemeGraphics/VertexBuffer.hpp>
#include <MemeGraphics/Material.hpp>

namespace ml
{
	/* * * * * * * * * * * * * * * * * * * * */

	class ML_GRAPHICS_API RenderBatch final
		: public ITrackable
	{
	public:
		/* * * * * * * * * * * * * * * * * * * * */

		const VAO *	vao;
		const VBO *	vbo;
		Material *	mat;

		/* * * * * * * * * * * * * * * * * * * * */

		RenderBatch()
			: vao(NULL)
			, vbo(NULL)
			, mat()
		{
		}

		RenderBatch(const VAO * vao, const VBO * vbo, Material * mat)
			: vao(vao)
			, vbo(vbo)
			, mat(mat)
		{
		}

		RenderBatch(const RenderBatch & copy)
			: vao(copy.vao)
			, vbo(copy.vbo)
			, mat(copy.mat)
		{
		}

		~RenderBatch() {}

		/* * * * * * * * * * * * * * * * * * * * */
	};

	/* * * * * * * * * * * * * * * * * * * * */
}

#endif // !_ML_RENDER_BATCH_HPP_