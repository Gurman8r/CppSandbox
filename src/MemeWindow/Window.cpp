#include <MemeWindow/Window.h>
#include <MemeWindow/Screen.h>
#include <GLFW/glfw3.h>

namespace ml
{
	Window::ErrorCode Window::create(
		const std::string & title, 
		const VideoMode & mode, 
		const Flags & flags,
		const ContextSettings & settings)
	{
		m_handle	= NULL;
		m_title		= title;
		m_settings	= settings;
		m_mode		= mode;
		m_flags		= flags;
		m_position	= vec2i::Zero;
		m_size		= mode.size();

		if (!getMode().isValid())
		{
			return ErrorCode::ER_Invalid_Mode;
		}

		if (glfwInit() == GL_TRUE)
		{
			glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, m_settings.majorVersion);
			glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, m_settings.minorVersion);
			
			glfwWindowHint(GLFW_OPENGL_PROFILE, m_settings.profile);
			glfwWindowHint(GLFW_DEPTH_BITS,		m_settings.depthBits);
			glfwWindowHint(GLFW_STENCIL_BITS,	m_settings.stencilBits);
			glfwWindowHint(GLFW_SRGB_CAPABLE,	m_settings.sRgbCapable);

			glfwWindowHint(GLFW_RESIZABLE,		(m_flags & Flags::Resizable));
			glfwWindowHint(GLFW_VISIBLE,		(m_flags & Flags::Visible));
			glfwWindowHint(GLFW_DECORATED,		(m_flags & Flags::Decorated));
			glfwWindowHint(GLFW_FOCUSED,		(m_flags & Flags::Focused));
			glfwWindowHint(GLFW_AUTO_ICONIFY,	(m_flags & Flags::AutoIconify));
			glfwWindowHint(GLFW_FLOATING,		(m_flags & Flags::Floating));
			glfwWindowHint(GLFW_MAXIMIZED,		(m_flags & Flags::Maximized));

			if (GLFWwindow* ptr = glfwCreateWindow(m_mode.width, m_mode.height, m_title.c_str(), NULL, NULL))
			{
				if (m_handle = static_cast<GLFWwindow*>(ptr))
				{
					glfwMakeContextCurrent(ptr);

					glfwSetWindowSizeCallback(static_cast<GLFWwindow*>(m_handle),
						[](GLFWwindow* window, int w, int h)
					{});

					glfwSetWindowPosCallback(static_cast<GLFWwindow*>(m_handle),
						[](GLFWwindow* window, int w, int h)
					{});

					glfwSetCharCallback(static_cast<GLFWwindow*>(m_handle),
						[](GLFWwindow* window, uint32_t c)
					{});

					glfwSetScrollCallback(static_cast<GLFWwindow*>(m_handle),
						[](GLFWwindow* window, double x, double y)
					{});

					glfwSetWindowCloseCallback(static_cast<GLFWwindow*>(m_handle),
						[](GLFWwindow* window)
					{});

					glfwSetFramebufferSizeCallback(static_cast<GLFWwindow*>(m_handle),
						[](GLFWwindow* window, int width, int height)
					{});

					glfwSetWindowFocusCallback(static_cast<GLFWwindow*>(m_handle),
						[](GLFWwindow* window, int focused)
					{});

					return initialize();
				}
				return ErrorCode::ER_Invalid_Handle;
			}
			return ErrorCode::ER_GLFW_Create_Failure;
		}
		return ErrorCode::ER_GLFW_Init_Failure;
	}

	Window::ErrorCode Window::initialize()
	{
		return ErrorCode::ER_Success;
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

	Window & Window::setCentered()
	{
		return setPosition((Screen::size() - getSize()) / 2);
	}

	Window & Window::setCursorMode(CursorMode value)
	{
		glfwSetInputMode(static_cast<GLFWwindow*>(m_handle), GLFW_CURSOR, value);
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

	Window & Window::swapBuffers()
	{
		glfwSwapBuffers(static_cast<GLFWwindow*>(m_handle));
		return (*this);
	}


	bool Window::isOpen() const
	{
		return !glfwWindowShouldClose(static_cast<GLFWwindow*>(m_handle));
	}
}