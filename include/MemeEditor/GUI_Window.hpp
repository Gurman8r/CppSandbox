#ifndef _ML_GUI_WINDOW_HPP_
#define _ML_GUI_WINDOW_HPP_

#include <MemeEditor/GUI_Base.hpp>
#include <MemeCore/IEventListener.hpp>

namespace ml
{
	// Base class for Windows
	class ML_EDITOR_API GUI_Window
		: public GUI_Base
		, public IEventListener
	{
	public:
		GUI_Window();
		virtual ~GUI_Window();

	public:
		virtual void onEvent(const IEvent * value) override = 0;
		virtual bool draw(CString title, bool * p_open) = 0;
	};
}

#endif // !_ML_GUI_WINDOW_HPP_