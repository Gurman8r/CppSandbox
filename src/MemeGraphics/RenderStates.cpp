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

	bool RenderStates::enable(const uint32_t value) const
	{
		return OpenGL::enable(value);
	}

	bool RenderStates::disable(const uint32_t value) const
	{
		return OpenGL::disable(value);
	}

	/* * * * * * * * * * * * * * * * * * * * */
}