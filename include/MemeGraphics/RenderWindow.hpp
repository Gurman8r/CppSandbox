#ifndef _ML_RENDER_WINDOW_HPP_
#define _ML_RENDER_WINDOW_HPP_

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

		bool setup() override;
		virtual bool setup(bool experimental);

		void onEvent(const IEvent * ev) override;

	private:

	};
}

#endif // !_ML_RENDER_WINDOW_HPP_