#include <MemeWindow/Window.hpp>
#include <MemeWindow/WindowEvents.hpp>
#include <MemeCore/Debug.hpp>
#include <GLFW/glfw3.h>

#define ML_WINDOW	static_cast<GLFWwindow*>
#define ML_MONITOR	static_cast<GLFWmonitor*>

namespace ml
{
	Window::Window()
		: m_window		(NULL)
		, m_monitor		(NULL)
		, m_share		(NULL)
		, m_title		("Window")
		, m_context		()
		, m_videoMode	()
		, m_style		(Window::Style::Default)
		, m_cursorMode	(Window::Cursor::Hidden)
		, m_position	(vec2i::Zero)
	{
		ML_EventSystem.addListener(WindowEvent::EV_WindowResized,			this);
		ML_EventSystem.addListener(WindowEvent::EV_WindowMoved,				this);
		ML_EventSystem.addListener(WindowEvent::EV_WindowClosed,			this);
		ML_EventSystem.addListener(WindowEvent::EV_WindowFocused,			this);
		ML_EventSystem.addListener(WindowEvent::EV_WindowCharTyped,			this);
		ML_EventSystem.addListener(WindowEvent::EV_WindowMouseScroll,		this);
		ML_EventSystem.addListener(WindowEvent::EV_WindowMouseMoved,		this);
		ML_EventSystem.addListener(WindowEvent::EV_WindowMouseEnter,		this);
		ML_EventSystem.addListener(WindowEvent::EV_WindowMouseButton,		this);
		ML_EventSystem.addListener(WindowEvent::EV_WindowKey,				this);
		ML_EventSystem.addListener(WindowEvent::EV_WindowFramebufferResized,this);
	}
	Window::~Window() {}

	bool Window::create(
		const String		& title, 
		const VideoMode		& videoMode, 
		const Style			& style, 
		const Context		& context)
	{
		m_title		= title;
		m_context	= context;
		m_videoMode	= videoMode;
		m_style		= style;

		if (glfwInit() == GL_TRUE)
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
				glfwMakeContextCurrent(ML_WINDOW(m_window));

				return initialize();
			}
			return ml::Debug::logError("Failed to Create GLFW Window");
		}
		return ml::Debug::logError("Failed to Initialize GLFW");
	}

	bool Window::initialize()
	{
		setMouseButtonCallback([](void * window, int32_t button, int32_t action, int32_t mods)
		{
			ML_EventSystem.fireEvent(WindowMouseButtonEvent(button, action, mods));
		});
		
		setScrollCallback([](void * window, double x, double y)
		{
			ML_EventSystem.fireEvent(WindowMouseScrollEvent(x, y));
		});
		
		setCharCallback([](void * window, uint32_t c)
		{
			ML_EventSystem.fireEvent(WindowCharTypedEvent(c));
		});
		
		setKeyCallback([](void * window, int32_t button, int32_t scan, int32_t action, int32_t mods)
		{
			ML_EventSystem.fireEvent(WindowKeyEvent(button, scan, action, mods));
		});
		
		setCursorPosCallback([](void * window, double x, double y)
		{
			ML_EventSystem.fireEvent(WindowMouseMoveEvent(x, y));
		});

		setWindowSizeCallback([](void * window, int32_t w, int32_t h)
		{
			ML_EventSystem.fireEvent(WindowResizedEvent(w, h));
		});
		
		setWindowPosCallback([](void * window, int32_t x, int32_t y)
		{
			ML_EventSystem.fireEvent(WindowMovedEvent(x, y));
		});
		
		setWindowCloseCallback([](void * window)
		{
			ML_EventSystem.fireEvent(WindowClosedEvent());
		});
		
		setWindowFocusCallback([](void * window, int32_t focused)
		{
			ML_EventSystem.fireEvent(WindowFocusedEvent(focused));
		});
		
		setCursorEnterCallback([](void * window, int32_t entered)
		{
			ML_EventSystem.fireEvent(WindowMouseEnterEvent(entered));
		});
		
		setFramebufferSizeCallback([](void * window, int32_t w, int32_t h)
		{
			ML_EventSystem.fireEvent(WindowFramebufferResizedEvent(w, h));
		});

		return true;
	}

	void Window::onEvent(const Event * value)
	{
		switch (value->eventID())
		{
		case WindowEvent::EV_WindowResized:
			if (auto ev = value->Cast<WindowResizedEvent>())
			{
				m_videoMode.size = { (uint32_t)ev->width, (uint32_t)ev->height };

				Debug::log("EV: {0}", *ev);
			}
			break;
		case WindowEvent::EV_WindowMoved:
			if (auto ev = value->Cast<WindowMovedEvent>())
			{
				m_position = { ev->x, ev->y };
			}
			break;
		case WindowEvent::EV_WindowClosed:
			if (auto ev = value->Cast<WindowClosedEvent>())
			{
			}
			break;
		case WindowEvent::EV_WindowFocused:
			if (auto ev = value->Cast<WindowFocusedEvent>())
			{
				m_focused = ev->value;
			}
			break;
		case WindowEvent::EV_WindowCharTyped:
			if (auto ev = value->Cast<WindowCharTypedEvent>())
			{
				m_char = (char)ev->value;
			}
			break;
		case WindowEvent::EV_WindowMouseScroll:
			if (auto ev = value->Cast<WindowMouseScrollEvent>())
			{
				m_scroll = { ev->x, ev->y };
			}
			break;
		case WindowEvent::EV_WindowMouseMoved:
			if (auto ev = value->Cast<WindowMouseMoveEvent>())
			{
			}
			break;
		case WindowEvent::EV_WindowMouseEnter:
			if (auto ev = value->Cast<WindowMouseEnterEvent>())
			{
			}
			break;
		case WindowEvent::EV_WindowFramebufferResized:
			if (auto ev = value->Cast<WindowFramebufferResizedEvent>())
			{
			}
			break;
		}
	}


	Window & Window::close()
	{
		glfwSetWindowShouldClose(ML_WINDOW(m_window), GLFW_TRUE);
		return (*this);
	}

	Window & Window::maximize()
	{
		glfwRestoreWindow(ML_WINDOW(m_window));
		return (*this);
	}

	Window & Window::minimize()
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

	
	Window & Window::setCursor(Cursor value)
	{
		m_cursorMode = value;
		glfwSetInputMode(ML_WINDOW(m_window), GLFW_CURSOR, value);
		return (*this);
	}

	Window & Window::setCursorPos(const vec2i & value)
	{
		glfwSetCursorPos(ML_WINDOW(m_window), value[0], value[1]);
		return (*this);
	}

	Window & Window::setClipboard(const String & value)
	{
		glfwSetClipboardString(ML_WINDOW(m_window), value.c_str());
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

	Window & Window::setTitle(const String & value)
	{
		m_title = value;
		glfwSetWindowTitle(ML_WINDOW(m_window), value.c_str());
		return (*this);
	}


	bool Window::isFocused() const
	{
		return m_focused;
	}

	bool Window::isOpen() const
	{
		return !glfwWindowShouldClose(ML_WINDOW(m_window));
	}

	int32_t Window::getAttrib(int32_t value) const
	{
		return glfwGetWindowAttrib(ML_WINDOW(m_window), value);
	}

	vec2f Window::getCursorPos() const
	{
		static double x, y;
		glfwGetCursorPos(ML_WINDOW(m_window), &x, &y);
		return vec2f((float)x, (float)y);
	}

	char Window::getChar() const
	{
		static char temp = 0;
		temp = m_char;
		m_char = 0;
		return temp;
	}

	String Window::getClipboard() const
	{
		return String(glfwGetClipboardString(ML_WINDOW(m_window)));
	}

	vec2i Window::getFramebufferSize() const
	{
		static vec2i temp;
		glfwGetFramebufferSize(ML_WINDOW(m_window), &temp[0], &temp[1]);
		return temp;
	}

	bool Window::getKey(int32_t value) const
	{
		return (glfwGetKey(ML_WINDOW(m_window), value) == GLFW_PRESS);
	}

	bool Window::getMouseButton(int32_t value) const
	{
		return (glfwGetMouseButton(ML_WINDOW(m_window), value) == GLFW_PRESS);
	}

	vec2f Window::getScroll() const
	{
		return vec2f(m_scroll);
	}

	float Window::getTime() const
	{
		return static_cast<float>(glfwGetTime());
	}


	void Window::setMouseButtonCallback(WindowMouseButtonClbk callback)
	{
		glfwSetMouseButtonCallback(
			ML_WINDOW(m_window),
			reinterpret_cast<GLFWmousebuttonfun>(callback));
	}

	void Window::setScrollCallback(WindowScrollClbk callback)
	{
		glfwSetScrollCallback(
			ML_WINDOW(m_window),
			reinterpret_cast<GLFWscrollfun>(callback));
	}

	void Window::setCharCallback(WindowCharClbk callback)
	{
		glfwSetCharCallback(
			ML_WINDOW(m_window),
			reinterpret_cast<GLFWcharfun>(callback));
	}

	void Window::setKeyCallback(WindowKeyClbk callback)
	{
		glfwSetKeyCallback(
			ML_WINDOW(m_window),
			reinterpret_cast<GLFWkeyfun>(callback));
	}

	void Window::setCursorPosCallback(WindowMouseMovedClbk callback)
	{
		glfwSetCursorPosCallback(
			ML_WINDOW(m_window),
			reinterpret_cast<GLFWcursorposfun>(callback));
	}

	void Window::setWindowSizeCallback(WindowResizedClbk callback)
	{
		glfwSetWindowSizeCallback(
			ML_WINDOW(m_window),
			reinterpret_cast<GLFWwindowposfun>(callback));
	}

	void Window::setWindowPosCallback(WindowMovedClbk callback)
	{
		glfwSetWindowPosCallback(
			ML_WINDOW(m_window), 
			reinterpret_cast<GLFWwindowposfun>(callback));
	}

	void Window::setWindowCloseCallback(WindowClosedClbk callback)
	{
		glfwSetWindowCloseCallback(
			ML_WINDOW(m_window),
			reinterpret_cast<GLFWwindowclosefun>(callback));
	}

	void Window::setWindowFocusCallback(WindowFocusedClbk callback)
	{
		glfwSetWindowFocusCallback(
			ML_WINDOW(m_window),
			reinterpret_cast<GLFWwindowfocusfun>(callback));
	}

	void Window::setCursorEnterCallback(WindowMouseEnterClbk callback)
	{
		glfwSetCursorEnterCallback(
			ML_WINDOW(m_window),
			reinterpret_cast<GLFWcursorenterfun>(callback));
	}

	void Window::setFramebufferSizeCallback(WindowFboSizeClbk callback)
	{
		glfwSetFramebufferSizeCallback(
			ML_WINDOW(m_window),
			reinterpret_cast<GLFWframebuffersizefun>(callback));
	}

}