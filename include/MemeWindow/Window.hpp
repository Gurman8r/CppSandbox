#ifndef _WINDOW_HPP_
#define _WINDOW_HPP_

#include <MemeCore/Vector4.hpp>
#include <MemeCore/IEventListener.hpp>
#include <MemeCore/INonCopyable.hpp>
#include <MemeWindow/Context.hpp>
#include <MemeWindow/Icon.hpp>
#include <MemeWindow/VideoMode.hpp>

namespace ml
{
	

	class ML_WINDOW_API Window
		: public ITrackable
		, public INonCopyable
		, public IEventListener
	{
	public:
		using WindowResizedClbk		= void(*)(void *, int32_t, int32_t);
		using WindowMovedClbk		= void(*)(void *, int32_t, int32_t);
		using WindowClosedClbk		= void(*)(void *);
		using WindowFocusedClbk		= void(*)(void *, int32_t);
		using WindowCharClbk		= void(*)(void *, uint32_t);
		using WindowScrollClbk		= void(*)(void *, double, double);
		using WindowMouseMovedClbk	= void(*)(void *, double, double);
		using WindowMouseEnterClbk	= void(*)(void *, int32_t);
		using WindowMouseButtonClbk	= void(*)(void *, int32_t, int32_t, int32_t);
		using WindowKeyClbk			= void(*)(void *, int32_t, int32_t, int32_t, int32_t);
		using WindowFboSizeClbk		= void(*)(void *, int32_t, int32_t);

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
			const String	& title, 
			const VideoMode	& mode, 
			const Style		& style, 
			const Context	& context);

		virtual bool initialize();
		virtual void onEvent(const Event * ev) override;

		Window & close();
		Window & maximize();
		Window & minimize();
		Window & pollEvents();
		Window & swapBuffers();

		Window & setCursor(Cursor value);
		Window & setCursorPos(const vec2i & value);
		Window & setClipboard(const String & value);
		Window & setIcons(const std::vector<Icon> & value);
		Window & setPosition(const vec2i & value);
		Window & setSize(const vec2u & value);
		Window & setTitle(const String & value);

		bool	isFocused() const;
		bool	isOpen() const;

		int32_t	getAttrib(int32_t value) const;
		vec2f	getCursorPos() const;
		char	getChar() const;
		String	getClipboard() const;
		vec2i	getFramebufferSize() const;
		bool	getKey(int32_t value) const;
		bool	getMouseButton(int32_t button) const;
		vec2f	getScroll() const;
		float	getTime() const;

		inline const Context &		context()	const { return m_context; }
		inline const VideoMode &	videoMode()	const { return m_videoMode; }
		inline const Style &		style()		const { return m_style; }
		inline const vec2i &		position()	const { return m_position; }
		inline const vec2u &		size()		const { return videoMode().size; }
		inline const String &		title()		const { return m_title; }
		inline const uint32_t &		width()		const { return size()[0]; }
		inline const uint32_t &		height()	const { return size()[1]; }
		inline const float			aspect()	const { return (float)width() / (float)height(); };
		inline const Cursor			getCursor() const { return m_cursorMode; }

	public:
		void setMouseButtonCallback(WindowMouseButtonClbk callback);
		void setScrollCallback(WindowScrollClbk callback);
		void setCharCallback(WindowCharClbk callback);
		void setKeyCallback(WindowKeyClbk callback);
		void setCursorPosCallback(WindowMouseMovedClbk callback);
		void setWindowSizeCallback(WindowResizedClbk callback);
		void setWindowPosCallback(WindowMovedClbk callback);
		void setWindowCloseCallback(WindowClosedClbk callback);
		void setWindowFocusCallback(WindowFocusedClbk callback);
		void setCursorEnterCallback(WindowMouseEnterClbk callback);
		void setFramebufferSizeCallback(WindowFboSizeClbk callback);

	protected:
		void *		m_window;
		void *		m_monitor;
		void *		m_share;
		Context		m_context;
		VideoMode	m_videoMode;
		Style		m_style;
		Cursor		m_cursorMode;
		vec2i		m_position;
		String		m_title;
		bool		m_focused;
		vec2d		m_scroll;

		mutable char m_char;
	};
}
#endif // !_WINDOW_HPP_
