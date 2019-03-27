#include <MemeGraphics/RenderTarget.hpp>
#include <MemeGraphics/OpenGL.hpp>
#include <MemeCore/Debug.hpp>

namespace ml
{
	/* * * * * * * * * * * * * * * * * * * * */
	
	RenderTarget & RenderTarget::clear()
	{
		OpenGL::clear(GL::DepthBufferBit);
		return (*this);
	}
	
	RenderTarget & RenderTarget::clear(const vec4f & color)
	{
		OpenGL::clearColor(color[0], color[1], color[2], color[3]);
		OpenGL::clear(GL::ColorBufferBit | GL::DepthBufferBit);
		return (*this);
	}

	void RenderTarget::setViewport(const vec2i & pos, const vec2i & size) const
	{
		OpenGL::viewport(pos[0], pos[1], size[0], size[1]);
	}
	
	/* * * * * * * * * * * * * * * * * * * * */

	RenderTarget & RenderTarget::draw(const IDrawable & value)
	{
		RenderBatch batch;
		return draw(value, batch);
	}

	RenderTarget & RenderTarget::draw(const IDrawable & value, const RenderBatch & batch)
	{
		value.draw((*this), batch);
		return (*this);
	}

	RenderTarget & RenderTarget::draw(const RenderBatch & batch)
	{
		return (batch.vertices ? draw((*batch.vertices), batch) : (*this));
	}

	/* * * * * * * * * * * * * * * * * * * * */

	RenderTarget & RenderTarget::draw(const VertexList & vertices, const RenderBatch & batch)
	{
		return draw(vertices.contiguous(), batch);
	}

	RenderTarget & RenderTarget::draw(const FloatList & vertices, const RenderBatch & batch)
	{
		return draw(vertices.data(), vertices.size(), batch);
	}

	RenderTarget & RenderTarget::draw(const float * vertices, size_t vertexCount, const RenderBatch & batch)
	{
		if (batch.shader && batch.uniforms)
		{
			batch.shader->applyUniforms(*batch.uniforms);
			batch.shader->bind();
		}

		if (batch.vbo && batch.vbo)
		{
			batch.vbo->bind();
			batch.vbo->bufferSubData(vertices, (uint32_t)vertexCount, 0);
			batch.vbo->unbind();

			return draw((*batch.vao), (*batch.vbo));
		}

		return (*this);
	}

	/* * * * * * * * * * * * * * * * * * * * */

	RenderTarget & RenderTarget::draw(const VAO & vao, const VBO & vbo, const IBO & ibo)
	{
		if (vao && vbo && ibo)
		{
			vao.bind();
			vbo.bind();
			ibo.bind();
			{
				OpenGL::drawElements(vao.mode(), ibo.count(), ibo.type(), NULL);
			}
			ibo.unbind();
			vbo.unbind();
			vao.unbind();
		}
		return (*this);
	}
	
	RenderTarget & RenderTarget::draw(const VAO & vao, const VBO & vbo)
	{
		if (vao && vbo)
		{
			vao.bind();
			vbo.bind();
			{
				OpenGL::drawArrays(vao.mode(), 0, vbo.size());
			}
			vbo.unbind();
			vao.unbind();
		}
		return (*this);
	}

	/* * * * * * * * * * * * * * * * * * * * */
}