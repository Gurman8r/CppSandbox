#include <MemeGraphics/RenderStates.hpp>
#include <MemeGraphics/OpenGL.hpp>

namespace ml
{
	/* * * * * * * * * * * * * * * * * * * * */

	RenderStates::RenderStates()
	{
	}

	RenderStates::RenderStates(const RenderStates & copy)
	{
	}

	RenderStates::~RenderStates()
	{
	}

	/* * * * * * * * * * * * * * * * * * * * */

	void RenderStates::backup()
	{
		// Backup GL state
		last_active_texture = ML_GL.getInt(ml::GL::ActiveTexture);
		ML_GL.activeTexture(ml::GL::Texture0);

		last_program		= ML_GL.getInt(ml::GL::CurrentProgram);
		last_texture		= ML_GL.getInt(ml::GL::TextureBinding2D);
		last_sampler		= ML_GL.getInt(ml::GL::SamplerBinding);
		last_array_buffer	= ML_GL.getInt(ml::GL::ArrayBufferBinding);
		last_vertex_array	= ML_GL.getInt(ml::GL::VertexArrayBinding);

		ML_GL.getIntv(ml::GL::PolygonMode, last_polygon_mode);
		ML_GL.getIntv(ml::GL::Viewport, last_viewport);
		ML_GL.getIntv(ml::GL::ScissorBox, last_scissor_box);

		last_blend_src_rgb			= ML_GL.getInt(ml::GL::BlendSourceRGB);
		last_blend_dst_rgb			= ML_GL.getInt(ml::GL::BlendDestRGB);
		last_blend_src_alpha		= ML_GL.getInt(ml::GL::BlendSourceAlpha);
		last_blend_dst_alpha		= ML_GL.getInt(ml::GL::BlendDestAlpha);
		last_blend_equation_rgb		= ML_GL.getInt(ml::GL::BlendEquationRGB);
		last_blend_equation_alpha	= ML_GL.getInt(ml::GL::BlendEquationAlpha);

		last_enable_blend			= ML_GL.isEnabled(ml::GL::Blend);
		last_enable_cull_face		= ML_GL.isEnabled(ml::GL::CullFace);
		last_enable_depth_test		= ML_GL.isEnabled(ml::GL::DepthTest);
		last_enable_scissor_test	= ML_GL.isEnabled(ml::GL::ScissorTest);
	}

	void RenderStates::restore()
	{
		// Restore modified GL state
		ML_GL.useShader(last_program);
		ML_GL.bindTexture(ml::GL::Texture2D, last_texture);
		ML_GL.bindSampler(0, last_sampler);
		ML_GL.activeTexture(last_active_texture);
		ML_GL.bindVertexArray(last_vertex_array);
		ML_GL.bindBuffer(ml::GL::ArrayBuffer, last_array_buffer);
		ML_GL.blendEquationSeparate(last_blend_equation_rgb, last_blend_equation_alpha);
		ML_GL.blendFuncSeparate(last_blend_src_rgb, last_blend_dst_rgb, last_blend_src_alpha, last_blend_dst_alpha);

		if (last_enable_blend) ML_GL.enable(ml::GL::Blend); else ML_GL.disable(ml::GL::Blend);
		if (last_enable_cull_face) ML_GL.enable(ml::GL::CullFace); else ML_GL.disable(ml::GL::CullFace);
		if (last_enable_depth_test) ML_GL.enable(ml::GL::DepthTest); else ML_GL.disable(ml::GL::DepthTest);
		if (last_enable_scissor_test) ML_GL.enable(ml::GL::ScissorTest); else ML_GL.disable(ml::GL::ScissorTest);

		ML_GL.polygonMode(ml::GL::FrontAndBack, (uint32_t)last_polygon_mode[0]);
		ML_GL.viewport(last_viewport[0], last_viewport[1], (int32_t)last_viewport[2], (int32_t)last_viewport[3]);
		ML_GL.scissor(last_scissor_box[0], last_scissor_box[1], (int32_t)last_scissor_box[2], (int32_t)last_scissor_box[3]);
	}

	/* * * * * * * * * * * * * * * * * * * * */
}