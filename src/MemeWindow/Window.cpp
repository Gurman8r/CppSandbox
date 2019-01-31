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
		ML_EventSystem.addListener(WindowEvent::EV_WindowResized,	this);
		ML_EventSystem.addListener(WindowEvent::EV_WindowMoved,		this);
		ML_EventSystem.addListener(WindowEvent::EV_WindowChar,		this);
		ML_EventSystem.addListener(WindowEvent::EV_WindowScroll,	this);
		ML_EventSystem.addListener(WindowEvent::EV_WindowClosed,	this);
		ML_EventSystem.addListener(WindowEvent::EV_WindowFocused,	this);
		ML_EventSystem.addListener(WindowEvent::EV_WindowMouseMove,	this);
		ML_EventSystem.addListener(WindowEvent::EV_WindowMouseEnter,this);
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
			return ml::Debug::LogError("Failed to Create GLFW Window");
		}
		return ml::Debug::LogError("Failed to Initialize GLFW");
	}

	bool Window::initialize()
	{
		glfwSetWindowSizeCallback(ML_WINDOW(m_window),
			[](GLFWwindow * window, int32_t w, int32_t h)
		{ ML_EventSystem.fireEvent(WindowResizedEvent(w, h)); });

		glfwSetWindowPosCallback(ML_WINDOW(m_window),
			[](GLFWwindow * window, int32_t x, int32_t y)
		{ ML_EventSystem.fireEvent(WindowMovedEvent(x, y)); });

		glfwSetCharCallback(ML_WINDOW(m_window),
			[](GLFWwindow * window, uint32_t c)
		{ ML_EventSystem.fireEvent(WindowCharEvent(c)); });

		glfwSetScrollCallback(ML_WINDOW(m_window),
			[](GLFWwindow * window, double x, double y)
		{ ML_EventSystem.fireEvent(WindowScrollEvent(x, y)); });

		glfwSetWindowCloseCallback(ML_WINDOW(m_window),
			[](GLFWwindow * window)
		{ ML_EventSystem.fireEvent(WindowClosedEvent()); });

		glfwSetWindowFocusCallback(ML_WINDOW(m_window),
			[](GLFWwindow * window, int32_t focused)
		{ ML_EventSystem.fireEvent(WindowFocusedEvent(focused)); });

		glfwSetCursorPosCallback(ML_WINDOW(m_window),
			[](GLFWwindow * window, double x, double y) 
		{ ML_EventSystem.fireEvent(WindowMouseMoveEvent(x, y)); });

		glfwSetCursorEnterCallback(ML_WINDOW(m_window),
			[](GLFWwindow * window, int32_t entered)
		{ ML_EventSystem.fireEvent(WindowMouseEnterEvent(entered)); });

		return true;
	}

	void Window::onEvent(const Event * value)
	{
		switch (value->eventID())
		{
		case WindowEvent::EV_WindowResized:
			if (auto ev = value->As<WindowResizedEvent>())
			{
				m_videoMode.size = { (uint32_t)ev->width, (uint32_t)ev->height };
			}
			break;
		case WindowEvent::EV_WindowMoved:
			if (auto ev = value->As<WindowMovedEvent>())
			{
				m_position = { ev->x, ev->y };
			}
			break;
		case WindowEvent::EV_WindowChar:
			if (auto ev = value->As<WindowCharEvent>())
			{
				m_char = (char)ev->value;
			}
			break;
		case WindowEvent::EV_WindowScroll:
			if (auto ev = value->As<WindowScrollEvent>())
			{
				m_scroll = { ev->x, ev->y };
			}
			break;
		case WindowEvent::EV_WindowClosed:
			if (auto ev = value->As<WindowClosedEvent>())
			{
			}
			break;
		case WindowEvent::EV_WindowFocused:
			if (auto ev = value->As<WindowFocusedEvent>())
			{
				m_focused = ev->value;
			}
			break;
		case WindowEvent::EV_WindowMouseMove:
			if (auto ev = value->As<WindowMouseMoveEvent>())
			{
			}
			break;
		case WindowEvent::EV_WindowMouseEnter:
			if (auto ev = value->As<WindowMouseEnterEvent>())
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

	Window & Window::setIcons(const std::vector<Icon> & value)
	{
		if (const uint32_t count = (uint32_t)value.size())
		{
			static std::vector<GLFWimage> temp;

			temp.resize(value.size());

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
		return (vec2f)m_scroll;
	}

	float Window::getTime() const
	{
		return static_cast<float>(glfwGetTime());
	}
	
}