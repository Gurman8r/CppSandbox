#ifndef _RENDER_WINDOW_H_
#define _RENDER_WINDOW_H_

#include <MemeWindow/Window.h>
#include <MemeGraphics/RenderTarget.h>

namespace ml
{
	class ML_GRAPHICS_API RenderWindow
		: public Window
		, public RenderTarget
	{
	public:
		RenderWindow() {}
		virtual ~RenderWindow() {}

		void validate();
		bool initialize() override;

		

	private:

	};
}

#endif // !_RENDER_WINDOW_H_
