#include <MemeGraphics/RenderTarget.hpp>
#include <MemeGraphics/OpenGL.hpp>
#include <MemeCore/Debug.hpp>

namespace ml
{
	// Drawing
	
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
		// Update Shader
		if (batch.shader)
		{
			batch.shader->setUniforms(*batch.uniforms);
			batch.shader->bind();
		}
		
		if (batch.vbo && batch.vbo)
		{
			if (!batch.ibo && batch.vertices)
			{
				(*batch.vbo)
					.bind()
					.bufferSubData(batch.vertices->contiguous(), 0)
					.unbind();
				return draw((*batch.vao), (*batch.vbo));
			}
			else if (batch.ibo && !batch.vertices)
			{
				return draw((*batch.vao), (*batch.vbo), (*batch.ibo));
			}
		}
		return (*this);
	}

	RenderTarget & RenderTarget::draw(VAO & vao, VBO & vbo, IBO & ibo)
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

	RenderTarget & RenderTarget::draw(VAO & vao, VBO & vbo)
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

	
	void RenderTarget::setViewport(const vec2i & pos, const vec2i & size) const
	{
		OpenGL::viewport(pos[0], pos[1], size[0], size[1]);
	}
}