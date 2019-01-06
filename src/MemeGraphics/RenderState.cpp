#include <MemeGraphics/RenderState.h>

namespace ml
{
	RenderState::RenderState()
		: texture(NULL)
		, shader(NULL)
		, mesh(NULL)
		, color(Color::White)
		, transform()
		, textureHandle(NULL)
	{
	}

	RenderState::RenderState(const RenderState & copy)
		: texture(copy.texture)
		, shader(copy.shader)
		, mesh(copy.mesh)
		, color(copy.color)
		, transform(copy.transform)
		, textureHandle(copy.textureHandle)
	{
	}
}