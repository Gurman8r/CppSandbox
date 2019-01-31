#ifndef _RENDER_WINDOW_H_
#define _RENDER_WINDOW_H_

#include <MemeWindow/Window.hpp>
#include <MemeGraphics/RenderTarget.hpp>

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

		void onEvent(const Event * ev) override;

	private:

	};
}

#endif // !_RENDER_WINDOW_H_
