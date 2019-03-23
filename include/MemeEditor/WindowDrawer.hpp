#ifndef _ML_WINDOW_DRAWER_HPP_
#define _ML_WINDOW_DRAWER_HPP_

#include <MemeEditor/GUIDrawer.hpp>
#include <MemeCore/IEventListener.hpp>

namespace ml
{
	class ML_EDITOR_API WindowDrawer
		: public GUIDrawer
		, public IEventListener
	{
	public:
		WindowDrawer() {}
		virtual ~WindowDrawer() {}

	public:
		virtual void onEvent(const IEvent * value) override = 0;
		virtual bool draw(CString title, bool * p_open) = 0;
	};
}

#endif // !_ML_WINDOW_DRAWER_HPP_