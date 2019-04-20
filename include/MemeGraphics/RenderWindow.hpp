#ifndef _ML_RENDER_WINDOW_HPP_
#define _ML_RENDER_WINDOW_HPP_

#include <MemeWindow/Window.hpp>
#include <MemeGraphics/RenderTarget.hpp>

namespace ml
{
	/* * * * * * * * * * * * * * * * * * * * */

	class ML_GRAPHICS_API RenderWindow
		: public Window
		, public RenderTarget
	{
	public:
		RenderWindow();
		virtual ~RenderWindow();

	public:
		bool setup() override;
		void onEvent(const IEvent * ev) override;
	};

	/* * * * * * * * * * * * * * * * * * * * */
}

#endif // !_ML_RENDER_WINDOW_HPP_