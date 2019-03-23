#ifndef _ML_WINDOW_HPP_
#define _ML_WINDOW_HPP_

#include <MemeCore/Vector4.hpp>
#include <MemeCore/IEventListener.hpp>
#include <MemeCore/INonCopyable.hpp>
#include <MemeWindow/Cursor.hpp>
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
		enum : uint32_t
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
			Default	= Resizable | Decorated | Focused | AutoIconify,
		};

		/* * * * * * * * * * * * * * * * * * * * */

	public:
		Window();
		virtual ~Window();

		/* * * * * * * * * * * * * * * * * * * * */

	public:
		bool create(const String & title, const VideoMode & mode, uint32_t style, const Context	& context);

		virtual void clean();
		virtual bool setup();
		virtual void onEvent(const IEvent * ev) override;

		/* * * * * * * * * * * * * * * * * * * * */

	public:
		Window & close();
		Window & destroy();
		Window & makeContextCurrent();
		Window & maximize();
		Window & restore();
		Window & iconify();
		Window & pollEvents();
		Window & swapBuffers();
		Window & terminate();

		/* * * * * * * * * * * * * * * * * * * * */

	public:
		Window & setClipboardString(const String & value);
		Window & setCursor(void * value);
		Window & setCursorPos(const vec2i & value);
		Window & setIcons(const std::vector<Icon> & value);
		Window & setInputMode(Cursor::Mode value);
		Window & setPosition(const vec2i & value);
		Window & setSize(const vec2u & value);
		Window & setSwapInterval(int32_t value);
		Window & setTitle(const String & value);

		/* * * * * * * * * * * * * * * * * * * * */
		
	public:
		bool	isFocused() const;
		bool	isOpen() const;
		int32_t	getAttrib(int32_t value) const;
		char	getChar() const;
		CString	getClipboardString() const;
		vec2f	getCursorPos() const;
		vec2i	getFramebufferSize() const;
		int32_t	getKey(int32_t value) const;
		int32_t	getInputMode() const;
		int32_t	getMouseButton(int32_t button) const;
		float	getTime() const;

		/* * * * * * * * * * * * * * * * * * * * */

	public:
		inline const Context &		getContext()	const { return m_context; }
		inline const uint32_t &		getStyle()		const { return m_style; }
		inline const vec2i &		getPosition()	const { return m_position; }
		inline const vec2u &		getSize()		const { return m_videoMode.size; }
		inline const String &		getTitle()		const { return m_title; }

		/* * * * * * * * * * * * * * * * * * * * */

	public:
		template <typename T = uint32_t> inline const T width()		  const { return static_cast<T>(getSize()[0]); }
		template <typename T = uint32_t> inline const T height()	  const { return static_cast<T>(getSize()[1]); }
		template <typename T = int32_t>  inline const T frameWidth()  const { return static_cast<T>(getFramebufferSize()[0]); }
		template <typename T = int32_t>  inline const T frameHeight() const { return static_cast<T>(getFramebufferSize()[1]); }

		inline const float aspect() const 
		{
			return ((width() && height()) 
				? (width<float>() / height<float>()) 
				: 0.0f);
		};

		inline const float frameAspect() const 
		{
			return ((frameWidth() && frameHeight())
				? (frameWidth<float>() / frameHeight<float>()) 
				: 0.0f);
		};

		/* * * * * * * * * * * * * * * * * * * * */

	public:
		void *	createCursor(uint32_t value) const;
		void	destroyCursor(void * value) const;

		/* * * * * * * * * * * * * * * * * * * * */

	public:
		using ErrorFun			= void(*)(int32_t, CString);
		using CharFun			= void(*)(void *, uint32_t);
		using CursorEnterFun	= void(*)(void *, int32_t);
		using CursorPosFun		= void(*)(void *, double, double);
		using FramebufferSizeFun= void(*)(void *, int32_t, int32_t);
		using KeyFun			= void(*)(void *, int32_t, int32_t, int32_t, int32_t);
		using MouseButtonFun	= void(*)(void *, int32_t, int32_t, int32_t);
		using ScrollFun			= void(*)(void *, double, double);
		using WindowCloseFun	= void(*)(void *);
		using WindowFocusFun	= void(*)(void *, int32_t);
		using WindowPosFun		= void(*)(void *, int32_t, int32_t);
		using WindowSizeFun		= void(*)(void *, int32_t, int32_t);

		ErrorFun			setErrorCallback(ErrorFun callback);
		CharFun				setCharCallback(CharFun callback);
		CursorEnterFun		setCursorEnterCallback(CursorEnterFun callback);
		CursorPosFun		setCursorPosCallback(CursorPosFun callback);
		FramebufferSizeFun	setFramebufferSizeCallback(FramebufferSizeFun callback);
		KeyFun				setKeyCallback(KeyFun callback);
		MouseButtonFun		setMouseButtonCallback(MouseButtonFun callback);
		ScrollFun			setScrollCallback(ScrollFun callback);
		WindowCloseFun		setWindowCloseCallback(WindowCloseFun callback);
		WindowFocusFun		setWindowFocusCallback(WindowFocusFun callback);
		WindowPosFun		setWindowPosCallback(WindowPosFun callback);
		WindowSizeFun		setWindowSizeCallback(WindowSizeFun callback);

		/* * * * * * * * * * * * * * * * * * * * */

	protected:
		void *			m_window;
		void *			m_monitor;
		void *			m_share;
		Context			m_context;
		VideoMode		m_videoMode;
		uint32_t		m_style;
		vec2i			m_position;
		String			m_title;
		mutable char	m_char;

		/* * * * * * * * * * * * * * * * * * * * */
	};
}
#endif // !_ML_WINDOW_HPP_