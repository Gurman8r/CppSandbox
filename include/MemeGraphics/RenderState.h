#ifndef _RENDER_STATE_H_
#define _RENDER_STATE_H_

#include <MemeGraphics/Color.h>
#include <MemeGraphics/Texture.h>

namespace ml
{
	struct ML_GRAPHICS_API RenderState final
		: public ITrackable
	{
		RenderState();
		RenderState(const RenderState & copy);
	};
}

#endif // !_RENDER_STATE_H_
