#ifndef _WINDOW_H_
#define _WINDOW_H_

#include <MemeCore/Vector4.h>
#include <MemeCore/EventListener.h>
#include <MemeCore/INonCopyable.h>
#include <MemeWindow/Context.h>
#include <MemeWindow/Icon.h>
#include <MemeWindow/VideoMode.h>

namespace ml
{
	class ML_WINDOW_API Window
		: public ITrackable
		, public INonCopyable
		, public EventListener
	{
	public:
		enum Cursor : uint32_t
		{
			Normal	 = 0x34001,
			Hidden	 = 0x34002,
			Disabled = 0x34003,
		};

	public:
		enum Style : uint32_t
		{
			None		= (0 << 0),
			Resizable	= (1 << 0),
			Visible		= (1 << 1),
			Decorated	= (1 << 2),
			Focused		= (1 << 3),
			AutoIconify	= (1 << 4),
			Floating	= (1 << 5),
			Maximized	= (1 << 6),

			// Resizable | Visible | Decorated | Focused | AutoIconify
			Default	= Resizable | Visible | Decorated | Focused | AutoIconify,
		};
		inline friend Style operator|(Style lhs, Style rhs)
		{
			return (Window::Style)((uint32_t)lhs | (uint32_t)rhs);
		}
		inline friend Style & operator|=(Style & lhs, const Style & rhs)
		{
			return (lhs = (lhs | rhs));
		}

	public:
		Window();
		virtual ~Window();

	public:
		bool create(
			const string	& title, 
			const VideoMode		& mode, 
			const Style			& style, 
			const Context		& context);

		virtual bool initialize();
		virtual void onEvent(const Event * ev) override;

		Window & close();
		Window & maximize();
		Window & minimize();
		Window & pollEvents();
		Window & swapBuffers();

		Window & setCursor(Cursor value);
		Window & setIcons(const std::vector<Icon> & value);
		Window & setPosition(const vec2i & value);
		Window & setSize(const vec2u & value);
		Window & setTitle(const string & value);

		bool	isFocused() const;
		bool	isOpen() const;
		vec2f	getCursorPos() const;
		char	getChar() const;
		bool	getKey(int32_t value) const;
		bool	getMouseButton(int32_t button) const;
		vec2f	getScroll() const;
		float	getTime() const;

		inline const Context &		context()	const { return m_context; }
		inline const VideoMode &	videoMode()	const { return m_videoMode; }
		inline const Style &		style()		const { return m_style; }
		inline const vec2i &		position()	const { return m_position; }
		inline const vec2u &		size()		const { return videoMode().size; }
		inline const string &	title()		const { return m_title; }
		inline const uint32_t &		width()		const { return size()[0]; }
		inline const uint32_t &		height()	const { return size()[1]; }
		inline const float			aspect()	const { return (float)width() / (float)height(); };

		inline bool good() const { return m_window; }

	protected:
		void *		m_window;
		void *		m_monitor;
		void *		m_share;
		Context		m_context;
		VideoMode	m_videoMode;
		Style		m_style;
		Cursor		m_cursorMode;
		vec2i		m_position;
		string	m_title;
		bool		m_focused;
		vec2d		m_scroll;

		mutable char m_char;
	};

}
#endif // !_WINDOW_H_
