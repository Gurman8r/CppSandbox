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
		, m_title		(get_type().name())
		, m_context		()
		, m_videoMode	()
		, m_style		(Window::Default)
		, m_position	(vec2i::Zero)
	{
		ML_EventSystem.addListener(WindowEvent::EV_WindowSize,		this);
		ML_EventSystem.addListener(WindowEvent::Ev_WindowPos,		this);
		ML_EventSystem.addListener(WindowEvent::EV_WindowClose,		this);
		ML_EventSystem.addListener(WindowEvent::EV_WindowFocus,		this);
		ML_EventSystem.addListener(WindowEvent::EV_Char,			this);
		ML_EventSystem.addListener(WindowEvent::EV_Scroll,			this);
		ML_EventSystem.addListener(WindowEvent::EV_CursorPos,		this);
		ML_EventSystem.addListener(WindowEvent::EV_CursorEnter,		this);
		ML_EventSystem.addListener(WindowEvent::EV_MouseButton,		this);
		ML_EventSystem.addListener(WindowEvent::EV_Key,				this);
		ML_EventSystem.addListener(WindowEvent::EV_FramebufferSize,	this);
	}
	
	Window::~Window() 
	{
		destroy();
		terminate();
	}

	/* * * * * * * * * * * * * * * * * * * * */

	bool Window::create(
		const String		& title, 
		const VideoMode		& videoMode, 
		const Style			& style, 
		const Context		& context)
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
			
			glfwWindowHint(GLFW_RESIZABLE,		(m_style & Style::Resizable));
			glfwWindowHint(GLFW_VISIBLE,		(m_style & Style::Visible));
			glfwWindowHint(GLFW_DECORATED,		(m_style & Style::Decorated));
			glfwWindowHint(GLFW_FOCUSED,		(m_style & Style::Focused));
			glfwWindowHint(GLFW_AUTO_ICONIFY,	(m_style & Style::AutoIconify));
			glfwWindowHint(GLFW_FLOATING,		(m_style & Style::Floating));
			glfwWindowHint(GLFW_MAXIMIZED,		(m_style & Style::Maximized));

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
		setErrorCallback([](int32_t code, const char * desc) 
		{
			Debug::logError("Window Error {0}: {1}", code, desc);
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
			ML_EventSystem.fireEvent(KeyEvent(button, scan, action, mods));
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

	void Window::onEvent(const Event * value)
	{
		switch (value->eventID())
		{
		case WindowEvent::EV_Char:
			if (auto ev = value->Cast<CharEvent>())
			{
				m_char = (char)ev->value;
			}
			break;
		case WindowEvent::EV_CursorEnter:
			if (auto ev = value->Cast<CursorEnterEvent>())
			{
			}
			break;
		case WindowEvent::EV_CursorPos:
			if (auto ev = value->Cast<CursorPosEvent>())
			{
			}
			break;
		case WindowEvent::EV_FramebufferSize:
			if (auto ev = value->Cast<FramebufferSizeEvent>())
			{
			}
			break;
		case WindowEvent::EV_Key:
			if (auto ev = value->Cast<KeyEvent>())
			{
			}
			break;
		case WindowEvent::EV_MouseButton:
			if (auto ev = value->Cast<MouseButtonEvent>())
			{
			}
			break;
		case WindowEvent::EV_Scroll:
			if (auto ev = value->Cast<ScrollEvent>())
			{
			}
			break;
		case WindowEvent::EV_WindowClose:
			if (auto ev = value->Cast<WindowCloseEvent>())
			{
			}
			break;
		case WindowEvent::EV_WindowFocus:
			if (auto ev = value->Cast<WindowFocusEvent>())
			{
			}
			break;
		case WindowEvent::Ev_WindowPos:
			if (auto ev = value->Cast<WindowPosEvent>())
			{
				m_position = { ev->x, ev->y };
			}
			break;
		case WindowEvent::EV_WindowSize:
			if (auto ev = value->Cast<WindowSizeEvent>())
			{
				m_videoMode.size = { (uint32_t)ev->width, (uint32_t)ev->height };
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
		static char temp = 0;
		temp = m_char;
		m_char = 0;
		return temp;
	}

	String	Window::getClipboardString() const
	{
		return String(glfwGetClipboardString(ML_WINDOW(m_window)));
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

	float	Window::getTime() const
	{
		return static_cast<float>(glfwGetTime());
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
		if (glfwSetCursorPosCallback(ML_WINDOW(m_window),
			reinterpret_cast<GLFWcursorposfun>(callback)))
		{
			return callback;
		}
		return NULL;
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
	
	Window::WindowCloseFun Window::setWindowCloseCallback(WindowCloseFun callback)
	{
		return glfwSetWindowCloseCallback(ML_WINDOW(m_window), reinterpret_cast<GLFWwindowclosefun>(callback))
			? callback
			: NULL;
	}
	
	Window::WindowFocusFun Window::setWindowFocusCallback(WindowFocusFun callback)
	{
		return glfwSetWindowFocusCallback(ML_WINDOW(m_window), reinterpret_cast<GLFWwindowfocusfun>(callback))
			? callback
			: NULL;
	}
	
	Window::WindowPosFun Window::setWindowPosCallback(WindowPosFun callback)
	{
		return glfwSetWindowPosCallback(ML_WINDOW(m_window), reinterpret_cast<GLFWwindowposfun>(callback))
			? callback
			: NULL;
	}
	
	Window::WindowSizeFun Window::setWindowSizeCallback(WindowSizeFun callback)
	{
		return glfwSetWindowSizeCallback(ML_WINDOW(m_window), reinterpret_cast<GLFWwindowposfun>(callback))
			? callback
			: NULL;
	}

	/* * * * * * * * * * * * * * * * * * * * */
}