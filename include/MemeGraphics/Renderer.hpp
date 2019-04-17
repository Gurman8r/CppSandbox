#ifndef _ML_RENDERER_HPP_
#define _ML_RENDERER_HPP_

#include <MemeGraphics/IDrawable.hpp>
#include <MemeGraphics/RenderFlags.hpp>

namespace ml
{
	/* * * * * * * * * * * * * * * * * * * * */

	class ML_GRAPHICS_API Renderer
		: public ITrackable
		, public IDrawable
	{
	public:
		Renderer(
			const IDrawable		* drawable, 
			const Shader		* shader,
			const RenderFlags	& flags,
			const UniformSet	& uniforms);

		virtual ~Renderer() {}

	public:
		void draw(RenderTarget & target, RenderBatch batch) const override;

	public:
		inline const IDrawable		* drawable()	const { return m_drawable;	}
		inline const Shader			* shader()		const { return m_shader;	}
		inline const RenderFlags	& renderFlags() const { return m_flags;		}
		inline const UniformSet		& uniforms()	const { return m_uniforms;	}

	public:
		inline const IDrawable		* drawable()		  { return m_drawable;	}
		inline const Shader			* shader()			  { return m_shader;	}
		inline		 RenderFlags	& renderFlags()		  { return m_flags;		}
		inline		 UniformSet		& uniforms()		  { return m_uniforms;	}

	private:
		const IDrawable *	m_drawable;
		const Shader *		m_shader;
		RenderFlags			m_flags;
		UniformSet			m_uniforms;
	};

	/* * * * * * * * * * * * * * * * * * * * */
}

#endif // !_ML_RENDERER_HPP_