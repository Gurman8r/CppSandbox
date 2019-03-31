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

		bool enable(const uint32_t value) const;
		bool disable(const uint32_t value) const;

	private:

	};
}

#endif // !_ML_RENDER_STATES_HPP_