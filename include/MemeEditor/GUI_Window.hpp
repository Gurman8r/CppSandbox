#ifndef _ML_GUI_WINDOW_HPP_
#define _ML_GUI_WINDOW_HPP_

#include <MemeEditor/GUI_Base.hpp>
#include <MemeCore/IEventListener.hpp>
#include <MemeCore/Function.hpp>

namespace ml
{
	class ML_EDITOR_API GUI_Window
		: public GUI_Base
		, public IEventListener
	{
	public:
		using base_type = typename GUI_Window;

	public:
		GUI_Window();
		virtual ~GUI_Window();

	public:
		uint32_t getID() const;

	public:
		virtual void onEvent(const IEvent * value) override = 0;
		virtual bool draw(CString title, bool * p_open) = 0;

	public:
		virtual bool beginDraw(CString title, bool * p_open, int32_t flags = 0);
		virtual bool endDraw();

	public:
		template <
			typename _Fun
		> inline bool drawFun(CString title, bool * p_open, _Fun fun)
		{
			if (draw(title, p_open))
			{
				fun();
			}
			return endDraw();
		}

	public:
		inline CString	title() const { return m_title; }
		inline bool *	open()	const { return m_open; }
		inline int32_t	flags() const { return m_flags; }

	protected:
		CString	m_title;
		bool *	m_open;
		int32_t m_flags;
	};
}

#endif // !_ML_GUI_WINDOW_HPP_