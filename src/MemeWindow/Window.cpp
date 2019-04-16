#include <MemeWindow/Window.hpp>
#include <MemeWindow/WindowEvents.hpp>
#include <MemeCore/Debug.hpp>
#include <GLFW/glfw3.h>

#define ML_WINDOW	static_cast<GLFWwindow*>
#define ML_MONITOR	static_cast<GLFWmonitor*>

namespace ml
{
	/* * * * * * * * * * * * * * * * * * * * */

	Window::Window()
		: m_window		(NULL)
		, m_monitor		(NULL)
		, m_share		(NULL)
		, m_title		(GetTypeInfo().name())
		, m_context		()
		, m_videoMode	()
		, m_style		(Window::Default)
		, m_position	(vec2i::Zero)
	{
		ML_EventSystem.addListener(WindowEvent::EV_Char,			this);
		ML_EventSystem.addListener(WindowEvent::EV_CursorEnter,		this);
		ML_EventSystem.addListener(WindowEvent::EV_CursorPos,		this);
		ML_EventSystem.addListener(WindowEvent::EV_FramebufferSize,	this);
		ML_EventSystem.addListener(WindowEvent::EV_Key,				this);
		ML_EventSystem.addListener(WindowEvent::EV_MouseButton,		this);
		ML_EventSystem.addListener(WindowEvent::EV_Scroll,			this);
		ML_EventSystem.addListener(WindowEvent::EV_WindowClose,		this);
		ML_EventSystem.addListener(WindowEvent::EV_WindowError,		this);
		ML_EventSystem.addListener(WindowEvent::EV_WindowFocus,		this);
		ML_EventSystem.addListener(WindowEvent::EV_WindowSize,		this);
		ML_EventSystem.addListener(WindowEvent::Ev_WindowPos,		this);
	}
	
	Window::~Window() 
	{
		destroy();
		terminate();
	}

	/* * * * * * * * * * * * * * * * * * * * */

	bool Window::create(const String & title, const VideoMode & videoMode, uint32_t style, const Context & context)
	{
		m_title		= title;
		m_videoMode	= videoMode;
		m_style		= style;
		m_context	= context;

		if (glfwInit() == GLFW_TRUE)
		{
			glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, m_context.majorVersion);
			glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, m_context.minorVersion);
			
			glfwWindowHint(GLFW_OPENGL_PROFILE, m_context.profile);
			glfwWindowHint(GLFW_DEPTH_BITS,		m_context.depthBits);
			glfwWindowHint(GLFW_STENCIL_BITS,	m_context.stencilBits);
			glfwWindowHint(GLFW_SRGB_CAPABLE,	m_context.srgbCapable);
			
			glfwWindowHint(GLFW_RESIZABLE,		(m_style & Window::Resizable));
			glfwWindowHint(GLFW_VISIBLE,		(m_style & Window::Visible));
			glfwWindowHint(GLFW_DECORATED,		(m_style & Window::Decorated));
			glfwWindowHint(GLFW_FOCUSED,		(m_style & Window::Focused));
			glfwWindowHint(GLFW_AUTO_ICONIFY,	(m_style & Window::AutoIconify));
			glfwWindowHint(GLFW_FLOATING,		(m_style & Window::Floating));
			glfwWindowHint(GLFW_MAXIMIZED,		(m_style & Window::Maximized));

			if (m_window = ML_WINDOW(glfwCreateWindow(
				width(), 
				height(), 
				m_title.c_str(),
				ML_MONITOR(m_monitor),
				ML_WINDOW(m_share))))
			{
				makeContextCurrent();

				return true;
			}
			return ml::Debug::logError("Failed to Create GLFW Window");
		}
		return ml::Debug::logError("Failed to Initialize GLFW");
	}

	bool Window::setup()
	{
		setErrorCallback([](int32_t code, CString desc) 
		{
			ML_EventSystem.fireEvent(WindowErrorEvent(code, desc));
		});

		setCharCallback([](void * window, uint32_t c)
		{
			ML_EventSystem.fireEvent(CharEvent(c));
		});

		setCursorEnterCallback([](void * window, int32_t entered)
		{
			ML_EventSystem.fireEvent(CursorEnterEvent(entered));
		});

		setCursorPosCallback([](void * window, double x, double y)
		{
			ML_EventSystem.fireEvent(CursorPosEvent(x, y));
		});

		setFramebufferSizeCallback([](void * window, int32_t w, int32_t h)
		{
			ML_EventSystem.fireEvent(FramebufferSizeEvent(w, h));
		});

		setKeyCallback([](void * window, int32_t button, int32_t scan, int32_t action, int32_t mods)
		{
			ML_EventSystem.fireEvent(KeyEvent(button, scan, action,
				(bool)(mods & ML_MOD_SHIFT),
				(bool)(mods & ML_MOD_CTRL),
				(bool)(mods & ML_MOD_ALT),
				(bool)(mods & ML_MOD_SUPER)
			));
		});

		setMouseButtonCallback([](void * window, int32_t button, int32_t action, int32_t mods)
		{
			ML_EventSystem.fireEvent(MouseButtonEvent(button, action, mods));
		});
		
		setScrollCallback([](void * window, double x, double y)
		{
			ML_EventSystem.fireEvent(ScrollEvent(x, y));
		});

		setWindowCloseCallback([](void * window)
		{
			ML_EventSystem.fireEvent(WindowCloseEvent());
		});

		setWindowFocusCallback([](void * window, int32_t focused)
		{
			ML_EventSystem.fireEvent(WindowFocusEvent(focused));
		});
		
		setWindowPosCallback([](void * window, int32_t x, int32_t y)
		{
			ML_EventSystem.fireEvent(WindowPosEvent(x, y));
		});

		setWindowSizeCallback([](void * window, int32_t width, int32_t height)
		{
			ML_EventSystem.fireEvent(WindowSizeEvent(width, height));
		});

		return true;
	}

	void Window::onEvent(const IEvent * value)
	{
		switch (*value)
		{
		case WindowEvent::EV_Char:
			if (const auto * ev = value->as<CharEvent>())
			{
				m_char = (char)ev->value;
			}
			break;
		case WindowEvent::EV_CursorEnter:
			if (const auto * ev = value->as<CursorEnterEvent>())
			{
			}
			break;
		case WindowEvent::EV_CursorPos:
			if (const auto * ev = value->as<CursorPosEvent>())
			{
			}
			break;
		case WindowEvent::EV_FramebufferSize:
			if (const auto * ev = value->as<FramebufferSizeEvent>())
			{
			}
			break;
		case WindowEvent::EV_Key:
			if (const auto * ev = value->as<KeyEvent>())
			{
			}
			break;
		case WindowEvent::EV_MouseButton:
			if (const auto * ev = value->as<MouseButtonEvent>())
			{
			}
			break;
		case WindowEvent::EV_Scroll:
			if (const auto * ev = value->as<ScrollEvent>())
			{
			}
			break;
		case WindowEvent::EV_WindowClose:
			if (const auto * ev = value->as<WindowCloseEvent>())
			{
			}
			break;
		case WindowEvent::EV_WindowError:
			if (const auto * ev = value->as<WindowErrorEvent>())
			{
				Debug::logError("{0}", (*ev));
			}
			break;
		case WindowEvent::EV_WindowFocus:
			if (const auto * ev = value->as<WindowFocusEvent>())
			{
			}
			break;
		case WindowEvent::Ev_WindowPos:
			if (const auto * ev = value->as<WindowPosEvent>())
			{
				m_position = ev->position();
			}
			break;
		case WindowEvent::EV_WindowSize:
			if (const auto * ev = value->as<WindowSizeEvent>())
			{
				m_videoMode.size = (vec2u)ev->size();
			}
			break;
		}
	}

	/* * * * * * * * * * * * * * * * * * * * */

	Window & Window::close()
	{
		glfwSetWindowShouldClose(ML_WINDOW(m_window), GLFW_TRUE);
		return (*this);
	}

	Window & Window::destroy()
	{
		glfwDestroyWindow(ML_WINDOW(m_window));
		return (*this);
	}

	Window & Window::makeContextCurrent()
	{
		glfwMakeContextCurrent(ML_WINDOW(m_window));
		return (*this);
	}
	
	Window & Window::maximize()
	{
		glfwMaximizeWindow(ML_WINDOW(m_window));
		return (*this);
	}

	Window & Window::restore()
	{
		glfwRestoreWindow(ML_WINDOW(m_window));
		return (*this);
	}

	Window & Window::iconify()
	{
		glfwIconifyWindow(ML_WINDOW(m_window));
		return (*this);
	}

	Window & Window::pollEvents()
	{
		glfwPollEvents();
		return (*this);
	}

	Window & Window::swapBuffers()
	{
		glfwSwapBuffers(ML_WINDOW(m_window));
		return (*this);
	}

	Window & Window::terminate()
	{
		glfwTerminate();
		return (*this);
	}

	/* * * * * * * * * * * * * * * * * * * * */

	Window & Window::setClipboardString(const String & value)
	{
		glfwSetClipboardString(ML_WINDOW(m_window), value.c_str());
		return (*this);
	}

	Window & Window::setCursor(void * value)
	{
		glfwSetCursor(ML_WINDOW(m_window), static_cast<GLFWcursor*>(value));
		return (*this);
	}
	
	Window & Window::setCursorPos(const vec2i & value)
	{
		glfwSetCursorPos(ML_WINDOW(m_window), value[0], value[1]);
		return (*this);
	}

	Window & Window::setIcons(const std::vector<Icon> & value)
	{
		if (const uint32_t count = (uint32_t)value.size())
		{
			static std::vector<GLFWimage> temp;

			temp.resize(count);

			for (size_t i = 0; i < count; i++)
			{
				temp[i] = GLFWimage {
					(int32_t)value[i].width,
					(int32_t)value[i].height,
					std::remove_cv_t<uint8_t *>(value[i].pixels),
				};
			}

			glfwSetWindowIcon(ML_WINDOW(m_window), count, &temp[0]);
		}
		else
		{
			glfwSetWindowIcon(ML_WINDOW(m_window), 0, NULL);
		}
		return (*this);
	}

	Window & Window::setInputMode(Cursor::Mode value)
	{
		glfwSetInputMode(ML_WINDOW(m_window), GLFW_CURSOR, value);
		return (*this);
	}

	Window & Window::setPosition(const vec2i & value)
	{
		m_position = value;
		glfwSetWindowPos(ML_WINDOW(m_window), value[0], value[1]);
		return (*this);
	}

	Window & Window::setSize(const vec2u & value)
	{
		m_videoMode.size = value;
		glfwSetWindowSize(ML_WINDOW(m_window), value[0], value[1]);
		return (*this);
	}

	Window & Window::setSwapInterval(int32_t value)
	{
		glfwSwapInterval(value);
		return (*this);
	}

	Window & Window::setTitle(const String & value)
	{
		m_title = value;
		glfwSetWindowTitle(ML_WINDOW(m_window), value.c_str());
		return (*this);
	}

	/* * * * * * * * * * * * * * * * * * * * */

	bool	Window::isFocused() const
	{
		return getAttrib(GLFW_FOCUSED);
	}

	bool	Window::isOpen() const
	{
		return !glfwWindowShouldClose(ML_WINDOW(m_window));
	}
	
	int32_t Window::getAttrib(int32_t value) const
	{
		return glfwGetWindowAttrib(ML_WINDOW(m_window), value);
	}

	char	Window::getChar() const
	{
		static char temp;
		temp = m_char;
		m_char = NULL;
		return temp;
	}

	CString Window::getClipboardString() const
	{
		return (glfwGetClipboardString(ML_WINDOW(m_window)));
	}

	vec2f	Window::getCursorPos() const
	{
		static vec2d temp;
		glfwGetCursorPos(ML_WINDOW(m_window), &temp[0], &temp[1]);
		return (vec2f)temp;
	}

	vec2i	Window::getFramebufferSize() const
	{
		static vec2i temp;
		glfwGetFramebufferSize(ML_WINDOW(m_window), &temp[0], &temp[1]);
		return temp;
	}

	int32_t	Window::getKey(int32_t value) const
	{
		return glfwGetKey(ML_WINDOW(m_window), value);
	}

	int32_t Window::getInputMode() const
	{
		return static_cast<Cursor::Mode>(glfwGetInputMode(ML_WINDOW(m_window), GLFW_CURSOR));
	}

	int32_t	Window::getMouseButton(int32_t value) const
	{
		return glfwGetMouseButton(ML_WINDOW(m_window), value);
	}

	double	Window::getTime() const
	{
		return glfwGetTime();
	}

	/* * * * * * * * * * * * * * * * * * * * */

	void * Window::createCursor(uint32_t value) const
	{
		return glfwCreateStandardCursor(value);
	}

	void Window::destroyCursor(void * value) const
	{
		glfwDestroyCursor(static_cast<GLFWcursor *>(value));
	}

	/* * * * * * * * * * * * * * * * * * * * */

	Window::ErrorFun Window::setErrorCallback(ErrorFun callback)
	{
		return glfwSetErrorCallback(reinterpret_cast<GLFWerrorfun>(callback))
			? callback
			: NULL;
	}
	
	Window::CharFun Window::setCharCallback(CharFun callback)
	{
		return glfwSetCharCallback(ML_WINDOW(m_window), reinterpret_cast<GLFWcharfun>(callback))
			? callback
			: NULL;
	}
	
	Window::CursorEnterFun Window::setCursorEnterCallback(CursorEnterFun callback)
	{
		return glfwSetCursorEnterCallback(ML_WINDOW(m_window), reinterpret_cast<GLFWcursorenterfun>(callback))
			? callback
			: NULL;
	}
	
	Window::CursorPosFun Window::setCursorPosCallback(CursorPosFun callback)
	{
		return glfwSetCursorPosCallback(ML_WINDOW(m_window), reinterpret_cast<GLFWcursorposfun>(callback))
			? callback
			: NULL;
	}
	
	Window::FramebufferSizeFun Window::setFramebufferSizeCallback(FramebufferSizeFun callback)
	{
		return glfwSetFramebufferSizeCallback(ML_WINDOW(m_window), reinterpret_cast<GLFWframebuffersizefun>(callback))
			? callback
			: NULL;
	}
	
	Window::KeyFun Window::setKeyCallback(KeyFun callback)
	{
		return glfwSetKeyCallback(ML_WINDOW(m_window), reinterpret_cast<GLFWkeyfun>(callback))
			? callback
			: NULL;
	}
	
	Window::MouseButtonFun Window::setMouseButtonCallback(MouseButtonFun callback)
	{
		return glfwSetMouseButtonCallback(ML_WINDOW(m_window), reinterpret_cast<GLFWmousebuttonfun>(callback))
			? callback
			: NULL;
	}
	
	Window::ScrollFun Window::setScrollCallback(ScrollFun callback)
	{
		return glfwSetScrollCallback(ML_WINDOW(m_window), reinterpret_cast<GLFWscrollfun>(callback))
			? callback 
			: NULL;
	}
	
	Window::CloseFun Window::setWindowCloseCallback(CloseFun callback)
	{
		return glfwSetWindowCloseCallback(ML_WINDOW(m_window), reinterpret_cast<GLFWwindowclosefun>(callback))
			? callback
			: NULL;
	}
	
	Window::FocusFun Window::setWindowFocusCallback(FocusFun callback)
	{
		return glfwSetWindowFocusCallback(ML_WINDOW(m_window), reinterpret_cast<GLFWwindowfocusfun>(callback))
			? callback
			: NULL;
	}
	
	Window::PositionFun Window::setWindowPosCallback(PositionFun callback)
	{
		return glfwSetWindowPosCallback(ML_WINDOW(m_window), reinterpret_cast<GLFWwindowposfun>(callback))
			? callback
			: NULL;
	}
	
	Window::SizeFun Window::setWindowSizeCallback(SizeFun callback)
	{
		return glfwSetWindowSizeCallback(ML_WINDOW(m_window), reinterpret_cast<GLFWwindowposfun>(callback))
			? callback
			: NULL;
	}

	/* * * * * * * * * * * * * * * * * * * * */
}