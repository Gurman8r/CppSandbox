#ifndef _ML_RENDER_STATES_HPP_
#define _ML_RENDER_STATES_HPP_

#include <MemeGraphics/Export.hpp>
#include <MemeCore/ITrackable.hpp>

namespace ml
{
	// WIP
	class ML_GRAPHICS_API RenderStates final
		: public ITrackable
	{
	public:
		RenderStates();
		RenderStates(const RenderStates & copy);
		~RenderStates();

		void backup();
		void restore();

	private:
		uint32_t	last_active_texture;

		int32_t		last_program;
		int32_t		last_texture;
		int32_t		last_sampler;
		int32_t		last_array_buffer;
		int32_t		last_vertex_array;

		int32_t *	last_polygon_mode;
		int32_t *	last_viewport;
		int32_t *	last_scissor_box;

		uint32_t	last_blend_src_rgb;
		uint32_t	last_blend_dst_rgb;
		uint32_t	last_blend_src_alpha;
		uint32_t	last_blend_dst_alpha;
		uint32_t	last_blend_equation_rgb;
		uint32_t	last_blend_equation_alpha;

		bool		last_enable_blend;
		bool		last_enable_cull_face;
		bool		last_enable_depth_test;
		bool		last_enable_scissor_test;
	};
}

#endif // !_ML_RENDER_STATES_HPP_