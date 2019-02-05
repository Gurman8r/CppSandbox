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
		using ErrorCallback				= void(*)(int32_t, const char *);
		using CharCallback				= void(*)(void *, uint32_t);
		using CursorEnterCallback		= void(*)(void *, int32_t);
		using CursorPosCallback			= void(*)(void *, double, double);
		using FramebufferSizeCallback	= void(*)(void *, int32_t, int32_t);
		using KeyCallback				= void(*)(void *, int32_t, int32_t, int32_t, int32_t);
		using MouseButtonCallback		= void(*)(void *, int32_t, int32_t, int32_t);
		using ScrollCallback			= void(*)(void *, double, double);
		using WindowCloseCallback		= void(*)(void *);
		using WindowFocusCallback		= void(*)(void *, int32_t);
		using WindowPosCallback			= void(*)(void *, int32_t, int32_t);
		using WindowSizeCallback		= void(*)(void *, int32_t, int32_t);

	public:
		enum CursorMode : uint32_t
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

		inline void * ptr() { return m_window; }

	public:
		bool create(
			const String	& title, 
			const VideoMode	& mode, 
			const Style		& style, 
			const Context	& context);

		virtual bool setup();
		virtual void onEvent(const Event * ev) override;

	public:
		Window & close();
		Window & destroy();
		Window & makeContextCurrent();
		Window & restore();
		Window & iconify();
		Window & pollEvents();
		Window & swapBuffers();
		Window & terminate();

	public:
		Window & setClipboard(const String & value);
		Window & setCursor(CursorMode value);
		Window & setCursorPos(const vec2i & value);
		Window & setIcons(const std::vector<Icon> & value);
		Window & setPosition(const vec2i & value);
		Window & setSize(const vec2u & value);
		Window & setSwapInterval(int32_t value);
		Window & setTitle(const String & value);
		
	public:
		bool	isFocused() const;
		bool	isOpen() const;

		int32_t	getAttrib(int32_t value) const;
		char	getChar() const;
		String	getClipboard() const;
		vec2f	getCursorPos() const;
		vec2i	getFramebufferSize() const;
		bool	getKey(int32_t value) const;
		bool	getMouseButton(int32_t button) const;
		vec2f	getScroll() const;
		float	getTime() const;

	public:
		inline const Context &		context()	const { return m_context; }
		inline const VideoMode &	videoMode()	const { return m_videoMode; }
		inline const Style &		style()		const { return m_style; }
		inline const vec2i &		position()	const { return m_position; }
		inline const vec2u &		size()		const { return videoMode().size; }
		inline const String &		title()		const { return m_title; }
		inline const uint32_t &		width()		const { return size()[0]; }
		inline const uint32_t &		height()	const { return size()[1]; }
		inline const float			aspect()	const { return (float)width() / (float)height(); };
		inline const CursorMode		cursorMode()const { return m_cursorMode; }

	public:
		void setCharCallback(CharCallback callback);
		void setCursorEnterCallback(CursorEnterCallback callback);
		void setCursorPosCallback(CursorPosCallback callback);
		void setErrorCallback(ErrorCallback callback);
		void setFramebufferSizeCallback(FramebufferSizeCallback callback);
		void setKeyCallback(KeyCallback callback);
		void setMouseButtonCallback(MouseButtonCallback callback);
		void setScrollCallback(ScrollCallback callback);
		void setWindowCloseCallback(WindowCloseCallback callback);
		void setWindowFocusCallback(WindowFocusCallback callback);
		void setWindowPosCallback(WindowPosCallback callback);
		void setWindowSizeCallback(WindowSizeCallback callback);

	protected:
		void *		m_window;
		void *		m_monitor;
		void *		m_share;
		Context		m_context;
		VideoMode	m_videoMode;
		Style		m_style;
		CursorMode	m_cursorMode;
		vec2i		m_position;
		String		m_title;
		bool		m_focused;
		vec2d		m_scroll;

		mutable char m_char;
	};
}
#endif // !_WINDOW_HPP_
