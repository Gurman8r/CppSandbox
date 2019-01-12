#include <MemeGraphics/RenderTarget.h>
#include <MemeGraphics/OpenGL.h>

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
	

	RenderTarget & RenderTarget::draw(const IRenderer & value)
	{
		RenderBatch batch;
		value.draw((*this), batch);
		return (*this);
	}

	RenderTarget & RenderTarget::draw(const RenderBatch & batch)
	{
		if (batch.vao && batch.vbo && batch.shader)
		{
			(*batch.vao).bind();

			(*batch.shader)
				.setUniform(Uniform::Proj, (*batch.proj))
				.setUniform(Uniform::Color, (*batch.color))
				.setUniform(Uniform::Texture, (*batch.texture))
				.bind();

			(*batch.vbo)
				.bind()
				.bufferSubData((*batch.vertices))
				.unbind();
			
			OpenGL::drawArrays((*batch.vao).mode(), 0, (*batch.vertices).size());

			(*batch.vao).unbind();
		}
		return (*this);
	}

	
	void RenderTarget::setViewport(const vec2i & pos, const vec2i & size) const
	{
		OpenGL::viewport(pos[0], pos[1], size[0], size[1]);
	}
}