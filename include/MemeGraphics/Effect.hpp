#ifndef _ML_EFFECT_HPP_
#define _ML_EFFECT_HPP_

#include <MemeGraphics/FrameBuffer.hpp>
#include <MemeGraphics/RenderBuffer.hpp>
#include <MemeGraphics/Shader.hpp>
#include <MemeGraphics/Model.hpp>

namespace ml
{
	/* * * * * * * * * * * * * * * * * * * * */

	// Effect is Framebuffer + Renderbuffer
	class ML_GRAPHICS_API Effect
		: public ITrackable
		, public IDrawable
		, public IReadable
	{
	public:
		Effect();
		~Effect();

		bool cleanup() override;
		bool loadFromFile(const String & filename) override;

		bool create(const vec2i & size, uint32_t attachment);
		bool resize(const vec2i & size);

		void bind() const;
		void unbind() const;

		bool setModel(const Model * value);
		bool setShader(const Shader * value);

		void draw(RenderTarget & target, RenderBatch batch) const override;

	public:
		inline const Model	 * model()	 const { return m_model;   }
		inline const Shader  * shader()  const { return m_shader;  }
		inline const Texture & texture() const { return m_texture; }
		inline		 Texture & texture()	   { return m_texture; }

	private:
		const Model *	m_model;
		const Shader *	m_shader;
		Texture			m_texture;
		vec2i			m_size;
		uint32_t		m_attachment;
		FBO				m_fbo;
		RBO				m_rbo;
	};

	/* * * * * * * * * * * * * * * * * * * * */
}

#endif // !_ML_EFFECT_HPP_