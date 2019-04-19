#ifndef _ML_RENDERER_HPP_
#define _ML_RENDERER_HPP_

#include <MemeGraphics/IDrawable.hpp>
#include <MemeGraphics/RenderStates.hpp>

namespace ml
{
	/* * * * * * * * * * * * * * * * * * * * */

	class ML_GRAPHICS_API Renderer final
		: public ITrackable
		, public IDrawable
	{
	public:
		Renderer(
			const IDrawable		* drawable,
			const Shader		* shader,
			const RenderStates	& states,
			const UniformSet	& uniforms);

		Renderer();
		Renderer(const Renderer & copy);
		virtual ~Renderer();

	public:
		inline const IDrawable		* drawable()	const { return m_drawable;	}
		inline const Shader			* shader()		const { return m_shader;	}
		inline const RenderStates	& states()		const { return m_states;	}
		inline const UniformSet		& uniforms()	const { return m_uniforms;	}

	public:
		inline const IDrawable *	& drawable()	{ return m_drawable;	}
		inline const Shader *		& shader()		{ return m_shader;		}
		inline RenderStates			& states()		{ return m_states;		}
		inline UniformSet			& uniforms()	{ return m_uniforms;	}

	public:
		void draw(RenderTarget & target, RenderBatch batch) const override;

	private:
		const IDrawable *	m_drawable;
		const Shader *		m_shader;
		RenderStates		m_states;
		UniformSet			m_uniforms;
	};

	/* * * * * * * * * * * * * * * * * * * * */
}

#endif // !_ML_RENDERER_HPP_