#include <MemeGraphics/RenderTarget.hpp>
#include <MemeGraphics/OpenGL.hpp>

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
			if (batch.proj)
			{
				batch.shader->setUniform(Uniform::Proj, (*batch.proj));
			}
			if (batch.view)
			{
				batch.shader->setUniform(Uniform::View, (*batch.view));
			}
			if (batch.model)
			{
				batch.shader->setUniform(Uniform::Model, (*batch.model));
			}
			if (batch.color)
			{
				batch.shader->setUniform(Uniform::Color, (*batch.color));
			}
			if (batch.texture)
			{
				batch.shader->setUniform(Uniform::Texture, (*batch.texture));
			}

			batch.shader->bind((batch.texture != NULL));
		}
		
		if (batch.vbo && batch.vertices)
		{
			(*batch.vbo)
				.bind()
				.bufferSubData((*batch.vertices).contiguous(), 0)
				.unbind();
		}

		return draw((*batch.vao), (*batch.vbo));

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