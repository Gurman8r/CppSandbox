#ifndef _ML_EFFECT_HPP_
#define _ML_EFFECT_HPP_

#include <MemeGraphics/FrameBuffer.hpp>
#include <MemeGraphics/RenderBuffer.hpp>
#include <MemeGraphics/Shader.hpp>
#include <MemeGraphics/Model.hpp>

namespace ml
{
	// Effect is a Framebuffer + Renderbuffer
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

		bool create(const vec2i & size, GL::Attachment attachment);
		bool reload(const vec2i & size);
		bool resize(const vec2i & size);

		void bind() const;
		void unbind() const;

		bool setModel(const Model * value);
		bool setShader(const Shader * value);
		bool setTexture(const Texture * value);

		void draw(RenderTarget & target, RenderBatch batch) const override;

	public:
		inline const Model * model() const { return m_model; }
		inline const Shader * shader() const { return m_shader; }
		inline const Texture * texture() const { return m_texture; }

		inline Texture * texture_ref()
		{
			return std::remove_cv_t<Texture *>(m_texture);
		}

	private:
		const Model *	m_model;
		const Shader *	m_shader;
		const Texture *	m_texture;

		vec2i m_size;
		GL::Attachment m_attachment;

		mutable FBO m_fbo;
		mutable RBO m_rbo;
	};
}

#endif // !_ML_EFFECT_HPP_