#include <MemeGraphics/Effect.hpp>
#include <MemeGraphics/OpenGL.hpp>
#include <MemeCore/Debug.hpp>
#include <MemeGraphics/RenderTarget.hpp>

namespace ml
{
	Effect::Effect()
		: m_fbo		()
		, m_rbo		()
		, m_model	(NULL)
		, m_shader	(NULL)
		, m_texture	(NULL)
	{
	}

	Effect::Effect(const Effect & copy)
		: m_fbo		(copy.m_fbo)
		, m_rbo		(copy.m_rbo)
		, m_model	(copy.m_model)
		, m_shader	(copy.m_shader)
		, m_texture	(copy.m_texture)
	{
	}

	Effect::~Effect()
	{
	}


	bool Effect::create(const vec2i & size)
	{
		if (!m_fbo && !m_rbo)
		{
			m_fbo.create().bind();

			m_rbo.create(size[0], size[1]).bind();
			m_rbo.bufferStorage(ml::GL::Depth24_Stencil8);
			m_rbo.setFramebuffer(ml::GL::DepthStencilAttachment);
			m_rbo.unbind();
			
			// Check Framebuffer Status
			if (!ml::OpenGL::checkFramebufferStatus(ml::GL::Framebuffer))
			{
				return ml::Debug::logError("Framebuffer is not complete");
			}

			m_fbo.unbind();

			return true;
		}
		return false;
	}

	void Effect::bind() const
	{
		m_fbo.bind();
	}
	
	void Effect::unbind() const
	{
		m_fbo.unbind();
	}


	bool Effect::setModel(const Model * value)
	{
		return (m_model = value);
	}

	bool Effect::setShader(const Shader * value)
	{
		return (m_shader = value);
	}

	bool Effect::setTexture(const Texture * value)
	{
		if (m_fbo && (m_texture = value))
		{
			m_fbo.bind();

			m_fbo.setTexture(
				GL::ColorAttachment0, 
				(*m_texture),
				m_texture->target(),
				m_texture->level());

			m_fbo.unbind();

			return true;
		}
		return false;
	}


	void Effect::draw(RenderTarget & target, RenderBatch batch) const
	{
		if (m_model && m_shader && m_texture)
		{
			m_shader->setUniform("u_texture", (*m_texture));

			m_shader->bind();

			target.draw((*m_model));
		}
	}
}