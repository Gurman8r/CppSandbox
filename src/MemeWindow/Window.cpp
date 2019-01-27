#include <MemeWindow/Window.h>
#include <MemeCore/DebugUtility.h>

#include <GLFW/glfw3.h>
#define ML_WINDOW	static_cast<GLFWwindow*>
#define ML_MONITOR	static_cast<GLFWmonitor*>

namespace ml
{
	Window::Window()
		: m_window		(NULL)
		, m_monitor		(NULL)
		, m_share		(NULL)
		, m_title		("Meme Window")
		, m_context		()
		, m_videoMode	()
		, m_style		(Window::Style::Default)
		, m_cursorMode	(Window::Cursor::Hidden)
		, m_position	(vec2i::Zero)
	{
	}
	Window::Window(const Window & copy)
		: m_window		(copy.m_window)
		, m_monitor		(copy.m_monitor)
		, m_share		(copy.m_share)
		, m_title		(copy.m_title)
		, m_context		(copy.m_context)
		, m_videoMode	(copy.m_videoMode)
		, m_style		(copy.m_style)
		, m_cursorMode	(copy.m_cursorMode)
		, m_position	(copy.m_position)
	{
	}
	Window::~Window()
	{
	}

	bool Window::create(
		const std::string	& title, 
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
			[](GLFWwindow * window, int32_t width, int32_t height)
		{
			// Window Size Callback
		});

		glfwSetWindowPosCallback(ML_WINDOW(m_window),
			[](GLFWwindow * window, int32_t width, int32_t h)
		{
			// Window Pos Callback
		});

		glfwSetCharCallback(ML_WINDOW(m_window),
			[](GLFWwindow * window, uint32_t c)
		{
			// Char (Typed) Callback
		});

		glfwSetScrollCallback(ML_WINDOW(m_window),
			[](GLFWwindow * window, double x, double y)
		{
			// Scroll Callback
		});

		glfwSetWindowCloseCallback(ML_WINDOW(m_window),
			[](GLFWwindow * window)
		{
			// Window Closed Callback
		});

		glfwSetFramebufferSizeCallback(ML_WINDOW(m_window),
			[](GLFWwindow * window, int32_t width, int32_t height)
		{
			// Framebuffer Resized Callback
		});

		glfwSetWindowFocusCallback(ML_WINDOW(m_window),
			[](GLFWwindow * window, int32_t focused)
		{
			// Window Focused Callback
		});

		return true;
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

	Window & Window::setTitle(const std::string & value)
	{
		m_title = value;
		glfwSetWindowTitle(ML_WINDOW(m_window), value.c_str());
		return (*this);
	}


	bool Window::isOpen() const
	{
		return !glfwWindowShouldClose(ML_WINDOW(m_window));
	}

	float Window::getTime() const
	{
		return static_cast<float>(glfwGetTime());
	}
	
}