#ifndef _ML_EDITOR_WINDOW_HPP_
#define _ML_EDITOR_WINDOW_HPP_

#include <MemeEditor/EditorBase.hpp>
#include <MemeCore/IEventListener.hpp>

namespace ml
{
	// Base Window Class
	class ML_EDITOR_API EditorWindow
		: public EditorBase
		, public IEventListener
	{
	public:
		EditorWindow();
		virtual ~EditorWindow();

	public:
		virtual void onEvent(const IEvent * value) override = 0;
		virtual bool draw(CString title, bool * p_open) = 0;
	};
}

#endif // !_ML_EDITOR_WINDOW_HPP_