#include <MemeGraphics/Effect.hpp>
#include <MemeGraphics/OpenGL.hpp>
#include <MemeCore/Debug.hpp>
#include <MemeGraphics/RenderTarget.hpp>

namespace ml
{
	/* * * * * * * * * * * * * * * * * * * * */

	Effect::Effect()
		: m_fbo		()
		, m_rbo		()
		, m_model	(NULL)
		, m_shader	(NULL)
		, m_texture	(NULL)
	{
	}

	Effect::~Effect()
	{
	}

	/* * * * * * * * * * * * * * * * * * * * */

	bool Effect::cleanup()
	{
		if (m_fbo && m_rbo)
		{
			m_fbo.clean();
			m_rbo.clean();
			return (!m_fbo && !m_rbo);
		}
		return false;
	}

	bool Effect::loadFromFile(const String & filename)
	{
		return true;
	}

	/* * * * * * * * * * * * * * * * * * * * */

	bool Effect::create(const vec2i & size, GL::Attachment attachment)
	{
		if (!m_fbo && !m_rbo)
		{
			m_size = size;
			m_attachment = attachment;

			// Create FBO
			m_fbo.create();
			m_fbo.bind();

			// Create RBO
			m_rbo.create(m_size[0], m_size[1]);
			m_rbo.bind();
			m_rbo.bufferStorage(ml::GL::Depth24_Stencil8);
			m_rbo.setFramebuffer(ml::GL::DepthStencilAttachment);
			m_rbo.unbind();
			
			// Check Framebuffer Status
			if (!ml::OpenGL::checkFramebufferStatus(ml::GL::Framebuffer))
			{
				return ml::Debug::logError("Framebuffer is not complete");
			}

			m_fbo.unbind();

			return (m_fbo && m_rbo);
		}
		return false;
	}

	bool Effect::reload(const vec2i & size)
	{
		if (cleanup())
		{
			create(size, m_attachment);
			setModel(m_model);
			setShader(m_shader);
			setTexture(m_texture);
			return true;
		}
		return false;
	}

	bool Effect::resize(const vec2i & size)
	{
		if (size != vec2i::Zero)
		{
			if (Texture * t = texture_ref())
			{
				t->cleanup();
				t->create(size);
			}
			return reload(size);
		}
		return false;
	}

	/* * * * * * * * * * * * * * * * * * * * */

	void Effect::bind() const
	{
		m_fbo.bind();
	}
	
	void Effect::unbind() const
	{
		m_fbo.unbind();
	}

	/* * * * * * * * * * * * * * * * * * * * */

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
				m_attachment,
				(*m_texture),
				m_texture->target(),
				m_texture->level());

			m_fbo.unbind();

			return true;
		}
		return false;
	}

	/* * * * * * * * * * * * * * * * * * * * */

	void Effect::draw(RenderTarget & target, RenderBatch batch) const
	{
		if (m_model && m_shader && m_texture)
		{
			m_shader->setUniform("Frag.mainTex", (*m_texture));

			m_shader->bind();

			target.draw((*m_model));
		}
	}

	/* * * * * * * * * * * * * * * * * * * * */
}