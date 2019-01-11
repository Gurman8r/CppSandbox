#include <MemeWindow/Window.h>
#include <MemeCore/DebugUtility.h>
#include <GLFW/glfw3.h>

namespace ml
{
	bool Window::create(
		const std::string & title, 
		const VideoMode & mode, const Style & style, const Context & context)
	{
		m_handle	= NULL;
		m_title		= title;
		m_context	= context;
		m_mode		= mode;
		m_style		= style;
		m_position	= vec2i::Zero;
		m_size		= mode.size();

		if (glfwInit() == GL_TRUE)
		{
			glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, m_context.majorVersion);
			glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, m_context.minorVersion);
			
			glfwWindowHint(GLFW_OPENGL_PROFILE, m_context.profile);
			glfwWindowHint(GLFW_DEPTH_BITS,		m_context.depthBits);
			glfwWindowHint(GLFW_STENCIL_BITS,	m_context.stencilBits);
			glfwWindowHint(GLFW_SRGB_CAPABLE,	m_context.sRgbCapable);

			glfwWindowHint(GLFW_RESIZABLE,		(m_style & Style::Resizable));
			glfwWindowHint(GLFW_VISIBLE,		(m_style & Style::Visible));
			glfwWindowHint(GLFW_DECORATED,		(m_style & Style::Decorated));
			glfwWindowHint(GLFW_FOCUSED,		(m_style & Style::Focused));
			glfwWindowHint(GLFW_AUTO_ICONIFY,	(m_style & Style::AutoIconify));
			glfwWindowHint(GLFW_FLOATING,		(m_style & Style::Floating));
			glfwWindowHint(GLFW_MAXIMIZED,		(m_style & Style::Maximized));

			if (m_handle = static_cast<GLFWwindow*>(glfwCreateWindow(
				this->width(),
				this->height(),
				this->title().c_str(),
				NULL,
				NULL)))
			{
				glfwMakeContextCurrent(static_cast<GLFWwindow*>(m_handle));

				return initialize();
			}
			return ml::Debug::LogError("Failed to Create GLFW Window");
		}
		return ml::Debug::LogError("Failed to Initialize GLFW");
	}

	bool Window::initialize()
	{
		glfwSetWindowSizeCallback(static_cast<GLFWwindow*>(m_handle),
			[](GLFWwindow * window, int32_t width, int32_t height)
		{
			// Window Size Callback
		});

		glfwSetWindowPosCallback(static_cast<GLFWwindow*>(m_handle),
			[](GLFWwindow * window, int32_t width, int32_t h)
		{
			// Window Pos Callback
		});

		glfwSetCharCallback(static_cast<GLFWwindow*>(m_handle),
			[](GLFWwindow * window, uint32_t c)
		{
			// Char Callback
		});

		glfwSetScrollCallback(static_cast<GLFWwindow*>(m_handle),
			[](GLFWwindow * window, double x, double y)
		{
			// Scroll Callback
		});

		glfwSetWindowCloseCallback(static_cast<GLFWwindow*>(m_handle),
			[](GLFWwindow * window)
		{
			// Window Closed Callback
		});

		glfwSetFramebufferSizeCallback(static_cast<GLFWwindow*>(m_handle),
			[](GLFWwindow * window, int32_t width, int32_t height)
		{
			// Framebuffer Resized Callback
		});

		glfwSetWindowFocusCallback(static_cast<GLFWwindow*>(m_handle),
			[](GLFWwindow * window, int32_t focused)
		{
			// Window Focused Callback
		});

		return true;
	}


	Window & Window::close()
	{
		glfwSetWindowShouldClose(static_cast<GLFWwindow*>(m_handle), GLFW_TRUE);
		return (*this);
	}

	Window & Window::maximize()
	{
		glfwRestoreWindow(static_cast<GLFWwindow*>(m_handle));
		return (*this);
	}

	Window & Window::minimize()
	{
		glfwIconifyWindow(static_cast<GLFWwindow*>(m_handle));
		return (*this);
	}

	Window & Window::pollEvents()
	{
		glfwPollEvents();
		return (*this);
	}

	Window & Window::swapBuffers()
	{
		glfwSwapBuffers(static_cast<GLFWwindow*>(m_handle));
		return (*this);
	}

	
	Window & Window::setCursor(CursorMode value)
	{
		m_cursorMode = value;
		glfwSetInputMode(static_cast<GLFWwindow*>(m_handle), GLFW_CURSOR, value);
		return (*this);
	}

	Window & Window::setIcons(const std::vector<Icon> & value)
	{
		if (const uint32_t count = (uint32_t)value.size())
		{
			static std::vector<GLFWimage> temp;

			temp.resize(value.size());

			for (std::size_t i = 0; i < count; i++)
			{
				temp[i] = GLFWimage {
					(int32_t)value[i].width,
					(int32_t)value[i].height,
					std::remove_cv_t<uint8_t *>(value[i].pixels),
				};
			}

			glfwSetWindowIcon(static_cast<GLFWwindow*>(m_handle), count, &temp[0]);
		}
		else
		{
			glfwSetWindowIcon(static_cast<GLFWwindow*>(m_handle), 0, NULL);
		}
		return (*this);
	}

	Window & Window::setPosition(const vec2i & value)
	{
		m_position = value;
		glfwSetWindowPos(static_cast<GLFWwindow*>(m_handle), value[0], value[1]);
		return (*this);
	}

	Window & Window::setSize(const vec2u & value)
	{
		m_size = value;
		glfwSetWindowSize(static_cast<GLFWwindow*>(m_handle), value[0], value[1]);
		return (*this);
	}

	Window & Window::setTitle(const std::string & value)
	{
		m_title = value;
		glfwSetWindowTitle(static_cast<GLFWwindow*>(m_handle), value.c_str());
		return (*this);
	}


	bool Window::isOpen() const
	{
		return !glfwWindowShouldClose(static_cast<GLFWwindow*>(m_handle));
	}

	float Window::getTime() const
	{
		return static_cast<float>(glfwGetTime());
	}
	
}