#ifndef _RENDER_WINDOW_H_
#define _RENDER_WINDOW_H_

#include <MemeWindow/Window.h>
#include <MemeGraphics/RenderTarget.h>
#include <MemeCore/Vector2.h>

namespace ml
{
	class ML_GRAPHICS_API RenderWindow
		: public Window
		, public RenderTarget
	{
	public:
		RenderWindow() {}
		virtual ~RenderWindow() {}

		bool initialize() override;

		RenderWindow & setViewport(const vec2i & pos, const vec2u & size);

	private:

	};
}

#endif // !_RENDER_WINDOW_H_
